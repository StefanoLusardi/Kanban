#pragma once
#include <QString>
#include "KanbanItemDbHandler.h"
#include "PageItemDbHandler.h"

class QSqlQuery;
class QSqlDatabase;

const QString DATABASE_FILENAME { "kanban.db" };

class DbManager
{
public:
    static void debugQuery(const QSqlQuery& query);
    DbManager& operator=(const DbManager& rhs) = delete;

    static DbManager& instance();
    ~DbManager();

protected:
    DbManager(const QString& path = DATABASE_FILENAME);

private:
    std::unique_ptr<QSqlDatabase> mDb;

public:
    const KanbanItemDbHandler kanbanItemDbHandler;
    const PageItemDbHandler pageItemDbHandler;
};