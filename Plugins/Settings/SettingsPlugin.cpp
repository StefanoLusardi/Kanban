#include "SettingsPluginInterface.h"
#include "SettingsPlugin.h"
#include "SettingsView.h"
#include "Model.h"
#include "../Widgets/Widgets/Widgets.h"

#include <QBoxLayout>
#include <QToolButton>

SettingsPlugin::SettingsPlugin(QBoxLayout* /*mainViewLayout*/, QBoxLayout* pluginButtonsLayout, Model* model, SettingsPluginInterface* parent) : QObject (parent)
{
	// Register Plugin Button into pluginButtonsLayout
	mPluginButton = new QToolButton(); // no smart_ptr because later container->addWidget() steals ownership

	mPluginButton->setFixedSize(100, 100);
	mPluginButton->setAutoRaise(true);
	mPluginButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	mPluginButton->setCheckable(true);
	mPluginButton->setChecked(false);
	mPluginButton->setText(parent->name());
	QIcon buttonIcon;
    buttonIcon.addFile(QString::fromUtf8(":/images/Light/button/settings.png"), QSize(50, 50), QIcon::Normal, QIcon::On);
    mPluginButton->setIcon(buttonIcon);

	const auto insertIdx = pluginButtonsLayout->count() - 1;
    pluginButtonsLayout->insertWidget(insertIdx, mPluginButton);

	// Create Plugin Ui class
	mPluginView = new SettingsView(model);
	
	mFramelessWindow = new FramelessWindow();
	mFramelessWindow->setWindowTitle("Settings");
	mFramelessWindow->setContent(mPluginView);

	connect(mFramelessWindow, &FramelessWindow::aboutToClose, [this]()
	{
		mPluginButton->setChecked(false);
	});

	connect(mPluginButton, &QToolButton::clicked, [this]()
    {
        mFramelessWindow->setVisible(mPluginButton->isChecked());
    });

	connect(model->getSettingsModel().get(), &SettingsModel::styleChanged, [this](const QString styleName)
	{
		mFramelessWindow->changeStyle(styleName);
	});
}

void SettingsPlugin::initData() const
{
	if (mPluginView)
		mPluginView->loadConfig();
}

void SettingsPlugin::release() const
{
	if (mPluginView)
		mPluginView->saveConfig();

	delete mFramelessWindow;
}