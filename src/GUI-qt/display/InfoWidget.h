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


#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QAction>
#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QTextBrowser>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QDialog>

/**
 * @brief The InfoWidget class
 * The InfoWidget class displays a textual message in a tabbed widget.
 */
class InfoWidget : public QDialog
{
    Q_OBJECT
public:
    explicit
    InfoWidget( QWidget* parent = 0 );

    /**
       showText displays the given text in a tabbed widget.
       It creates a new tab with the given title or updates an existing tab.
     */
    void
    showText( const QString& titel,
              const QString& text );

protected:
    virtual void
    closeEvent( QCloseEvent* event );

private slots:
    void
    deleteTab( int );

    void
    closeWidget();

private:
    QTabWidget* tabWidget;
    QRect       lastGeometry;
};

#endif // INFOWIDGET_H
