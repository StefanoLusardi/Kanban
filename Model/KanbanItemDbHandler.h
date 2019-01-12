#pragma once

#include <vector>

class QSqlDatabase;
class KanbanItem;

//#include "KanbanItem.h"

template <typename Item>
class DbHandler
{
public:
	virtual ~DbHandler() = default;

protected:
	virtual void init() const = 0;
    virtual void insertItem(Item& item) const = 0;
    virtual void removeItem(int id) const = 0;
};

class KanbanItemManager : public DbHandler<KanbanItem>
{
public:
    explicit KanbanItemManager(QSqlDatabase& db) : mDb{db} {}

protected:
	void init() const override {}
    void insertItem(KanbanItem& kanbanItem) const override {}
    void removeItem(int id) const override {}

    void removeKanbanItemsForPage(int pageId) const {}
    std::vector<KanbanItem> getKanbanItemsForPage(int pageId) const { return{}; }

private:
    QSqlDatabase& mDb;
};

class KanbanItemManager
{
public:
    explicit KanbanItemManager(QSqlDatabase& db);
    void init() const;

    void addKanban(KanbanItem& kanbanItem) const;
    void removeKanbanItem(int id) const;
    void removeKanbanItemsForPage(int pageId) const;
    std::vector<KanbanItem> getKanbanItemsForPage(int pageId) const;

private:
    QSqlDatabase& mDb;
};