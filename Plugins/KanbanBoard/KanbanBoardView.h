#pragma once

#include <QWidget>
class Model;

namespace Ui { class KanbanBoardView; };

class KanbanBoardView : public QWidget
{
	Q_OBJECT

public:
	KanbanBoardView(Model* model, QWidget *parent = Q_NULLPTR);
	~KanbanBoardView();

	void loadConfig();
	void saveConfig() const;

	void newPage(const QString& pageName);
	void addPage(int pageIdx, const QString& pageName);
	void removePage(const QString& pageName);

private:
	Ui::KanbanBoardView *ui;
	Model* mModel;
};
