#include "PageItemDbHandler.h"

#include "KanbanItemDbHandler.h"
#include "KanbanItem.h"
#include "DbManager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

PageItemDbHandler::PageItemDbHandler(QSqlDatabase & db) : mDb(db)
{
}

void PageItemDbHandler::init() const
{
    if (!mDb.tables().contains("page_items")) 
	{
        QSqlQuery query(mDb);
        query.exec(QString("CREATE TABLE page_items")
        + " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        + "page_id INTEGER, "
        + "name TEXT "
		+ ")");

        DbManager::debugQuery(query);
    }
}
