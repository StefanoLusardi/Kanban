#include "KanbanBoardPluginInterface.h"
#include "KanbanBoardPlugin.h"
#include "KanbanBoardView.h"
#include "KanbanPageView.h"
#include "Model.h"

#include <QBoxLayout>
#include <QItemSelectionModel>
#include <QTabWidget>
#include <QPushButton>
#include "../Common/Utils.h"
#include <QInputDialog>

KanbanBoardPlugin::KanbanBoardPlugin(QBoxLayout* mainViewLayout, QBoxLayout* pluginButtonsLayout, Model* model, KanbanBoardPluginInterface* parent) : QObject (parent)
{
	// Create Plugin Ui class
	mPluginView = new KanbanBoardView(model);
	mainViewLayout->addWidget(mPluginView); // Steals ownership

	// Register Plugin Button into pluginButtonsLayout
	mAddPageButton = new QPushButton(); // no smart_ptr because later container->addWidget() steals ownership
	mAddPageButton->setText("New Kanban Board");
    pluginButtonsLayout->addWidget(mAddPageButton);

	connect(mAddPageButton, &QPushButton::clicked, [this]()
	{
		bool ok;
		QString text = QInputDialog::getText(nullptr, "Kanban Borad name", "Set name for the new Kanban Board", QLineEdit::Normal, QString(), &ok);

		if (ok && !text.isEmpty())
		{
			mPluginView->newPage(text);
		}
	});
}

void KanbanBoardPlugin::initData() const
{
	if (mPluginView)
		mPluginView->loadConfig();
}

void KanbanBoardPlugin::release() const
{
	if (mPluginView)
		mPluginView->saveConfig();
}
