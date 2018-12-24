#pragma once

#include <QObject>
#include <QDialog>
#include <QPushButton>

class Model;
class QBoxLayout;
class KanbanBoardPluginInterface;
class KanbanBoardView;

class KanbanBoardPlugin : public QObject
{
	Q_OBJECT

public:
	KanbanBoardPlugin(QBoxLayout* mainViewLayout, QBoxLayout* pluginButtonsLayout, Model* model, KanbanBoardPluginInterface* parent);
	~KanbanBoardPlugin() = default;

	void initData() const;
	void release() const;

private:
	KanbanBoardView* mPluginView;
};

