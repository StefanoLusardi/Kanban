#include "DarkStyle.h"

#include <QStyleFactory>
#include <QApplication>
#include <QPalette>
#include <QFont>
#include <QFile>

DarkStyle::DarkStyle() : QProxyStyle(QStyleFactory::create(QStringLiteral("Fusion")))
{
}

void DarkStyle::polish(QPalette& palette)
{
	// modify palette to dark
	palette.setColor(QPalette::Window, QColor(53, 53, 53));
	palette.setColor(QPalette::WindowText, Qt::white);
	palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
	palette.setColor(QPalette::Base, QColor(42, 42, 42));
	palette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
	palette.setColor(QPalette::ToolTipBase, Qt::white);
	palette.setColor(QPalette::ToolTipText, QColor(53, 53, 53));
	palette.setColor(QPalette::Text, QColor(230, 230, 230));
	palette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
	palette.setColor(QPalette::Dark, QColor(35, 35, 35));
	palette.setColor(QPalette::Shadow, QColor(20, 20, 20));
	palette.setColor(QPalette::Button, QColor(53, 53, 53));
	palette.setColor(QPalette::ButtonText, Qt::white);
	palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
	palette.setColor(QPalette::BrightText, Qt::red);
	palette.setColor(QPalette::Link, QColor(42, 130, 218));
	palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
	palette.setColor(QPalette::HighlightedText, Qt::white);
	palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));
}

void DarkStyle::polish(QApplication* app)
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
