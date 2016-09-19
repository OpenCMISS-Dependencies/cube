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



#include "MainWidget.h"
#include "CubeError.h"
#include "PluginManager.h"
#include <iostream>
#include <iomanip>
#include <QFileDialog>
#include <QApplication>
#include "Globals.h"


using namespace std;

// this is the main method
// handles command-line arguments and starts the GUI

int
main( int argc, char* argv[] )
{
    cube::CubeStrategy strategy = cube::CUBE_ALL_IN_MEMORY_STRATEGY;

    QApplication app( argc, argv );

    MainWidget* mainWidget     = NULL;
    bool        disablePlugins = false;

    QStringList args = app.arguments();
    // set default name for settings
    QCoreApplication::setOrganizationName( "FZJ" );
    QCoreApplication::setApplicationName( "Cube" );

    if ( args.removeAll( "-disable-plugins" ) > 0 )
    {
        PluginManager::disablePlugins();
        disablePlugins = true;
    }
    if ( args.removeAll( "-verbose" ) > 0 )
    {
        Globals::setVerbose( true );
    }
    if ( args.removeAll( "-experimental" ) > 0 )
    {
        Globals::setExperimental( true );
    }
    if ( args.removeAll( "-preload" ) > 0 )
    {
        strategy = cube::CUBE_ALL_IN_MEMORY_PRELOAD_STRATEGY;
    }
    if ( args.removeAll( "-lastN" ) > 0 )
    {
        strategy = cube::CUBE_LAST_N_ROWS_STRATEGY;
    }
    if ( args.removeAll( "-help" ) > 0 || args.size() > 2 )
    {
        std::cout << "Usage: cube [-disable-plugins] [-verbose] [-preload] [-lastN] [-help] filename\n";
        return 0;
    }

    try
    {
        mainWidget = new MainWidget( strategy );
        if ( args.size() == 2 )
        {
            QString name = args[ 1 ];
            // if the command line argument is a directory then
            // open a file dialog for getting the file name
            if ( !QFile( name ).exists() || QDir( name ).exists() )
            {
                name = QFileDialog::getOpenFileName( NULL,
                                                     app.tr( "Choose a file to open" ),
                                                     name,
                                                     app.tr( "Cube3/4 files (*cube *cube.gz *.cubex);;Cube4 files (*.cubex);;Cube files (*.cube.gz *.cube);;All files (*.*);;All files (*)" ) );
            }
            if ( name.length() > 0 )
            {
                mainWidget->readFile( name );
            }
        }
        if ( disablePlugins )
        {
            Globals::setStatusMessage( "plugins are disabled..." );
        }
        mainWidget->show();

        int return_code =  app.exec();

        delete mainWidget;
        return return_code;
    }
    catch ( const cube::RuntimeError& err )
    {
        delete mainWidget;
        std::cerr << "ERROR in main: " << err.get_msg().c_str() << std::endl;
    }
    return -1;
}
