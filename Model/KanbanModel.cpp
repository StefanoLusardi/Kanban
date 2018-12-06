#include "KanbanModel.h"
#include "Roles.h"

#include <QMimeData>
#include <QDataStream>

int KanbanModel::rowCount(const QModelIndex& parent) const
{
	return mKanbanItems.size();
}

QVariant KanbanModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid()) { return {}; }

	const KanbanItem& label = mKanbanItems[index.row()];

	switch(role)
	{
	case Qt::DisplayRole: return label.getText();
	case Qt::DecorationRole: return label.getColor();
	case Roles::State: return label.getState();
	default: return {};
	}
}

bool KanbanModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (!index.isValid()) {return false;}

	KanbanItem& label = mKanbanItems[index.row()];

	switch(role)
	{
	case Qt::DisplayRole: label.setText(value.toString()); break;
	case Qt::DecorationRole: label.setColor(value.value<QColor>()); break;
	case Roles::State: label.setState(value.toString()); break;
	default: break;
	}
	
	emit dataChanged(index, index);
	return true;
}

Qt::ItemFlags KanbanModel::flags(const QModelIndex& index) const
{
	const Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | Qt::ItemIsEditable | defaultFlags;

}

bool KanbanModel::removeRows(int row, int count, const QModelIndex& parent)
{
	if (row < 0 || row > rowCount() || count < 0 || (row+count) > rowCount()) { return false; }

	beginRemoveRows(parent, row, row+count-1);
	mKanbanItems.erase(mKanbanItems.begin()+row, mKanbanItems.begin()+row+count);
	endRemoveRows();
	return true;
}

Qt::DropActions KanbanModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions KanbanModel::supportedDragActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList KanbanModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    return types;
}

QMimeData *KanbanModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    for (const QModelIndex& index : indexes) 
	{
        if (index.isValid()) 
		{
            const QString text = data(index, Qt::DisplayRole).toString();
            const QColor color = data(index, Qt::DecorationRole).value<QColor>();
            const QString state = data(index, Roles::State).toString();
            stream << text << color << state;
        }
    }

    mimeData->setData("application/vnd.text.list", encodedData);
    return mimeData;
}

bool KanbanModel::canDropMimeData(const QMimeData *data, Qt::DropAction, int, int column, const QModelIndex&) const
{
	if (!data->hasFormat("application/vnd.text.list"))
        return false;

    if (column > 0)
        return false;

    return true;
}

bool KanbanModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
	if (!canDropMimeData(data, action, row, column, parent))
        return false;

	if (action == Qt::IgnoreAction)
		return true;

	if (!data->hasFormat("application/vnd.text.list"))
		return false;

	if (column > 0)
		return false;

	//QByteArray encodedData = data->data("application/vnd.text.list");
	//QDataStream stream(&encodedData, QIODevice::ReadOnly);

	//while (!stream.atEnd())
	//{
	//	QString text;
	//	QColor color;
	//	QString state;

	//	stream >> text >> color >> state;

	//	const QModelIndex idx = addKanban({text, color, state});
	//	setData(idx, text, Qt::DisplayRole);
	//	setData(idx, color, Qt::DecorationRole);
	//	setData(idx, state, Roles::State);
	//}

	return true;
}

QModelIndex KanbanModel::addKanban(const KanbanItem& label)
{
	const int row = rowCount();
	beginInsertRows(QModelIndex(), row, row);
	mKanbanItems.emplace_back(label);
	endInsertRows();
	return index(row, 0, QModelIndex());
}

bool KanbanModel::insertRows(int position, int rows, const QModelIndex &parent)
 {
     beginInsertRows(QModelIndex(), position, position+rows-1);
 
     for (int row = 0; row < rows; ++row) 
	 {
         mKanbanItems.emplace_back(QString());
     } 
     endInsertRows();
     return true;
 }

