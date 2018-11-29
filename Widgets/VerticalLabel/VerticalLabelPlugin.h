#pragma once

#include "VerticalLabel.h"
#include <QtCore/QtPlugin>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class VerticalLabelPlugin : public QObject, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" FILE "verticallabelplugin.json")
	Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	VerticalLabelPlugin(QObject *parent = Q_NULLPTR) : QObject(parent) { initialized = false; }

	bool isContainer() const override { return false; }

	bool isInitialized() const override { return initialized; }

	QIcon icon() const override { return QIcon(); }

	QString group() const override { return "KB::Plugins"; }

	QString includeFile() const override { return "../../Widgets/VerticalLabel/VerticalLabel.h"; }

	QString name() const override { return "VerticalLabel"; }

	QString toolTip() const override { return QString("Vertical Label widget"); }

	QString whatsThis() const override { return QString("Vertical label that provides several options for text alignment"); }

	QWidget* createWidget(QWidget* parent) override { return new VerticalLabel(parent); }

	void initialize(QDesignerFormEditorInterface* core) override
	{
		if (initialized)
			return;

		initialized = true;
	}

	QString domXml() const override 
	{
		return "<widget class=\"VerticalLabel\" name=\"verticalLabel\">\n"
			" <property name=\"geometry\">\n"
			"  <rect>\n"
			"   <x>0</x>\n"
			"   <y>0</y>\n"
			"   <width>100</width>\n"
			"   <height>100</height>\n"
			"  </rect>\n"
			" </property>\n"
			"</widget>\n";
	}

private:
	bool initialized;
};
