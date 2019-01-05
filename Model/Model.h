#pragma once

#include "model_global.h"

#include "KanbanModel.h"
#include "PageModel.h"

class MODEL_EXPORT Model
{
public:
	explicit Model();

	std::shared_ptr<KanbanModel> getKanbanModel(int page) const { return mKanbanModel.at(page); }
	std::shared_ptr<PageModel> getPageModel() const { return mPageModel; }

	void loadData() const;
	void saveData() const;

	int addPage(const QString& pageName);

private:
	std::vector<std::shared_ptr<KanbanModel>> mKanbanModel;
	std::shared_ptr<PageModel> mPageModel;
};
