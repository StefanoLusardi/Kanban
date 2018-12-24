#pragma once

#include <map>
#include <QWidget>

class QSplitter;
class KanbanModel;
class KanbanColumnView;
class QItemSelectionModel;

namespace Ui { class KanbanBoardView; }

class KanbanBoardView : public QWidget
{
	Q_OBJECT

public:
	KanbanBoardView(QWidget *parent = Q_NULLPTR);
	~KanbanBoardView();
	void setModel(KanbanModel* kanbanModel);

	void loadConfig();
	void saveConfig() const;

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
	void onDeleteColumnView(const QString& deletedColumnName);
	void onAddColumnViewKanban(const QString& columnName);
	void onKanbanSelected(const QString& columnSenderName, const QStringList& kanbanTextList);

private:
	Ui::KanbanBoardView *ui;
	KanbanModel* mKanbanModel;
	QItemSelectionModel* mSelectionKanbanModel;
	std::map<QString, KanbanColumnView*> mColumnViews;
	QSplitter *mColumnSplitter;
	QString mSelectedColumnName;

	//static QColor stringToColor(const QString& str);
	//static QString colorToString(const QColor& color);
	//static QString getUniqueName(const QString& name, const QStringList& allNames);

	QStringList getColumnViewNames() const;
	void setSelectedColumnView(const QString& selectedColumnName);
	void createColumn(const QString& columnName, const QColor& columnColor, bool isCollapsed = false);
	void createKanban(const QString& text, const QColor& color, const QString& state) const;
};
