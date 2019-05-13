#pragma once

#include <QObject>

class QBoxLayout;
class QPushButton;

class FramelessWindow;
class Model;
class SettingsPluginInterface;
class SettingsView;

class SettingsPlugin : public QObject
{
	Q_OBJECT

public:
	SettingsPlugin(QBoxLayout* mainViewLayout, QBoxLayout* pluginButtonsLayout, Model* model, SettingsPluginInterface* parent);
	~SettingsPlugin() = default;

	void initData() const;
	void release() const;

private:
	QPushButton* mPluginButton;
	SettingsView* mPluginView;
	FramelessWindow* mFramelessWindow;
};