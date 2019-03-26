#pragma once

#include "FramelessWindow.h"
#include <QtCore/QtPlugin>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class FramelessWindowPlugin : public QObject, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" FILE "framelesswindowplugin.json")
	Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	FramelessWindowPlugin(QObject *parent = Q_NULLPTR) : QObject(parent) { initialized = false; }

	bool isContainer() const override { return true; }

	bool isInitialized() const override { return initialized; }

	QIcon icon() const override { return QIcon(); }

	QString group() const override { return "KB::Plugins"; }

	QString includeFile() const override { return "FramelessWindow.h"; }

	QString name() const override { return "FramelessWindow"; }

	QString toolTip() const override { return QString("FramelessWindow"); }

	QString whatsThis() const override { return QString("FramelessWindow"); }

	QWidget* createWidget(QWidget* parent) override { return new FramelessWindow(parent); }

	void initialize(QDesignerFormEditorInterface* core) override
	{
		if (initialized)
			return;

		initialized = true;
	}

	QString domXml() const override 
	{
	return "<widget class=\"FramelessWindow\" name=\"framelessWindow\">\n"
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
