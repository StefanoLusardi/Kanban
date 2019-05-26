#pragma once

#include "model_global.h"

#include <QString>

class MODEL_EXPORT KanbanItem
{
public:
	KanbanItem(int pageIdx, const QString& text, const QString& color, const QString& column, int id=-1) 
	: mId{id}, 
	mPageIdx{pageIdx},
	mText{text}, 
	mColor{color},
	mColumn{column}
	{ }

	bool operator==(const KanbanItem& item) const
	{
		return mPageIdx == item.mPageIdx
		&& mText == item.mText
		&& mColor == item.mColor
		&& mColumn == item.mColumn;
	}

	int getId() const { return mId; }
	int getPageIdx() const { return mPageIdx; }
	QString getText() const { return mText; }
	QString getColor() const { return mColor; }
	QString getColumn() const { return mColumn; }

	void setId(const int id) { mId = id; }
	void setPageIdx(int pageIdx) { mPageIdx = pageIdx; }
	void setText(const QString& text) { mText = text; }
	void setColor(const QString& color) { mColor = color; }
	void setColumn(const QString& column) { mColumn = column; }

private:
	int mId;
	int mPageIdx;
	QString mText;
	QString mColor;
	QString mColumn;
};
