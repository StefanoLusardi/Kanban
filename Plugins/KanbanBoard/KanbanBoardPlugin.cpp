#include "KanbanBoardPluginInterface.h"
#include "KanbanBoardPlugin.h"
#include "KanbanBoardView.h"
#include "Model.h"

#include <QBoxLayout>
#include <QItemSelectionModel>


KanbanBoardPlugin::KanbanBoardPlugin(QBoxLayout* mainViewLayout, QBoxLayout* /*pluginButtonsLayout*/, Model* model, KanbanBoardPluginInterface* parent) : QObject (parent)
{
	// Create Plugin Ui class
	mPluginView = new KanbanBoardView();
	
	auto kanbanModel = model->getKanbanModel();
	mPluginView->setModel(kanbanModel.get());

	mainViewLayout->addWidget(mPluginView); // Steals ownership
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
