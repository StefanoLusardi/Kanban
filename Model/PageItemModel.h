#pragma once

#include "model_global.h"
#include "PageItem.h"

#include <vector>
#include <optional>
//#include <QAbstractItemModel>

class DbManager;

class MODEL_EXPORT PageItemModel //: public QAbstractItemModel
{
public:
	PageItemModel(DbManager& db);
	virtual ~PageItemModel() = default;

	void loadPageItems();
	void savePageItems();

	std::vector<PageItem> pages() const;
	int insertPage(const QString& pageName, std::optional<int> pageIdx = {});
	void removePage(const QString& pageName);
	
	int getPageIndex(const QString& pageName) const;

private:
	DbManager& mDb;
	std::vector<PageItem> mPageItems;
};

