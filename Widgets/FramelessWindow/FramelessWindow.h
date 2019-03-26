#pragma once

/*
###############################################################################
#                                                                             #
# The MIT License                                                             #
#                                                                             #
# Copyright (C) 2017 by Juergen Skrotzky (JorgenVikingGod@gmail.com)          #
#               >> https://github.com/Jorgen-VikingGod                        #
#                                                                             #
# Sources: https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle  #
#                                                                             #
###############################################################################
*/

#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QtCore/qglobal.h>
#if defined API_FRAMELESSWINDOW
	#define API Q_DECL_EXPORT
#else
	#define API Q_DECL_IMPORT
#endif

#include <QtWidgets/QWidget>

namespace Ui { class FramelessWindow; }

class API FramelessWindow : public QWidget
{
	Q_OBJECT

public:
	explicit FramelessWindow(QWidget* parent = Q_NULLPTR);
	virtual ~FramelessWindow();
	void setContent(QWidget* w);

private:
	bool leftBorderHit(const QPoint& pos);
	bool rightBorderHit(const QPoint& pos);
	bool topBorderHit(const QPoint& pos);
	bool bottomBorderHit(const QPoint& pos);
	void styleWindow(bool bActive, bool bNoState);

signals:
	void aboutToClose();

public slots:
	void setWindowTitle(const QString& text);
	void setWindowIcon(const QIcon& ico);
	void changeStyle(const QString& styleName);

private slots:
	void on_applicationStateChanged(Qt::ApplicationState state);
	void on_minimizeButton_clicked();
	void on_restoreButton_clicked();
	void on_maximizeButton_clicked();
	void on_closeButton_clicked();
	void on_windowTitlebar_doubleClicked();

protected:
	virtual void changeEvent(QEvent* event);
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void checkBorderDragging(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual bool eventFilter(QObject* obj, QEvent* event);
	virtual void closeEvent (QCloseEvent *event);

private:
	Ui::FramelessWindow* ui;
	QRect m_StartGeometry;
	const quint8 CONST_DRAG_BORDER_SIZE = 15;
	bool m_bMousePressed;
	bool m_bDragTop;
	bool m_bDragLeft;
	bool m_bDragRight;
	bool m_bDragBottom;
};

#endif  // FRAMELESSWINDOW_H
