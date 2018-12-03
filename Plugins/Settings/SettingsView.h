#pragma once

#include <QWidget>

class KanbanModel;

namespace Ui { class SettingsView; }

class SettingsView : public QWidget
{
	Q_OBJECT

public:
	SettingsView(QWidget *parent = Q_NULLPTR);
	~SettingsView();

	void setModel(KanbanModel* kanbanModel);

private:
	Ui::SettingsView *ui;
	KanbanModel* mKanbanModel;
};
