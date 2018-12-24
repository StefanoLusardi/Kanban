#include "KanbanModel.h"
#include "DbManager.h"

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
	//case Qt::DecorationRole: label.setColor(value.value<QColor>()); break;
	case Qt::DecorationRole: label.setColor(value.toString()); break;
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
    
    int countLeft = count;
	while(countLeft--) 
	{
        const KanbanItem& kanbanItem = mKanbanItems.at(row + countLeft);
        mDb.kanbanItemDbHandler.removeKanban(kanbanItem.getId());
    }
	
	mKanbanItems.erase(mKanbanItems.begin()+row, mKanbanItems.begin()+row+count);
	endRemoveRows();
	return true;
}

Qt::DropActions KanbanModel::supportedDropActions() const
{
    return /*Qt::CopyAction |*/ Qt::MoveAction;
}

Qt::DropActions KanbanModel::supportedDragActions() const
{
    return /*Qt::CopyAction |*/ Qt::MoveAction;
}

QStringList KanbanModel::mimeTypes() const
{
	return {"application/vnd.text.list"};
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
            const QString color = data(index, Qt::DecorationRole).toString();
            //const QColor color = data(index, Qt::DecorationRole).value<QColor>();
            const QString state = data(index, Roles::State).toString();
            stream << text << color << state;
        }
    }

    mimeData->setData("application/vnd.text.list", encodedData);
    return mimeData;
}

QModelIndex KanbanModel::addKanban(const KanbanItem& kanbanItem)
{
	const int row = rowCount();
	beginInsertRows(QModelIndex(), row, row);

	//std::unique_ptr<KanbanItem> kanbanItemDb(new KanbanItem(kanbanItem));
	KanbanItem kanbanItemDb(kanbanItem);
    mDb.kanbanItemDbHandler.addKanbanToPage(mPageId, kanbanItemDb);

	mKanbanItems.emplace_back(kanbanItemDb);
	endInsertRows();
	return index(row, 0, QModelIndex());
}

QModelIndex KanbanModel::getKanbanIndex(const QString& text)
{
	const auto item = std::find_if(mKanbanItems.begin(), mKanbanItems.end(), [&text](const KanbanItem& k){return text == k.getText();});
	
	if (item != mKanbanItems.end())
	{
		const int row = std::distance(mKanbanItems.begin(), item);
		return index(row);
	}
	
	return {};
}

void KanbanModel::removeKanbanForPage()
{
    mDb.kanbanItemDbHandler.removeKanbanForPage(mPageId);
    // mPageId = -1;
}

void KanbanModel::loadKanbanItems(int pageId)
{
    if (pageId < 0) 
	{
        mKanbanItems.clear();
        return;
    }

	beginResetModel();
    mKanbanItems = mDb.kanbanItemDbHandler.getKanbanItemsForPage(pageId);
	endResetModel();
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

