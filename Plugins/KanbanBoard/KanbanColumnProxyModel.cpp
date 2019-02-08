#include "KanbanColumnProxyModel.h"
#include "Model.h"

#include <QMimeData>
#include <QDataStream>


bool KanbanColumnProxyModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	QByteArray encodedData = data->data("application/vnd.text.list");
	QDataStream stream(&encodedData, QIODevice::ReadOnly);

	bool isInserted = true;
	while (!stream.atEnd())
	{
		QString text;
		QString color;
		QString columnName;

		stream >> text >> color >> columnName;

		auto model = static_cast<KanbanItemModel*>(sourceModel());
		const QModelIndex idx = model->addKanban(text, color, mName);

		if (!idx.isValid())
			isInserted = false;
	}

	return isInserted;
}

bool KanbanColumnProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
	const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
	const QString state = sourceModel()->data(index, KanbanItemModel::Roles::ColumnName).toString();
	return state.contains(filterRegExp());
}
