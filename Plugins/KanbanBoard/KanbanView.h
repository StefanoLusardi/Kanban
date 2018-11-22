#pragma once

#include <QWidget>

class KanbanModel;
class KanbanColumnView;
class QItemSelectionModel;

namespace Ui { class KanbanView; }

class KanbanView : public QWidget
{
	Q_OBJECT

public:
	KanbanView(QWidget *parent = Q_NULLPTR);
	~KanbanView();
	void loadData();
	void setModel(KanbanModel* kanbanModel);
	void setSelectionModel(QItemSelectionModel* selectionKanbanModel);	

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
	void onCreateColumn();
	void onCreateKanban();
	void onRenameKanban();
	void onDeleteKanban();
	void onChangeState();
	void onDeleteColumnView(const QString& columnName);
	void onAddColumnViewKanban(const QString& columnName);

private:
	Ui::KanbanView *ui;
	KanbanModel* mKanbanModel;
	QItemSelectionModel* mSelectionKanbanModel;
	std::map<QString, KanbanColumnView*> mColumnViews;

	// TODO: Keep track of the last selected item across all the columns
	//QPersistentModelIndex* lastSelectedIdx;

	QString getUniqueName(const QString& name) const;
	void createColumn(const QString& columnName);
	void createKanban(const QString& text, const QColor& color, const QString& state) const;
};
