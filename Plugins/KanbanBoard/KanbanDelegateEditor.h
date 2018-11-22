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
	void setState(const QString& state) const;
	void setColor(const QColor& color) const;

	QString getText() const;
	QString getState() const;
	QColor getColor() const;

private:
	Ui::KanbanDelegateEditor *ui;
};
