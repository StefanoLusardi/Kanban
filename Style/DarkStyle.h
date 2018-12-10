/* 
 * Qt5 Dark Style from:
 * https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle 
 */

#pragma once

#include <QProxyStyle>

class QPalette;
class QApplication;

class DarkStyle : public QProxyStyle
{
	Q_OBJECT

public:
	explicit DarkStyle();

	void polish(QPalette& palette) override;
	void polish(QApplication* app) override;
};

