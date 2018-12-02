#include "VerticalLabel.h"
#include <QPainter>

VerticalLabel::VerticalLabel(QWidget* parent) : QLabel(parent), mTextColor{Qt::black}, mTextOrientation{TextOrientation::Left}
{
}

void VerticalLabel::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	// Draw rect contour
	/*p.setPen(Qt::red);
	p.drawRect(rect().adjusted(0, 0, -1, -1));*/

	switch(mTextOrientation) 
	{ 	
	case TextOrientation::Left: 
	{
		QLinearGradient g(rect().topLeft(), rect().bottomRight());
		g.setColorAt(0.2, mBackgroundColor);
		g.setColorAt(1.0, Qt::white);
		p.fillRect(rect(), g);

		p.rotate(90);
		p.setPen(mTextColor);
		const auto fw = fontMetrics().width(text());
		const auto fh = fontMetrics().height();
		const auto w = width() / 2 - fh / 2;
		const auto h = height() / 2 - fw / 2;
		p.drawText(h, -w, text());
		break;
	}
	case TextOrientation::Right: 
	{
		QLinearGradient g(rect().topLeft(), rect().bottomRight());
		g.setColorAt(0.2, mBackgroundColor);
		g.setColorAt(1.0, Qt::white);
		p.fillRect(rect(), g);

		p.rotate(270);
		p.setPen(mTextColor);
		const auto fw = fontMetrics().width(text());
		const auto fh = fontMetrics().height();
		const auto w = width() / 2 + fh / 2;
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