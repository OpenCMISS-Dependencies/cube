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


#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QMainWindow>
#include <QLayout>
#include "Constants.h"
#include "TabWidget.h"
#include "PluginServices.h"
#include "Settings.h"
#include "TabManager.h"
#include "InfoWidget.h"

class TreeListener;
class PluginServices;
class CubePlugin;
class ContextFreePlugin;
class TreeItemMarkerDialog;

namespace cube
{
class Cube;
}

/**
 * Pluginmanager
 */

class PluginManager : public QObject, public InternalSettingsHandler
{
    Q_OBJECT
public:
    static PluginManager*
    getInstance();

    static void
    disablePlugins();

    void
    initializePlugins();

    /** returns main plugin menu, which has to be inserted into the menu bar */
    QMenu*
    getPluginMenu() const;

    QMenu*
    getPluginHelpMenu() const;

    QMenu*
    getContextFreePluginMenu() const;

    void
    opened( cube::Cube*    cube,
            const QString& file,
            TabManager*    tabManager );
    void
    openPlugins();

    void
    closed();

    /** return PluginService object for use outside of a plugin */
    PluginServices*
    createInternalService();

    // ---- virtual methods of class InternalSettingsHandler
    void
    loadGlobalStartupSettings( QSettings& settings );
    void
    saveGlobalStartupSettings( QSettings& settings );
    void
    saveGlobalSettings( QSettings& settings );
    void
    loadGlobalSettings( QSettings& settings );
    void
    saveExperimentSettings( QSettings& settings );
    void
    loadExperimentSettings( QSettings& settings );
    QString
    settingName();

    void
    setPluginStatus( bool& all,
                     bool& none,
                     QHash<QString, bool>& plugins );
    void
    getPluginStatus( bool& all,
                     bool& none,
                     QHash<QString, bool>& plugins );

    // ----- functions that emit signals in PluginServices

    /** called before context menu of a tree widget is shown */
    void
    contextMenuIsShown( TreeType  type,
                        TreeItem* item );

    /** called, after tree widget item has been selected or deselected */
    void
    treeItemIsSelected( TreeType  type,
                        TreeItem* item );

    /** called after order of tabs has changed */
    void
    setTabOrder( const QList<DisplayType>& order );

    /** Sets the value of the given name and notifies all plugins that the value has changed .
     *  If sender is given, the sender isn't notified.
     */
    void
    setGlobalValue( const QString&  name,
                    const QVariant& value,
                    PluginServices* sender = 0 );

    void
    genericUserAction( UserAction action );

    void
    tabActivated( TreeType type );

    // ----- end functions that emit signals in PluginServices

    const QList<ColorMap*>
    getColorMaps()
    {
        return colormaps.values();
    }

    QMainWindow*
    getMainWindow();
    void
    setMainWindow( QMainWindow* main );

    void
    resizeTreeItemMarker( int iconHeight );

    QList<TreeItemMarker*>
    getTreeItemMarker();

    QAction*
    getMarkerConfigAction();

    friend class PluginServices;

private slots:
    void
    enablePlugin( bool );
    void
    showSettingsWidget();
    void
    checkPluginMenuStatus();
    void
    showPluginHelp();
    void
    showTreeItemMarkerSettings();

    // context free plugins
    void
    startPlugin();
    void
    closeContextFreePlugin();
    void
    configureSearchPath();

private:
    PluginManager();
    void
    openAllPlugins();
    bool
    openPlugin( int index );
    void
    closePlugin( int index );
    void
    setMarkerConfig();
    void
    fillHelpMenu();
    void
    fillContextFreePluginMenu();

    QMainWindow*       mainWindow;           // required for toolbar, status bar...
    TabManager*        tabManager;
    QMenu*             pluginMenu;
    cube::Cube*        cube;
    QList<DisplayType> order;
    QString            cubeFileName;

    QList<QMenu* >         pluginMenuList;               // contains parent menu for each plugin
    QList<PluginServices*> pluginServicesList;           // contains plugin service for each plugin and internal service
    QList<QAction*>        pluginActivationAction;       // active/deactivate action for each plugin
    QAction*               markerMenuAction;             // pops up marker configuration dialog
    QAction*               configPathItem;
    QAction*               initialSettingsItem;
    QMenu*                 pluginStatusMenu;
    QMenu*                 helpMenu;
    QMenu*                 contextFreePluginMenu;

    QMultiHash<PluginServices*, ColorMap*>  colormaps;   // colormaps which are registered by a plugin
    QMultiHash<PluginServices*, QToolBar* > toolBarHash; // contains the ToolBar(s) for each registered service

    QHash<QString, bool>     pluginStatus;               // contains current status (enabled/disabled) for each plugin
    QHash<QString, QVariant> globalValues;               // allow the plugins to read and write global values
    PluginServices*          internalServices;
    bool                     enableAllPlugins;           // if true, all plugins are ensabled
    bool                     disableAllPlugins;          // if true, all plugins are disabled
    static bool              disableAlways;
    static PluginManager*    single;

    // context free plugins
    ContextFreePlugin* currentContextFreePlugin;
    QAction*           closeContextAction;

    InfoWidget*           helpWidget;
    TreeItemMarkerDialog* markerDialog;
    QStringList           pluginPath;
};

#include <QListWidget>

class PathDialog : public QDialog
{
    Q_OBJECT
public:
    static QStringList
    getPath( QStringList init = QStringList() );

private slots:
    void
    choose();
    void
    add();
    void
    edit();
    void
    deleteRow();
    void
    up();
    void
    down();

private:
    PathDialog( QWidget* parent = 0 );
    QLineEdit*   line;
    QListWidget* listWidget;
};

#endif // PLUGINMANAGER_H
