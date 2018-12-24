#pragma once

#include <QtCore/qglobal.h>
#if defined API_DESELECTABLELISTVIEW
	#define API Q_DECL_EXPORT
#else
	#define API Q_DECL_IMPORT
#endif

#include <QtWidgets/QListView>

class API DeselectableListView : public QListView
{
	Q_OBJECT

public:
	DeselectableListView(QWidget *parent = Q_NULLPTR);

signals:
	void deselectAll();

protected:
	void mousePressEvent(QMouseEvent* event) override;
};
