#include "ui_KanbanView.h"
#include "KanbanBoardView.h"
#include "KanbanDelegate.h"
#include "KanbanColumnProxyModel.h"
#include "KanbanColumnView.h"

#include "../../Model/KanbanModel.h"
#include "../../Model/Roles.h"

#include <QItemSelectionModel>
#include <QInputDialog>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QSplitter>
#include <QDebug>

KanbanBoardView::KanbanBoardView(QWidget *parent)
	: QWidget(parent),
	ui{new Ui::KanbanBoardView()}
{	
	ui->setupUi(this);

	connect(ui->mButtonCreate, &QPushButton::clicked, this, &KanbanBoardView::onCreateKanban);
	connect(ui->mButtonRename, &QPushButton::clicked, this, &KanbanBoardView::onRenameKanban);
	connect(ui->mButtonDelete, &QPushButton::clicked, this, &KanbanBoardView::onDeleteKanban);
	connect(ui->mButtonNewColumn, &QPushButton::clicked, this, &KanbanBoardView::onCreateColumn);
	connect(ui->mNewSpoilerButton, &QPushButton::clicked, this, &KanbanBoardView::onCreateColumn);

	mColumnSplitter = new QSplitter(Qt::Horizontal, this);
	mColumnSplitter->setFrameShape(QFrame::Box);
    mColumnSplitter->setHandleWidth(5);
	//mSplitter->setStyleSheet("QSplitter::handle {image: url(images/splitter.png);} ");
	mColumnSplitter->setStyleSheet("QSplitter::handle {background: rgb(0, 0, 0);} ");
	mColumnSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	ui->mSpoilerLayout->insertWidget(ui->mSpoilerLayout->count()-1, mColumnSplitter);
}

KanbanBoardView::~KanbanBoardView()
{
	delete ui;
}

void KanbanBoardView::onCreateColumn()
{
	bool ok;
	QString columnName = QInputDialog::getText(this, "Kanban Column", "Create new Kanban column", QLineEdit::Normal, QString(), &ok);
	
	// TODO: get color from user dialog
	QColor columnColor {Qt::darkGreen};

	if (ok && !columnName.isEmpty())
	{ 
		createColumn(columnName, columnColor);
	}	
}

void KanbanBoardView::createColumn(const QString& columnName, const QColor& columnColor)
{
	auto columnView = new KanbanColumnView(getUniqueName(columnName), columnColor, mColumnSplitter);
	auto columnModel = new KanbanColumnProxyModel(columnName, columnView);

	columnView->setModel(columnModel);
	columnView->setDelegate(new KanbanDelegate(this));

	columnModel->setSourceModel(mKanbanModel);
	columnModel->setFilterRegExp(QRegExp(columnName, Qt::CaseInsensitive, QRegExp::FixedString));
    
	connect(columnView, &KanbanColumnView::columnDeleted, this, &KanbanBoardView::onDeleteColumnView);
	connect(columnView, &KanbanColumnView::kanbanCreated, this, &KanbanBoardView::onAddColumnViewKanban);

	mColumnSplitter->addWidget(columnView);
	mColumnViews.emplace(columnName, columnView);
}

void KanbanBoardView::onCreateKanban()
{
	// TODO: create custom dialog to set Kanban text, state and color

	bool ok;
	QString text = QInputDialog::getText(this, "Kanban Text", "Set text for the new Kanban item", QLineEdit::Normal, QString(), &ok);

	if (ok && !text.isEmpty())
	{
		createKanban(text, Qt::cyan, QString("A"));
	}
}

void KanbanBoardView::onAddColumnViewKanban(const QString& columnName)
{
	// TODO: try to merge somehow with with KanbanView::onCreateKanban()

	bool ok;
	QString text = QInputDialog::getText(this, "Kanban Text", "Set text for the new Kanban item", QLineEdit::Normal, QString(), &ok);

	if (ok && !text.isEmpty())
	{
		createKanban(text, mColumnViews[columnName]->getColor(), columnName);
	}
}

void KanbanBoardView::createKanban(const QString& text, const QColor& color, const QString& state) const
{
	const KanbanItem kanban(text, color, state);
	const QModelIndex idx = mKanbanModel->addKanban(kanban);
	mKanbanModel->setData(idx, state, Roles::State);
	mSelectionKanbanModel->setCurrentIndex(idx, QItemSelectionModel::Select);
}

void KanbanBoardView::onChangeState()
{
	if (mSelectionKanbanModel->selectedIndexes().isEmpty()) { return; }

	const QModelIndex idx = mSelectionKanbanModel->selectedIndexes().first();
	const QString oldState = mKanbanModel->data(idx, Roles::State).toString();

	bool ok;
	QString newState = QInputDialog::getText(this, "Kanban State", "Edit Kanban state", QLineEdit::Normal, oldState, &ok);

	if (ok && !newState.isEmpty())
	{ 
		mKanbanModel->setData(idx, newState, Roles::State);
	}
}

void KanbanBoardView::onRenameKanban()
{
	if (mSelectionKanbanModel->selectedIndexes().isEmpty()) { return; }

	const QModelIndex idx = mSelectionKanbanModel->selectedIndexes().first();
	const QString oldText = mKanbanModel->data(idx).toString();
	
	bool ok;
	QString newText = QInputDialog::getText(this, "Kanban Text", "Set new text for the selected Kanban", QLineEdit::Normal, oldText, &ok);

	if (ok && !newText.isEmpty())
	{
		mKanbanModel->setData(idx, newText, Qt::DisplayRole);
	}
}

void KanbanBoardView::onDeleteKanban()
{
	if (mSelectionKanbanModel->selectedIndexes().isEmpty()) { return; }

	QModelIndex firstSelectedIdx = mSelectionKanbanModel->selectedIndexes().first();
	QModelIndexList indexes = mSelectionKanbanModel->selectedIndexes();
	for (const QModelIndex& idx : indexes)
	{
		mKanbanModel->removeRow(idx.row());		
	}
	
	QModelIndex prevIdx = mKanbanModel->index(firstSelectedIdx.row()-1);
	if (prevIdx.isValid()) 
	{
		mSelectionKanbanModel->setCurrentIndex(prevIdx, QItemSelectionModel::SelectCurrent);
		return;
	}
	
	QModelIndex nextIdx = mKanbanModel->index(firstSelectedIdx.row()+1);
	if (nextIdx.isValid()) 
	{
		mSelectionKanbanModel->setCurrentIndex(nextIdx, QItemSelectionModel::SelectCurrent);
		return;
	}
}

void KanbanBoardView::onDeleteColumnView(const QString& columnName)
{
	if(auto columnIt = mColumnViews.find(columnName); columnIt!=mColumnViews.end())
	{
		ui->mSpoilerLayout->removeWidget(columnIt->second);
		columnIt->second->setParent(nullptr);
		mColumnViews.erase(columnIt);
		update();
		// delete columnIt->second; // NO! columnView has already a parent that takes its ownership. Do not delete it explicitly.
	}
}

void KanbanBoardView::setModel(KanbanModel* kanbanModel)
{
	mKanbanModel = kanbanModel;
}

void KanbanBoardView::setSelectionModel(QItemSelectionModel* selectionKanbanModel)
{
	mSelectionKanbanModel = selectionKanbanModel;	
}

void KanbanBoardView::loadData()
{
	// TODO: replace these dummy data with an actual configuration reader class

	createColumn("A", Qt::darkGreen);
	createColumn("B", Qt::darkMagenta);
	createColumn("C", Qt::darkYellow);
	createColumn("D", Qt::darkCyan);
	createColumn("E", Qt::darkGray);
}

void KanbanBoardView::dragEnterEvent(QDragEnterEvent* event)
{
	qDebug() << "dragEnterEvent";
	event->acceptProposedAction();
}

void KanbanBoardView::dragMoveEvent(QDragMoveEvent* event)
{
	qDebug() << "dragMoveEvent";
	event->acceptProposedAction();
}

void KanbanBoardView::dragLeaveEvent(QDragLeaveEvent* event)
{
	qDebug() << "dragLeaveEvent";
	event->accept();
}

void KanbanBoardView::dropEvent(QDropEvent* event)
{
	qDebug() << "dropEvent";

	const QMimeData *mimeData = event->mimeData();
	if (mimeData->hasText())
	{
		QByteArray encodedData = mimeData->data("application/vnd.text.list");
		QDataStream stream(&encodedData, QIODevice::ReadOnly);

		QString text;
		QColor color;
		QString state;
		stream >> text >> color >> state;

		createKanban(text, color, state);
	}
}

QString KanbanBoardView::getUniqueName(const QString& name) const
{
    QStringList spoilerNames;
    for (auto idx = 0; idx < ui->mSpoilerLayout->count(); ++idx)
    {        
        if(const auto w = ui->mSpoilerLayout->itemAt(idx)->widget(); w)
        {            
            if (const auto s = dynamic_cast<KanbanColumnView*>(w); s)
            {
                spoilerNames.append(s->getTitle());
            }
        }
    }

    int i = 0;
    QString try_this = name;

    auto strings_match = [&try_this](const QString& exists) -> bool
    {
        return try_this == exists;
    };

    while (std::count_if(spoilerNames.begin(), spoilerNames.end(), strings_match) > 0)
    {
        try_this = name + " (" + QString::number(++i) + ")";
    }

    return try_this;
}
