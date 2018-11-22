#pragma once

#include <QWidget>
#include <QParallelAnimationGroup>

class QSortFilterProxyModel;
class QStyledItemDelegate;

namespace Ui { class KanbanColumnView; }

class KanbanColumnView : public QWidget
{
    Q_OBJECT

public:
    explicit KanbanColumnView(const QString& title, QWidget *parent = nullptr);
    ~KanbanColumnView();
    QString getTitle() const;
	void setModel(QSortFilterProxyModel* model) const;
	void setDelegate(QStyledItemDelegate* delegate) const;

signals:
	void kanbanCreated(const QString& columnName);
    void columnDeleted(const QString& columnName);

protected:
	void setupListView() const;
    QString toVertical(const QString &title) const;
	void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    Ui::KanbanColumnView *ui;
    QParallelAnimationGroup mAnimation;
    int mMaxSize {300};
    int mMinSize {40};
    bool mIsCollapsed;

};
