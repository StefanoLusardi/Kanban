#include "KanbanItemModel.h"
#include "DbManager.h"

#include <QDebug>
#include <QMimeData>
#include <QDataStream>

QHash<int, QByteArray> KanbanItemModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "text";
    roles[Qt::DecorationRole] = "color";
    roles[Roles::ColumnName] = "column";
    return roles;
}

int KanbanItemModel::rowCount(const QModelIndex& parent) const { return mKanbanItems.size(); }

QVariant KanbanItemModel::data(const QModelIndex& index, int role) const
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

bool KanbanItemModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (!index.isValid()) { return false; }

	KanbanItem& item = mKanbanItems[index.row()];

	switch (role)
	{
	case Qt::DisplayRole : 
		item.setText(value.toString()); 
		break;

	case Qt::DecorationRole : 
		item.setColor(value.toString()); 
		break;

	case Roles::ColumnName : 
		item.setColumn(value.toString()); 
		break;

	default : assert(0);
	}

	emit dataChanged(index, index);
	mDb.mManagerKanbanItem.setData(item.getId(), roleNames()[role], value); 
	return true;
}

Qt::ItemFlags KanbanItemModel::flags(const QModelIndex& index) const
{
	const Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
	if (index.isValid())
		return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | defaultFlags;
	else
		return Qt::ItemIsDropEnabled | Qt::ItemIsEditable | defaultFlags;
}

bool KanbanItemModel::insertRows(int position, int rows, const QModelIndex& parent)
{
	// TODO: check if it used. Deprecated
	beginInsertRows(QModelIndex(), position, position + rows - 1);
	
	qDebug() << "Rows Inserted: " << position;

	//for (int row = 0; row < rows; ++row) { mKanbanItems.emplace_back(KanbanItem(0)); }
	endInsertRows();
	return true;
}

bool KanbanItemModel::removeRows(int row, int count, const QModelIndex& parent)
{
	if (row < 0 || row > rowCount() || count < 0 || (row + count) > rowCount()) { return false; }

	beginRemoveRows(parent, row, row + count - 1);

	int countLeft = count;
	while (countLeft--)
	{
		const KanbanItem& kanbanItem = mKanbanItems.at(row + countLeft);
		mDb.mManagerKanbanItem.removeItem(kanbanItem.getId());
	}

	mKanbanItems.erase(mKanbanItems.begin() + row, mKanbanItems.begin() + row + count);
	endRemoveRows();
	return true;
}

Qt::DropActions KanbanItemModel::supportedDropActions() const { return /*Qt::CopyAction |*/ Qt::MoveAction; }

Qt::DropActions KanbanItemModel::supportedDragActions() const { return /*Qt::CopyAction |*/ Qt::MoveAction; }

QStringList KanbanItemModel::mimeTypes() const { return {"application/vnd.text.list"}; }

QMimeData* KanbanItemModel::mimeData(const QModelIndexList& indexes) const
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

QModelIndex KanbanItemModel::addKanban(const QString& text, const QString& color, const QString& columnName)
{
	KanbanItem item(mPageId, text, color, columnName);
	const auto alreadyExists = std::any_of(mKanbanItems.begin(), mKanbanItems.end(), [item](const KanbanItem& it)
	{
		return it == item;
	});

	if (alreadyExists) return {};
	
	const int row = rowCount();
	beginInsertRows(QModelIndex(), row, row);

	mDb.mManagerKanbanItem.insertItem(item);

	mKanbanItems.emplace_back(item);
	endInsertRows();
	return index(row, 0, QModelIndex());
	
}

QModelIndex KanbanItemModel::getKanbanIndex(const QString& kanbanText)
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

void KanbanItemModel::removeAllItems() const
{
	mDb.mManagerKanbanItem.removeAllItems(mPageId);
}

void KanbanItemModel::loadKanbanItems()
{
	if (mPageId < 0)
	{
		mKanbanItems.clear();
		return;
	}

	beginResetModel();
	mKanbanItems = mDb.mManagerKanbanItem.getItems(mPageId);
	endResetModel();
}

void KanbanItemModel::saveKanbanItems()
{
	if (mPageId < 0)
	{
		return;
	}
}
