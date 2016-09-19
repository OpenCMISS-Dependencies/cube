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


#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include "PluginServices.h"
class SettingsToolBar;


class InternalSettingsHandler : public SettingsHandler
{
public:
    /** this method is only called once, at startup */
    virtual void
    loadGlobalStartupSettings( QSettings& settings ) = 0;

    /** this method is only called once, if program ends */
    virtual void
    saveGlobalStartupSettings( QSettings& settings ) = 0;
};

/**
   Three types of settings are available:
   - global settings without a group name (= groupname "General"), which are used to save settings
    options itself and the plugin settings. They are automatically saved in the native format.
   - global settings with default group or named group are also saved in native format. The default
    group with name "last" is automatically loaded if the program starts and automatically saved on
    exit. The named settings are created if the menu item "Save Global Settings as" is selected.
   - experiment settings contain settings which depend on the loaded cube file. They are saved in
    cubeFileBasename.ini. They also contain the global settings. Experiment settings are automatically
    saved with groupname "last" if the cube file is closed. They may also contain additional named
    entries.

   group structure of global settings in native format:
    -General
    -settings
      -last
      -settingNameA
   group structure of experiment settings in cubeFile.ini:
    -globalSettings
    -experiment
      -last
      -experimentNameA
 */
class Settings : public QObject
{
    Q_OBJECT
public:
    /**
     * @param settingsMenu menu to fill with setting
     * @param handler list of SettingHandler whose methods will be called if settings are loaded or saved */
    Settings( QMenu*                                 settingsMenu,
              const QList<InternalSettingsHandler*>& handler );

    /** destroys the Settings object and saves the startup settings */
    ~Settings();

    /** loads the default experiment settings from the given location */
    void
    cubeLoaded( const QString& cubeFile );

    /** saves the default experiment settings to file */
    void
    cubeClosed();

public slots:
    /** saves the experiment settings of the previouly loaded cube
       @param group name of the settings */
    void
    saveExperimentSettings( const QString& group = "last" );

    /** load the experiment settings from the previouly loaded cube
       @param group name of the settings */
    void
    loadExperimentSettings( const QString& group = "last" );

private slots:
    void
    setGlobalBoolSetting( bool state );
    void
    saveNamedGlobalSettings();
    void
    loadNamedGlobalSettings();
    void
    deleteNamedGlobalSettings();

private:
    /** fills the given settings menu with menu items */
    void
    fillMenu( QMenu* settingsMenu );

    QAction*
    getSettingMenuAction( const QString& label,
                          const QString& name,
                          bool           initialState );
    bool
    settingIsTrue( const QString& name );

    /** saves global settings to system location */
    void
    saveGlobalSettings( const QString& group = "last" );

    /** loads global settings from system location */
    void
    loadGlobalSettings( const QString& group = "last" );

    void
    loadExperimentSettings( QSettings&     settings,
                            const QString& group );
    void
    saveExperimentSettings( QSettings&     settings,
                            const QString& group );
    void
    loadGlobalSettings( QSettings&     settings,
                        const QString& group = "last" );
    void
    saveGlobalSettings( QSettings&     settings,
                        const QString& group = "last" );

    QList<InternalSettingsHandler*> handlerList;

    QList<QAction*>  settingActions;
    QString          iniFile;                // location of ini file to save experiment settings
    SettingsToolBar* bar;                    // toolbar for experiment specific settings
};

#endif // SETTINGS_H
