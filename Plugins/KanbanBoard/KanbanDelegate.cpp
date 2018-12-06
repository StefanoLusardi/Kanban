#include "KanbanDelegateEditor.h"
#include "KanbanDelegate.h"

#include <QPainter>

#include "../../Model/Roles.h"

void KanbanDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	painter->save();

	const int xRound = 10;
	const int yRound = 100;

	const QColor color = index.model()->data(index, Qt::DecorationRole).value<QColor>();
	painter->setBrush(color);
    painter->drawRoundRect(option.rect, xRound, yRound);

	const QString text = index.model()->data(index, Qt::DisplayRole).toString();
    painter->setPen(Qt::black);
	painter->drawText(option.rect, Qt::AlignCenter, text);

    if (option.state.testFlag(QStyle::State_Selected)) 
	{
		const int width = 5;
        const QColor selectedColor = QColor::fromRgb(0, 0, 0, 255);
		const QPen pen(QBrush(selectedColor), width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
		painter->setPen(pen);
		painter->setBrush(Qt::NoBrush);
        painter->drawRoundRect(option.rect, xRound, yRound);
    }

    painter->restore();
}

QSize KanbanDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	auto size = QStyledItemDelegate::sizeHint(option, index);
	return QSize(size.width(), 4 * size.height());
}

QWidget* KanbanDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	KanbanDelegateEditor *editor = new KanbanDelegateEditor(parent);
	editor->setFocus();
	return editor;
}

void KanbanDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    KanbanDelegateEditor *delegateEditor = static_cast<KanbanDelegateEditor*>(editor);

	const QString text  = index.model()->data(index, Qt::DisplayRole).toString();
	const QColor color = index.model()->data(index, Qt::DecorationRole).value<QColor>();
	const QString state = index.model()->data(index, Roles::State).toString();

    delegateEditor->setText(text);
    delegateEditor->setColor(color);
    delegateEditor->setState(state);
}

void KanbanDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	KanbanDelegateEditor *delegateEditor = static_cast<KanbanDelegateEditor*>(editor);

    const QString text = delegateEditor->getText();
    const QColor color = delegateEditor->getColor();
    const QString state = delegateEditor->getState();

    model->setData(index, text, Qt::DisplayRole);
    model->setData(index, color, Qt::DecorationRole);
    model->setData(index, state, Roles::State);
}

void KanbanDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}
