#pragma once

#include "model_global.h"

#include "KanbanModel.h"

class MODEL_EXPORT Model
{
public:
	Model();

	std::shared_ptr<KanbanModel> getKanbanModel() const { return mKanbanModel; }

	void loadData();

private:
	std::shared_ptr<KanbanModel> mKanbanModel;
};
