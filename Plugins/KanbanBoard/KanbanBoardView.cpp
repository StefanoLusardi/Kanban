#include "ui_KanbanView.h"
#include "KanbanBoardView.h"
#include "KanbanDelegate.h"
#include "KanbanColumnProxyModel.h"
#include "KanbanColumnView.h"

#include "../Common/PluginConfig.h"
#include "../Common/Utils.h"
#include "Model.h"

#include <QItemSelectionModel>
#include <QInputDialog>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QSplitter>
#include <QListView>



KanbanBoardView::KanbanBoardView(QWidget *parent)
	: QWidget(parent),
	ui{new Ui::KanbanBoardView()}
{	
	ui->setupUi(this);
	setAcceptDrops(true);

	connect(ui->mButtonCreate, &QPushButton::clicked, this, &KanbanBoardView::onCreateKanban);
	connect(ui->mButtonRename, &QPushButton::clicked, this, &KanbanBoardView::onRenameKanban);
	connect(ui->mButtonDelete, &QPushButton::clicked, this, &KanbanBoardView::onDeleteKanban);
	connect(ui->mButtonNewColumn, &QPushButton::clicked, this, &KanbanBoardView::onCreateColumn);
	connect(ui->mNewSpoilerButton, &QPushButton::clicked, this, &KanbanBoardView::onCreateColumn);

	mColumnSplitter = new QSplitter(Qt::Horizontal, this);
	mColumnSplitter->setFrameShape(QFrame::Box);
    mColumnSplitter->setHandleWidth(10);
	mColumnSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	ui->mColumnViewLayout->insertWidget(ui->mColumnViewLayout->count()-1, mColumnSplitter);
}

KanbanBoardView::~KanbanBoardView()
{
	delete ui;
}

void KanbanBoardView::onCreateColumn()
{
	bool ok;
	QString columnName = QInputDialog::getText(this, "Kanban Column", "Create new Kanban column", QLineEdit::Normal, QString(), &ok);
	
	const QColor columnColor {Utils::getRandomColor()};

	if (ok && !columnName.isEmpty())
	{ 
		createColumn(columnName, columnColor);
	}
}

void KanbanBoardView::createColumn(const QString& columnName, const QColor& columnColor, bool isCollapsed)
{
	auto columnView = new KanbanColumnView(Utils::getUniqueName(columnName, getColumnViewNames()), columnColor, isCollapsed, mColumnSplitter);
	auto columnModel = new KanbanColumnProxyModel(columnName, columnView);

	columnModel->setSourceModel(mKanbanModel);
	columnModel->setFilterRegExp(QRegExp(columnName, Qt::CaseInsensitive, QRegExp::FixedString));

	columnView->setDelegate(new KanbanDelegate(this));
	columnView->setModel(columnModel);

	connect(columnView, &KanbanColumnView::columnDeleted, this, &KanbanBoardView::onDeleteColumnView);
	connect(columnView, &KanbanColumnView::kanbanCreated, this, &KanbanBoardView::onAddColumnViewKanban);
	connect(columnView, &KanbanColumnView::kanbanSelected, this, &KanbanBoardView::onKanbanSelected);

	mColumnSplitter->addWidget(columnView);
	mColumnViews.emplace(columnName, columnView);
}

void KanbanBoardView::onKanbanSelected(const QString& columnSenderName, const QStringList& kanbanTextList)
{
	// Deselect all kanban items (except for those on the sender column)
	for (const auto& [columnName, columnView] : mColumnViews)
	{
		if (columnName != columnSenderName)
		{
			columnView->deselectAllKanbanItems();
		}
	}
	
	// Select current items in selectionModel
	mSelectionKanbanModel->clearSelection();
	for (auto&& text : kanbanTextList)
	{
		const auto idx = mKanbanModel->getKanbanIndex(text);
		mSelectionKanbanModel->setCurrentIndex(idx, QItemSelectionModel::Select);
	}

	// Update last selected column
	setSelectedColumnView(columnSenderName);

	update();
}

void KanbanBoardView::setSelectedColumnView(const QString& selectedColumnName)
{
	mSelectedColumnName = selectedColumnName;
}

void KanbanBoardView::onCreateKanban()
{
	bool ok;
	QString text = QInputDialog::getText(this, "Kanban Text", "Set text for the new Kanban item", QLineEdit::Normal, QString(), &ok);

	if (ok && !text.isEmpty())
	{
		createKanban(text, Utils::getRandomColor(), mSelectedColumnName);
	}

	update();
}

void KanbanBoardView::onAddColumnViewKanban(const QString& columnName)
{
	bool ok;
	QString text = QInputDialog::getText(this, "Kanban Text", "Set text for the new Kanban item", QLineEdit::Normal, QString(), &ok);

	if (ok && !text.isEmpty())
	{
		createKanban(text, mColumnViews[columnName]->getColor(), columnName);
	}

	update();
}

void KanbanBoardView::createKanban(const QString& text, const QColor& color, const QString& state) const
{
	const KanbanItem kanban(text, Utils::colorToString(color), state);
	const QModelIndex idx = mKanbanModel->addKanban(kanban);
	mKanbanModel->setData(idx, state, KanbanModel::Roles::State);
}

void KanbanBoardView::onChangeState()
{
	if (mSelectionKanbanModel->selectedIndexes().isEmpty()) { return; }

	const QModelIndex firstSelectedIdx = mSelectionKanbanModel->selectedIndexes().first();
	const QString oldState = mKanbanModel->data(firstSelectedIdx, KanbanModel::Roles::State).toString();

	bool ok;
	QString newState = QInputDialog::getText(this, "Kanban State", "Set new state for the selected Kanban(s)", QLineEdit::Normal, oldState, &ok);

	if (ok && !newState.isEmpty())
	{ 
		for (auto&& idx : mSelectionKanbanModel->selectedIndexes())
		{
			mKanbanModel->setData(idx, newState, KanbanModel::Roles::State);
		}
	}

	update();
}

void KanbanBoardView::onRenameKanban()
{
	if (mSelectionKanbanModel->selectedIndexes().isEmpty()) { return; }

	const QModelIndex firstSelectedIdx = mSelectionKanbanModel->selectedIndexes().first();
	const QString oldText = mKanbanModel->data(firstSelectedIdx).toString();

	bool ok;
	QString newText = QInputDialog::getText(this, "Kanban Text", "Set new text for the selected Kanban(s)", QLineEdit::Normal, oldText, &ok);

	if (ok && !newText.isEmpty())
	{
		for (auto&& idx : mSelectionKanbanModel->selectedIndexes())
		{
			mKanbanModel->setData(idx, newText, Qt::DisplayRole);
		}
	}

	update();
}

void KanbanBoardView::onDeleteKanban()
{
	if (mSelectionKanbanModel->selectedIndexes().isEmpty()) { return; }

	// Sort selected indexes in reverse order before deleting them, otherwise the model is screwed up.
	auto sortedIndexes = mSelectionKanbanModel->selectedIndexes();

	// Reverse order
	std::sort(sortedIndexes.begin(), sortedIndexes.end(), [](auto idx1, auto idx2) { return idx2 < idx1; });
	
	for (const QModelIndex& idx : sortedIndexes)
	{
		mKanbanModel->removeRow(idx.row());		
	}

	update();
}

void KanbanBoardView::onDeleteColumnView(const QString& deletedColumnName)
{
	if(auto columnIt = mColumnViews.find(deletedColumnName); columnIt!=mColumnViews.end())
	{
		std::vector<int> rowsToDelete;
		for (auto row = 0; row < mKanbanModel->rowCount(); ++row)
		{
			const QString columnName = mKanbanModel->index(row).data(KanbanModel::Roles::State).toString();
			if (columnName == deletedColumnName)
			{
				rowsToDelete.push_back(row);
			}
		}

		// Reverse order
		std::sort(rowsToDelete.begin(), rowsToDelete.end(), [](auto idx1, auto idx2) { return idx2 < idx1; });
		
		// Delete from the model all Kanban Items that belong to deletedColumnName in reversed order
		for (auto&& row : rowsToDelete)
		{
			mKanbanModel->removeRow(row);		
		}

		ui->mColumnViewLayout->removeWidget(columnIt->second);
		columnIt->second->setParent(nullptr);
		mColumnViews.erase(columnIt);
		update();
		// delete columnIt->second; // NO! columnView has already a parent that takes its ownership. Do not delete it explicitly.
	}
}

void KanbanBoardView::setModel(KanbanModel* kanbanModel)
{
	mKanbanModel = kanbanModel;
	mSelectionKanbanModel = new QItemSelectionModel(mKanbanModel, this);
	mSelectionKanbanModel->clearSelection();
}

void KanbanBoardView::loadConfig()
{
	const QJsonObject config = PluginConfigManager::parse("KanbanBoard.json");
	const QJsonValue rootNode = config.value("columns");

	for (auto&& node : rootNode.toArray())
	{
		const auto name = node.toObject().value(QString("name")).toString();
		const auto color = node.toObject().value(QString("color")).toString();
		const auto isCollapsed = node.toObject().value(QString("collapsed")).toBool();
		
		createColumn(name, Utils::stringToColor(color), isCollapsed);
	}
}

void KanbanBoardView::saveConfig() const
{
	QJsonArray columns;
	for (auto&& [columnName, columnView] : mColumnViews)
	{
		QJsonObject node{};

		node.insert("collapsed", columnView->isCollapsed());
		node.insert("color", Utils::colorToString(columnView->getColor()));
		node.insert("name", columnName);

		columns.append(node);
	}

	QJsonObject config;
	config.insert("columns", columns);
	PluginConfigManager::write("KanbanBoard.json", config);
}

void KanbanBoardView::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasText() || event->mimeData()->hasUrls())
	{
		event->acceptProposedAction();
	}
}

void KanbanBoardView::dragMoveEvent(QDragMoveEvent* event)
{
	event->acceptProposedAction();
}

void KanbanBoardView::dragLeaveEvent(QDragLeaveEvent* event)
{
	event->accept();
}

void KanbanBoardView::dropEvent(QDropEvent* event)
{
	const auto getColumnViewUnderDrop = [&] (const QPoint& droppedPoint)
	{
		// Returns the KanbanColumnView on which the drop happened.
		// The loop over the widget hierarchy is needed to find a KanbanColumnView (a child of mColumnSplitter),
		// since the widget returned by childAt() is the deepest one, not necessarily a KanbanColumnView;

		QWidget* parent = mColumnSplitter;
		QWidget* child = childAt(droppedPoint);

		// Avoid potentially infinite loop. Set maxDepth to have a fixed number of recursive calls.
		/*while (parent != child->parentWidget())
			child = child->parentWidget();*/
		const int maxDepth {5};
		for (auto i=0; i<maxDepth; ++i)
		{
			if (parent == child->parentWidget()) { break; } 
			child = child->parentWidget(); 
		}
		return dynamic_cast<KanbanColumnView*>(child);
	};
	
	if (const auto column = getColumnViewUnderDrop(event->pos()); column)
	{
		const auto mimeData = event->mimeData();
		if (mimeData->hasText() && !mimeData->hasUrls())
		{
			const auto droppedText = mimeData->text();
			const auto columnName = column->getTitle();
			createKanban(droppedText, mColumnViews[columnName]->getColor(), columnName);
		}

		if (mimeData->hasUrls())
		{
			const auto columnName = column->getTitle();
			for (auto&& url : mimeData->urls())
			{
				createKanban(url.toDisplayString(), mColumnViews[columnName]->getColor(), columnName);
			}
		}
	}

	event->acceptProposedAction();
	
	update();
}

QStringList KanbanBoardView::getColumnViewNames() const
{
	QStringList columnViewNames;
	for (auto&& [name, view] : mColumnViews)
	{
		columnViewNames << name;
	}
	return columnViewNames;
}
