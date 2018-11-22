#pragma once

#include "KanbanItem.h"

#include <QAbstractListModel>

class KanbanModel : public QAbstractListModel
{
	Q_OBJECT

public:
	KanbanModel(QObject *parent = Q_NULLPTR) : QAbstractListModel(parent) {}
	virtual ~KanbanModel() = default;

	Qt::ItemFlags flags(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;
	bool insertRows(int position, int rows, const QModelIndex& parent) override;
	bool removeRows(int row, int count, const QModelIndex& parent) override;

	bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const override;
	bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent) override;
	Qt::DropActions supportedDropActions() const override;
	Qt::DropActions supportedDragActions() const override;
	QStringList mimeTypes() const override;
	QMimeData* mimeData(const QModelIndexList& indexes) const override;

	QModelIndex addKanban(const KanbanItem& label);

private:
	std::vector<KanbanItem> mKanbanItems;
};
