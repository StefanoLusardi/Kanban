#include "KanbanBoardPluginInterface.h"
#include "KanbanBoardPlugin.h"
#include "KanbanBoardView.h"
#include "Model.h"
#include "../Common/Utils.h"

#include <QPushButton>
#include <QInputDialog>

KanbanBoardPlugin::KanbanBoardPlugin(QBoxLayout* mainViewLayout, QBoxLayout* pluginButtonsLayout, Model* model, KanbanBoardPluginInterface* parent) : QObject (parent)
{
	// Create Plugin Ui class
	mPluginView = new KanbanBoardView(model);
	mainViewLayout->addWidget(mPluginView); // Steals ownership

	// Register Plugin Button into pluginButtonsLayout
	mAddPageButton = new QPushButton();
	mAddPageButton->setText("New Kanban Board");
    pluginButtonsLayout->addWidget(mAddPageButton);  // Steals ownership

	connect(mAddPageButton, &QPushButton::clicked, [this]()
	{
		bool ok;
		QString text = QInputDialog::getText(nullptr, "Kanban Board name", "Set name for the new Kanban Board", QLineEdit::Normal, QString(), &ok);

		if (ok && !text.isEmpty())
		{
			mPluginView->insertPage(text);
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
