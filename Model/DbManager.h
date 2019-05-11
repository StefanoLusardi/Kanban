#pragma once

#include "KanbanItemManager.h"
#include "PageItemManager.h"
#include "SettingsManager.h"

class QThreadPool;
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
	QThreadPool* mThreadPool;

public:
    const KanbanItemManager mManagerKanbanItem;
    const PageItemManager mManagerPageItem;
    const SettingsManager mManagerSettings;
};