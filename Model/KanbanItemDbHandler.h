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

    void addKanban(KanbanItem& kanbanItem) const;
    void removeKanbanItem(int id) const;
    void removeKanbanItemsForPage(int pageId) const;
    std::vector<KanbanItem> getKanbanItemsForPage(int pageId) const;

private:
    QSqlDatabase& mDb;
};