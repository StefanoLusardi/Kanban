#pragma once

#include "model_global.h"

#include <QString>

class MODEL_EXPORT PageItem
{
public:
	explicit PageItem(int pageIdx, const QString& pageName, int id=-1) 
	: mId{id}, 
	mPageIdx{pageIdx}, 
	mPageName{pageName}
	{ }

	int getId() const { return mId; }
	int getPageIdx() const { return mPageIdx; }
	QString getPageName() const { return mPageName; }

	void setId(int id) { mId = id; }
	void setPageIdx(int pageIdx) { mPageIdx = pageIdx; }
	void setPageName(const QString& pageName) { mPageName = pageName; }

private:
	int mId;
	int mPageIdx;
	QString mPageName;
};
