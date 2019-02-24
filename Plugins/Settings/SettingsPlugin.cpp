#include "SettingsPluginInterface.h"
#include "SettingsPlugin.h"
#include "SettingsView.h"
#include "Model.h"

#include <QDialog>
#include <QBoxLayout>
#include <QPushButton>

SettingsPlugin::SettingsPlugin(QBoxLayout* /*mainViewLayout*/, QBoxLayout* pluginButtonsLayout, Model* model, SettingsPluginInterface* parent) : QObject (parent)
{
	// Register Plugin Button into pluginButtonsLayout
	mPluginButton = new QPushButton(); // no smart_ptr because later container->addWidget() steals ownership
	mPluginButton->setCheckable(true);
	mPluginButton->setChecked(false);
	mPluginButton->setText(parent->name());
    pluginButtonsLayout->addWidget(mPluginButton);

	// Create Plugin Ui class
	mPluginDialog = new QDialog();
	mPluginView = new SettingsView(model);

	mPluginDialog->setLayout(new QVBoxLayout());
	mPluginDialog->layout()->addWidget(mPluginView); // Steals ownership

	connect(mPluginDialog, &QDialog::finished, [this]()
	{
		mPluginButton->setChecked(false);
	});

	connect(mPluginButton, &QPushButton::clicked, [this]()
    {
        mPluginDialog->setVisible(mPluginButton->isChecked());
    });
}

void SettingsPlugin::initData() const
{
	if (mPluginView)
		mPluginView->loadConfig();
}

void SettingsPlugin::release() const
{
	if (mPluginView)
		mPluginView->saveConfig();

	delete mPluginDialog;
}