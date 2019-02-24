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
	palette.setColor(QPalette::Text, QColor(30, 30, 30));
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

	QFont defaultFont = QApplication::font();
	defaultFont.setPointSize(10);
	app->setFont(defaultFont);

	QFile styleFile(QStringLiteral(":/darkstyle/darkstyle.qss"));
	if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		const QString styleSheet = QString::fromLatin1(styleFile.readAll());
		app->setStyleSheet(styleSheet);
		styleFile.close();
	}
}
