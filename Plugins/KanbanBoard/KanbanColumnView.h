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
    explicit KanbanColumnView(const QString& title, const QColor& columnColor, QWidget *parent = nullptr);
    ~KanbanColumnView();
    
	void setTitle(const QString& title);
	QString getTitle() const;

	void setColor(const QColor& color);
	QColor getColor() const;

	void setModel(QSortFilterProxyModel* model) const;
	void setDelegate(QStyledItemDelegate* delegate) const;

signals:
	void kanbanCreated(const QString& columnName);
    void columnDeleted(const QString& columnName);

protected:
	void setupListView() const;
	void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    Ui::KanbanColumnView *ui;
    QParallelAnimationGroup mAnimation;
	int mAnimationTime {2000};
    int mMaxSize {300};
    int mMinSize {40};
    bool mIsCollapsed;

};
