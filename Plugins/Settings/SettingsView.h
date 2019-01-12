#pragma once

#include <QWidget>

class Model;
class KanbanItemModel;

namespace Ui { class SettingsView; }

class SettingsView : public QWidget
{
	Q_OBJECT

public:
	SettingsView(Model* model, QWidget *parent = Q_NULLPTR);
	~SettingsView();

	void loadConfig();
	void saveConfig() const;

protected slots:
	void setAppStyle(const QString& style) const;

private:
	Ui::SettingsView* ui;
	KanbanItemModel* mKanbanModel;
	//Model* mModel;
};
