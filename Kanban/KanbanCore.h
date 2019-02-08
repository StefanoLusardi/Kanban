#pragma once

#include "../Plugins/Common/PluginInterface.h"
#include "ui_MainWindow.h"
#include <QtWidgets/QMainWindow>


class KanbanCore final
{	
public:
	KanbanCore(QWidget *parent = Q_NULLPTR) {}
	int run(QApplication& app);

protected:
	bool createModel(bool status);
	bool createUi(bool status);
	bool loadPlugins(bool status);
	bool setupPlugins(bool status);
	void unoadPlugins();
	void unloadModel();

private:
	QMainWindow* mMainWindow;
	std::unique_ptr<Ui::MainWindowClass> mUi;
	std::shared_ptr<Model> mModel;
	QList<PluginInterface*> mPlugins;
};
