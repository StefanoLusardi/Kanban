#include "Model.h"
#include "DbManager.h"

Model::Model() 
{
	mPageModel = std::make_shared<PageItemModel>(DbManager::instance());

	mPageModel->loadPageItems();
	for (auto&& page : mPageModel->pages())
	{
		mKanbanModel.emplace_back(std::make_shared<KanbanItemModel>(DbManager::instance(), page.getPageIdx()));
	}
}

void Model::loadData() const
{
	mPageModel->loadPageItems();
	for (auto&& page : mPageModel->pages())
	{
		mKanbanModel.at(page.getPageIdx())->loadKanbanItems();
	}
}

void Model::saveData() const
{
	mPageModel->savePageItems();
	
	for (auto&& page : mPageModel->pages())
	{
		mKanbanModel.at(page.getPageIdx())->saveKanbanItems();
	}
}

int Model::insertPage(const QString& pageName)
{
	const auto pageIdx = mPageModel->insertPage(pageName);
	mKanbanModel.emplace_back(std::make_shared<KanbanItemModel>(DbManager::instance(), pageIdx));
	return pageIdx;
}

void Model::removePage(const QString& pageName)
{
	const auto pageIdx = mPageModel->getPageIndex(pageName);
	mKanbanModel.at(pageIdx)->removeAllItems();
	mPageModel->removePage(pageName);
}