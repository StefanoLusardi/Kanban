#pragma once

#include <QtCore/qglobal.h>
#if defined API_VERTICALLABEL
	#define API Q_DECL_EXPORT
#else
	#define API Q_DECL_IMPORT
#endif

#include <QtWidgets/QLabel>

class API VerticalLabel : public QLabel
{
	Q_OBJECT
	Q_PROPERTY(QColor TextColor READ textColor WRITE setTextColor)
	Q_PROPERTY(QColor ContourColor READ contourColor WRITE setContourColor)
	Q_PROPERTY(QColor BackgroundColor READ backgroudColor WRITE setBackgroudColor)
	Q_PROPERTY(TextOrientation TextOrientation READ textOrientation WRITE setTextOrientation)

public:
	enum TextOrientation {Left, Right};
	Q_ENUM(TextOrientation)

	VerticalLabel(QWidget *parent = Q_NULLPTR);

	bool isSelected() const { return mIsSelected; }
	void setIsSelected(bool selected) { mIsSelected = selected; }

	QColor textColor() const { return mTextColor; }
	void setTextColor(const QColor& color ) { mTextColor = color; }
	
	QColor contourColor() const { return mContourColor; }
	void setContourColor(const QColor& color ) { mContourColor = color; }

	QColor backgroudColor() const { return mBackgroundColor; }
	void setBackgroudColor(const QColor& color ) { mBackgroundColor = color; }

	TextOrientation textOrientation() const { return mTextOrientation; }
	void setTextOrientation(const TextOrientation& orientation ) { mTextOrientation = orientation; }

signals:
	void leftClicked(bool isSelected);
	void rightClicked();

protected:
	void paintEvent(QPaintEvent*) override;
	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;
	void mousePressEvent(QMouseEvent* event) override;

private:
	bool mIsSelected;
	QColor mTextColor;
	QColor mContourColor;
	QColor mBackgroundColor;
	TextOrientation mTextOrientation;
};
