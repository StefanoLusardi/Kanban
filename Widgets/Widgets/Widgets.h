#pragma once

#include "../VerticalLabel/VerticalLabelPlugin.h"

#include <QtDesigner/QtDesigner>
#include <QtCore/qplugin.h>

class Widgets : public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface")

public:
    explicit Widgets(QObject *parent = 0)
	: QObject(parent)
	{
		mWidgets.append(new VerticalLabelPlugin(this));
	}

	QList<QDesignerCustomWidgetInterface*> customWidgets() const override
	{
		return mWidgets;
	}

private:
    QList<QDesignerCustomWidgetInterface*> mWidgets;
};