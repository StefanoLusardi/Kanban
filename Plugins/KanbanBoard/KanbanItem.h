#pragma once

#include <QString>
#include <QColor>

class KanbanItem
{
public:
	KanbanItem(const QString& text = QString(), const QColor& color = Qt::cyan, const QString& state = QString()) 
	: mId{-1}, 
	mText{text}, 
	mColor{color},
	mState{state}
	{ }

	int getId() const { return mId; }
	QString getText() const { return mText; }
	QColor getColor() const { return mColor; }
	QString getState() const { return mState; }

	void setId(const int id) { mId = id; }
	void setText(const QString& text) { mText = text; }
	void setColor(const QColor& color) { mColor = color; }
	void setState(const QString& state) { mState = state; }

private:
	int mId;
	QString mText;
	QColor mColor;
	QString mState;
};