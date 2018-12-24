#pragma once

#include "model_global.h"

#include <QString>
//#include <QColor>

/* TODO: note that in order to include QColor we need to link against Qt5Gui.
*  Think if there is a better way to avoid to include it and use only Qt5Core.
*  Maybe define a custom simple Color class and use some conversion functions to/from QColor.
*/

class MODEL_EXPORT KanbanItem
{
public:
	KanbanItem(const QString& text = QString(), const QString& color = QString("#FFFFFFFF"), const QString& state = QString()) 
	: mId{-1}, 
	mText{text}, 
	mColor{color},
	mState{state}
	{ }

	int getId() const { return mId; }
	int getPageId() const { return mPageId; }
	QString getText() const { return mText; }
	QString getColor() const { return mColor; }
	QString getState() const { return mState; }

	void setId(const int id) { mId = id; }
	void setPageId(int pageId) { mPageId = pageId; }
	void setText(const QString& text) { mText = text; }
	void setColor(const QString& color) { mColor = color; }
	void setState(const QString& state) { mState = state; }

private:
	int mId;
	int mPageId;
	QString mText;
	QString mColor;
	QString mState;
};
