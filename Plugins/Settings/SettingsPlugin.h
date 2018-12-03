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
	SettingsPlugin(QBoxLayout* container, Model* model, SettingsPluginInterface *parent);
	~SettingsPlugin() = default;

private:
	QPushButton* mPluginButton;
	std::unique_ptr<QDialog> mPluginDialog;
	std::unique_ptr<SettingsView> mPluginView;
};