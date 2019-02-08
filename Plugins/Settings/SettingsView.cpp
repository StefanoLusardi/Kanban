#include "SettingsView.h"
#include "ui_SettingsView.h"

#include <QMessageBox>
#include <QStyleFactory>

#include "Model.h"
#include "../Common/PluginConfig.h"

SettingsView::SettingsView(Model* model, QWidget *parent)
	: QWidget(parent),
	ui{new Ui::SettingsView},
	mModel{model->getSettingsModel().get()}
{
	ui->setupUi(this);
	
	//ui->mListView->setModel(mModel);
	//connect(mModel, &QAbstractItemModel::dataChanged, [this]()
	//{
	//	const int itemCount = mModel->rowCount();
	//	ui->mItemCountLabel->setText(QString::number(itemCount));
	//});

	connect(ui->mStyleCombobox, &QComboBox::currentTextChanged, this, &SettingsView::setAppStyle);
	connect(ui->mAboutButton, &QPushButton::clicked, [this]()
	{
		QMessageBox::about(this, "About Kanban",
            "<p><b>Kanban</b> application made with <b>Qt5</b>"
            "<p>Stefano Lusardi, 2018");
	});
}

SettingsView::~SettingsView()
{
	delete ui;
}

void SettingsView::loadConfig()
{
	if (const auto config = PluginConfigManager::parse(mConfingFile); config)
	{
		const auto style = config.value().value("style").toString();
		//const auto size = config.value().value("size").toString();
		
		if (QStyleFactory::keys().contains(style))
		{
			setAppStyle(style);
		}
	}
}

void SettingsView::saveConfig() const
{
	const auto styleName = "---";

	QJsonObject style{};
	style.insert("style", styleName);

	QJsonObject config;
	config.insert("settings", style);
	PluginConfigManager::write(mConfingFile, config);
}

void SettingsView::setAppStyle(const QString& style) const
{
	QApplication::setStyle(QStyleFactory::create(style));	
}
