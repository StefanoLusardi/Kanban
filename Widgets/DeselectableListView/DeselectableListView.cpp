#include "DeselectableListView.h"
#include <QMouseEvent>

DeselectableListView::DeselectableListView(QWidget *parent)
	: QListView(parent)
{
}

void DeselectableListView::mousePressEvent(QMouseEvent* event)
{
	QModelIndex item = indexAt(event->pos());

	if (item.isValid())
	{
		QListView::mousePressEvent(event);
	}
	else
	{
		clearSelection();
		selectionModel()->setCurrentIndex({}, QItemSelectionModel::Clear);
		emit deselectAll();
	}
}