#pragma once

#include "model_global.h"
#include "KanbanItem.h"

#include <QAbstractListModel>

class DbManager;

class MODEL_EXPORT KanbanModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum Roles {State = Qt::UserRole+1};

	KanbanModel(DbManager& db, int pageId, QObject *parent = Q_NULLPTR) : QAbstractListModel(parent), mDb{db}, mPageId{pageId} {}
	virtual ~KanbanModel() = default;

	Qt::ItemFlags flags(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;
	void removeKanbanForPage();
	void loadKanbanItems(int pageId);
	bool insertRows(int position, int rows, const QModelIndex& parent) override;
	bool removeRows(int row, int count, const QModelIndex& parent) override;

	Qt::DropActions supportedDropActions() const override;
	Qt::DropActions supportedDragActions() const override;
	QStringList mimeTypes() const override;
	QMimeData* mimeData(const QModelIndexList& indexes) const override;

	QModelIndex addKanban(const KanbanItem& kanbanItem);
	QModelIndex getKanbanIndex(const QString& kanbanTitle);

private:
	DbManager& mDb;
	const int mPageId;
	std::vector<KanbanItem> mKanbanItems;
};
