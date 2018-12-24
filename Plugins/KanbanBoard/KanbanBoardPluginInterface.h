#pragma once

#include "../Common/PluginInterface.h"
#include "KanbanBoardPlugin.h"

#include <QObject>
#include <QBoxLayout>

class KanbanBoardPluginInterface : public QObject, PluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID PluginInterface_iid)
	Q_INTERFACES(PluginInterface)

public:
	QString id() const  override { return "KANBAN_BOARD"; }
    QString name() const  override { return "Kanban Board"; }
	QString description() const  override { return "A Kanban Board act as a canvas to create, display and modify kanban items."; }
	QList<QString> dependencies() const  override { return {}; }
	int priority() const override { return 0; }
		
	void setup(QBoxLayout* mainViewLayout, QBoxLayout* pluginButtonsLayout, Model* model) override
	{
        mPlugin = std::make_unique<KanbanBoardPlugin>(mainViewLayout, pluginButtonsLayout, model, this);
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
    std::unique_ptr<KanbanBoardPlugin> mPlugin;
};
