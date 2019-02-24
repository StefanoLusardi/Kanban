#pragma once

#include <ui_MainWindow.h>

class QMainWindow;
class Model;
class SplashScreen;
class FramelessWindow;
class PluginInterface;

class KanbanCore final
{
public:
	explicit KanbanCore(QWidget *parent = Q_NULLPTR) { }
	int run();


protected:
	bool showSplashScreen(bool status);
	bool createModel(bool status);
	bool createUi(bool status);
	bool loadPlugins(bool status);
	bool setupPlugins(bool status);
	bool setupWindow(bool status);
	void unloadPlugins();
	void unloadModel();
	void shutdown();

private:
	QMainWindow* mMainWindow{};
	std::unique_ptr<Ui::MainWindowClass> mUi;
	std::shared_ptr<SplashScreen> mSplashScreen;
	std::shared_ptr<FramelessWindow> mFramelessWindow;
	std::shared_ptr<Model> mModel;
	QList<PluginInterface*> mPlugins;
};
