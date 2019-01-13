#include "VerticalLabel.h"
#include <QPainter>
#include <QMouseEvent>

VerticalLabel::VerticalLabel(QWidget* parent) 
	: QLabel(parent), 
	mIsSelected{false},
	mTextColor{Qt::black}, 	
	mContourColor{Qt::black}, 
	mBackgroundColor{Qt::white},
	mTextOrientation{TextOrientation::Left}
{
}

void VerticalLabel::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	// Draw Background
	QLinearGradient g(rect().topLeft(), rect().bottomRight());
	g.setColorAt(0.2, mBackgroundColor);
	g.setColorAt(1.0, Qt::white);
	p.fillRect(rect(), g);

	// Draw Contour if selected
	if(mIsSelected)
	{
		const qreal penWidth {2};
		p.setPen({mContourColor, penWidth});
		p.drawRect(rect().adjusted(1, 1, -1, -1));
	}

	// Draw Text
	switch(mTextOrientation) 
	{ 	
	case TextOrientation::Left: 
	{
		p.rotate(90);
		p.setPen(mTextColor);
		const auto fw = fontMetrics().width(text());
		const auto fh = fontMetrics().height();
		const auto w = width() / 2 - fh / 2 + 3;
		const auto h = height() / 2 - fw / 2;
		p.drawText(h, -w, text());
		break;
	}
	case TextOrientation::Right: 
	{
		p.rotate(270);
		p.setPen(mTextColor);
		const auto fw = fontMetrics().width(text());
		const auto fh = fontMetrics().height();
		const auto w = width() / 2 + fh / 2 - 3;
		const auto h = height() / 2 + fw / 2;
		p.drawText(-h, w, text());
		break;
	}
	default: assert(0); break;
	}
}

QSize VerticalLabel::minimumSizeHint() const
{
	QSize s = QLabel::minimumSizeHint();
	return {s.height(), s.width()};
}

QSize VerticalLabel::sizeHint() const
{
	QSize s = QLabel::sizeHint();
	return {s.height(), s.width()};
}

void VerticalLabel::mousePressEvent(QMouseEvent* event)
{
	if (event->buttons().testFlag(Qt::LeftButton))
	{
		mIsSelected = !mIsSelected;
		emit leftClicked(mIsSelected);
	}

	if (event->buttons().testFlag(Qt::RightButton))
	{
		emit rightClicked();	
	}

	QLabel::mousePressEvent(event);
}
