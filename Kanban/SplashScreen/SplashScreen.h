#pragma once
#include <QSplashScreen>
#include <QPainter>
#include <QUrl>

class SplashScreen : public QSplashScreen
{
	Q_OBJECT

public:
	SplashScreen(const int &alignement = Qt::AlignRight, const QColor &textColor = Qt::white, const QString &pixmapPath = QString(":/images/splash.png"), QObject *parent = Q_NULLPTR);
	~SplashScreen();
    QFont setupFont() const;
    void writeMessage(const QString& message);
    void drawContents(QPainter *painter) override;
    void setupMessageArea(const QSize &messageArea = QSize(200, 50));

private:
    QString mMessage;
    const int mAlignement;
    const QColor mTextColor;
    QRect mMessageArea;
};
