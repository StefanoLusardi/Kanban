#pragma once

#include <QWidget>

class Model;
class SettingsModel;
//class KanbanItemModel;

namespace Ui { class SettingsView; }

class SettingsView : public QWidget
{
	Q_OBJECT
	const QString mConfingFile {"Settings.json"};

public:
	SettingsView(Model* model, QWidget *parent = Q_NULLPTR);
	~SettingsView();

	void loadConfig();
	void saveConfig() const;

protected slots:
	void setAppStyle(const QString& style) const;

private:
	Ui::SettingsView* ui;
	SettingsModel* mModel;
};
