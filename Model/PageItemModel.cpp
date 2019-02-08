#include "PageItemModel.h"
#include "DbManager.h"

PageItemModel::PageItemModel(DbManager& db) : mDb{db}
{
	
}

void PageItemModel::loadPageItems()
{
	mPageItems = mDb.mManagerPageItem.getItems(-1);
}

void PageItemModel::savePageItems()
{
	mDb.mManagerPageItem.saveAllItems();
}

std::vector<PageItem> PageItemModel::pages() const
{
	return mPageItems;
}

int PageItemModel::insertPage(const QString& pageName, std::optional<int> pageIdx)
{
	// In case pageIdx is not given, insert after last element
	const auto idx = pageIdx.value_or(mPageItems.size());
	PageItem item(idx, pageName);
	mDb.mManagerPageItem.insertItem(item);
	mPageItems.emplace_back(item);

	return idx;
}

void PageItemModel::removePage(const QString& pageName)
{
	const auto item = std::find_if(mPageItems.begin(), mPageItems.end(), 
		[pageName](const auto& it){return it.getPageName() == pageName;});

	mDb.mManagerPageItem.removeItem(item->getId());
	mPageItems.erase(item);
}

int PageItemModel::getPageIndex(const QString& pageName) const
{
	const auto item = std::find_if(mPageItems.begin(), mPageItems.end(), 
		[pageName](const auto& it){return it.getPageName() == pageName;});
	
	return item->getPageIdx();
}
