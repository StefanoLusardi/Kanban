#pragma once

#include <QtUiPlugin/QtUiPlugin>
#include "DarkStyle.h"

class MyStylePlugin : public QStylePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QStyleFactoryInterface" FILE "styleplugin.json")

public:
	QStyle *create(const QString &key) override
	{
		const auto styleKey = key.toLower();
		if (styleKey == "dark") 
		{
			return new DarkStyle;
		}    	
    	if (styleKey == "light") 
		{
			return QStyleFactory::create("fusion");
		}

		return nullptr;
	}
};
