#pragma once

#include "model_global.h"

#include <QString>

class MODEL_EXPORT KanbanItem
{
public:
	KanbanItem(int pageId, const QString& text = QString(), const QString& color = QString("#FFFFFFFF"), const QString& column = QString()) 
	: mId{-1}, 
	mPageId{pageId},
	mText{text}, 
	mColor{color},
	mColumn{column}
	{ }

	int getId() const { return mId; }
	int getPageId() const { return mPageId; }
	QString getText() const { return mText; }
	QString getColor() const { return mColor; }
	QString getColumn() const { return mColumn; }

	void setId(const int id) { mId = id; }
	void setPageId(int pageId) { mPageId = pageId; }
	void setText(const QString& text) { mText = text; }
	void setColor(const QString& color) { mColor = color; }
	void setColumn(const QString& column) { mColumn = column; }

private:
	int mId;
	int mPageId;
	QString mText;
	QString mColor;
	QString mColumn;
};
