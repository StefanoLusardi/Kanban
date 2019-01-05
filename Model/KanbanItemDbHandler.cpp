#include "KanbanItemDbHandler.h"
#include "KanbanItem.h"
#include "DbManager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>


KanbanItemDbHandler::KanbanItemDbHandler(QSqlDatabase& db) : mDb(db)
{
}

void KanbanItemDbHandler::init() const
{
    if (!mDb.tables().contains("kanban_items")) 
	{
        QSqlQuery query(mDb);
        query.exec(QString("CREATE TABLE kanban_items")
        + " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        + "page_id INTEGER, "
        + "column TEXT, "
        + "color TEXT, "
        + "text TEXT"
		+ ")");

        DbManager::debugQuery(query);
    }
}

void KanbanItemDbHandler::addKanban(KanbanItem& kanbanItem) const
{
    QSqlQuery query(mDb);
    query.prepare(QString("INSERT INTO kanban_items")
        + " (page_id, column, color, text)"
        + " VALUES ("
        + ":page_id, "
        + ":column, "
        + ":color, "
        + ":text"
        + ")");
    query.bindValue(":page_id", kanbanItem.getPageId());
    query.bindValue(":column", kanbanItem.getColumn());
    query.bindValue(":color", kanbanItem.getColor());
    query.bindValue(":text", kanbanItem.getText());
    query.exec();

    DbManager::debugQuery(query);
    kanbanItem.setId(query.lastInsertId().toInt());
}

void KanbanItemDbHandler::removeKanbanItem(int id) const
{
    QSqlQuery query(mDb);
    query.prepare("DELETE FROM kanban_items WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();

    DbManager::debugQuery(query);
}

void KanbanItemDbHandler::removeKanbanItemsForPage(int pageId) const
{
    QSqlQuery query(mDb);
    query.prepare("DELETE FROM kanban_items WHERE page_id = (:page_id)");
    query.bindValue(":page_id", pageId);
    query.exec();

    DbManager::debugQuery(query);
}

std::vector<KanbanItem> KanbanItemDbHandler::getKanbanItemsForPage(int pageId) const
{
    QSqlQuery query(mDb);
    query.prepare("SELECT * FROM kanban_items WHERE page_id = (:page_id)");
    query.bindValue(":page_id", pageId);
    query.exec();

    DbManager::debugQuery(query);

	std::vector<KanbanItem> kanbanItems;
    while(query.next()) 
	{
		KanbanItem item(pageId);
        item.setId(query.value("id").toInt());
        item.setPageId(query.value("page_id").toInt());
        item.setColumn(query.value("column").toString());
        item.setColor(query.value("color").toString());
        item.setText(query.value("text").toString());

        kanbanItems.push_back(item);
    }
    return kanbanItems;
}
