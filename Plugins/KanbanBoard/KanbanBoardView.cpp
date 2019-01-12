#include "KanbanBoardView.h"
#include "ui_KanbanBoardView.h"
#include "KanbanPageView.h"
#include "Model.h"

#include <QTabBar>
#include "../Common/PluginConfig.h"

KanbanBoardView::KanbanBoardView(Model* model, QWidget *parent)
	: QWidget(parent),
	ui{new Ui::KanbanBoardView()},
	mModel{model}
{
	ui->setupUi(this);
	ui->tabWidget->tabBar()->setExpanding(true);
}

KanbanBoardView::~KanbanBoardView()
{
	delete ui;
}

void KanbanBoardView::loadConfig()
{
	const QJsonObject config = PluginConfigManager::parse("KanbanBoard.json");
	const QJsonValue pagesNode = config.value("pages");

	for (auto&& pageItem : mModel->getPageModel()->pages())
	{
		// Create a Kanban Page, if it already exists in the Model
		const auto pageName {pageItem.getPageName()};
		const auto pageIdx  {pageItem.getPageIdx()};
		const auto page = new KanbanPageView(pageName, mModel->getKanbanModel(pageIdx).get(), this);
		ui->tabWidget->addTab(page, pageName);

		// If there is a config for the current page, load it.
		for (auto&& node : pagesNode.toArray())
		{
			const auto pageNode = node.toObject();
			if (pageName == pageNode.value("page").toString())
			{
				page->loadConfig(pageNode);
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
	PluginConfigManager::write("KanbanBoard.json", config);
}

void KanbanBoardView::insertPage(const QString& pageName)
{
	const auto pageIdx = mModel->insertPage(pageName);
	const auto page = new KanbanPageView(pageName, mModel->getKanbanModel(pageIdx).get(), this);
	ui->tabWidget->addTab(page, pageName);
}

void KanbanBoardView::removePage(const QString& pageName) const
{
	for (auto i = 0; i < ui->tabWidget->count(); ++i)
	{
		if (ui->tabWidget->tabText(i) == pageName)
		{
			const auto page = ui->tabWidget->widget(i);
			ui->tabWidget->removeTab(i);

			delete page;
			break;
		}
	}
}
