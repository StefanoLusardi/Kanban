#include "KanbanItemManager.h"
#include "KanbanItem.h"
#include "DbManager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include <QtConcurrent/QtConcurrent>

KanbanItemManager::KanbanItemManager(QSqlDatabase& db): mDb{db}
{
}

void KanbanItemManager::init() const
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

void KanbanItemManager::insertItem(KanbanItem& kanbanItem) const
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
    query.bindValue(":page_id", kanbanItem.getPageIdx());
    query.bindValue(":column", kanbanItem.getColumn());
    query.bindValue(":color", kanbanItem.getColor());
    query.bindValue(":text", kanbanItem.getText());

	query.exec();

	DbManager::debugQuery(query);
	kanbanItem.setId(query.lastInsertId().toInt());
}

void KanbanItemManager::removeItem(int id) const
{
	QSqlQuery query(mDb);
    query.prepare("DELETE FROM kanban_items WHERE id = (:id)");
    query.bindValue(":id", id);

	query.exec();
	DbManager::debugQuery(query);
}

void KanbanItemManager::removeAllItems(int pageId) const
{
    QSqlQuery query(mDb);
    query.prepare("DELETE FROM kanban_items WHERE page_id = (:page_id)");
    query.bindValue(":page_id", pageId);

	query.exec();
	DbManager::debugQuery(query);
}

void KanbanItemManager::saveAllItems() const
{
}

void KanbanItemManager::setData(int id, const char* property, const QVariant& value) const
{
	QSqlQuery query(mDb);
    query.prepare("UPDATE kanban_items "
		"SET " + QString(property) + " = (:value) " +
		"WHERE id = (:id)");
    query.bindValue(":value", value);
    query.bindValue(":id", id);

	query.exec();
	DbManager::debugQuery(query);
}

std::vector<KanbanItem> KanbanItemManager::getItems(int pageId) const
{
    QSqlQuery query(mDb);
    query.prepare("SELECT * FROM kanban_items WHERE page_id = (:page_id)");
    query.bindValue(":page_id", pageId);
	query.exec();

    DbManager::debugQuery(query);

	std::vector<KanbanItem> kanbanItems;
    while(query.next()) 
	{
		const auto id { query.value("id").toInt() };
		const auto text { query.value("text").toString()};
		const auto color { query.value("color").toString()};
		const auto column { query.value("column").toString()};

        kanbanItems.emplace_back(pageId, text, color, column, id);
    }
	return kanbanItems;
}
