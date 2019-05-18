#include "ui_KanbanButtonsView.h"
#include "KanbanButtonsView.h"

KanbanButtonsView::KanbanButtonsView(QWidget *parent) : QWidget(parent), ui{new Ui::KanbanButtonsView()}
{
	ui->setupUi(this);

	// Kanban Items
	connect(ui->mButtonKanbanCreate, &QToolButton::clicked, [this](){emit createKanbanSignal();} );
	connect(ui->mButtonKanbanRename, &QToolButton::clicked, [this](){emit renameKanbanSignal();} );
	connect(ui->mButtonKanbanDelete, &QToolButton::clicked, [this](){emit deleteKanbanSignal();} );
	
	// Column Items											
	connect(ui->mButtonColumnCreate, &QToolButton::clicked, [this](){emit createColumnSignal();} );
	connect(ui->mButtonColumnRename, &QToolButton::clicked, [this](){emit renameColumnSignal();} );
	connect(ui->mButtonColumnDelete, &QToolButton::clicked, [this](){emit deleteColumnSignal();} );
}

KanbanButtonsView::~KanbanButtonsView()
{
	delete ui;
}
