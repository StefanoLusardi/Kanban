#pragma once

#include <vector>
#include <memory>

class QSqlDatabase;

class PageItemManager
{
public:
    explicit PageItemManager(QSqlDatabase& db);
    void init() const;

    //void addKanban(KanbanItem& kanbanItem) const;
    //void removeKanbanItem(int id) const;
    //void removeKanbanItemsForPage(int pageId) const;
    //std::vector<KanbanItem> getKanbanItemsForPage(int pageId) const;

private:
    QSqlDatabase& mDb;
};