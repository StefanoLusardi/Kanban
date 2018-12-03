#include "SettingsView.h"
#include "ui_SettingsView.h"

SettingsView::SettingsView(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::SettingsView)
{
	ui->setupUi(this);
}

SettingsView::~SettingsView()
{
	delete ui;
}

void SettingsView::setModel(KanbanModel* kanbanModel)
{
	mKanbanModel = kanbanModel;
}
