
#include "MainWindow.h"
#include "../Model/Model.h"
//#include "KanbanView.h"

#include <QDebug>
#include <QItemSelectionModel>
#include <QPluginLoader>
#include <QDir>
#include <qapplication.h>

//MainWindow::MainWindow(QWidget *parent)
//	: QMainWindow(parent),
//	mKanbanView{new KanbanView(this)}
//{
//	ui.setupUi(this);
//
//	KanbanModel* kanbanModel = new KanbanModel(this);
//	mKanbanView->setModel(kanbanModel);
//
//	QItemSelectionModel* selectionKanbanModel = new QItemSelectionModel(kanbanModel, this);
//	mKanbanView->setSelectionModel(selectionKanbanModel);
//
//	setCentralWidget(mKanbanView);
//	mKanbanView->loadData();
//}

bool MainWindow::run(QApplication& mainApp)
{
	// TODO: provide better failure handling and status reporting

	bool status { true };

	status |= loadModel(status);
	status |= loadPlugins(status);
	status |= loadUi(status);

	if (status)
	{
		// TODO: probably the return values of exec() and quit() should not be ignored

		mainApp.exec();
		mainApp.quit();
	}
	else
	{
		qDebug() << "ERROR, unable to start";
	}

	unoadPlugins();
	//unloadModel();
	return status;
}

void MainWindow::unoadPlugins()
{
	// TODO: consider to reverse the order of mPlugins before release in case there are dependancies between plugins

	for (auto plugin : mPlugins)
	{
		plugin->release();
	}
}

bool MainWindow::loadModel(bool status)
{
	if (!status) return status;

	mModel = std::make_shared<Model>();

	return status;
}

bool MainWindow::loadPlugins(bool status)
{
	if (!status) return status;

	auto pluginsDir = QDir(qApp->applicationDirPath());
	const QString pluginsPath = "Plugins";
    if (!pluginsDir.exists(pluginsPath)) { return false; } // Plugin directory not found
    pluginsDir.cd(pluginsPath);

	for (const QString& fileName : pluginsDir.entryList(QDir::Files))
	{
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader.instance();

		if (plugin)
		{
			PluginInterface *plugInterface = qobject_cast<PluginInterface*>(plugin);
			mPlugins.append(plugInterface);			
		}
		else if (loader.errorString().contains("(Cannot mix debug and release libraries.)"))
		{
			// Debug vs release plugin mismatch
			qDebug() << loader.errorString();
			continue;
		}
		else
		{ 
			qDebug() << "Error loading plugin. " + loader.errorString();
		}
	}

	return status;
}

bool MainWindow::loadUi(bool status)
{
	if (!status) return status;
	mMainWindow = std::make_unique<QMainWindow>();
	mUi = std::make_unique<Ui::MainWindowClass>();
	mUi->setupUi(mMainWindow.get());
	
	for (auto plugin : mPlugins)
	{
		plugin->setup(mUi->mPluginsLayout, mModel.get());
		//plugin->setup(mUi->mPluginsLayout, nullptr);
	}

	mMainWindow->show();

	return status;
}