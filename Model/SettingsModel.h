#pragma once

#include "model_global.h"

#include <QObject>

class DbManager;

class MODEL_EXPORT SettingsModel  : public QObject
{
	Q_OBJECT

public:
	SettingsModel(DbManager& db, QObject* parent = Q_NULLPTR);
	~SettingsModel() = default;

	void loadSettings();
	void saveSettings();

	void setStyle(const QString& styleName);
	QString getStyle() const;

signals:
	void styleChanged(const QString& styleName);

private:
	DbManager& mDb;
	QString mStyle;
};

