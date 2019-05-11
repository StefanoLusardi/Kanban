#include "KanbanDelegateEditor.h"
#include "KanbanDelegate.h"
#include "Model.h"

#include <QPainter>

void KanbanDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{ 
	painter->save();
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing); 
	
	// Draw contour (item selected or mouse over)
    QColor contourColor = Qt::transparent;	
	if (option.state.testFlag(QStyle::State_Selected)) 
	{
		contourColor = option.palette.color(QPalette::BrightText);
		contourColor.setAlpha(128);
    }
	else if (option.state.testFlag(QStyle::State_MouseOver))
	{
		contourColor = option.palette.color(QPalette::Highlight);
		contourColor.setAlpha(128);
	}
	painter->fillRect(option.rect, contourColor);

	// Draw Kanban Item rectangle (internal background)
	const int margin = 3;
	QLinearGradient itemBackground(option.rect.topLeft(), option.rect.bottomRight());
	const QColor itemColor = index.model()->data(index, Qt::DecorationRole).value<QColor>();
	itemBackground.setColorAt(0.5, itemColor);
	itemBackground.setColorAt(1.0, QColor(255, 255, 255, 128));
	painter->fillRect(option.rect.adjusted(margin, margin, -margin, -margin), itemBackground);

	// Draw the text
	const QString text = index.model()->data(index, Qt::DisplayRole).toString();
    painter->setPen(Qt::black);
	painter->drawText(option.rect, Qt::AlignCenter, text);	

    painter->restore();
}

QSize KanbanDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const auto size = QStyledItemDelegate::sizeHint(option, index);
	return QSize(size.width(), 2*size.height());
}

QWidget* KanbanDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	KanbanDelegateEditor *editor = new KanbanDelegateEditor(parent);
	connect(editor, &KanbanDelegateEditor::editingFinished, this, &KanbanDelegate::commitAndCloseEditor);
	editor->setFocus();
	return editor;
}

void KanbanDelegate::commitAndCloseEditor()
{
	KanbanDelegateEditor *editor = static_cast<KanbanDelegateEditor*>(sender());
	emit commitData(editor);
	emit closeEditor(editor);
}

void KanbanDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    KanbanDelegateEditor *delegateEditor = static_cast<KanbanDelegateEditor*>(editor);

	const QString text  = index.model()->data(index, Qt::DisplayRole).toString();
	const QColor color = index.model()->data(index, Qt::DecorationRole).value<QColor>();
	const QString column = index.model()->data(index, KanbanItemModel::Roles::ColumnName).toString();

    delegateEditor->setText(text);
    delegateEditor->setColor(color);
    delegateEditor->setColumn(column);
}

void KanbanDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	KanbanDelegateEditor *delegateEditor = static_cast<KanbanDelegateEditor*>(editor);

    const QString text = delegateEditor->getText();
    const QColor color = delegateEditor->getColor();
    const QString column = delegateEditor->getColumn();

    model->setData(index, text, Qt::DisplayRole);
    model->setData(index, color, Qt::DecorationRole);
    model->setData(index, column, KanbanItemModel::Roles::ColumnName);
}

void KanbanDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}
