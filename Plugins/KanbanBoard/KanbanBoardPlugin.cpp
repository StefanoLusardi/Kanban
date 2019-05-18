#include "KanbanBoardPluginInterface.h"
#include "KanbanBoardPlugin.h"
#include "KanbanBoardView.h"
#include "Model.h"
#include "../Common/Utils.h"

#include <QToolButton>
#include <QInputDialog>

KanbanBoardPlugin::KanbanBoardPlugin(QBoxLayout* mainViewLayout, QBoxLayout* pluginButtonsLayout, Model* model, KanbanBoardPluginInterface* parent) : QObject (parent)
{
	// Register Plugin Button into pluginButtonsLayout
	mAddPageButton = new QToolButton();
	mAddPageButton->setMinimumSize(QSize(100, 100));
	mAddPageButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	mAddPageButton->setAutoRaise(true);
	mAddPageButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	mAddPageButton->setText("New Board");
	QIcon buttonIcon;
    buttonIcon.addFile(QString::fromUtf8(":/images/Light/button/new_board.png"), QSize(), QIcon::Normal, QIcon::Off);
    mAddPageButton->setIcon(buttonIcon);
    mAddPageButton->setIconSize(QSize(50, 50));
	
	const auto insertIdx = pluginButtonsLayout->count() - 1;
    pluginButtonsLayout->insertWidget(insertIdx, mAddPageButton);  // Steals ownership

	// Create Plugin Ui class
	mPluginView = new KanbanBoardView(model, pluginButtonsLayout);
	mainViewLayout->addWidget(mPluginView); // Steals ownership

	connect(mAddPageButton, &QToolButton::clicked, [this]()
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
