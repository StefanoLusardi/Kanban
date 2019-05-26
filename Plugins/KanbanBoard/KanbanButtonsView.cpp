#include "ui_KanbanButtonsView.h"
#include "KanbanButtonsView.h"

KanbanButtonsView::KanbanButtonsView(QWidget *parent) : QWidget(parent), ui{new Ui::KanbanButtonsView()}
{
	ui->setupUi(this);

	// Kanban Items
	connect(ui->mButtonKanbanCreate, &QToolButton::clicked, [this](){emit createKanbanSignal(mCurrentPageName);} );
	connect(ui->mButtonKanbanRename, &QToolButton::clicked, [this](){emit renameKanbanSignal(mCurrentPageName);} );
	connect(ui->mButtonKanbanDelete, &QToolButton::clicked, [this](){emit deleteKanbanSignal(mCurrentPageName);} );
																							 
	// Column Items																			 
	connect(ui->mButtonColumnCreate, &QToolButton::clicked, [this](){emit createColumnSignal(mCurrentPageName);} );
	connect(ui->mButtonColumnRename, &QToolButton::clicked, [this](){emit renameColumnSignal(mCurrentPageName);} );
	connect(ui->mButtonColumnDelete, &QToolButton::clicked, [this](){emit deleteColumnSignal(mCurrentPageName);} );
}

KanbanButtonsView::~KanbanButtonsView()
{
	delete ui;
}

void KanbanButtonsView::setCurrentPage(const QString& pageName)
{
	mCurrentPageName = pageName;
}
