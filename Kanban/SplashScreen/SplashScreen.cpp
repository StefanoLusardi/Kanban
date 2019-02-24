#include "SplashScreen.h"
#include <QApplication>

SplashScreen::SplashScreen(const int &alignement, const QColor &textColor, const QString &pixmapPath, QObject *parent)
: QSplashScreen(), mAlignement(alignement), mTextColor(textColor)
{
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::SplashScreen);
    const QPixmap splashPixmap(pixmapPath);
	setPixmap(splashPixmap);
	//setFont(setupFont());
}

SplashScreen::~SplashScreen()
{
}

void SplashScreen::setupMessageArea(const QSize &messageArea)
{
    auto splashSize = size();

    mMessageArea.setTop(splashSize.height() - messageArea.height());
    mMessageArea.setWidth(splashSize.width());
    mMessageArea.setHeight(messageArea.height());
}

QFont SplashScreen::setupFont() const
{
    QFont splashFont;
    splashFont.setFamily("Arial");
    splashFont.setPixelSize(40);
    splashFont.setStretch(100);
    splashFont.setBold(true);
    return splashFont;
}

void SplashScreen::writeMessage(const QString& message)
{
    mMessage = message;
    showMessage(mMessage, mAlignement, mTextColor);
	qApp->processEvents();
}

void SplashScreen::drawContents(QPainter* painter)
{
    painter->setPen(Qt::white);
    painter->drawText(mMessageArea, mAlignement, mMessage);
}
