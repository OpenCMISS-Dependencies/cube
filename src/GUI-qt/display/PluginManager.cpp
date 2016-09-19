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


#include <assert.h>
#include <QMenu>
#include <QFileDialog>

#include "PluginManager.h"
#include "PluginServices.h"
#include "PluginList.h"
#include "PluginSettingDialog.h"
#include "TreeItem.h"
#include "TreeItemMarkerDialog.h"
#include "MainWidget.h"
#include "Globals.h"
#include "TreeView.h"

#define NO_PLUGIN -1

// workaround:leak in plugin may cause segfault https://bugreports.qt-project.org/browse/QTBUG-39070
#define NO_PLUGIN_UNLOAD

PluginManager* PluginManager::single        = 0;
bool           PluginManager::disableAlways = false;

PluginManager*
PluginManager::getInstance()
{
    if ( single == 0 )
    {
        single = new PluginManager();
    }
    return single;
}

PluginManager::PluginManager()
{
    cube         = 0;
    tabManager   = 0;
    helpWidget   = 0;
    cubeFileName = "";
    markerDialog = 0;

    currentContextFreePlugin = 0;

    /** create menu items which will be filled after plugins have been loaded */
    pluginMenu = new QMenu( tr( "&Plugins" ) );
    connect( pluginMenu, SIGNAL( aboutToShow() ), this, SLOT( checkPluginMenuStatus() ) );
    pluginMenu->setVisible( false );

    configPathItem = new QAction( "Configure plugin search path", 0 );
    pluginMenu->addAction( configPathItem );
    configPathItem->setWhatsThis( "Allows to activate or deactivate plugins independent of the cube file" );
    connect( configPathItem, SIGNAL( triggered() ), this, SLOT( configureSearchPath() ) );

    initialSettingsItem = new QAction( "Initial activation settings", 0 );
    initialSettingsItem->setWhatsThis( "Allows to activate or deactivate plugins independent of the cube file" );
    connect( initialSettingsItem, SIGNAL( triggered() ), this, SLOT( showSettingsWidget() ) );

    pluginStatusMenu = new QMenu( "Activate/deactivate plugins" );
    pluginStatusMenu->setWhatsThis( "Allows to activate or deactivate plugins which are available for the current cube file" );

    contextFreePluginMenu = new QMenu( tr( "Start" ) );
    contextFreePluginMenu->setStatusTip( tr( "Opens a context free plugin" ) );
    contextFreePluginMenu->setEnabled( false );

    helpMenu =   new QMenu( tr( "Plugins" ) );

    markerMenuAction = new QAction( "Configure Tree Item Marker", 0 );
    markerMenuAction->setWhatsThis( "Allows to change colors and icons of the items that are marked by plugins" );
    markerMenuAction->setEnabled( false );
    connect( markerMenuAction, SIGNAL( triggered() ), this, SLOT( showTreeItemMarkerSettings() ) );
    markerMenuAction->setVisible( false );
}

/** is called after global settings are loaded or plugin path has been changed */
void
PluginManager::initializePlugins()
{
    if ( disableAlways )
    {
        return;
    }

    PluginList::updatePluginList( pluginPath );

    QList<QMenu* > prevMenuList = pluginMenuList;

    pluginMenu->clear();
    pluginMenu->addAction( configPathItem );
    pluginMenu->addAction( initialSettingsItem );
    pluginMenu->addMenu( pluginStatusMenu );
    pluginMenu->addSeparator();
    pluginStatusMenu->clear();
    pluginActivationAction.clear();
    pluginMenuList.clear();

    enableAllPlugins  = true;
    disableAllPlugins = false;
    getPluginStatus( enableAllPlugins, disableAllPlugins, pluginStatus );

    // Create initial plugin menu structure
    int pluginCount = PluginList::getCubePluginCount();
    if ( pluginCount > 0 )
    {
        markerMenuAction->setVisible( true );

        for ( int index = 0; index < pluginCount; ++index )
        {
            CubePlugin* plugin  = PluginList::getCubePlugin( index );
            QMenu*      subMenu = 0;
            foreach( QMenu * menu, prevMenuList )
            {
                if ( plugin->name() == menu->title() )
                {
                    subMenu = menu;
                    break;
                }
            }
            if ( !subMenu )
            {
                subMenu = new QMenu( plugin->name(), pluginMenu );
                subMenu->menuAction()->setVisible( false );
                pluginServicesList.append( 0 );
            }
            pluginMenu->addMenu( subMenu );
            pluginMenuList.append( subMenu );

            // add "active/deactivate plugin" menu item
            QAction* pa = new QAction( plugin->versionLabel(), this );
            pluginActivationAction.append( pa );
            pa->setCheckable( true );
            pa->setChecked( true );
            pa->setStatusTip( tr( "Activate/deactivate plugin" ) );
            pa->setWhatsThis( "If this menu item is checked, the plugin is activated" );
            pa->setData( index );
            connect( pa, SIGNAL( triggered( bool ) ), this, SLOT( enablePlugin( bool ) ) );
            pluginStatusMenu->addAction( pa );
            pa->setChecked( false );
            pa->setVisible( false );
        }
    }
    fillHelpMenu();
    fillContextFreePluginMenu();
}

QAction*
PluginManager::getMarkerConfigAction()
{
    return markerMenuAction;
}

void
PluginManager::setMainWindow( QMainWindow* main )
{
    mainWindow = main;
}

void
PluginManager::disablePlugins()
{
    PluginList::disablePlugins();
    disableAlways = true;
}

void
PluginManager::opened( cube::Cube* cube, const QString& file, TabManager* tabManager )
{
    this->tabManager   = tabManager;
    this->cube         = cube;
    this->cubeFileName = file;
}

void
PluginManager::openPlugins()
{
    if ( cube == 0 || tabManager == 0 )
    {
        return; // not opened
    }
    if ( !disableAlways )
    {
        openAllPlugins();
    }
    setMarkerConfig();
}

bool
PluginManager::openPlugin( int index )
{
    bool        ok     = true; // plugin has been loaded and is valid
    CubePlugin* plugin = PluginList::getCubePlugin( index );

#ifndef NO_PLUGIN_UNLOAD
    if ( !plugin )
    {
        plugin = PluginList::loadPluginAt( index );
    }
#endif

    if ( plugin && !plugin->isInitialized )
    {
        PluginServices* services = new PluginServices( this, index );
        pluginServicesList[ index ] = services;

        ok                    = plugin->cubeOpened( services );
        plugin->isInitialized = true;
    }

    pluginActivationAction.at( index )->setVisible( true );
    if ( ok )
    {
        pluginMenuList.at( index )->menuAction()->setVisible( true );
        pluginActivationAction.at( index )->setChecked( true );
    }
    else
    {
        pluginMenuList.at( index )->menuAction()->setVisible( false );
        pluginActivationAction.at( index )->setChecked( false );
        this->closePlugin( index );
    }
    setMarkerConfig();

    return ok;
}

void
PluginManager::closePlugin( int index )
{
    CubePlugin* plugin = PluginList::getCubePlugin( index );

    if ( plugin == 0 )
    {
        return; // already unloaded
    }

    QMenu* subMenu = pluginMenuList.at( index );
    subMenu->clear(); // clear all plugin related menu items

    PluginServices* service = pluginServicesList[ index ];

    // remove plugin toolbar(s)
    foreach( QToolBar * bar, toolBarHash.values( service ) )
    {
        mainWindow->removeToolBar( bar );
        toolBarHash.remove( service, bar );
    }

    // remove plugin colormaps
    foreach( ColorMap * map, colormaps.values( service ) )
    {
        if ( map == Globals::getColorMap() )
        {
            Globals::setDefaultColorMap();
        }
        colormaps.remove( service, map );
    }

    if ( service )
    {
        service->closeTabs();
        plugin->cubeClosed();
        plugin->isInitialized = false;

        // delete corresponding service
        delete service;
        pluginServicesList[ index ] = 0;
    }

    setMarkerConfig();

#ifndef NO_PLUGIN_UNLOAD
    PluginList::unloadPluginAt( index );
#endif
}

QMenu*
PluginManager::getPluginMenu() const
{
    return pluginMenu;
}

QMenu*
PluginManager::getPluginHelpMenu() const
{
    return helpMenu;
}

QMenu*
PluginManager::getContextFreePluginMenu() const
{
    return contextFreePluginMenu;
}

void
PluginManager::openAllPlugins()
{
    for ( int i = 0; i < PluginList::getCubePluginCount(); ++i )
    {
        QString name = PluginList::getCubePluginName( i );

        bool load = true;
        if ( enableAllPlugins )
        {
            load = true;
        }
        else if ( disableAllPlugins )
        {
            load = false;
        }
        else
        {
            load = pluginStatus.value( name, true ); // load plugin, if not not set to false in settings
        }

        if ( load )
        {
            openPlugin( i );
        }
        else
        {
            closePlugin( i );
        }
    }
}

// called if cube file has been closed
void
PluginManager::closed()
{
    for ( int i = 0; i < pluginMenuList.size(); ++i )
    {
        closePlugin( i );
        pluginActivationAction.at( i )->setVisible( false );
    }

    this->tabManager   = 0;
    this->cube         = 0;
    this->cubeFileName = "";

    // reinitialize plugin status to startup settings
    getPluginStatus( enableAllPlugins, disableAllPlugins, pluginStatus );

    // remove internal services
    foreach( PluginServices * service, pluginServicesList )
    {
        if ( service && service->index < 0 )
        {
            pluginServicesList.removeOne( service );
        }
    }
    // end remove internal services

    if ( markerMenuAction )
    {
        markerMenuAction->setEnabled( false );
    }
    setMarkerConfig();
}

/** slot for plugin activation/deactivation menu */
void
PluginManager::enablePlugin( bool enable )
{
    QAction* act = dynamic_cast<QAction*>( sender() );
    assert( act );

    int index = act->data().toInt();
    if ( enable )
    {
        CubePlugin* plugin = PluginList::getCubePlugin( index );
        bool        ok     = openPlugin( index );
        if ( !ok )
        {
            QString msg = "Cannot activate " + plugin->name();
            if ( !plugin->getDeactivationMessage().isEmpty() )
            {
                msg = msg + ": " + plugin->getDeactivationMessage();
            }
            Globals::setStatusMessage( msg, Warning );
        }
        else
        {
            //tabManager->updateTreeItems();
            tabManager->reinit();
            //tabManager->treeItemSelected( tabManager->getTree(METRICTREE) );
            QString msg = plugin->name() + " has been activated.";
            Globals::setStatusMessage( msg );
        }
    }
    else
    {
        closePlugin( index  );
    }
}

void
PluginManager::configureSearchPath()
{
    pluginPath = PathDialog::getPath( pluginPath );

    initializePlugins();
    openPlugins();
}

void
PluginManager::showSettingsWidget()
{
    PluginSettingDialog::showSettingsDialog( tabManager );
}

/**
 * checkPluginMenuStatus hides empty plugin menus
 */
void
PluginManager::checkPluginMenuStatus()
{
    foreach( QMenu * menu, pluginMenuList )
    {
        if ( menu->isEmpty() )
        {
            menu->menuAction()->setVisible( false );
        }
        else
        {
            menu->menuAction()->setVisible( true );
        }
    }
}

// ---------- emit signals for plugins -----------------

void
PluginManager::contextMenuIsShown( TreeType type, TreeItem* treeItem )
{
    foreach( PluginServices * service, pluginServicesList )
    {
        if ( service )
        {
            emit service->contextMenuIsShown( type, treeItem );
        }
    }
}

void
PluginManager::treeItemIsSelected( TreeType type, TreeItem* treeItem )
{
    foreach( PluginServices * service, pluginServicesList )
    {
        if ( service )
        {
            emit service->treeItemIsSelected( type, treeItem );
        }
    }
}

void
PluginManager::setTabOrder( const QList<DisplayType>& order )
{
    this->order = order;
    foreach( PluginServices * service, pluginServicesList )
    {
        if ( service )
        {
            emit service->orderHasChanged( order );
        }
    }
}

void
PluginManager::setGlobalValue( const QString& name, const QVariant& value, PluginServices* sender )
{
    globalValues.insert( name, value );

    foreach( PluginServices * service, pluginServicesList )
    {
        if ( service && service != sender )
        {
            emit service->globalValueChanged( name );
        }
    }
}

void
PluginManager::tabActivated( TreeType type )
{
    foreach( PluginServices * service, pluginServicesList )
    {
        if ( service )
        {
            emit service->tabActivated( type );
        }
    }
}

QMainWindow*
PluginManager::getMainWindow()
{
    return mainWindow;
}


void
PluginManager::genericUserAction( UserAction action )
{
    foreach( PluginServices * service, pluginServicesList )
    {
        if ( service )
        {
            emit service->genericUserAction( action );
        }
    }
}

// ---------- end emit signals for plugins -----------------



/**
 * reads the initial plugin status from settings
 */
void
PluginManager::getPluginStatus( bool& all, bool& none, QHash<QString, bool>& plugins )
{
    all     = enableAllPlugins;
    none    = disableAllPlugins;
    plugins = pluginStatus;
}


/**
 * @brief PluginManager::setPluginStatus
 * Activates or deactivates the available plugins and saves the status
 * @param all all plugins will be activated, if true
 * @param none all plugins will be deactivated, if true
 * @param plugins Hash with pluginName->activated value pairs
 */
void
PluginManager::setPluginStatus( bool& all, bool& none, QHash<QString, bool>& plugins )
{
    enableAllPlugins  = all;
    disableAllPlugins = none;
    pluginStatus      = plugins;

    if ( cube )
    {
        openAllPlugins();
    }
}

// --- settings for initial activation/deactivation of the plugins

/** this method is only called once if program ends */
void
PluginManager::saveGlobalStartupSettings( QSettings& settings )
{
    if ( disableAlways )
    {
        return;
    }

    QStringList enabledList, disabledList;
    foreach( QString pluginName, pluginStatus.keys() )
    {
        if ( pluginStatus.value( pluginName ) )
        {
            enabledList.append( pluginName );
        }
        else
        {
            disabledList.append( pluginName );
        }
    }

    settings.setValue( "pluginEnabledList", enabledList );
    settings.setValue( "pluginDisabledList", disabledList );
    settings.setValue( "pluginPath", pluginPath );
}

/** this method is only called once at program start */
void
PluginManager::loadGlobalStartupSettings( QSettings& settings )
{
    if ( disableAlways )
    {
        return;
    }

    QStringList enabledList, disabledList;
    enabledList  = settings.value( "pluginEnabledList" ).toStringList();
    disabledList = settings.value( "pluginDisabledList" ).toStringList();
    pluginPath   = settings.value( "pluginPath" ).toStringList();

    foreach( QString plugin, enabledList )
    {
        pluginStatus.insert( plugin, true );
    }
    foreach( QString plugin, disabledList )
    {
        pluginStatus.insert( plugin, false );
    }

    enableAllPlugins  = false; // always false, since the activation status of each plugin is saved
    disableAllPlugins = false;
}

// ----- implementation of SettingsHandler interface to save settings of each plugin  -----------

QString
PluginManager::settingName()
{
    return "plugins";
}

void
PluginManager::saveGlobalSettings( QSettings& settings )
{
    // save settings for each registered pluginService
    foreach( PluginServices * service, pluginServicesList )
    {
        if ( service && service->settingsHandler )
        {
            settings.beginGroup( service->settingsHandler->settingNameCompact() );
            service->settingsHandler->saveGlobalSettings( settings );
            settings.endGroup();
        }
    }
}

void
PluginManager::loadGlobalSettings( QSettings& settings )
{
    // load settings for pluginService with registered settingsHandler
    foreach( PluginServices * service, pluginServicesList )
    {
        if ( service && service->settingsHandler )
        {
            settings.beginGroup( service->settingsHandler->settingNameCompact() );
            service->settingsHandler->loadGlobalSettings( settings );
            settings.endGroup();
        }
    }
}

void
PluginManager::saveExperimentSettings( QSettings& settings )
{
    // save settings for each registered pluginService
    foreach( PluginServices * service, pluginServicesList )
    {
        if ( service && service->settingsHandler )
        {
            settings.beginGroup( service->settingsHandler->settingNameCompact() );
            service->settingsHandler->saveExperimentSettings( settings );
            settings.endGroup();
        }
    }
}

void
PluginManager::loadExperimentSettings( QSettings& settings )
{
    // load settings for each registered pluginService
    foreach( PluginServices * service, pluginServicesList )
    {
        if ( service && service->settingsHandler )
        {
            settings.beginGroup( service->settingsHandler->settingNameCompact() );
            service->settingsHandler->loadExperimentSettings( settings );
            settings.endGroup();
        }
    }
}



PluginServices*
PluginManager::createInternalService()
{
    PluginServices* service = new PluginServices( this, NO_PLUGIN );
    pluginServicesList.append( service );
    return service;
}

void
PluginManager::fillHelpMenu()
{
    helpMenu->clear();
    for ( int i = 0; i < PluginList::getCubePluginCount(); ++i )
    {
        CubePlugin* plugin = PluginList::getCubePlugin( i );
        if ( plugin )
        {
            QString helpMsg = plugin->getHelpText();
            if ( !helpMsg.isEmpty() )
            {
                QAction* action = new QAction( plugin->name(), this );
                action->setData( i );
                helpMenu->addAction( action );
                connect( action, SIGNAL( triggered( bool ) ), this, SLOT( showPluginHelp() ) );
            }
        }
    }
}

void
PluginManager::showPluginHelp()
{
    QAction*    action = qobject_cast<QAction*>( sender() );
    int         index  = action->data().toInt();
    CubePlugin* plugin = PluginList::getCubePlugin( index );
    if ( !helpWidget )
    {
        helpWidget = new InfoWidget( Globals::getMainWindow() );
        helpWidget->setWindowTitle( "Plugin Help" );
    }
    helpWidget->showText( plugin->name(), plugin->getHelpText() );
    helpWidget->raise();
}

// ============== context free plugins ====================================

#include "ContextFreeServices.h"

/**
 * @brief PluginManager::setContextFreePluginMenu
 * @param menu the menu which is filled with the labels of context free plugins
 */
void
PluginManager::fillContextFreePluginMenu()
{
    contextFreePluginMenu->clear();
    int pluginCount = PluginList::getContextFreePluginCount();
    if ( pluginCount == 0 )
    {
        contextFreePluginMenu->setEnabled( false );
    }
    else
    {
        contextFreePluginMenu->setEnabled( true );
        for ( int index = 0; index < pluginCount; ++index )
        {
            ContextFreePlugin* plugin = PluginList::getContextFreePlugin( index );
            QAction*           pa     = new QAction( plugin->versionLabel(), this );
            contextFreePluginMenu->addAction( pa );
            pa->setData( index );
            connect( pa, SIGNAL( triggered() ), this, SLOT( startPlugin() ) );
        }
        contextFreePluginMenu->addSeparator();
        closeContextAction = new QAction( "Close current Plugin", this );
        closeContextAction->setEnabled( false );
        contextFreePluginMenu->addAction( closeContextAction );
        connect( closeContextAction, SIGNAL( triggered() ), this, SLOT( closeContextFreePlugin() ) );
    }
}

/** slot for menu action */
void
PluginManager::startPlugin()
{
    // close active plugin, if exists
    if ( currentContextFreePlugin )
    {
        currentContextFreePlugin->closed();
    }

    dynamic_cast<MainWidget *>(mainWindow)->setContext( MainWidget::CONTEXT_FREE );

    // open chosen plugin
    QAction*           action = qobject_cast<QAction*>( sender() );
    int                index  = action->data().toInt();
    ContextFreePlugin* plugin = PluginList::getContextFreePlugin( index ); // selected plugin from menu
    currentContextFreePlugin = plugin;
    plugin->opened( ContextFreeServices::getInstance() );
    closeContextAction->setEnabled( true );
}

/** closes active context free plugin */
void
PluginManager::closeContextFreePlugin()
{
    if ( currentContextFreePlugin )
    {
        currentContextFreePlugin->closed();

        // remove layout items of the previously started plugin
        QWidget* w      = ContextFreeServices::getInstance()->getWidget();
        QLayout* layout = w->layout();
        while ( layout->count() > 0 )
        {
            QWidget* child = layout->itemAt( 0 )->widget();
            delete child;
        }
        delete layout;

        closeContextAction->setEnabled( false );

        dynamic_cast<MainWidget *>(mainWindow)->setContext( MainWidget::CONTEXT_INIT );
    }
}

// ============== end context free plugins ====================================

void
PluginManager::resizeTreeItemMarker( int iconHeight )
{
    foreach( PluginServices * service, pluginServicesList )
    {
        if ( service )
        {
            service->resizeTreeItemMarker( iconHeight );
        }
    }
}

QList<TreeItemMarker*>
PluginManager::getTreeItemMarker()
{
    QList<TreeItemMarker*> list;

    foreach( PluginServices * service, pluginServicesList )
    {
        if ( service )
        {
            foreach( TreeItemMarker * marker, service->markerList )
            list.append( marker );
        }
    }
    return list;
}

/**
 * @brief PluginManager::setMarkerConfig
 * enables or disables the marker dialog menu item and updates the marker dialog
 */
void
PluginManager::setMarkerConfig()
{
    if ( markerMenuAction )
    {
        if ( getTreeItemMarker().size() == 0 )
        {
            markerMenuAction->setEnabled( false );
        }
        else
        {
            markerMenuAction->setEnabled( true );
        }
    }
    if ( markerDialog && markerDialog->isVisible() )
    {
        if ( getTreeItemMarker().size() == 0 )
        {
            markerDialog->grayItems( false );
            markerDialog->setVisible( false );
        }
        else
        {
            showTreeItemMarkerSettings();
        }
    }
}

void
PluginManager::showTreeItemMarkerSettings()
{
    QPoint pos;
    if ( markerDialog )
    {
        pos = markerDialog->pos();
    }
    delete markerDialog;
    markerDialog = new TreeItemMarkerDialog( tabManager );
    if ( !pos.isNull() )
    {
        markerDialog->move( pos );
    }
    markerDialog->setVisible( true );
}

//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------

#include <QApplication>

PathDialog::PathDialog( QWidget* parent ) : QDialog( parent )
{
    this->setWindowTitle( "plugin search path" );
    this->setModal( true );
    this->setMinimumWidth( 600 );

    this->setWhatsThis( tr( "Prepends the given pathes to the plugin search path. Already loaded plugins aren't replaced until restart of cube." ) );

    QVBoxLayout* topLayout = new QVBoxLayout;
    this->setLayout( topLayout );

    QWidget*     horizontal = new QWidget();
    QHBoxLayout* hlay       = new QHBoxLayout;
    hlay->setContentsMargins( 0, 0, 0, 0 );
    horizontal->setLayout( hlay );
    topLayout->addWidget( horizontal );

    line = new QLineEdit();
    hlay->addWidget( line );

    QPushButton* but = new QPushButton( QApplication::style()->standardIcon( QStyle::SP_FileDialogStart ), "" );
    connect( but, SIGNAL( pressed() ), this, SLOT( choose() ) );
    hlay->addWidget( but );

    listWidget = new QListWidget( this );
    listWidget->setSelectionMode( QAbstractItemView::SingleSelection );
    connect( listWidget, SIGNAL( itemActivated( QListWidgetItem* ) ), this, SLOT( edit() ) );

    topLayout->addWidget( listWidget );

    horizontal = new QWidget();
    hlay       = new QHBoxLayout;
    hlay->setContentsMargins( 0, 0, 0, 0 );
    horizontal->setLayout( hlay );
    topLayout->addWidget( horizontal );

    but = new QPushButton( "add" );
    connect( but, SIGNAL( pressed() ), this, SLOT( add() ) );
    hlay->addWidget( but );

    but = new QPushButton( "delete" );
    connect( but, SIGNAL( pressed() ), this, SLOT( deleteRow() ) );
    hlay->addWidget( but );

    but = new QPushButton( QApplication::style()->standardIcon( QStyle::SP_ArrowUp ), "" );
    connect( but, SIGNAL( pressed() ), this, SLOT( up() ) );
    hlay->addWidget( but );

    but = new QPushButton( QApplication::style()->standardIcon( QStyle::SP_ArrowDown ), "" );
    connect( but, SIGNAL( pressed() ), this, SLOT( down() ) );
    hlay->addWidget( but );
    hlay->addStretch( 1 );


    but = new QPushButton( QApplication::style()->standardIcon( QStyle::SP_DialogOkButton ), "Ok" );
    connect( but, SIGNAL( pressed() ), this, SLOT( close() ) );
    hlay->addWidget( but );
}

QStringList
PathDialog::getPath( QStringList init )
{
    PathDialog* pd = new PathDialog( Globals::getMainWindow() );

    foreach( QString str, init )
    {
        new QListWidgetItem( str, pd->listWidget );
    }

    pd->exec();

    QStringList list;
    for ( int i = 0; i < pd->listWidget->count(); i++ )
    {
        list.append( pd->listWidget->item( i )->text() );
    }
    return list;
}


void
PathDialog::edit()
{
    QString txt = listWidget->currentItem()->text();
    line->setText( txt );
}

void
PathDialog::up()
{
    int              newPos = ( listWidget->currentRow() + listWidget->count() - 1 ) % listWidget->count();
    QListWidgetItem* item   = listWidget->takeItem( listWidget->currentRow() );
    listWidget->insertItem( newPos, item );
    listWidget->setCurrentItem( item );
}

void
PathDialog::down()
{
    int              newPos = ( listWidget->currentRow() + 1 ) % listWidget->count();
    QListWidgetItem* item   = listWidget->takeItem( listWidget->currentRow() );
    listWidget->insertItem( newPos, item );
    listWidget->setCurrentItem( item );
}

void
PathDialog::deleteRow()
{
    listWidget->takeItem( listWidget->currentRow() );
}


void
PathDialog::add()
{
    QString path = line->text().trimmed();
    if ( !path.isEmpty() )
    {
        new QListWidgetItem( path, listWidget );
    }
}

void
PathDialog::choose()
{
    QString dir = QFileDialog::getExistingDirectory( Globals::getMainWindow(), tr( "Open Directory" ), line->text().trimmed()
                                                     , QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
    line->setText( dir );
}
