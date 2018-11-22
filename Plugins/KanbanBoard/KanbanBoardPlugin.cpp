#include "KanbanBoardPluginInterface.h"
#include "KanbanBoardPlugin.h"
#include "KanbanBoardView.h"

#include <QBoxLayout>

// TODO: remove these
#include "KanbanModel.h"
#include <QItemSelectionModel>

KanbanBoardPlugin::KanbanBoardPlugin(QBoxLayout* container, Model* model, KanbanBoardPluginInterface* parent) : QObject (parent)
{
	// Register Container Button
	mPluginButton = std::make_unique<QPushButton>(); // no parent, ok because is a unique_ptr
	mPluginButton->setCheckable(true);
	mPluginButton->setChecked(false);
	mPluginButton->setText(parent->name());
    mPluginButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    container->addWidget(mPluginButton.get());	

	// Create Plugin Ui class
	mPluginDialog = std::make_unique<QDialog>();  // no parent, ok because is a unique_ptr
	mPluginView = std::make_unique<KanbanBoardView>();
	

	{
		// TODO: this section is here only for demo purpose. It must be remove from here.
		// The model for KanbanBoardView should be provided by the KanbanBoardPlugin constructor

		KanbanModel* kanbanModel = new KanbanModel(this);
		mPluginView->setModel(kanbanModel);

		QItemSelectionModel* selectionKanbanModel = new QItemSelectionModel(kanbanModel, this);
		mPluginView->setSelectionModel(selectionKanbanModel);
		
		mPluginDialog->setLayout(new QVBoxLayout());
		mPluginDialog->layout()->addWidget(mPluginView.get());

		mPluginView->loadData();	
	}


	connect(mPluginDialog.get(), &QDialog::finished, [this]()
	{
		mPluginButton->setChecked(false);
	});

	connect(mPluginButton.get(), &QPushButton::clicked, [this]()
    {
        mPluginDialog->setVisible(mPluginButton->isChecked());
    });
}