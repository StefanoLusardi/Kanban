#include "KanbanDelegateEditor.h"
#include "KanbanDelegate.h"
#include "Model.h"

#include <QPainter>


void KanbanDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{ 
	painter->setRenderHint(QPainter::Antialiasing); 
	painter->save();

	// Draw Kanban Item rectangle (external contour)
	QPainterPath itemPath;
	const qreal radius = option.rect.height() * 0.25;
	const qreal penWidth = 2;
	itemPath.addRoundedRect(option.rect, radius, radius);
	painter->setPen({Qt::black, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin});
	painter->drawPath(itemPath);

	// Draw Kanban Item rectangle (internal background)
	QLinearGradient itemBackground(option.rect.topLeft(), option.rect.bottomRight());
	const QColor itemColor = index.model()->data(index, Qt::DecorationRole).value<QColor>();
	itemBackground.setColorAt(0.3, itemColor);
	itemBackground.setColorAt(1.0, Qt::white);
	painter->fillPath(itemPath, itemBackground);

	// Draw the text
	const QString text = index.model()->data(index, Qt::DisplayRole).toString();
    painter->setPen(Qt::black);
	painter->drawText(option.rect, Qt::AlignCenter, text);

	// Draw contour (item selected or mouse over)
	if (option.state.testFlag(QStyle::State_Selected) || option.state.testFlag(QStyle::State_MouseOver)) 
	{
		QPainterPath selectedPath;
		selectedPath.addRoundedRect(option.rect, radius, radius);
		const qreal penWidthSelected = 5;

        QColor contourColor;		
		if (option.state.testFlag(QStyle::State_Selected))
		{
			contourColor = QColor::fromRgb(255, 32, 32, 255);
		}
		else
		{
			contourColor  = QColor::fromRgb(255, 128, 64, 255);
		}

		painter->setPen({QBrush(contourColor), penWidthSelected, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin});
		painter->drawPath(selectedPath);
    }

    painter->restore();
}

QSize KanbanDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	const auto size = QStyledItemDelegate::sizeHint(option, index);
	return QSize(size.width(), 3 * size.height());
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
