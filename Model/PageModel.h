#pragma once

#include "model_global.h"
#include <map>
#include <QString>

class DbManager;

class MODEL_EXPORT PageItemModel
{
public:
	PageItemModel(DbManager& db);
	virtual ~PageItemModel() = default;

	std::map<int, QString> pages() const
	{
		return mPages;
	}

private:
	DbManager& mDb;
	std::map<int, QString> mPages;
};

