#include "DbManager.h"

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>


void DbManager::debugQuery(const QSqlQuery& query)
{
	if (query.lastError().type() == QSqlError::ErrorType::NoError) 
	{
		qDebug() << "Query OK:"  << query.lastQuery();
	} 
	else
	{
		qWarning() << "Query KO:" << query.lastError().text();
		qWarning() << "Query text:" << query.lastQuery();
	}
}

DbManager& DbManager::instance()
{
    static DbManager dbManager;
    return dbManager;
}

DbManager::DbManager(const QString& path) :
    mDb(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"))),
    mManagerKanbanItem(*mDb), 
	mManagerPageItem(*mDb),
	mManagerSettings(*mDb)
{
    mDb->setDatabaseName(path);
	mDb->open();

    mManagerKanbanItem.init();
    mManagerPageItem.init();
    mManagerSettings.init();
}

DbManager::~DbManager()
{
    mDb->close();
}

