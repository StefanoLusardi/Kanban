#include "MainWindow.h"
#include "KanbanView.h"
#include "KanbanModel.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	mKanbanView{new KanbanView(this)}
{
	ui.setupUi(this);

	KanbanModel* kanbanModel = new KanbanModel(this);
	mKanbanView->setModel(kanbanModel);

	QItemSelectionModel* selectionKanbanModel = new QItemSelectionModel(kanbanModel, this);
	mKanbanView->setSelectionModel(selectionKanbanModel);

	setCentralWidget(mKanbanView);
	mKanbanView->loadData();
}
