#include "SettingsPluginInterface.h"
#include "SettingsPlugin.h"
#include "SettingsView.h"

#include <QBoxLayout>

#include "../../Model/Model.h"

SettingsPlugin::SettingsPlugin(QBoxLayout* container, Model* model, SettingsPluginInterface* parent) : QObject (parent)
{
	// Register Plugin Button into parent container
	mPluginButton = new QPushButton(); // no smart_ptr because later container->addWidget() steals ownership
	mPluginButton->setCheckable(true);
	mPluginButton->setChecked(false);
	mPluginButton->setText(parent->name());
    mPluginButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    container->addWidget(mPluginButton);

	// Create Plugin Ui class
	mPluginDialog = std::make_unique<QDialog>();  // no parent, ok because is a unique_ptr
	mPluginView = std::make_unique<SettingsView>();
	
	auto kanbanModel = model->getKanbanModel();
	mPluginView->setModel(kanbanModel.get());

	mPluginDialog->setLayout(new QVBoxLayout());
	mPluginDialog->layout()->addWidget(mPluginView.get());

	connect(mPluginDialog.get(), &QDialog::finished, [this]()
	{
		mPluginButton->setChecked(false);
	});

	connect(mPluginButton, &QPushButton::clicked, [this]()
    {
        mPluginDialog->setVisible(mPluginButton->isChecked());
    });
}