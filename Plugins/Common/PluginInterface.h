#pragma once

#include <QObject>

class Model;
class QBoxLayout;

class PluginInterface
{
public:
	virtual ~PluginInterface() = default;

	virtual QString id() const = 0;
	virtual QString name() const = 0;
	virtual QString description() const = 0;
	virtual QList<QString> dependencies() const = 0;
	virtual int priority() const = 0;

    virtual void setup(QBoxLayout* mainViewLayout, QBoxLayout* pluginButtonsLayout, Model* model) = 0;
    virtual void initData() = 0;
	virtual void release() = 0;
};

QT_BEGIN_NAMESPACE

#define PluginInterface_iid "kanban.plugin.id/0.1"

Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

QT_END_NAMESPACE

