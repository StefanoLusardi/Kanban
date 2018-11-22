#include "ui_KanbanView.h"
#include "KanbanBoardView.h"
#include "KanbanDelegate.h"
#include "KanbanModel.h"
#include "KanbanColumnProxyModel.h"
#include "KanbanColumnView.h"
#include "Roles.h"

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
}

KanbanBoardView::~KanbanBoardView()
{
	delete ui;
}

void KanbanBoardView::onCreateColumn()
{
	bool ok;
	QString columnName = QInputDialog::getText(this, "Kanban Column", "Create new Kanban column", QLineEdit::Normal, QString(), &ok);

	if (ok && !columnName.isEmpty())
	{ 
		createColumn(columnName);
	}	
}

void KanbanBoardView::createColumn(const QString& columnName)
{
	auto columnModel = new KanbanColumnProxyModel(columnName, this);
	columnModel->setSourceModel(mKanbanModel);
	columnModel->setFilterRegExp(QRegExp(columnName, Qt::CaseInsensitive, QRegExp::FixedString));

	auto columnView = new KanbanColumnView(getUniqueName(columnName), this);
	columnView->setModel(columnModel);
	columnView->setDelegate(new KanbanDelegate(this));
    
	connect(columnView, &KanbanColumnView::columnDeleted, this, &KanbanBoardView::onDeleteColumnView);
	connect(columnView, &KanbanColumnView::kanbanCreated, this, &KanbanBoardView::onAddColumnViewKanban);

    ui->mSpoilerLayout->insertWidget(ui->mSpoilerLayout->count()-1, columnView);
	ui->mSpoilerLayout->insertWidget(ui->mSpoilerLayout->count()-1, new QSplitter(Qt::Vertical, columnView));
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
		createKanban(text, Qt::cyan, columnName);
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
	if(const auto columnIt = mColumnViews.find(columnName); columnIt!=mColumnViews.end())
	{
		mColumnViews.erase(columnIt);	
		ui->mSpoilerLayout->removeWidget(columnIt->second);
		columnIt->second->setParent(nullptr);
//    delete columnIt->second; // NO! columnView has already a parent that takes its ownership. Do not delete it explicitly.
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

	createColumn("A");
	createColumn("B");
	createColumn("C");
	createColumn("D");
	createColumn("E");

	createKanban("a_123", Qt::yellow, "A");
	createKanban("a_456", Qt::magenta, "A");
	createKanban("b_123", Qt::magenta, "B");
	createKanban("b_456", Qt::darkCyan, "B");
	createKanban("c_000", Qt::cyan, "C");
	createKanban("d_001", Qt::darkMagenta, "D");
	createKanban("d_002", Qt::magenta, "D");
	createKanban("d_003", Qt::darkYellow, "D");
	createKanban("d_004", Qt::yellow, "D");
	createKanban("d_005", Qt::cyan, "D");
	createKanban("d_006", Qt::darkCyan, "D");
	createKanban("d_007", Qt::darkMagenta, "D");
	createKanban("d_008", Qt::magenta, "D");
	createKanban("d_009", Qt::darkYellow, "D");
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