#include "KanbanColumnProxyModel.h"
#include "Model.h"

#include <QMimeData>
#include <QDataStream>


bool KanbanColumnProxyModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	QByteArray encodedData = data->data("application/vnd.text.list");
	QDataStream stream(&encodedData, QIODevice::ReadOnly);

	while (!stream.atEnd())
	{
		QString text;
		QString color;
		QString state;

		stream >> text >> color >> state;

		auto model = static_cast<KanbanModel*>(sourceModel());
		const QModelIndex idx = model->addKanban({text, color, mName});
		sourceModel()->setData(idx, text, Qt::DisplayRole);
		sourceModel()->setData(idx, color, Qt::DecorationRole);
		sourceModel()->setData(idx, mName, KanbanModel::Roles::State);
	}

	return true;
}

bool KanbanColumnProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
	const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
	const QString state = sourceModel()->data(index, KanbanModel::Roles::State).toString();
	return state.contains(filterRegExp());
}
