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


#include <QThread>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <cassert>
#include <QDebug>
#include <iostream>

#include "CubeServices.h"
#include "VampirPlugin.h"
#include "VampirConnecter.h"
#include "VampirConnectionDialog.h"

using namespace std;

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( VampirPlugin, VampirPlugin );
#endif



QString
VampirPlugin::name() const
{
    return "VampirPlugin";
}

void
VampirPlugin::version( int& major, int& minor, int& bugfix ) const
{
    major  = 1;
    minor  = 0;
    bugfix = 0;
}

bool
VampirPlugin::cubeOpened( PluginServices* service )
{
    this->service   = service;
    contextMenuItem = 0;

    if ( !QFile( service->getStatName() ).exists() )
    {
        errorMsg = service->getStatName() + " cannot be opened.";
        return false; // statistics file doesn't exist
    }

    connect( service, SIGNAL( contextMenuIsShown( TreeType, TreeItem* ) ),
             this, SLOT( contextMenuIsShown( TreeType, TreeItem* ) ) );
    connect( service, SIGNAL( globalValueChanged( QString ) ),
             this, SLOT( globalValueChanged( QString ) ) );

    QMenu*   menu       = service->enablePluginMenu();
    QAction* menuAction = menu->addAction( "Connect to &vampir" );
    menuAction->setStatusTip( tr( "Connect to vampir and display a trace file" ) );
    menuAction->setWhatsThis( "Connect to vampir and display a trace file" );
    connect( menuAction, SIGNAL( triggered() ), this, SLOT( vampirSettings() ) );
    return true;
}

QString
VampirPlugin::getDeactivationMessage()
{
    return errorMsg;
}

void
VampirPlugin::cubeClosed()
{
}

QString
VampirPlugin::getHelpText() const
{
    return "";
}

#define STATISTIC_MARKER "max severe instance"
void
VampirPlugin::contextMenuIsShown( TreeType type, TreeItem* item )
{
    contextMenuItem     = item;
    contextMenuTreeType = type;
    bool isActive = false;

    foreach( const TreeItemMarker * marker, item->getMarkerList() )
    {
        // only enable vampir action, if the statistics plugin has marked the selected tree item
        if ( marker->getLabel() == QString( STATISTIC_MARKER ) )
        {
            isActive = true;
            break;
        }
    }
    QAction* action = service->addContextMenuItem( type, tr( "Show max severity in Vampir" ) );
    action->setStatusTip( tr( "Shows the most severe instance of pattern in Vampir" ) );
    action->setWhatsThis( tr( "Shows the most severe instance of pattern in Vampir. Only available if a statistics file for the current cube file exists." ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( onShowMaxSeverity() ) );
    action->setEnabled( isActive );
}

void
VampirPlugin::onShowMaxSeverity()
{
    QString      message          = "";
    const int    zoomSteps        = 3;
    const double offsetMultiplier = 0.1;
    double       duration         = exit - enter;
    double       exitTime         = exit + offsetMultiplier * duration;

    bool connected = false;

    for ( int step = 1; step <= zoomSteps; step++ )
    {
        double enterTime = max( enter
                                - ( ( zoomSteps - step ) * 5 ) * duration
                                - offsetMultiplier * duration, 0.0 );
        foreach( VampirConnecter * con, connecterList )
        {
            if ( con->IsActive() )
            {
                message  += QString::fromStdString( con->ZoomIntervall( enterTime, exitTime, step ) );
                connected = true;
            }
        }
    }

    if ( !connected )
    {
        message = "Not connected to Vampir.";
    }

    if ( !message.isEmpty() )
    {
        service->setMessage( message, Error );
    }
}

void
VampirPlugin::vampirSettings()
{
    VampirConnectionDialog* dialog;
    dialog = new VampirConnectionDialog( service, service->getParentWidget(), service->getCubeFileName(), &connecterList );

    dialog->setModal( false );
    dialog->show();
}

/**
 * get values from statistics plugin
 */
void
VampirPlugin::globalValueChanged( const QString& name )
{
    if ( name == "Statistics::MaxSevereEventEnter" )
    {
        enter = service->getGlobalValue( name ).toDouble();
    }
    else if ( name == "Statistics::MaxSevereEventExit" )
    {
        exit = service->getGlobalValue( name ).toDouble();
    }
}
