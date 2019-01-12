#pragma once

#include "ItemManager.h"

#include <vector>

class QSqlDatabase;
class KanbanItem;

class KanbanItemManager : public ItemManager<KanbanItem>
{
public:
	explicit KanbanItemManager(QSqlDatabase& db);

	void init() const override;
    void insertItem(KanbanItem& kanbanItem) const override;
    void removeItem(int id) const override;
	std::vector<KanbanItem> getItems(int pageId) const override;
	void removeAllItems(int pageId) const override;

private:
    QSqlDatabase& mDb;
};