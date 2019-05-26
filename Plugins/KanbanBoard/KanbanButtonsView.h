#pragma once

#include <QWidget>

namespace Ui { class KanbanButtonsView; }

class KanbanButtonsView : public QWidget
{
	Q_OBJECT

public:
	KanbanButtonsView(QWidget *parent = Q_NULLPTR);
	~KanbanButtonsView();

	void setCurrentPage(const QString& pageName);

signals:
	void createKanbanSignal(const QString& currentPageName);
	void renameKanbanSignal(const QString& currentPageName);
	void deleteKanbanSignal(const QString& currentPageName);
	void createColumnSignal(const QString& currentPageName);
	void renameColumnSignal(const QString& currentPageName);
	void deleteColumnSignal(const QString& currentPageName);

private:
	Ui::KanbanButtonsView *ui;
	QString mCurrentPageName;
};
