#include "KanbanCore.h"
#include "../Model/Model.h"
#include "FramelessWindow/framelesswindow.h"

#include <QDebug>
#include <QItemSelectionModel>
#include <QPluginLoader>
#include <QDir>
#include <QApplication>
#include <QStyleFactory>

int KanbanCore::run(QApplication& app)
{
	// TODO: provide better failure handling and status reporting
	bool status { true };

	status |= createModel(status);
	status |= createUi(status);
	status |= loadPlugins(status);
	status |= setupPlugins(status);

	if (!status)
	{
		qDebug() << "ERROR, unable to start";
		return -1;
	}
	
	// TODO: get the current theme from the model or the settings plugin
	QApplication::setStyle(QStyleFactory::create("light"));	
	//QApplication::setStyle(QStyleFactory::create("dark"));	

	FramelessWindow framelessWindow;
	framelessWindow.setWindowTitle("Kanban");
	framelessWindow.setWindowState(Qt::WindowFullScreen);
	framelessWindow.setContent(mMainWindow);
	framelessWindow.show();

	//mMainWindow->setWindowState(Qt::WindowMaximized);
	//mMainWindow->show();

	// TODO: probably the return value of exec() should not be ignored
	const auto execReturn = app.exec();
	app.quit();	

	unoadPlugins();
	unloadModel();
	return execReturn;
}

bool KanbanCore::createModel(bool status)
{
	if (!status) return status;

	mModel = std::make_shared<Model>();
	mModel->loadData();

	return status;
}

bool KanbanCore::createUi(bool status)
{
	if (!status) return status;

	mMainWindow = new QMainWindow();
	mUi = std::make_unique<Ui::MainWindowClass>();
	mUi->setupUi(mMainWindow);

	return status;
}

bool KanbanCore::loadPlugins(bool status)
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

bool KanbanCore::setupPlugins(bool status)
{
	if (!status) return status;
	
	for (const auto& plugin : mPlugins)
	{
		plugin->setup(mUi->mMainViewLayout, mUi->mPluginButtonsLayout, mModel.get());
		plugin->initData();
	}

	return status;
}

void KanbanCore::unoadPlugins()
{
	// TODO: consider to reverse the order of mPlugins before release in case there are dependancies between plugins
	for (auto plugin : mPlugins)
	{
		plugin->release();
	}
}

void KanbanCore::unloadModel()
{
	mModel->saveData();
}
