#include "KanbanColumnProxyModel.h"
#include "Model.h"

#include <QMimeData>
#include <QDataStream>

bool KanbanColumnProxyModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	QByteArray encodedData = data->data("application/vnd.text.list");
	QDataStream stream(&encodedData, QIODevice::ReadOnly);

	bool isInserted = false;

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
	auto idxList = model->addKanbanList(itemsToAdd, mName);

	// if at least one item is inserted and is valid notify the view
	if (std::any_of(idxList.begin(), idxList.end(), [](const QModelIndex& idx) { return idx.isValid(); }))
		isInserted = true;

	/*
	auto model = static_cast<KanbanItemModel*>(sourceModel());
	for (auto&& item : itemsToAdd)
	{
		const QModelIndex idx = model->addKanban(item.getText(), item.getColor(), mName);

		// if at least one item is inserted
		if (idx.isValid())
			isInserted = true;
	}

	if (isInserted)
	{
		QtConcurrent::run([=]()
		{
			auto model = static_cast<KanbanItemModel*>(sourceModel());
			for (auto&& item : itemsToAdd)
			{
				model->addKanbanDB(item.getText(), item.getColor(), mName);
			}
		});
	}
	*/
	
	return isInserted;
}

bool KanbanColumnProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
	const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
	const QString state = sourceModel()->data(index, KanbanItemModel::Roles::ColumnName).toString();
	return state.contains(filterRegExp());
}
