#pragma once

#include "ItemManager.h"
#include "PageItem.h"
#include "KanbanItem.h"
#include <vector>

class QSqlDatabase;

class PageItemManager : public ItemManager<PageItem>
{
public:
    explicit PageItemManager(QSqlDatabase& db);
    void init() const override;
	void insertItem(PageItem& item) const override;
	void removeItem(int id) const override;
	std::vector<PageItem> getItems(int id) const override;
	void removeAllItems(int pageId) const override;
	void saveAllItems(std::vector<PageItem> kanbanItems) const override;

private:
    QSqlDatabase& mDb;
};