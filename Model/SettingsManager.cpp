#include "SettingsManager.h"
#include "DbManager.h"

#include <QSqlQuery>
#include <QVariant>

SettingsManager::SettingsManager(QSqlDatabase & db) : mDb(db)
{
}

void SettingsManager::init() const
{
    if (!mDb.tables().contains("settings")) 
	{
        QSqlQuery query(mDb);
        query.exec(QString("CREATE TABLE settings")
        + " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        + "style TEXT"
		+ ")");

        DbManager::debugQuery(query);
    }
}

void SettingsManager::insertItem(SettingsItem& item) const
{
	QSqlQuery query(mDb);
    query.prepare(QString("INSERT INTO settings")
        + " (style)"
        + " VALUES ("
        + ":styleName"
        + ")");
    query.bindValue(":styleName", item.getStyleName());
    query.exec();

    DbManager::debugQuery(query);
}

std::vector<SettingsItem> SettingsManager::getItems(int /*id*/) const
{	
	QSqlQuery query(mDb);
    query.prepare("SELECT * FROM settings");
    query.exec();

    DbManager::debugQuery(query);

	std::vector<SettingsItem> settings;
    while(query.next()) 
	{
		const auto styleName { query.value("style").toString() };
		const auto id { query.value("id").toInt() };
		
        settings.emplace_back(styleName, id);
    }

    return settings;
}

void SettingsManager::setData(int id, const char* property, const QVariant& value) const
{
	QSqlQuery query(mDb);
    query.prepare("UPDATE settings "
		"SET " + QString(property) + " = (:value) " +
		"WHERE id = (:id)");
    query.bindValue(":value", value);
    query.bindValue(":id", id);
	query.exec();

    DbManager::debugQuery(query);
}
