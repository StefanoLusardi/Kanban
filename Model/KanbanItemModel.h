#pragma once

#include "model_global.h"
#include "KanbanItem.h"

#include <QAbstractListModel>
#include <QMutexLocker>

class DbManager;

class MODEL_EXPORT KanbanItemModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum Roles {ColumnName = Qt::UserRole+1};

	KanbanItemModel(DbManager& db, int pageId, QObject *parent = Q_NULLPTR) : QAbstractListModel(parent), mDb{db}, mPageId{pageId}, mMutex{new QMutex()} {}
	virtual ~KanbanItemModel() = default;

	QHash<int, QByteArray> roleNames() const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;
	bool insertRows(int position, int rows, const QModelIndex& parent) override;
	bool removeRows(int row, int count, const QModelIndex& parent) override;
	//bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild) override;

	Qt::DropActions supportedDropActions() const override;
	Qt::DropActions supportedDragActions() const override;
	QStringList mimeTypes() const override;
	QMimeData* mimeData(const QModelIndexList& indexes) const override;

	QModelIndexList addKanbanList(std::vector<KanbanItem> kanbanItemsToInsert, const QString& columnName);
	QModelIndex addKanban(KanbanItem&& item);
	QModelIndex getKanbanIndex(const QString& kanbanText);

	void loadKanbanItems();
	void saveKanbanItems();
	void removeAllItems() const;

private:
	DbManager& mDb;
	const int mPageId;
	std::vector<KanbanItem> mKanbanItems;
	QMutex* mMutex;
};
