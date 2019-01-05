#include "Model.h"
#include "DbManager.h"

Model::Model() 
{
	mPageModel = std::make_shared<PageModel>(DbManager::instance());

	for (auto&& [pageIdx, pageName] : mPageModel->pages())
	{
		mKanbanModel.emplace_back(std::make_shared<KanbanModel>(DbManager::instance(), pageIdx));
	}
}

void Model::loadData() const
{
	for (auto&& [pageIdx, pageName] : mPageModel->pages())
	{
		mKanbanModel.at(pageIdx)->loadKanbanItems();
	}
}

void Model::saveData() const
{
	//mPageModel->savePages();
	
	for (auto&& [pageIdx, pageName] : mPageModel->pages())
	{
		mKanbanModel.at(pageIdx)->saveKanbanItems();
	}
}

int Model::addPage(const QString& pageName)
{
	auto idx = mPageModel->pages().size();
	mPageModel->pages()[idx] = pageName;

	mKanbanModel.emplace_back(std::make_shared<KanbanModel>(DbManager::instance(), idx));

	return static_cast<int>(idx);
}
