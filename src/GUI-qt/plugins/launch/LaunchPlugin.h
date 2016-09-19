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


#ifndef LAUNCHPLUGIN_H
#define LAUNCHPLUGIN_H
#include <QtGui>
#include "CubePlugin.h"
#include "PluginServices.h"
#include "LaunchInfo.h"



class LaunchPlugin : public QObject, CubePlugin
{
    Q_OBJECT
                            Q_INTERFACES( CubePlugin )
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA( IID "LaunchPlugin" )
#endif


public:
    virtual QString
    name() const;

    virtual bool
    cubeOpened( PluginServices* service );

    virtual void
    cubeClosed();

    virtual void
    version( int& major,
             int& minor,
             int& bugfix ) const;

    virtual QString
    getHelpText() const;

private slots:
    void
    contextMenuIsShown( TreeType  type,
                        TreeItem* item );
    virtual void
    orderHasChanged( const QList<DisplayType>& order );

private slots:
    void
    onLaunch();

private:
    QList<LaunchInfo*>                      launchInfoList;
    PluginServices*                         service;
    QHash<int, QPair<TreeType, TreeItem*> > contextHash;
    const TreeItemMarker*                   marker;
};

#endif // LAUNCHPLUGIN_H
