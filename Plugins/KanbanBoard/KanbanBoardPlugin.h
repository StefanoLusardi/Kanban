#pragma once

#include <QObject>

class QBoxLayout;
class QPushButton;

class Model;
class KanbanBoardView;
class KanbanBoardPluginInterface;

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
	QPushButton* mAddPageButton;
};

