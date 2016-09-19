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


#ifndef TREEITEMMARKERSETTINGS_H
#define TREEITEMMARKERSETTINGS_H

#include <QDialog>
#include <QList>
#include <QLabel>

class TabManager;
class TreeItemMarker;

/**
 * @brief The TreeItemMarkerDialog class creates a dialog which allows to modify the automatically assigned
 * tree item marker attributes (background color, show icon/background).
 */
class TreeItemMarkerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit
    TreeItemMarkerDialog( TabManager*,
                          QWidget* parent = 0 );

public slots:
    void
    accept();
    void
    reject();
    void
    apply();
    void
    grayItems( bool enabled );

private slots:
    void
    enableColor( bool enabled );
    void
    enableIcon( bool enabled );
    void
    chooseColor();

private:
    QList<TreeItemMarker*> itemMarker;
    QList<QPushButton*>    buttons;
    QList<QColor>          colors;
    QList<bool>            iconIsVisible;
    QList<bool>            colorIsVisible;
    TabManager*            tabManager;
};

#endif // TREEITEMMARKERSETTINGS_H
