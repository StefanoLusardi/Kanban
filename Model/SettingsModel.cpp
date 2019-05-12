#include "SettingsModel.h"
#include "DbManager.h"

#include <QVariant>
#include <QtConcurrent/QtConcurrent>

SettingsModel::SettingsModel(DbManager& db, QObject* parent) : QObject{parent}, mDb{db}
{
}

void SettingsModel::loadSettings()
{
	const auto settings = mDb.mManagerSettings.getItems(-1);
	if (!settings.empty())
	{
		setStyle(settings.at(0).getStyleName());
	}
	else
	{
		const QString defaultStyle { "Dark" };
		SettingsItem defaultSettings {defaultStyle, 1};
		mDb.mManagerSettings.insertItem(defaultSettings);
	}
}

void SettingsModel::saveSettings()
{
	mDb.mManagerSettings.saveAllItems();
}

void SettingsModel::setStyle(const QString& styleName)
{
	mStyle = styleName;
	emit styleChanged(styleName);
	QtConcurrent::run([this]{mDb.mManagerSettings.setData(1, "style", QVariant(mStyle));});
}

QString SettingsModel::getStyle() const
{
	return mStyle;
}
