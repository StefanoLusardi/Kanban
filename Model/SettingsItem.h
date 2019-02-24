#pragma once

#include "model_global.h"

#include <QString>

class MODEL_EXPORT SettingsItem
{
public:
	explicit SettingsItem(const QString& styleName, int id=-1) 
	: mId{id}, 
	mStyleName{styleName}
	{ }

	int getId() const { return mId; }
	void setId(int id) { mId = id; }

	QString getStyleName() const { return mStyleName; }
	void setStyleName(const QString& styleName) { mStyleName = styleName; }

private:
	int mId;
	QString mStyleName;
};
