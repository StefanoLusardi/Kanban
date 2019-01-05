#include "ui_KanbanDelegateEditor.h"
#include "KanbanDelegateEditor.h"

KanbanDelegateEditor::KanbanDelegateEditor(QWidget *parent)
	: QWidget(parent),
	ui{new Ui::KanbanDelegateEditor()}
{
	ui->setupUi(this);

	QList<QColor> colors 
	{
		Qt::cyan, 
		Qt::magenta, 
		Qt::yellow, 
		Qt::gray,
		Qt::green,
		Qt::red,
		Qt::blue,
		Qt::darkCyan, 
		Qt::darkMagenta, 
		Qt::darkYellow,
	};

	int idx = 0;
	for (const auto& color : colors)
	{
		ui->mColorEditor->addItem(QString());
		ui->mColorEditor->setItemData(idx, color, Qt::DecorationRole);
		++idx;
	}

	connect(ui->mColorEditor, qOverload<int>(&QComboBox::currentIndexChanged), [this](int)
	{
		emit editingFinished();
	});
}

KanbanDelegateEditor::~KanbanDelegateEditor()
{
	delete ui;
}

void KanbanDelegateEditor::setText(const QString& text) const { ui->mTextEditor->setText(text); }

void KanbanDelegateEditor::setColumn(const QString& column) const { ui->mStateEditor->setText(column); }

void KanbanDelegateEditor::setColor(const QColor& color) const
{
	const int idx = ui->mColorEditor->findData(color, Qt::DecorationRole);
	ui->mColorEditor->setCurrentIndex(idx);
}

QString KanbanDelegateEditor::getText() const { return ui->mTextEditor->text(); }

QString KanbanDelegateEditor::getColumn() const { return ui->mStateEditor->text(); }

QColor KanbanDelegateEditor::getColor() const
{
	return ui->mColorEditor->currentData(Qt::DecorationRole).value<QColor>();
}
