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

void KanbanItemDbHandler::addKanbanToPage(int pageId, KanbanItem& kanbanItem) const
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
    query.bindValue(":page_id", pageId);
    query.bindValue(":column", kanbanItem.getState());
    query.bindValue(":color", kanbanItem.getColor());
    query.bindValue(":text", kanbanItem.getText());
    query.exec();
    DbManager::debugQuery(query);
    kanbanItem.setId(query.lastInsertId().toInt());
    kanbanItem.setPageId(pageId);
}

void KanbanItemDbHandler::removeKanban(int id) const
{
    QSqlQuery query(mDb);
    query.prepare("DELETE FROM kanban_items WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();
    DbManager::debugQuery(query);
}

void KanbanItemDbHandler::removeKanbanForPage(int pageId) const
{
    QSqlQuery query(mDb);
    query.prepare("DELETE FROM kanban_items WHERE page_id = (:page_id)");
    query.bindValue(":page_id", pageId);
    query.exec();
    DbManager::debugQuery(query);
}

//std::unique_ptr<std::vector<std::unique_ptr<KanbanItem>>> KanbanItemDbHandler::getKanbanItemsForPage(int pageId) const
std::vector<KanbanItem> KanbanItemDbHandler::getKanbanItemsForPage(int pageId) const
{
    QSqlQuery query(mDb);
    query.prepare("SELECT * FROM kanban_items WHERE page_id = (:page_id)");
    query.bindValue(":page_id", pageId);
    query.exec();
    DbManager::debugQuery(query);

	//std::unique_ptr<std::vector<std::unique_ptr<KanbanItem>>> list(new std::vector<std::unique_ptr<KanbanItem>>());
	std::vector<KanbanItem> list;
    while(query.next()) 
	{
		//std::unique_ptr<KanbanItem> picture(new KanbanItem());
		KanbanItem item;
        item.setId(query.value("id").toInt());
        item.setPageId(query.value("page_id").toInt());
        item.setState(query.value("column").toString());
        item.setColor(query.value("color").toString());
        item.setText(query.value("text").toString());

        //list.push_back(std::move(item));
        list.push_back(item);
    }
    return list;
}
