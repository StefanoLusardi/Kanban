#pragma once

#include "../Common/PluginInterface.h"
#include "SettingsPlugin.h"

#include <QObject>
#include <QBoxLayout>

class SettingsPluginInterface : public QObject, PluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID PluginInterface_iid)
	Q_INTERFACES(PluginInterface)

public:
	QString id() const  override { return "SETTINGS"; }
    QString name() const  override { return "Settings"; }
	QString description() const  override { return "Settings component provides global options for the application."; }
	QList<QString> dependencies() const  override { return {}; }
	int priority() const override { return 0; }
		
	void setup(QBoxLayout* mainViewLayout, QBoxLayout* pluginButtonsLayout, Model* model) override
	{
        mPlugin = std::make_unique<SettingsPlugin>(mainViewLayout, pluginButtonsLayout, model, this);
	}

	void initData() override
	{
		mPlugin->initData();
	}

	void release() override
	{
		mPlugin->release();
	}

protected:
    std::unique_ptr<SettingsPlugin> mPlugin;
};
