#pragma once

#include <map>
#include <QWidget>

class Model;
class QSplitter;
class KanbanItemModel;
class KanbanColumnView;
class QItemSelectionModel;

namespace Ui { class KanbanPageView; }

class KanbanPageView : public QWidget
{
	Q_OBJECT

public:
	KanbanPageView(const QString& pageName, KanbanItemModel* model, QWidget *parent = Q_NULLPTR);
	~KanbanPageView();

	void loadConfig(const QJsonObject& pageConfig);
	void saveConfig(QJsonArray& config) const;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
	void onCreateKanban();
	void onRenameKanban();
	void onDeleteKanban();

	void onCreateColumn();
	void onRenameColumn();
	void onDeleteColumn();

	void onChangeColumn();
	void onDeleteColumnView(const QString& deletedColumnName);
	void onAddColumnViewKanban(const QString& columnName);
	void onKanbanSelected(const QString& columnSenderName, const QStringList& kanbanTextList);

private:
	Ui::KanbanPageView *ui;
	QString mPageName;
	KanbanItemModel* mKanbanModel;
	QItemSelectionModel* mSelectionKanbanModel;
	std::map<QString, KanbanColumnView*> mColumnViews;
	QSplitter *mSplitterColumnViews;
	QString mSelectedColumnName;

	QStringList getColumnViewNames() const;
	void setModel(KanbanItemModel* kanbanModel);
	void setSelectedColumnView(const QString& selectedColumnName);
	void createColumn(const QString& columnName, const QColor& columnColor, bool isCollapsed = false);
	void createKanban(const QString& text, const QColor& color, const QString& columnName) const;
};
