#pragma once

#include <QObject>
#include <QDialog>
#include <QPushButton>

class Model;
class QBoxLayout;
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
	std::unique_ptr<QDialog> mPluginDialog;
};