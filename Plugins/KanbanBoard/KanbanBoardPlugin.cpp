#include "KanbanBoardPluginInterface.h"
#include "KanbanBoardPlugin.h"
#include "KanbanBoardView.h"

#include <QBoxLayout>

// TODO: remove these?
#include "../../Model/Model.h"
#include <QItemSelectionModel>

KanbanBoardPlugin::KanbanBoardPlugin(QBoxLayout* container, Model* model, KanbanBoardPluginInterface* parent) : QObject (parent)
{
	// Register Container Button
	mPluginButton = new QPushButton(); // no smart_ptr because later container->addWidget() steals ownership
	mPluginButton->setCheckable(true);
	mPluginButton->setChecked(false);
	mPluginButton->setText(parent->name());
    mPluginButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    container->addWidget(mPluginButton);	

	// Create Plugin Ui class
	mPluginDialog = std::make_unique<QDialog>();  // no parent, ok because is a unique_ptr
	mPluginView = std::make_unique<KanbanBoardView>();
	
	auto kanbanModel = model->getKanbanModel();
	mPluginView->setModel(kanbanModel.get());

	auto selectionKanbanModel = new QItemSelectionModel(kanbanModel.get(), this);
	mPluginView->setSelectionModel(selectionKanbanModel);
		
	mPluginDialog->setLayout(new QVBoxLayout());
	mPluginDialog->layout()->addWidget(mPluginView.get());

	{

		mPluginView->loadData();	
	}


	connect(mPluginDialog.get(), &QDialog::finished, [this]()
	{
		mPluginButton->setChecked(false);
	});

	connect(mPluginButton, &QPushButton::clicked, [this]()
    {
        mPluginDialog->setVisible(mPluginButton->isChecked());
    });
}