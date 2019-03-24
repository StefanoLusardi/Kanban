#include "SplashScreen.h"
#include "ui_SplashScreen.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QStyledItemDelegate>

SplashScreen::SplashScreen(QWidget *parent)
	: QSplashScreen(parent), ui{ new Ui::SplashScreen }
{
	ui->setupUi(this);
}

SplashScreen::~SplashScreen()
{
	delete ui;
}

void SplashScreen::writeMessage(const QString& message)
{
	ui->mMessageLabel->setText(message);
	qApp->processEvents();
}

void SplashScreen::centerOnScreen()
{
	setGeometry(
		QStyle::alignedRect(
        Qt::LeftToRight,
        Qt::AlignCenter,
        this->size(),
        qApp->desktop()->availableGeometry()));
}
