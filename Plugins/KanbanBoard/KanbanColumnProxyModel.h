#pragma once

#include <QSortFilterProxyModel>

class KanbanColumnProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	KanbanColumnProxyModel(const QString& name, QObject* parent = Q_NULLPTR) : QSortFilterProxyModel(parent), mName{name} {}
	~KanbanColumnProxyModel() = default;
	bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)	override;

protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
	const QString mName;
};

