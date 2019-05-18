#pragma once

#include <QWidget>

namespace Ui { class KanbanButtonsView; }

class KanbanButtonsView : public QWidget
{
	Q_OBJECT

public:
	KanbanButtonsView(QWidget *parent = Q_NULLPTR);
	~KanbanButtonsView();

signals:
	void createKanbanSignal();
	void renameKanbanSignal();
	void deleteKanbanSignal();
	void createColumnSignal();
	void renameColumnSignal();
	void deleteColumnSignal();

private:
	Ui::KanbanButtonsView *ui;
};
