#pragma once

#include <QtCore/qglobal.h>
#if defined API_HORIZONTALLABEL
	#define API Q_DECL_EXPORT
#else
	#define API Q_DECL_IMPORT
#endif

#include <QtWidgets/QLabel>

class API HorizontalLabel : public QLabel
{
	Q_OBJECT
	Q_PROPERTY(QColor TextColor READ textColor WRITE setTextColor)
	Q_PROPERTY(QColor BackgroundColor READ backgroudColor WRITE setBackgroudColor)

public:
	HorizontalLabel(QWidget *parent = Q_NULLPTR);

	QColor textColor() const { return mTextColor; }
	void setTextColor(const QColor& color ) { mTextColor = color; }

	QColor backgroudColor() const { return mBackgroundColor; }
	void setBackgroudColor(const QColor& color ) { mBackgroundColor = color; }

protected:
	void paintEvent(QPaintEvent*) override;

private:
	QColor mTextColor;
	QColor mBackgroundColor;
};