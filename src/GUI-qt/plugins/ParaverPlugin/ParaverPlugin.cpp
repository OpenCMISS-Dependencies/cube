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


#include <cassert>
#include <iostream>
#include <string>

#include <QThread>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include <QDialogButtonBox>
#include <QMessageBox>
#include <QFileDialog>
#include "ParaverPlugin.h"
#include "ParaverConnecter.h"

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( ParaverPlugin, ParaverPlugin );
#endif

using namespace std;

QString
ParaverPlugin::name() const
{
    return "ParaverPlugin";
}

void
ParaverPlugin::version( int& major, int& minor, int& bugfix ) const
{
    major  = 1;
    minor  = 0;
    bugfix = 0;
}

bool
ParaverPlugin::cubeOpened( PluginServices* service )
{
    this->service   = service;
    this->connecter = 0;
    contextMenuItem = 0;
    configLine      = 0;
    fileLine        = 0;


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
    QAction* menuAction = menu->addAction( "Connect to &paraver" );
    menuAction->setStatusTip( tr( "Connect to paraver and display a trace file" ) );
    menuAction->setWhatsThis( "Connect to paraver and display a trace file" );
    connect( menuAction, SIGNAL( triggered() ), this, SLOT( paraverSettings() ) );
    return true;
}

QString
ParaverPlugin::getDeactivationMessage()
{
    return errorMsg;
}

void
ParaverPlugin::cubeClosed()
{
}

QString
ParaverPlugin::getHelpText() const
{
    return "";
}

#define STATISTIC_MARKER "max severe instance"
void
ParaverPlugin::contextMenuIsShown( TreeType type, TreeItem* item )
{
    contextMenuItem     = item;
    contextMenuTreeType = type;
    bool isActive = false;

    foreach( const TreeItemMarker * marker, item->getMarkerList() )
    {
        // only enable paraver action, if the statistics plugin has marked the selected tree item
        if ( marker->getLabel() == QString( STATISTIC_MARKER ) )
        {
            isActive = true;
            break;
        }
    }
    QAction* action = service->addContextMenuItem( type, tr( "Show max severity in paraver" ) );
    action->setStatusTip( tr( "Shows the most severe instance of pattern in paraver" ) );
    action->setWhatsThis( tr( "Shows the most severe instance of pattern in paraver. Only available if a statistics file for the current cube file exists." ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( onShowMaxSeverity() ) );
    action->setEnabled( isActive );
}

QString
ParaverPlugin::connectToParaver( string const& configFileName,
                                 string const& fileName )
{
    qDebug() << "Etablishing new paraver connection";

    connecter = new ParaverConnecter( fileName, configFileName );
    string message = connecter->InitiateAndOpenTrace();
    if ( message != "" )
    {
        delete connecter;
        connecter = 0;
    }
    return QString::fromStdString( message );
}

void
ParaverPlugin::onShowMaxSeverity()
{
    QString message;

    if ( !connecter )
    {
        message = "Not connected to a trace browser.";
    }
    else
    {
        const int    zoomSteps        = 3;
        const double offsetMultiplier = 0.1;
        double       duration         = exit - enter;
        double       exitTime         = exit + offsetMultiplier * duration;

        for ( int step = 1; step <= zoomSteps; step++ )
        {
            double enterTime = max( enter
                                    - ( ( zoomSteps - step ) * 5 ) * duration
                                    - offsetMultiplier * duration, 0.0 );
            if ( connecter->IsActive() )
            {
                message = QString::fromStdString( connecter->ZoomIntervall( enterTime, exitTime, step ) );
            }
        }
    }
    if ( !message.isEmpty() )
    {
        service->setMessage( message, Information );
    }
}

#define MAKE_STR( x ) _MAKE_STR( x )
#define _MAKE_STR( x ) #x

void
ParaverPlugin::paraverSettings()
{
    int const spacing = 20;

    QDialog dialog( service->getParentWidget() );
    dialog.setWindowTitle( "Connect to paraver" );

    QFontMetrics fm( dialog.font() );

    int h = 10 * fm.ascent();
    int w = 4 * fm.width( "Connect to paraver:" );
    dialog.setMinimumSize( w, h );

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing( spacing );

    QHBoxLayout* layout1 = new QHBoxLayout();
    layout1->setSpacing( spacing );

    // define configuration file label and editor
    QLabel* configLabel = new QLabel();
    configLabel->setText( "Configuration file:" );
    layout1->addWidget( configLabel );

    configLine = new QLineEdit( MAKE_STR( PV_DEF_CFG ) );
    layout1->addWidget( configLine );
    QPushButton* configFileButton = new QPushButton( "Browse" );
    layout1->addWidget( configFileButton );
    connect( configFileButton, SIGNAL( clicked() ), this, SLOT( getConfigFileName() ) );

    layout->addLayout( layout1 );

    // define file label and editor
    QHBoxLayout* layout2 = new QHBoxLayout();
    layout2->setSpacing( spacing );

    QLabel* fileLabel = new QLabel();
    fileLabel->setText( "Trace file:" );
    layout2->addWidget( fileLabel );

    QString paraverFileName = service->getStatName().replace( ".stat", ".prv" );

    fileLine = new QLineEdit( paraverFileName );
    layout2->addWidget( fileLine );
    QPushButton* fileButton = new QPushButton( "Browse" );
    layout2->addWidget( fileButton );
    connect( fileButton, SIGNAL( clicked() ), this, SLOT( getTraceFileName() ) );

    layout->addLayout( layout2 );

    QDialogButtonBox* buttonBox    = new QDialogButtonBox();
    QPushButton*      okButton     = buttonBox->addButton( QDialogButtonBox::Ok );
    QPushButton*      cancelButton = buttonBox->addButton( QDialogButtonBox::Cancel );
    connect( okButton, SIGNAL( clicked() ), &dialog, SLOT( accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), &dialog, SLOT( reject() ) );
    layout->addWidget( buttonBox );

    dialog.setLayout( layout );

    if ( dialog.exec() )
    {
        QString result =
            connectToParaver( configLine->displayText().toStdString(),
                              fileLine->displayText().toStdString() );
        if ( result != "" )
        {
            QMessageBox::critical( service->getParentWidget(),
                                   tr( "Paraver connection" ), result );
        }
    }
}

#undef _MAKE_STR
#undef MAKE_STR

void
ParaverPlugin::getTraceFileName()
{
    QString openFileName =
        QFileDialog::getOpenFileName( service->getParentWidget(), tr( "Choose a file to open" ),
                                      fileLine->displayText(),
                                      tr( "Trace files (*.elg *.esd *.otf *.otf2 *.prv);;All files (*.*);;All files (*)" ) );
    if ( openFileName.length() > 0 )
    {
        fileLine->setText( openFileName );
    }
}

void
ParaverPlugin::getConfigFileName()
{
    QString configFileName = QFileDialog::getOpenFileName( service->getParentWidget(),
                                                           tr( "Choose a file to open" ), configLine->displayText(),
                                                           tr( "Config files (*.cfg);;All files (*.*);;All files (*)" ) );
    if ( configFileName.length() > 0 )
    {
        configLine->setText( configFileName );
    }
}


void
ParaverPlugin::globalValueChanged( const QString& name )
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
