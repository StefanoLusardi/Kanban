#pragma once

#include "DeselectableListView.h"
#include <QtCore/QtPlugin>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class DeselectableListViewPlugin : public QObject, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" FILE "deselectablelistviewplugin.json")
	Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	DeselectableListViewPlugin(QObject *parent = Q_NULLPTR) : QObject(parent) { initialized = false; }

	bool isContainer() const override { return false; }

	bool isInitialized() const override { return initialized; }

	QIcon icon() const override { return QIcon(); }

	QString group() const override { return "KB::Plugins"; }

	QString includeFile() const override { return "DeselectableListView.h"; }

	QString name() const override { return "DeselectableListView"; }

	QString toolTip() const override { return QString("Deselectable List View"); }

	QString whatsThis() const override { return QString("List View that allows to deselect all items in a single click"); }

	QWidget* createWidget(QWidget* parent) override { return new DeselectableListView(parent); }

	void initialize(QDesignerFormEditorInterface* core) override
	{
		if (initialized)
			return;

		initialized = true;
	}

	QString domXml() const override 
	{
		return "<widget class=\"DeselectableListView\" name=\"deselectableListView\">\n"
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
