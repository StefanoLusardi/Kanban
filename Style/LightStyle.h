#pragma once

#include <QProxyStyle>

class QPalette;
class QApplication;

class LightStyle :	public QProxyStyle
{
	Q_OBJECT

public:
	explicit LightStyle();
	
	void polish(QPalette& palette) override;
	void polish(QApplication* app) override;
};
