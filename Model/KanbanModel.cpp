#include "KanbanModel.h"
#include "DbManager.h"

#include <QMimeData>
#include <QDataStream>

int KanbanModel::rowCount(const QModelIndex& parent) const { return mKanbanItems.size(); }

QVariant KanbanModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid()) { return {}; }

	const KanbanItem& label = mKanbanItems[index.row()];

	switch (role)
	{
	case Qt::DisplayRole : return label.getText();
	case Qt::DecorationRole : return label.getColor();
	case Roles::ColumnName : return label.getColumn();
	default : return {};
	}
}

bool KanbanModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (!index.isValid()) { return false; }

	KanbanItem& label = mKanbanItems[index.row()];

	switch (role)
	{
	case Qt::DisplayRole : label.setText(value.toString()); break;
	case Qt::DecorationRole : label.setColor(value.toString()); break;
	case Roles::ColumnName : label.setColumn(value.toString()); break;
	default : break;
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

bool KanbanModel::insertRows(int position, int rows, const QModelIndex& parent)
{
	// TODO: check if it used. Deprecated
	beginInsertRows(QModelIndex(), position, position + rows - 1);

	for (int row = 0; row < rows; ++row) { mKanbanItems.emplace_back(KanbanItem(0)); }
	endInsertRows();
	return true;
}

bool KanbanModel::removeRows(int row, int count, const QModelIndex& parent)
{
	if (row < 0 || row > rowCount() || count < 0 || (row + count) > rowCount()) { return false; }

	beginRemoveRows(parent, row, row + count - 1);

	int countLeft = count;
	while (countLeft--)
	{
		const KanbanItem& kanbanItem = mKanbanItems.at(row + countLeft);
		mDb.kanbanItemDbHandler.removeKanbanItem(kanbanItem.getId());
	}

	mKanbanItems.erase(mKanbanItems.begin() + row, mKanbanItems.begin() + row + count);
	endRemoveRows();
	return true;
}

Qt::DropActions KanbanModel::supportedDropActions() const { return /*Qt::CopyAction |*/ Qt::MoveAction; }

Qt::DropActions KanbanModel::supportedDragActions() const { return /*Qt::CopyAction |*/ Qt::MoveAction; }

QStringList KanbanModel::mimeTypes() const { return {"application/vnd.text.list"}; }

QMimeData* KanbanModel::mimeData(const QModelIndexList& indexes) const
{
	QMimeData* mimeData = new QMimeData();
	QByteArray encodedData;

	QDataStream stream(&encodedData, QIODevice::WriteOnly);

	for (const QModelIndex& index : indexes)
	{
		if (index.isValid())
		{
			const QString text = data(index, Qt::DisplayRole).toString();
			const QString color = data(index, Qt::DecorationRole).toString();
			const QString columnName = data(index, Roles::ColumnName).toString();

			stream << text << color << columnName;
		}
	}

	mimeData->setData("application/vnd.text.list", encodedData);
	return mimeData;
}

//QModelIndex KanbanModel::addKanban(const KanbanItem& kanbanItem)
QModelIndex KanbanModel::addKanban(const QString& text, const QString& color, const QString& columnName)
{
	const int row = rowCount();
	beginInsertRows(QModelIndex(), row, row);

	KanbanItem kanbanItem(mPageId, text, color, columnName);
	mDb.kanbanItemDbHandler.addKanban(kanbanItem);

	mKanbanItems.emplace_back(kanbanItem);
	endInsertRows();
	return index(row, 0, QModelIndex());
}

QModelIndex KanbanModel::getKanbanIndex(const QString& kanbanText)
{
	const auto item = std::find_if(mKanbanItems.begin(), mKanbanItems.end(),
									[&kanbanText](const KanbanItem& k) { return kanbanText == k.getText(); });

	if (item != mKanbanItems.end())
	{
		const int row = std::distance(mKanbanItems.begin(), item);
		return index(row);
	}

	return {};
}

void KanbanModel::removeKanbanForPage()
{
	mDb.kanbanItemDbHandler.removeKanbanItemsForPage(mPageId);
}

void KanbanModel::loadKanbanItems()
{
	if (mPageId < 0)
	{
		mKanbanItems.clear();
		return;
	}

	beginResetModel();
	mKanbanItems = mDb.kanbanItemDbHandler.getKanbanItemsForPage(mPageId);
	endResetModel();
}

void KanbanModel::saveKanbanItems()
{
	if (mPageId < 0)
	{
		return;
	}

	//beginResetModel();
	//mKanbanItems = mDb.kanbanItemDbHandler.getKanbanItemsForPage(pageId);
	//endResetModel();
}
