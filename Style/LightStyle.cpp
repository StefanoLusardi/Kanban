#include <QStyleFactory>
#include <QApplication>
#include <QPalette>
#include <QFont>
#include <QFile>

#include "LightStyle.h"



LightStyle::LightStyle() : QProxyStyle(QStyleFactory::create(QStringLiteral("Fusion")))
{
}

void LightStyle::polish(QPalette& palette)
{
	// modify palette to light
	palette.setColor(QPalette::Window, QColor(245, 245, 245));
	palette.setColor(QPalette::WindowText, Qt::black);
	palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
	palette.setColor(QPalette::Base, QColor(235, 235, 235));
	palette.setColor(QPalette::AlternateBase, QColor(160, 160, 160));
	palette.setColor(QPalette::ToolTipBase, Qt::black);
	palette.setColor(QPalette::ToolTipText, QColor(110, 110, 110));
	palette.setColor(QPalette::Text, Qt::black);
	palette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
	palette.setColor(QPalette::Dark, QColor(120, 120, 120));
	palette.setColor(QPalette::Shadow, QColor(80, 80, 80));
	palette.setColor(QPalette::Button, QColor(200, 200, 200));
	palette.setColor(QPalette::ButtonText, Qt::black);
	palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
	palette.setColor(QPalette::BrightText, Qt::red);
	palette.setColor(QPalette::Link, QColor(42, 130, 218));
	palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(127, 127, 127));
	palette.setColor(QPalette::HighlightedText, Qt::black);
	palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));
}

void LightStyle::polish(QApplication* app)
{
	if (!app) return;

	// increase font size for better reading,
	// setPointSize was reduced from +2 because when applied this way in Qt5, the
	// font is larger than intended for some reason
	//QFont defaultFont = QApplication::font();
	//defaultFont.setPointSize(defaultFont.pointSize() + 1);
	//app->setFont(defaultFont);

	// loadstylesheet
	QFile qfDarkstyle(QStringLiteral(":/darkstyle/darkstyle.qss"));
	if (qfDarkstyle.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		// set stylesheet
		QString qsStylesheet = QString::fromLatin1(qfDarkstyle.readAll());
		app->setStyleSheet(qsStylesheet);
		qfDarkstyle.close();
	}
}
