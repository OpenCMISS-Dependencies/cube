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



#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>
#include "Settings.h"
#include "CubeServices.h"
#include "PluginManager.h"
#include "SettingsToolBar.h"

#define STARTUP "startup"


Settings::Settings( QMenu*                                 settingsMenu,
                    const QList<InternalSettingsHandler*>& handler )
{
    // create toolbar
    bar = new SettingsToolBar();
    PluginManager::getInstance()->getMainWindow()->addToolBar( bar );
    connect( bar, SIGNAL( saveExperimentSettings( QString ) ), this, SLOT( saveExperimentSettings( QString ) ) );
    connect( bar, SIGNAL( loadExperimentSettings( QString ) ), this, SLOT( loadExperimentSettings( QString ) ) );
    bar->setVisible( false );

    // fill settings menu with items
    fillMenu( settingsMenu );

    QSettings settings;
    settings.beginGroup( STARTUP );

    // initialize checkable menu items from settings
    foreach( QAction * action, settingActions )
    {
        QString  name = action->data().toString();
        QVariant val  = settings.value( name );
        if ( val.isValid() )
        {
            action->setChecked( val.toBool() );
        }
    }

    // load startup settings, which are only loaded once e.g. list of recently opened files
    handlerList = handler;
    foreach( InternalSettingsHandler * handler, handlerList )
    {
        handler->loadGlobalStartupSettings( settings );
    }
    settings.endGroup();

    loadGlobalSettings();
}

Settings::~Settings()
{
    QSettings settings;
    settings.beginGroup( STARTUP );
    foreach( InternalSettingsHandler * handler, handlerList )
    {
        handler->saveGlobalStartupSettings( settings );
    }
    settings.endGroup();
}

void
Settings::fillMenu( QMenu* settingsMenu )
{
    settingsMenu->setStatusTip( tr( "Ready" ) );
    settingsMenu->setWhatsThis( "This menu item offers the saving, loading, and the deletion of settings. On the one hand, global settings store the outlook of the application like the widget sizes, color and precision settings, the order of panes, etc. On the other hand, experiment settings can also store which data is loaded, which tree nodes are expanded, etc." );

    QAction* saveSettingsAct = new QAction( tr( "&Save global settings as..." ), this );
    saveSettingsAct->setStatusTip( tr( "Saves named global settings" ) );
    connect( saveSettingsAct, SIGNAL( triggered() ), this, SLOT( saveNamedGlobalSettings() ) );
    settingsMenu->addAction( saveSettingsAct );
    QAction* loadSettingsAct = new QAction( tr( "&Load global settings..." ), this );
    loadSettingsAct->setStatusTip( tr( "Loads the specified global settings" ) );
    connect( loadSettingsAct, SIGNAL( triggered() ), this, SLOT( loadNamedGlobalSettings() ) );
    settingsMenu->addAction( loadSettingsAct );
    QAction* delSettingsAct = new QAction( tr( "&Delete global settings..." ), this );
    connect( delSettingsAct, SIGNAL( triggered() ), this, SLOT( deleteNamedGlobalSettings() ) );
    settingsMenu->addAction( delSettingsAct );
    settingsMenu->addSeparator();

    QAction* saveExpSettingsAct = new QAction( tr( "Save experiment specific settings" ), this );
    saveExpSettingsAct->setStatusTip( tr( "Saves experiment specific settings info into an ini-file" ) );
    settingsMenu->addAction( saveExpSettingsAct );
    connect( saveExpSettingsAct, SIGNAL( triggered() ), this, SLOT( saveExperimentSettings() ) );
    settingsMenu->addSeparator();

    settingsMenu->addSeparator();
    // settingsMenu->addAction( getSettingMenuAction( tr( "Save experiment settings before cube file is closed" ), "saveExperiment", false ) );
    settingsMenu->addAction( getSettingMenuAction( tr( "Restore global settings for each experiment" ), "restoreExperimentGlobals", true ) );
    settingsMenu->addAction( getSettingMenuAction( tr( "Restore experiment specific settings" ), "restoreExperiment", false ) );
    settingsMenu->addAction( getSettingMenuAction( tr( "Restore window geometry for each experiment" ), "restoreExperimentGeometry", false ) );
    settingsMenu->addSeparator();
    settingsMenu->addAction( getSettingMenuAction( tr( "Show experiment settings toolbar" ), "showSettingsToolbar", true ) );
}

void
Settings::saveNamedGlobalSettings()
{
    QWidget*  widget = PluginManager::getInstance()->getMainWindow();
    QSettings settings;
    settings.beginGroup( "settings" );

    QStringList settingsList = settings.childGroups();
    bool        ok;
    QString     settingsName =
        QInputDialog::getItem( widget,
                               "Save settings",
                               "Save settings under the name:",
                               settingsList,
                               0,
                               true,
                               &ok );

    // save settings if everything is ok with the input
    if ( ok && !settingsName.isEmpty() )
    {
        saveGlobalSettings( settingsName );
    }
    settings.endGroup();
}

void
Settings::loadNamedGlobalSettings()
{
    QWidget*  widget = PluginManager::getInstance()->getMainWindow();
    QSettings settings;
    settings.beginGroup( "settings" );

    QStringList settingsList = settings.childGroups();
    bool        ok;
    QString     settingsName =
        QInputDialog::getItem( widget,
                               "Load settings",
                               "Load settings with name:",
                               settingsList,
                               0,
                               false,
                               &ok );
    if ( ok && !settingsName.isEmpty() )
    {
        loadGlobalSettings( settings, settingsName );
    }
    settings.endGroup();
}

void
Settings::deleteNamedGlobalSettings()
{
    QWidget*  widget = PluginManager::getInstance()->getMainWindow();
    QSettings settings;
    settings.beginGroup( "settings" );

    QStringList settingsList = settings.childGroups();
    bool        ok;
    QString     settingsName =
        QInputDialog::getItem( widget,
                               "Delete settings",
                               "Delete settings with name:",
                               settingsList,
                               0,
                               false,
                               &ok );
    if ( ok && !settingsName.isEmpty() )
    {
        settings.remove( settingsName );
    }
    settings.endGroup();
}

/**
   returns a checkable menu item. The value of the item will be saved to settings and restored from settings
   @param label menu label
   @param name variable name for the preferences
   @param initialState true, if checked is default
 */
QAction*
Settings::getSettingMenuAction( const QString& label, const QString& name, bool initialState )
{
    QAction* set = new QAction( label, this );
    settingActions.append( set );
    set->setCheckable( true );
    set->setData( name );
    connect( set, SIGNAL( toggled( bool ) ), this, SLOT( setGlobalBoolSetting( bool ) ) );
    QSettings settings;
    settings.beginGroup( STARTUP );
    if ( !settings.value( name ).isValid() ) // initialize if not saved in settings
    {
        settings.setValue( name, initialState );
    }
    settings.endGroup();
    set->setChecked( settingIsTrue( name ) );

    return set;
}

/** return true, if an entry with the given name and the value true exists in the global settings */
bool
Settings::settingIsTrue( const QString& name )
{
    QSettings settings;
    settings.beginGroup( STARTUP );
    return settings.value( name, false ).toBool();
    settings.endGroup();
}


/**
   slot, which saves state of an action to global settings in section [startup]
 */
void
Settings::setGlobalBoolSetting( bool state )
{
    QAction* action = qobject_cast<QAction*>( sender() );
    QString  name   = action->data().toString();

    QSettings settings;
    settings.beginGroup( STARTUP );
    settings.setValue( name, state );

    if ( name == "showSettingsToolbar" && !iniFile.isEmpty() )
    {
        if ( bar )
        {
            bar->setVisible( state );
        }
    }

    settings.endGroup();
}

void
Settings::cubeLoaded( const QString& cubeFile )
{
    // always load global settings after having read a cube file, because plugins only get active after
    // an experiment has been loaded and after that global plugin settings may be set
    loadGlobalSettings();

    iniFile = QString( cube::services::get_cube_name( cubeFile.toStdString() ).c_str() ).append( ".ini" );
    loadExperimentSettings();
    bar->setVisible( settingIsTrue( "showSettingsToolbar" ) );
}

void
Settings::cubeClosed()
{
    if ( !settingIsTrue( "saveExperiment" ) )
    {
        saveExperimentSettings();
    }
    saveGlobalSettings();
    iniFile = "";
    bar->setVisible( false );
}

void
Settings::loadExperimentSettings( const QString& group )
{
    QSettings experimentSettings( iniFile, QSettings::IniFormat );
    bar->cubeOpened( experimentSettings );
    loadExperimentSettings( experimentSettings, group );
}

void
Settings::saveExperimentSettings( const QString& group )
{
    QSettings experimentSettings( iniFile, QSettings::IniFormat );
    saveExperimentSettings( experimentSettings, group );
    bar->cubeOpened( experimentSettings ); // load new setting group
}

// --------------- private -----------------------
/** Settings::loadGlobalSettings loads global settings from system location */
void
Settings::loadGlobalSettings( const QString& group )
{
    QSettings settings;
    settings.beginGroup( "settings" );
    loadGlobalSettings( settings, group );
    settings.endGroup();
}

/** Settings::saveGlobalSettings saves global settings to system location */
void
Settings::saveGlobalSettings( const QString& group )
{
    QSettings settings;
    settings.beginGroup( "settings" );
    saveGlobalSettings( settings, group );
    settings.endGroup();
}

/** Settings::loadGlobalSettings loads the global settings from the given location settings. */
void
Settings::loadGlobalSettings( QSettings& settings, const QString& group )
{
    settings.beginGroup( group );
    foreach( SettingsHandler * handler, handlerList )
    {
        settings.beginGroup( handler->settingNameCompact() );
        handler->loadGlobalSettings( settings );
        settings.endGroup(); // settingName
    }
    settings.endGroup();     // group
}

/** Settings::saveGlobalSettings saves the global settings to the given location settings. */
void
Settings::saveGlobalSettings( QSettings& settings, const QString& group )
{
    settings.beginGroup( group );
    foreach( SettingsHandler * handler, handlerList )
    {
        settings.beginGroup( handler->settingNameCompact() );
        handler->saveGlobalSettings( settings );
        settings.endGroup(); // settingName
    }
    settings.endGroup();     // group
}

/** saves the global settings and the settings which belong to a specific experiment
 * @param group optional parameter, if more than one state of the experiment is saved, a name
 * for the state has to be given.
 */
void
Settings::saveExperimentSettings( QSettings& settings, const QString& group )
{
    saveGlobalSettings( settings, "globalSettings" );

    settings.beginGroup( "experiment" );
    settings.beginGroup( group ); // name selected by the user or "last"
    foreach( SettingsHandler * handler, handlerList )
    {
        settings.beginGroup( handler->settingNameCompact() );
        handler->saveExperimentSettings( settings );
        settings.endGroup(); // settingName
    }
    settings.endGroup();     // group
    settings.endGroup();     // experiment
}

void
Settings::loadExperimentSettings( QSettings& settings, const QString& group )
{
    bool restoreExperiment = true;
    if ( !settingIsTrue( "restoreExperiment" ) )
    {
        restoreExperiment = false; // experiment settings are disabled
    }
    else
    {
        settings.beginGroup( "experiment" );
        if ( !settings.childGroups().contains( group ) ) // local ini file is invalid/empty
        {
            restoreExperiment = false;
        }
        settings.endGroup();
    }

    if ( settingIsTrue( "restoreExperimentGlobals" ) )
    {
        loadGlobalSettings( settings, "globalSettings" );
    }

    if ( restoreExperiment )
    {
        settings.beginGroup( "experiment" );
        settings.beginGroup( group );
        foreach( SettingsHandler * handler, handlerList )
        {
            settings.beginGroup( handler->settingNameCompact() );
            handler->loadExperimentSettings( settings );
            settings.endGroup(); // settingName
        }
        settings.endGroup();     // group
        settings.endGroup();     // experiment
    }
}
