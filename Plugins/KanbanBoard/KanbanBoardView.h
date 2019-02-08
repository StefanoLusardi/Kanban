#pragma once

#include <QWidget>
class Model;

namespace Ui { class KanbanBoardView; };

class KanbanBoardView : public QWidget
{
	Q_OBJECT
	const QString mConfingFile {"Kanban Board.json"};

public:
	KanbanBoardView(Model* model, QWidget *parent = Q_NULLPTR);
	~KanbanBoardView();

	void loadConfig();
	void saveConfig() const;

	void insertPage(const QString& pageName);
	void removePage(const QString& pageName) const;

private:
	Ui::KanbanBoardView *ui;
	Model* mModel;
};
