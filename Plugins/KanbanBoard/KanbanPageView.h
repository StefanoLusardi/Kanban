#pragma once

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
	QString getPageName() const;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

public slots:
	void onCreateKanban(const QString& page);
	void onRenameKanban(const QString& page);
	void onDeleteKanban(const QString& page);

	void onCreateColumn(const QString& page);
	void onRenameColumn(const QString& page);
	void onDeleteColumn(const QString& page);

private slots:
	void onDeleteColumnView(const QString& deletedColumnName);
	void onAddColumnViewKanban(const QString& columnName);
	void onKanbanSelected(const QString& columnSenderName, const QStringList& kanbanTextList);

private:
	Ui::KanbanPageView *ui;
	QString mPageName;
	KanbanItemModel* mKanbanModel;
	QItemSelectionModel* mSelectionKanbanModel;
	std::vector<KanbanColumnView*> mColumnViews;
	QSplitter *mSplitterColumnViews;
	QString mSelectedColumnName;

	QStringList getColumnViewNames() const;
	void setModel(KanbanItemModel* kanbanModel);
	void setSelectedColumnView(const QString& selectedColumnName);
	void createColumn(const QString& columnName, const QColor& columnColor, bool isCollapsed = false);
	void createKanban(const QString& text, const QColor& color, const QString& columnName) const;

	std::vector<KanbanColumnView*>::iterator findColumn(const QString& columnName);
};
