#include "KanbanColumnProxyModel.h"
#include "Model.h"

#include <QMimeData>
#include <QDataStream>

bool KanbanColumnProxyModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	int beginRow;
    if (row != -1) beginRow = row;
	else if (parent.isValid()) beginRow = parent.row();
	else beginRow = rowCount(QModelIndex());

	//QByteArray encodedData = data->data("application/vnd.text.list");
	QByteArray encodedData = data->data(sourceModel()->mimeTypes().first());
	QDataStream stream(&encodedData, QIODevice::ReadOnly);

	std::vector<KanbanItem> kanbanItems;
	while (!stream.atEnd())
	{
		QString text;
		QString color;
		QString columnName;

		stream >> text >> color >> columnName;
		kanbanItems.emplace_back(-1, text, color, mName);
	}

	auto model = static_cast<KanbanItemModel*>(sourceModel());
	std::reverse(kanbanItems.begin(), kanbanItems.end());
	model->addKanbanItems(beginRow, kanbanItems);

	/*
	 * This is another implementation of the Drag&Drop mechanism.
	 * it's "more general": it does NOT require an explicit cast to the source model
	 * it's "less performing": first insert rows, THEN set their data.
	 */
	/*
	model->insertRows(beginRow, itemsToAdd.size(), QModelIndex());
	for (auto&& item : itemsToAdd)
	{
        QModelIndex idx = model->index(beginRow, 0, QModelIndex());
		model->setData(idx, item.getText(), Qt::DisplayRole);
		model->setData(idx, item.getColor(), Qt::DecorationRole);
		model->setData(idx, mName, KanbanItemModel::Roles::ColumnName);
        beginRow++;
	}
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