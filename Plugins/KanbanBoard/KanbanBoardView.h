#pragma once

#include <QWidget>
#include "KanbanPageView.h"
class Model;
class QBoxLayout;
class KanbanButtonsView;

namespace Ui { class KanbanBoardView; };

class KanbanBoardView : public QWidget
{
	Q_OBJECT
	const QString mConfigFile {"Kanban Board.json"};

public:
	KanbanBoardView(Model* model,  QBoxLayout* pluginButtonsLayout,  QWidget *parent = Q_NULLPTR);
	~KanbanBoardView();

	void loadConfig();
	void saveConfig() const;

	void insertPage(const QString& pageName);
	void removePage(const QString& pageName) const;

private:
	Ui::KanbanBoardView *ui;
	KanbanButtonsView* mButtonsUi;
	Model* mModel;
	
	void setupPageConnections(KanbanPageView* page, bool connecting = true) const;
};
