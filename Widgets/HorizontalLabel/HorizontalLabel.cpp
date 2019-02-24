#include "HorizontalLabel.h"
#include <QPainter>
#include <QMouseEvent>

HorizontalLabel::HorizontalLabel(QWidget *parent) 
	: QLabel(parent), 
	mIsSelected{false}, 
	mTextColor{Qt::black}, 
	mContourColor{Qt::black},
	mBackgroundColor{Qt::white}
{
}

void HorizontalLabel::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	// Draw Background
	QLinearGradient g(rect().topLeft(), rect().bottomRight());
	g.setColorAt(0.2, mBackgroundColor);
	g.setColorAt(1.0, QColor(255, 255, 255, 128));
	p.fillRect(rect(), g);
	
	// Draw Contour if selected
	if(mIsSelected)
	{
		const qreal penWidth {2};
		p.setPen({mContourColor, penWidth});
		p.drawRect(rect().adjusted(1, 1, -1, -1));
	}

	// Draw Text
	p.setPen(mTextColor);
	const auto fw = fontMetrics().width(text());
	const auto fh = fontMetrics().height();
	const auto w = width() / 2 - fw / 2 - 1;
	const auto h = height() / 2 + fh / 2 - 2;
	p.drawText(w, h, text());
}

void HorizontalLabel::mousePressEvent(QMouseEvent* event)
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