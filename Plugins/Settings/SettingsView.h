#pragma once

#include <QWidget>

class Model;
class KanbanModel;

namespace Ui { class SettingsView; }

class SettingsView : public QWidget
{
	Q_OBJECT

public:
	SettingsView(QWidget *parent = Q_NULLPTR);
	~SettingsView();
	void setModel(KanbanModel* kanbanModel);
	void setModel(Model* model);

	void loadConfig();
	void saveConfig() const;

protected slots:
	void setAppStyle(const QString& style) const;

private:
	Ui::SettingsView* ui;
	KanbanModel* mKanbanModel;
	Model* mModel;
};
