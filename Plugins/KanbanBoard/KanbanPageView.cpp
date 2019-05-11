#include "ui_KanbanPageView.h"
#include "KanbanPageView.h"
#include "KanbanDelegate.h"
#include "KanbanColumnProxyModel.h"
#include "KanbanColumnView.h"

#include "../Common/PluginConfig.h"
#include "../Common/Utils.h"
#include "Model.h"

#include <QInputDialog>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QSplitter>
#include <QListView>

KanbanPageView::KanbanPageView(const QString& pageName, KanbanItemModel* model, QWidget *parent)
	: QWidget(parent),
	ui{new Ui::KanbanPageView()},
	mPageName{pageName}
{	
	setModel(model);
	
	ui->setupUi(this);
	setAcceptDrops(true);

	// Create circular button mask
	auto w = ui->mButtonColumnCreate->iconSize().width();
	auto h = ui->mButtonColumnCreate->iconSize().height();
	auto m = std::min(w, h);

	auto bw = ui->mButtonColumnCreate->size().width();
	auto bh = ui->mButtonColumnCreate->size().height();
	auto bm = std::min(bw, bh);

	ui->mButtonColumnCreate->setFixedSize(bm, bm);

	auto pad = (bm - m)/2-1;

	QRect rect = QRect(QPoint(pad, pad), QSize(m+1, m+1));
	QRegion region = QRegion(rect,QRegion::Ellipse);
	ui->mButtonColumnCreate->setMask(region);
    //ui->mButtonColumnCreate->setMask(QRegion(QRect(-1, -1, w+2, h+2),QRegion::Ellipse));

	// Kanban Items
	connect(ui->mButtonKanbanCreate, &QPushButton::clicked, this, &KanbanPageView::onCreateKanban);
	connect(ui->mButtonKanbanRename, &QPushButton::clicked, this, &KanbanPageView::onRenameKanban);
	connect(ui->mButtonKanbanDelete, &QPushButton::clicked, this, &KanbanPageView::onDeleteKanban);

	// Column Items
	connect(ui->mButtonColumnCreate, &QPushButton::clicked, this, &KanbanPageView::onCreateColumn);
	connect(ui->mButtonColumnRename, &QPushButton::clicked, this, &KanbanPageView::onRenameColumn);
	connect(ui->mButtonColumnDelete, &QPushButton::clicked, this, &KanbanPageView::onDeleteColumn);

	// Splitter (parent of KanbanColumnView items)
	mSplitterColumnViews = new QSplitter(Qt::Horizontal, this);
	mSplitterColumnViews->setFrameShape(QFrame::Box);
    mSplitterColumnViews->setHandleWidth(10);
	mSplitterColumnViews->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	ui->mColumnViewLayout->insertWidget(ui->mColumnViewLayout->count()-1, mSplitterColumnViews);
}

KanbanPageView::~KanbanPageView()
{
	delete ui;
}

void KanbanPageView::loadConfig(const QJsonObject& pageConfig)
{
	const QJsonValue columnsNode = pageConfig.value("columns");
	for (auto&& node : columnsNode.toArray())
	{
		const auto name = node.toObject().value(QString("name")).toString();
		const auto color = node.toObject().value(QString("color")).toString();
		const auto isCollapsed = node.toObject().value(QString("collapsed")).toBool();
		
		createColumn(name, Utils::stringToColor(color), isCollapsed);
	}
}

void KanbanPageView::saveConfig(QJsonArray& config) const
{
	QJsonArray columnsNode;
	for (auto&& columnView : mColumnViews)
	{
		QJsonObject node{};

		node.insert("collapsed", columnView->isCollapsed());
		node.insert("color", Utils::colorToString(columnView->getColor()));
		node.insert("name", columnView->getTitle());

		columnsNode.append(node);
	}

	QJsonObject pageNode;
	pageNode.insert("columns", columnsNode);
	pageNode.insert("page", QJsonValue(mPageName));
	
	config.append(pageNode);
}

void KanbanPageView::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasText() || event->mimeData()->hasUrls())
	{
		event->acceptProposedAction();
	}
}

void KanbanPageView::dragMoveEvent(QDragMoveEvent* event)
{
	event->acceptProposedAction();
}

void KanbanPageView::dragLeaveEvent(QDragLeaveEvent* event)
{
	event->accept();
}

void KanbanPageView::dropEvent(QDropEvent* event)
{
	// Returns the KanbanColumnView on which the drop happened.
	// The loop over the widget hierarchy is needed to find a KanbanColumnView (a child of mColumnSplitter),
	// since the widget returned by childAt() is the deepest one, not necessarily a KanbanColumnView;
	const auto getColumnViewUnderDrop = [&] (const QPoint& droppedPoint)
	{
		QWidget* parent = mSplitterColumnViews;
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
		qDebug() << "###" << child->objectName();
		return dynamic_cast<KanbanColumnView*>(child);
	};
	
	if (const auto column = getColumnViewUnderDrop(event->pos()); column)
	{
		const auto mimeData = event->mimeData();
		if (mimeData->hasText() && !mimeData->hasUrls())
		{
			const auto droppedText = mimeData->text();
			const auto columnName = column->getTitle();
			createKanban(droppedText, column->getColor(), columnName);
		}

		if (mimeData->hasUrls())
		{
			const auto columnName = column->getTitle();
			for (auto&& url : mimeData->urls())
			{
				createKanban(url.toDisplayString(), column->getColor(), columnName);
			}
		}
	}

	event->acceptProposedAction();
}

void KanbanPageView::onCreateKanban()
{
	bool ok;
	QString text = QInputDialog::getText(this, "Kanban Text", "Set text for the new Kanban item", QLineEdit::Normal, QString(), &ok);

	if (ok && !text.isEmpty())
	{
		createKanban(text, Utils::getRandomColor(), mSelectedColumnName);
	}
}

void KanbanPageView::onRenameKanban()
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
}

void KanbanPageView::onDeleteKanban()
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
}

void KanbanPageView::onCreateColumn()
{
	bool ok;
	QString columnName = QInputDialog::getText(this, "Kanban Column", "Create new Kanban column", QLineEdit::Normal, QString(), &ok);
	
	const QColor columnColor {Utils::getRandomColor()};

	if (ok && !columnName.isEmpty())
	{ 
		createColumn(columnName, columnColor);
	}
}

void KanbanPageView::onRenameColumn()
{
	//Q_ASSERT(mSelectedColumnName != QString{} );

	bool ok;
	QString text = QInputDialog::getText(this, "Column Name", "Set new text for the selected column", QLineEdit::Normal, mSelectedColumnName, &ok);

	if (ok && !text.isEmpty())
	{
		if (const auto column = findColumn(mSelectedColumnName); column != mColumnViews.end())
		{
			(*column)->setTitle(text);
			(*column)->setFilter(text);

			for (auto row = 0; row < mKanbanModel->rowCount(); ++row)
			{
				auto idx = mKanbanModel->index(row);
				auto idxColumn = mKanbanModel->data(idx, KanbanItemModel::Roles::ColumnName).toString();

				if (idxColumn == mSelectedColumnName)
				{
					mKanbanModel->setData(idx, text, KanbanItemModel::Roles::ColumnName);
				}
			}
		}
	}
}

void KanbanPageView::onDeleteColumn()
{
	onDeleteColumnView(mSelectedColumnName);
}

void KanbanPageView::onKanbanSelected(const QString& columnSenderName, const QStringList& kanbanTextList)
{
	// Deselect all kanban items (except for those on the sender column)
	for (auto&& columnView : mColumnViews)
	{
		if (columnView->getTitle() != columnSenderName)
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
}

void KanbanPageView::onAddColumnViewKanban(const QString& columnName)
{
	bool ok;
	QString text = QInputDialog::getText(this, "Kanban Text", "Set text for the new Kanban item", QLineEdit::Normal, QString(), &ok);

	if (ok && !text.isEmpty())
	{
		if (const auto column = findColumn(columnName); *column!=nullptr)
		{
			createKanban(text, (*column)->getColor(), columnName);
		}
	}
}

// UNUSED
void KanbanPageView::onChangeColumn()
{
	if (mSelectionKanbanModel->selectedIndexes().isEmpty()) { return; }

	const QModelIndex firstSelectedIdx = mSelectionKanbanModel->selectedIndexes().first();
	const QString oldColumn = mKanbanModel->data(firstSelectedIdx, KanbanItemModel::Roles::ColumnName).toString();

	bool ok;
	QString newColumn = QInputDialog::getText(this, "Kanban Column", "Set new column for the selected Kanban(s)", QLineEdit::Normal, oldColumn, &ok);

	if (ok && !newColumn.isEmpty())
	{ 
		for (auto&& idx : mSelectionKanbanModel->selectedIndexes())
		{
			mKanbanModel->setData(idx, newColumn, KanbanItemModel::Roles::ColumnName);
		}
	}
}

void KanbanPageView::onDeleteColumnView(const QString& deletedColumnName)
{
	if(const auto columnIt = findColumn(deletedColumnName); columnIt!=mColumnViews.end())
	{
		std::vector<int> rowsToDelete;
		for (auto row = 0; row < mKanbanModel->rowCount(); ++row)
		{
			const QString columnName = mKanbanModel->index(row).data(KanbanItemModel::Roles::ColumnName).toString();
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

		ui->mColumnViewLayout->removeWidget(*columnIt);
		(*columnIt)->setParent(nullptr);
		mColumnViews.erase(columnIt);
		// delete columnIt->second; // NO! columnView has already a parent that takes its ownership. Do not delete it explicitly.
	}
}

void KanbanPageView::setModel(KanbanItemModel* kanbanModel)
{
	mKanbanModel = kanbanModel;
	mSelectionKanbanModel = new QItemSelectionModel(mKanbanModel, this);
	mSelectionKanbanModel->clearSelection();
}

void KanbanPageView::setSelectedColumnView(const QString& selectedColumnName)
{
	mSelectedColumnName = selectedColumnName;
}

void KanbanPageView::createKanban(const QString& text, const QColor& color, const QString& columnName) const
{
	const QModelIndex idx = mKanbanModel->addKanban(text, Utils::colorToString(color), columnName);
}

std::vector<KanbanColumnView*>::iterator KanbanPageView::findColumn(const QString& columnName)
{
	return std::find_if(mColumnViews.begin(), mColumnViews.end(), [columnName](auto&& c){return c->getTitle() == columnName;});
}

void KanbanPageView::createColumn(const QString& columnName, const QColor& columnColor, bool isCollapsed)
{
	auto columnView = new KanbanColumnView(Utils::getUniqueName(columnName, getColumnViewNames()), columnColor, isCollapsed, mSplitterColumnViews);
	auto columnModel = new KanbanColumnProxyModel(columnName, columnView);

	columnModel->setSourceModel(mKanbanModel);
	columnModel->setFilterRegExp(QRegExp(columnName, Qt::CaseInsensitive, QRegExp::FixedString));

	columnView->setDelegate(new KanbanDelegate(this));
	columnView->setModel(columnModel);

	connect(columnView, &KanbanColumnView::columnDeleted, this, &KanbanPageView::onDeleteColumnView);
	connect(columnView, &KanbanColumnView::kanbanCreated, this, &KanbanPageView::onAddColumnViewKanban);
	connect(columnView, &KanbanColumnView::kanbanSelected, this, &KanbanPageView::onKanbanSelected);

	mSplitterColumnViews->addWidget(columnView);
	mColumnViews.emplace_back(columnView);
}

QStringList KanbanPageView::getColumnViewNames() const
{
	QStringList columnViewNames;
	for (auto&& columnView : mColumnViews)
	{
		columnViewNames << columnView->getTitle();
	}
	return columnViewNames;
}
