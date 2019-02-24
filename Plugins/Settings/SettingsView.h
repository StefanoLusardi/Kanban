#pragma once

#include <QWidget>

class Model;
class SettingsModel;

namespace Ui { class SettingsView; }

class SettingsView : public QWidget
{
	Q_OBJECT
	const QString mConfigFile {"Settings.json"};

public:
	SettingsView(Model* model, QWidget *parent = Q_NULLPTR);
	~SettingsView();

	void loadConfig();
	void saveConfig() const;
	
protected slots:
	void setAppStyle(const QString& style);

private:
	Ui::SettingsView* ui;
	SettingsModel* mModel;
};
