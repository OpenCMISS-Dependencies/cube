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


#include <QDebug>
#include <QList>
#include <assert.h>
#include "PluginList.h"
#include "Globals.h"

PluginList* PluginList::single        = NULL;
bool        PluginList::disableAlways = false;

PluginList*
PluginList::getInstance()
{
    if ( !single )
    {
        single = new PluginList();
    }
    return single;
}

int
PluginList::getCubePluginCount()
{
    return getInstance()->cubePluginList.size();
}

CubePlugin*
PluginList::getCubePlugin( int idx )
{
    return static_cast<CubePlugin*> ( getInstance()->cubePluginList.at( idx ).plugin );
}

QString
PluginList::getCubePluginName( int idx )
{
    return getInstance()->cubePluginList.at( idx ).name;
}

int
PluginList::getContextFreePluginCount()
{
    return getInstance()->contextFreePluginList.size();
}

ContextFreePlugin*
PluginList::getContextFreePlugin( int idx )
{
    return static_cast<ContextFreePlugin*> ( getInstance()->contextFreePluginList.at( idx ).plugin );
}

QString
PluginList::getContextFreePluginName( int idx )
{
    return getInstance()->contextFreePluginList.at( idx ).name;
}

// ------------

int
getVersionNumber( PluginInterface* plugin )
{
    int major, minor, bugfix;
    plugin->version( major, minor, bugfix );
    return major * 10000 + minor * 100 + bugfix;
}

void
PluginList::updatePluginList( const QStringList& pluginPath )
{
    if ( !single )
    {
        single = new PluginList();
    }
    single->loadPluginList( pluginPath );
}

/**
 * @brief PluginList::loadPluginList initial load of all available plugins
 * @param idx
 */
void
PluginList::loadPluginList( const QStringList& pluginPath )
{
    if ( disableAlways )
    {
        return;
    }

    QList<QDir> pluginDirList;

    foreach( QString dir, pluginPath )
    {
        pluginDirList.append( dir );
    }

    // add user defined cube plugin directory, if environment variable "CUBE_PLUGIN_DIR" is set
    if ( qgetenv( "CUBE_PLUGIN_DIR" ).size() > 0 )
    {
        QStringList userList = QString( qgetenv( "CUBE_PLUGIN_DIR" ) ).split( QRegExp( ":|;" ), QString::SkipEmptyParts );
        foreach( QString dir, userList )
        {
            pluginDirList.append( dir );
        }
    }
    else
    {
        Globals::debug( "PluginList" ) << "no user defined plugin path CUBE_PLUGIN_DIR" << endl;
    }

    pluginDirList.append( qApp->applicationDirPath() );
    QDir libdir = QDir( QString( LIBDIR ).append( "/cube-plugins" ) );
    pluginDirList.append( libdir );

    QDir appdir = QDir( qApp->applicationDirPath() + "/../lib/cube-plugins" );
    if ( appdir.exists() && appdir != libdir )
    {
        pluginDirList.append( appdir );
    }
    QDir appdir64 = QDir( qApp->applicationDirPath() + "/../lib64/cube-plugins" );
    if ( appdir64.exists() && appdir64 != libdir )
    {
        pluginDirList.append( appdir64 );
    }
    QDir appdir32 = QDir( qApp->applicationDirPath() + "/../lib32/cube-plugins" );
    if ( appdir32.exists() && appdir32 != libdir )
    {
        pluginDirList.append( appdir32 );
    }

    Globals::debug( "PluginList" ) << "plugin search path: " << endl;
    foreach( QDir dir, pluginDirList )
    {
        Globals::debug( "PluginList" ) << " " << dir.absolutePath() << endl;
    }

    foreach( QDir dir, pluginDirList )
    {
        QStringList filters;
        filters << "*.so" << "*.dll";
        dir.setNameFilters( filters );
        foreach( QString fileName, dir.entryList( QDir::Files ) )
        {
            QString        location = dir.absoluteFilePath( fileName );
            QPluginLoader* loader   = new QPluginLoader( location );
            QObject*       plugin   = loader->instance();

            if ( plugin ) // plugin has been loaded
            {
                PluginInterface* validPlugin = qobject_cast<PluginInterface*>( plugin );
                if ( validPlugin ) // plugin implements PluginInterface
                {
                    if ( getPluginData( validPlugin ) )
                    {
                        //Globals::debug( "PluginList" ) << "plugin " << existingPlugin->name << " has already been loaded" << location << endl;
                        continue;
                    }

                    PluginData data;
                    data.location     = location;
                    data.plugin       = validPlugin;
                    data.pluginLoader = loader;
                    data.name         = data.plugin->name();

                    if ( loadContextFreePlugin( data ) )
                    {
                        Globals::debug( "PluginList" ) << "context free plugin loaded:" << data.location << endl;
                    }
                    else if ( loadCubePlugin( data ) )
                    {
                        Globals::debug( "PluginList" ) << "plugin loaded:" << data.location << endl;
                    }
                    else
                    {
                        validPlugin = 0;
                    }
                }
                if ( !validPlugin ) // unload invalid plugin
                {
                    Globals::debug( "PluginList" ) << "plugin " << dir.absoluteFilePath( fileName ) << " is not a valid CubePlugin version " << PLUGIN_VERSION << endl;
                    Globals::debug( "Qt error msg:" ) << loader->errorString() << endl;
                    loader->unload();
                    delete loader;
                }
            }
            else
            {
                Globals::debug( "PluginList" ) << "plugin " << dir.absoluteFilePath( fileName ) << " is not a valid CubePlugin version " << PLUGIN_VERSION << endl;
                Globals::debug( "Qt error msg:" ) << loader->errorString() << endl;
                delete loader;
            }
        }
    }
    if ( cubePluginList.size() == 0 )
    {
        Globals::debug( "PluginList" ) << "no plugins found" << endl;
    }
    /*else
       {
        Globals::debugStream() << "plugins found:" << endl;
        for ( int i = 0; i < cubePluginList.size(); i++ )
        {
            Globals::debugStream() << "  " << cubePluginList.at( i ).name << endl;
        }
       }*/
}


PluginData*
PluginList::getPluginData( PluginInterface* plugin )
{
    for ( int i = 0; i < cubePluginList.size(); i++ )
    {
        if ( plugin == cubePluginList.at( i ).plugin )
        {
            return &cubePluginList[ i ];
        }
    }
    for ( int i = 0; i < contextFreePluginList.size(); i++ )
    {
        if ( plugin == contextFreePluginList.at( i ).plugin )
        {
            return &contextFreePluginList[ i ];
        }
    }

    return 0;
}

/**
   if plugin is a valid CubePlugin, it is loaded and the loader is appended to the list pluginLoaderList
 */
bool
PluginList::loadCubePlugin( PluginData& data )
{
    CubePlugin* cubePlugin = dynamic_cast<CubePlugin*>( data.plugin );
    if ( cubePlugin )
    {
        cubePlugin->isInitialized = false;
        loadPlugin( data, cubePluginList );
    }
    return cubePlugin;
}

bool
PluginList::loadContextFreePlugin( PluginData& data )
{
    ContextFreePlugin* contextFreePlugin = dynamic_cast<ContextFreePlugin*>( data.plugin );
    if ( contextFreePlugin )
    {
        loadPlugin( data, contextFreePluginList );
    }
    return contextFreePlugin;
}


bool
PluginList::loadPlugin( PluginData& data, QList<PluginData>& list )
{
    bool ok = true;

    bool pluginIsLoaded = false;
    for ( int i = 0; i < list.size(); i++ )
    {
        if ( data.plugin ==  list[ i ].plugin )
        {
            pluginIsLoaded = true;
            break;
        }
    }

    // check, if plugin with different version already exists
    if ( !pluginIsLoaded )
    {
        int index = -1;
        for ( int i = 0; i < list.size(); i++ )
        {
            if ( data.plugin->name() == list[ i ].name )
            {
                index = i;
                break;
            }
        }

        if ( index != -1 ) // plugin already exists with different version
        {
            int version = getVersionNumber( data.plugin );

            // replace existing plugin, if version number is greater
            if ( version > getVersionNumber( list.at( index ).plugin ) )
            {
                QPluginLoader* oldLoader = list.at( index ).pluginLoader;
                oldLoader->unload();
                delete oldLoader;
                list.replace( index, data );
                Globals::debug( "PluginList" ) << "replace plugin with newer version: " << data.plugin->name() << version << data.location << endl;
            }
            else if ( version < getVersionNumber( list.at( index ).plugin ) )
            {
                Globals::debug( "PluginList" ) << "ignore older version of plugin " << data.plugin->name() << version << data.location << endl;
            }
            else
            {
                Globals::debug( "PluginList" ) << "ignore same version of plugin " << data.plugin->name() << version << data.location << endl;
                data.pluginLoader->unload();
            }
        }
        else // insert new plugin
        {
            list.append( data );
        }
    }
    else // plugin is already loaded ( = link to same file )
    {
        data.pluginLoader->unload();
        delete data.pluginLoader;
        data.pluginLoader = 0;
        ok                = false;
    }

    return ok;
}

void
PluginList::disablePlugins()
{
    disableAlways = true;
}

/* currently unused because of bug QTBUG-39070
 * all plugins are loaded, but are activated only if an initialization function is called
   void
   PluginList::unloadPlugins()
   {
    if ( !single )
    {
        return;
    }
    else
    {
        single->unloadPluginList();
    }
   }

   void
   PluginList::unloadPluginAt( int idx )
   {
    if ( single )
    {
        QPluginLoader* loader = single->pluginLoaderList.at( idx );
        if ( loader->isLoaded() )
        {
            QString file = loader->fileName();
            loader->unload();
            delete loader;
            loader = new QPluginLoader( file );
            single->pluginLoaderList.replace( idx, loader );
            single->pluginList.replace( idx, 0 );
        }
    }
   }

   CubePlugin*
   PluginList::loadPluginAt( int idx )
   {
    CubePlugin* cubePlugin = 0;
    if ( single )
    {
        if ( single->pluginList.at( idx ) ) // already loaded
        {
            return single->pluginList.at( idx );
        }
        else // load plugin
        {
            QPluginLoader* loader = single->pluginLoaderList.at( idx );
            QObject*       plugin = loader->instance();
            if ( plugin )
            {
                cubePlugin                = qobject_cast<CubePlugin*>( plugin );
                cubePlugin->isInitialized = false;
                if ( cubePlugin )
                {
                    Globals::debugStream() << "plugin reloaded:" << cubePlugin->name() << endl;
                    single->pluginList.replace( idx, cubePlugin );
                }
                else
                {
                    Globals::debugStream() << "plugin reload failed:" << cubePlugin->name() << endl;
                }
            }
            else
            {
                Globals::debugStream() << "plugin reload failed:" << endl;
            }
        }
    }
    return cubePlugin;
   }

   void
   PluginList::unloadPluginList()
   {
    int idx = 0;
    foreach( QPluginLoader * loader, this->pluginLoaderList )
    {
        if ( loader->isLoaded() )
        {
            loader->unload();

            // cannot use loader->instance() after unload() for a second time -> segfault
            QString file = loader->fileName();
            delete loader;
            loader = new QPluginLoader( file );
            pluginLoaderList.replace( idx, loader );
            pluginList.replace( idx, 0 );
        }
        idx++;
    }
   }

 */
