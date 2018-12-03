#pragma once

#include "../Plugins/Common/PluginInterface.h"
#include "ui_MainWindow.h"
#include <QtWidgets/QMainWindow>


class MainWindow final
{	
public:
	MainWindow(QWidget *parent = Q_NULLPTR) {}
	bool run(QApplication& mainApp);

protected:
	bool loadModel(bool status);
	bool loadPlugins(bool status);
	bool loadUi(bool status);
	void unoadPlugins();

private:
	std::unique_ptr<Ui::MainWindowClass> mUi;
	std::unique_ptr<QMainWindow> mMainWindow;
	std::shared_ptr<Model> mModel;
	QList<PluginInterface*> mPlugins;

	
	//KanbanView* mKanbanView;
};
