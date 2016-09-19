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


#ifndef CONTEXTFREESERVICES_H
#define CONTEXTFREESERVICES_H

#include <QWidget>
#include "Cube.h"

class ContextFreeServices : public QObject
{
    Q_OBJECT
public:
    static ContextFreeServices*
    getInstance();

    /** closes this plugin and loads the given cube */
    void
    openCube( cube::Cube* cube );

    /** returns a widget on the main screen which can be used by the plugin. If the widget is closed, all
     * children of the widget are deleted.
     */
    QWidget*
    getWidget();

    friend class MainWidget;
signals:
    void
    openCubeRequest( cube::Cube* cube );

private:
    ContextFreeServices();
    void
    setWidget( QWidget* ); // called by MainWidget

    static ContextFreeServices* single;
    QWidget*                    widget;
};

#endif // CONTEXTFREESERVICES_H
