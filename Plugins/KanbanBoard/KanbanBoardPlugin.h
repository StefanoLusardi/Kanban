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
	KanbanBoardPlugin(QBoxLayout* container, Model* model, KanbanBoardPluginInterface *parent);
	~KanbanBoardPlugin() = default;

private:
	std::unique_ptr<QDialog> mPluginDialog;
	std::unique_ptr<QPushButton> mPluginButton;
	std::unique_ptr<KanbanBoardView> mPluginView;
};

