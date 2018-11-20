#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class KanbanView;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

private:
	Ui::MainWindowClass ui;
	KanbanView* mKanbanView;
};
