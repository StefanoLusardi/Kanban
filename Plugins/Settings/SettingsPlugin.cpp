#include "SettingsPluginInterface.h"
#include "SettingsPlugin.h"
#include "SettingsView.h"
#include "Model.h"

#include <QBoxLayout>

SettingsPlugin::SettingsPlugin(QBoxLayout* /*mainViewLayout*/, QBoxLayout* pluginButtonsLayout, Model* model, SettingsPluginInterface* parent) : QObject (parent)
{
	// Register Plugin Button into pluginButtonsLayout
	mPluginButton = new QPushButton(); // no smart_ptr because later container->addWidget() steals ownership
	mPluginButton->setCheckable(true);
	mPluginButton->setChecked(false);
	mPluginButton->setText(parent->name());
    mPluginButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pluginButtonsLayout->addWidget(mPluginButton);

	// Create Plugin Ui class
	mPluginDialog = std::make_unique<QDialog>();  // no parent, ok because is a unique_ptr
	mPluginView = new SettingsView();
	
	auto kanbanModel = model->getKanbanModel();
	mPluginView->setModel(kanbanModel. get());
	mPluginView->setModel(model);

	mPluginDialog->setLayout(new QVBoxLayout());
	mPluginDialog->layout()->addWidget(mPluginView); // Steals ownership

	connect(mPluginDialog.get(), &QDialog::finished, [this]()
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
}