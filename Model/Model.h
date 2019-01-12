#pragma once

#include "model_global.h"

#include "KanbanItemModel.h"
#include "PageItemModel.h"

class MODEL_EXPORT Model
{
public:
	explicit Model();

	std::shared_ptr<KanbanItemModel> getKanbanModel(int page) const { return mKanbanModel.at(page); }
	std::shared_ptr<PageItemModel> getPageModel() const { return mPageModel; }

	void loadData() const;
	void saveData() const;

	int insertPage(const QString& pageName);
	void removePage(const QString& pageName);

private:
	std::vector<std::shared_ptr<KanbanItemModel>> mKanbanModel;
	std::shared_ptr<PageItemModel> mPageModel;
};
