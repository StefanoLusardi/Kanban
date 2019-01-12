#include "PageItemManager.h"

#include "KanbanItemManager.h"
#include "KanbanItem.h"
#include "DbManager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

PageItemManager::PageItemManager(QSqlDatabase & db) : mDb(db)
{
}

void PageItemManager::init() const
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
