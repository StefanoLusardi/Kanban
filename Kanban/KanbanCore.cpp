#include "KanbanCore.h"
#include "SplashScreen/SplashScreen.h"
#include "FramelessWindow/FramelessWindow.h"

#include "../Model/Model.h"
#include "../Plugins/Common/PluginInterface.h"

#include <QMainWindow>
#include <QPluginLoader>
#include <QDir>

int KanbanCore::run()
{	
	bool status { true };
	status |= showSplashScreen(status);
	status |= createModel(status);
	status |= createUi(status);
	status |= loadPlugins(status);
	status |= setupPlugins(status);
	status |= setupWindow(status);
	
	const auto execReturn = [this](bool status) -> int
	{
		if (status)
		{
			mFramelessWindow->connect(mModel->getSettingsModel().get(), &SettingsModel::styleChanged, mFramelessWindow.get(), &FramelessWindow::changeStyle);
			mFramelessWindow->connect(mFramelessWindow.get(), &FramelessWindow::aboutToClose, [this]() { shutdown(); });
			mModel->getSettingsModel()->loadSettings();
			const auto execReturn = qApp->exec();
			qApp->quit();
			return execReturn;
		}
		shutdown();
		return -1;
	}(status);
	
	return execReturn;
}

void KanbanCore::shutdown()
{
	unloadPlugins();
	unloadModel();
}

bool KanbanCore::showSplashScreen(bool status)
{
	if (!status) return status;

	mSplashScreen = std::make_shared<SplashScreen>();
    mSplashScreen->setupMessageArea();
	mSplashScreen->show();
	mSplashScreen->writeMessage("Setup Kanban");

	return status;
}

bool KanbanCore::createModel(bool status)
{
	if (!status) return status;

	mSplashScreen->writeMessage("Loading Data");

	mModel = std::make_shared<Model>();
	mModel->loadData();

	return status;
}

bool KanbanCore::createUi(bool status)
{
	if (!status) return status;

	mSplashScreen->writeMessage("Loading User Interface");

	mMainWindow = new QMainWindow();
	mUi = std::make_unique<Ui::MainWindowClass>();
	mUi->setupUi(mMainWindow);
	
	return status;
}

bool KanbanCore::loadPlugins(bool status)
{
	if (!status) return status;

	mSplashScreen->writeMessage("Loading Plugins");

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
			mSplashScreen->writeMessage("Loading " + plugInterface->name() + " Plugin");
		}
		else
		{ 
			mSplashScreen->writeMessage("Invalid plugin. " + loader.errorString());
		}
	}

	return status;
}

bool KanbanCore::setupPlugins(bool status)
{
	if (!status) return status;

	mSplashScreen->writeMessage("Initializing Plugins");
	
	for (const auto& plugin : mPlugins)
	{
		plugin->setup(mUi->mMainViewLayout, mUi->mPluginButtonsLayout, mModel.get());
		plugin->initData();
	}

	return status;
}

bool KanbanCore::setupWindow(bool status)
{
	if (!status) return status;

	mSplashScreen->writeMessage("Ready");
	mSplashScreen->finish(mMainWindow);

	mFramelessWindow = std::make_shared<FramelessWindow>();
	mFramelessWindow->setWindowTitle("Kanban");
	mFramelessWindow->setContent(mMainWindow);
	mFramelessWindow->setWindowState(Qt::WindowMaximized);
	mFramelessWindow->show();

	return status;
}

void KanbanCore::unloadPlugins()
{
	std::for_each(mPlugins.rbegin(), mPlugins.rend(), [](PluginInterface* plugin)
	{
		plugin->release();
	});
	mPlugins.clear();
}

void KanbanCore::unloadModel()
{
	mModel->saveData();
}
