#include "KanbanBoardView.h"
#include "ui_KanbanBoardView.h"
#include "KanbanPageView.h"
#include "KanbanButtonsView.h"
#include "Model.h"

#include <QTabBar>
#include "../Common/PluginConfig.h"

KanbanBoardView::KanbanBoardView(Model* model, QBoxLayout* pluginButtonsLayout, QWidget *parent)
	: QWidget(parent),
	ui{new Ui::KanbanBoardView()},
	mModel{model}
{
	ui->setupUi(this);
	ui->tabWidget->tabBar()->setExpanding(true);

	mButtonsUi = new KanbanButtonsView(this);
	const auto insertIdx = pluginButtonsLayout->count() - 1;
	pluginButtonsLayout->insertWidget(insertIdx, mButtonsUi);

	connect(ui->tabWidget, &QTabWidget::currentChanged, [this](int idx)
	{
		const auto w = ui->tabWidget->widget(idx);
		if (auto page = dynamic_cast<KanbanPageView*>(w); page)
		{
			page->repaint();
		}
	});
}

KanbanBoardView::~KanbanBoardView()
{
	delete ui;
}

void KanbanBoardView::setupPageConnections(KanbanPageView* page, bool connecting) const
{
	if (connecting)
	{
		connect(mButtonsUi, &KanbanButtonsView::createKanbanSignal, page, &KanbanPageView::onCreateKanban);
		connect(mButtonsUi, &KanbanButtonsView::renameKanbanSignal, page, &KanbanPageView::onRenameKanban);
		connect(mButtonsUi, &KanbanButtonsView::deleteKanbanSignal, page, &KanbanPageView::onDeleteKanban);
		connect(mButtonsUi, &KanbanButtonsView::createColumnSignal, page, &KanbanPageView::onCreateColumn);
		connect(mButtonsUi, &KanbanButtonsView::renameColumnSignal, page, &KanbanPageView::onRenameColumn);
		connect(mButtonsUi, &KanbanButtonsView::deleteColumnSignal, page, &KanbanPageView::onDeleteColumn);
	}
	else
	{
		disconnect(mButtonsUi, &KanbanButtonsView::createKanbanSignal, page, &KanbanPageView::onCreateKanban);
		disconnect(mButtonsUi, &KanbanButtonsView::renameKanbanSignal, page, &KanbanPageView::onRenameKanban);
		disconnect(mButtonsUi, &KanbanButtonsView::deleteKanbanSignal, page, &KanbanPageView::onDeleteKanban);
		disconnect(mButtonsUi, &KanbanButtonsView::createColumnSignal, page, &KanbanPageView::onCreateColumn);
		disconnect(mButtonsUi, &KanbanButtonsView::renameColumnSignal, page, &KanbanPageView::onRenameColumn);
		disconnect(mButtonsUi, &KanbanButtonsView::deleteColumnSignal, page, &KanbanPageView::onDeleteColumn);
	}
}

void KanbanBoardView::loadConfig()
{
	if (const auto config = PluginConfigManager::parse(mConfigFile); config)
	{
		const QJsonValue pagesNode = config.value().value("pages");

		for (auto&& pageItem : mModel->getPageModel()->pages())
		{
			// Create a Kanban Page, if it already exists in the Model
			const auto pageName{pageItem.getPageName()};
			const auto pageIdx{pageItem.getPageIdx()};
			const auto page = new KanbanPageView(pageName, mModel->getKanbanModel(pageIdx).get(), this);
			setupPageConnections(page);

			ui->tabWidget->addTab(page, pageName);

			// If there is a config for the current page, load it.
			for (auto&& node : pagesNode.toArray())
			{
				if (const auto pageNode = node.toObject(); pageName == pageNode.value("page").toString())
				{
					page->loadConfig(pageNode);
				}
			}
		}
	}
}

void KanbanBoardView::saveConfig() const
{
	QJsonArray pagesConfig;
	for (auto idx = 0; idx < ui->tabWidget->count(); ++idx)
	{
		const auto widget = ui->tabWidget->widget(idx);
		if (const auto page = dynamic_cast<KanbanPageView*>(widget); page)
		{
			page->saveConfig(pagesConfig);
		}
	}

	QJsonObject config;
	config.insert("pages", pagesConfig);
	PluginConfigManager::write(mConfigFile, config);
}

void KanbanBoardView::insertPage(const QString& pageName)
{
	const auto pageIdx = mModel->insertPage(pageName);
	const auto page = new KanbanPageView(pageName, mModel->getKanbanModel(pageIdx).get(), this);
	setupPageConnections(page);
	ui->tabWidget->addTab(page, pageName);
}

void KanbanBoardView::removePage(const QString& pageName) const
{
	for (auto i = 0; i < ui->tabWidget->count(); ++i)
	{
		if (ui->tabWidget->tabText(i) == pageName)
		{
			const auto pageWidget = ui->tabWidget->widget(i);
			ui->tabWidget->removeTab(i);

			if (const auto page = dynamic_cast<KanbanPageView*>(pageWidget); page)
				setupPageConnections(page, false);

			delete pageWidget;
			break;
		}
	}
}
