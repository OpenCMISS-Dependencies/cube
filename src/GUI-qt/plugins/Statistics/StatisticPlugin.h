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


#ifndef StatisticPlugin_H
#define StatisticPlugin_H
#include <QtGui>
#include "CubePlugin.h"
#include "PluginServices.h"

#include "Cube.h"
#include "CubeMetric.h"
class Statistics;

class StatisticPlugin : public QObject, CubePlugin
{
    Q_OBJECT
                               Q_INTERFACES( CubePlugin )
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA( IID "StatisticPlugin" )
#endif


public:
    virtual QString
    name() const;
    virtual bool
    cubeOpened( PluginServices* service );

    virtual void
    cubeClosed();

    void
    version( int& major,
             int& minor,
             int& bugfix ) const;

    QString
    getHelpText() const;

    virtual QString
    getDeactivationMessage();

private slots:
    void
    contextMenuIsShown( TreeType  type,
                        TreeItem* item );

    virtual void
    orderHasChanged( const QList<DisplayType>& order );

private slots:

    void
    onShowStatistics();
    void
    onShowMaxSeverityText();

private:
    PluginServices*       service;
    const TreeItemMarker* marker;
    TreeItem*             contextMenuItem;     // item on which the context menu has been invoked
    TreeType              contextMenuTreeType; // tree type of contextMenuItem
    Statistics*           statistics;
    QString               errorMsg;
};

#endif // StatisticPlugin_H
