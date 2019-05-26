#include "KanbanColumnProxyModel.h"
#include "Model.h"

#include <QMimeData>
#include <QDataStream>

bool KanbanColumnProxyModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	int beginRow;

    if (row != -1)
        beginRow = row;
	else if (parent.isValid())
        beginRow = parent.row();
	else
        beginRow = rowCount(QModelIndex());

	QByteArray encodedData = data->data("application/vnd.text.list");
	QDataStream stream(&encodedData, QIODevice::ReadOnly);

	std::vector<KanbanItem> itemsToAdd;
	while (!stream.atEnd())
	{
		QString text;
		QString color;
		QString columnName;

		stream >> text >> color >> columnName;

		KanbanItem item(-1, text, color, mName);
		itemsToAdd.push_back(item);
	}

	auto model = static_cast<KanbanItemModel*>(sourceModel());
	model->insertRows(beginRow, itemsToAdd.size(), QModelIndex());
	for (auto item : itemsToAdd)
	{
        QModelIndex idx = model->index(beginRow, 0, QModelIndex());
		model->setData(idx, item.getText(), Qt::DisplayRole);
		model->setData(idx, item.getColor(), Qt::DecorationRole);
		model->setData(idx, mName, KanbanItemModel::Roles::ColumnName);
        beginRow++;
	}

	/*
	auto idxList = model->addKanbanList(itemsToAdd, mName);

	// if at least one item is inserted and it is valid, notify the view
	bool isInserted = false;
	if (std::any_of(idxList.begin(), idxList.end(), [](const QModelIndex& idx) { return idx.isValid(); }))
		isInserted = true;

	return isInserted;
	*/

	return true;
}

bool KanbanColumnProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
	const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
	const QString state = sourceModel()->data(index, KanbanItemModel::Roles::ColumnName).toString();
	return state.contains(filterRegExp());
}

Qt::DropActions KanbanColumnProxyModel::supportedDragActions() const
{
	return Qt::MoveAction;
}

Qt::DropActions KanbanColumnProxyModel::supportedDropActions() const
{
	return Qt::MoveAction;
}

Qt::ItemFlags KanbanColumnProxyModel::flags(const QModelIndex& index) const
{
	const Qt::ItemFlags defaultFlags = QSortFilterProxyModel::flags(index);
	if(index.isValid())
		return defaultFlags | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable;
	else
		return defaultFlags | Qt::ItemIsDropEnabled;
}