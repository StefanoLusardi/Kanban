#pragma once

#include "model_global.h"

#include "KanbanItemModel.h"
#include "PageItemModel.h"
#include "SettingsModel.h"

class MODEL_EXPORT Model
{
public:
	explicit Model();

	std::shared_ptr<KanbanItemModel> getKanbanModel(int page) const { return mKanbanModel.at(page); }
	std::shared_ptr<PageItemModel> getPageModel() const { return mPageModel; }
	std::shared_ptr<SettingsModel> getSettingsModel() const { return mSettingsModel; }

	void loadData() const;
	void saveData() const;

	int insertPage(const QString& pageName);
	void removePage(const QString& pageName);

//signals:
	//void ss(const QString& ss);

private:
	std::vector<std::shared_ptr<KanbanItemModel>> mKanbanModel;
	std::shared_ptr<PageItemModel> mPageModel;
	std::shared_ptr<SettingsModel> mSettingsModel;
};
