#pragma once

#include <QColor>
#include <QString>
#include <QWidget>

namespace Ui { class KanbanDelegateEditor; }

class KanbanDelegateEditor : public QWidget
{
	Q_OBJECT

public:
	KanbanDelegateEditor(QWidget *parent = Q_NULLPTR);
	~KanbanDelegateEditor();

	void setText(const QString& text) const;
	void setColumn(const QString& column) const;
	void setColor(const QColor& color) const;

	QString getText() const;
	QString getColumn() const;
	QColor getColor() const;

signals:
	void editingFinished();

private:
	Ui::KanbanDelegateEditor *ui;
};
