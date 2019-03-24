#pragma once

#include <QSplashScreen>
namespace Ui { class SplashScreen; };

class SplashScreen : public QSplashScreen
{
	Q_OBJECT

public:
	SplashScreen(QWidget *parent = Q_NULLPTR);
	~SplashScreen();

	void writeMessage(const QString& message);
	void centerOnScreen();

private:
	Ui::SplashScreen *ui;
};
