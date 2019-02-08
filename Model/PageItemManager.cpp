#include "PageItemManager.h"
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
        + "page_idx INTEGER, "
        + "name TEXT "
		+ ")");

        DbManager::debugQuery(query);
    }
}

void PageItemManager::insertItem(PageItem& item) const
{
	QSqlQuery query(mDb);
    query.prepare(QString("INSERT INTO page_items")
        + " (page_idx, name)"
        + " VALUES ("
        + ":page_idx, "
        + ":name"
        + ")");
    query.bindValue(":page_idx", item.getPageIdx());
    query.bindValue(":name", item.getPageName());
    query.exec();

    DbManager::debugQuery(query);
    item.setId(query.lastInsertId().toInt());
}

void PageItemManager::removeItem(int id) const
{
	QSqlQuery query(mDb);
    query.prepare("DELETE FROM page_items WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();

    DbManager::debugQuery(query);
}

std::vector<PageItem> PageItemManager::getItems(int /*id*/) const
{
	QSqlQuery query(mDb);
    query.prepare("SELECT * FROM page_items");
    query.exec();

    DbManager::debugQuery(query);

	std::vector<PageItem> pageItems;
    while(query.next()) 
	{
		const auto id { query.value("id").toInt() };
		const auto pageIdx { query.value("page_idx").toInt() };
		const auto pageName { query.value("name").toString()};
		
        pageItems.emplace_back(pageIdx, pageName, id);
    }

    return pageItems;
}

void PageItemManager::removeAllItems(int /*pageId*/) const
{
	// TODO: implement
}

void PageItemManager::saveAllItems() const
{

}
