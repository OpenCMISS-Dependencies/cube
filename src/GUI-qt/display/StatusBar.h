/****************************************************************************
**  CUBE        http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 1998-2016                                                **
**  Forschungszentrum Juelich GmbH, Juelich Supercomputing Centre          **
**                                                                         **
**  This software may be modified and distributed under the terms of       **
**  a BSD-style license.  See the COPYING file in the package base         **
**  directory for details.                                                 **
****************************************************************************/



#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QStackedWidget>
#include <QSplitter>
#include "Globals.h"
#include "VersionCheckWidget.h"

/**
 * @brief The StatusBar class creates a one row status bar. By clicking on the bar, it shows a history of
 * messages.
 */
class StatusBar : public QWidget
{
    Q_OBJECT
public:
    StatusBar();
    /** Sets the text of the status bar and adds one line to the status bar history. Depending on the type,
     * the message will be shown in signal colors.
     */
    void
    addLine( const QString& line,
             MessageType    type );

    /** adds a widget next to the status bar */
    void
    addPermanentWidget( QWidget* );

signals:
    void
    minimized( bool );

protected:
    void
    mousePressEvent( QMouseEvent* event );
    void
    focusOutEvent( QFocusEvent* event );

private:
    QWidget*     oneRow;
    QHBoxLayout* rowLayout;
    QLabel*      label;
    QTextEdit*   textView;
    QWidget*     multiRow;
};


/**
 * @brief The StatusBarWidget class creates a widget which is a container for a widget and a status bar below it. If
 * the status bar isn't minimzed, a QSplitter is used to allow the user to resize the status bar.
 */
class StatusBarWidget : public QWidget
{
    Q_OBJECT
public:
    StatusBarWidget();
    /** sets the main widget, the status bar is placed below */
    void
    setWidget( QWidget* widget );

    StatusBar*
    getStatusBar();

private slots:
    void
    statusBarIsMinimized( bool mini );

private:
    QSplitter* splitter;
    QWidget*   widget;
    StatusBar* statusBar;
    QByteArray splitterSizes;
    bool       isMini;
};


#endif
