#pragma once

#include "model_global.h"

#include "KanbanModel.h"

class MODEL_EXPORT Model
{
public:
	Model();
	//static QString getRandomColor(const QString& mix = QString("#000000"));

	std::shared_ptr<KanbanModel> getKanbanModel() const { return mKanbanModel; }

	void loadData() const;

private:
	std::shared_ptr<KanbanModel> mKanbanModel;
};
