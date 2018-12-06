#include "HorizontalLabel.h"
#include <QPainter>

HorizontalLabel::HorizontalLabel(QWidget *parent) 
: QLabel(parent), 
	mTextColor{Qt::black}, 
	mBackgroundColor{Qt::white}
{
}

void HorizontalLabel::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	// Draw rect contour
	/*p.setPen(Qt::red);
	p.drawRect(rect().adjusted(0, 0, -1, -1));*/

	QLinearGradient g(rect().topLeft(), rect().bottomRight());
	g.setColorAt(0.2, mBackgroundColor);
	g.setColorAt(1.0, Qt::white);
	p.fillRect(rect(), g);

	p.setPen(mTextColor);
	const auto fw = fontMetrics().width(text());
	const auto fh = fontMetrics().height();
	const auto w = width() / 2 - fw / 2;
	const auto h = height() / 2 + fh / 2;
	p.drawText(w, h, text());
}
