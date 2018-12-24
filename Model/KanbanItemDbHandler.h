#pragma once

#include <vector>
#include <memory>

class QSqlDatabase;
class KanbanItem;

class KanbanItemDbHandler
{
public:
    explicit KanbanItemDbHandler(QSqlDatabase& db);
    void init() const;

    void addKanbanToPage(int pageId, KanbanItem& kanbanItem) const;
    void removeKanban(int id) const;
    void removeKanbanForPage(int pageId) const;
    //std::unique_ptr<std::vector<std::unique_ptr<KanbanItem>>> getKanbanItemsForPage(int pageId) const;
    std::vector<KanbanItem> getKanbanItemsForPage(int pageId) const;

private:
    QSqlDatabase& mDb;
};