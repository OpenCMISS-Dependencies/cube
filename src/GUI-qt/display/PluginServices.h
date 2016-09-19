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




#ifndef PLUGINSERVICES_H
#define PLUGINSERVICES_H
#include <QtGui>
#include <QToolBar>
#include <QMenu>

#include "CubeTypes.h"

#include "CubeVertex.h"
#include "Constants.h"
#include "TreeItem.h"
#include "AggregatedTreeItem.h"
#include "TabInterface.h"
#include "TreeItemMarker.h"
#include "ColorMap.h"
#include "Globals.h"

/**
 * The class PluginServices provides the interface for all plugins to the cube GUI and cube
 * data. If a function is added or the signature of a function has changed, the version number
 * of the identifier string in PluginInterface.h (PLUGIN_VERSION) has to be increased
 */

class MainWidget;
class PluginManager;
class Tree;
class TabWidget;

namespace cube
{
class Cube;
}

/**
 * SettingsHandler provides methods which are called, if the settings are loaded or saved
 * @see PluginServices::addSettingsHandler
 * @see examples/GUI/plugins/plugin-demo
 */
class SettingsHandler
{
public:
    virtual void
    loadGlobalSettings( QSettings& ) = 0;
    virtual void
    saveGlobalSettings( QSettings& ) = 0;

    virtual void
    loadExperimentSettings( QSettings& )
    {
    };
    virtual void
    saveExperimentSettings( QSettings& )
    {
    };
    virtual QString
    settingName() = 0;

    QString
    settingNameCompact() // blanks would be escaped in ini file with a longer sequence
    {
        return settingName().remove( ' ' );
    }
};

class PluginServices : public QObject
{
    Q_OBJECT
signals:
    /** This signal is emitted if the user selects one ore more tree items. To get all
     * selected items @see getSelections( TreeType type )
     * @param type METRICTREE, CALLTREE, CALLFLAT, SYSTEMTREE
     * @param item the recently selected item
     */
    void
    treeItemIsSelected( TreeType  type,
                        TreeItem* item );

    /** This signal is emitted if the order of the tabs has changed.
     *  @param order list which contains METRICWIDGET, CALLWIDGET, SYSTEMWIDGET in the order the user
     *  has selected
     */
    void
    orderHasChanged( const QList<DisplayType>& order );

    /** This signal is emitted if the context menu of the given tree item in tree widget type will be shown.
     *  Additional menu items may be added now.
     * @param type METRICTREE, CALLTREE, CALLFLAT, SYSTEMTREE
     * @param item the item, the user has clicked with the right mouse
     */
    void
    contextMenuIsShown( TreeType  type,
                        TreeItem* item );

    /** This signal is emitted for special user actions, eg. if a tree item is marked as loop  */
    void
    genericUserAction( UserAction action );

    /**
       This signal is emitted if a global value has changed.
       @param name the name of the global value
       @see getGlobalValue
     */
    void
    globalValueChanged( const QString& name );

    /**
       This signal is emitted if a tab is selected.
     */
    void
    tabActivated( TreeType type );

public:
    // ------------------------------------
    /** Returns a pointer to the cube data */
    cube::Cube*
    getCube() const;

    /** Returns the name of the loaded cube file including the path */
    const QString&
    getCubeFileName() const;

    /** Returns the basename of the currently loaded cube file including the path */
    QString
    getCubeBaseName() const;

    /** If a statistics file exists, the filename of that file is returned, otherwise an empty string */
    QString
    getStatName() const;

    /** returns a widget which can be used e.g. as parent widget of dialogs, if the plugins doesn't define tabs */
    QWidget*
    getParentWidget() const;

    // -------- functions to access tree items ---------------
    /** Returns a list with all tree items of the given tree */
    const QList<TreeItem*>&
    getTreeItems( TreeType type ) const;

    /** Returns a list with the top level tree items of the given tree.
     * For the calltree and systemtree, TopLevelItems actually refer to the visible root nodes in CUBE.*/
    const QList<TreeItem*>
    getTopLevelItems( TreeType type ) const;

    /** Returns the system tree item with the given id */
    TreeItem*
    getSystemTreeItem( uint32_t sysId ) const;

    /** Returns the call tree item with the given id */
    TreeItem*
    getCallTreeItem( uint32_t cnodeId ) const;

    /** Returns the metric tree item with the given id */
    TreeItem*
    getMetricTreeItem( std::string metricId ) const;

    // -------- end functions to access tree items ---------------

    /** Marks the given tree item item combination with a marker. The items of the left
     * and the middle tree are marked as dependency, the item of the rightmost tree is marked with the
     * given marker.
     * @param marker @see getTreeItemMarker
     * If an item with NULL value is given, the rightmost item left to this item is marked.
     * Examples: DemoPlugin, StatisticPlugin, LaunchPlugin
     * */
    void
    addMarker( const TreeItemMarker* marker,
               TreeItem*             metric,
               TreeItem*             call,
               TreeItem*             system );

    /** Marks the given tree item with a marker, use @see updateTreeView to activate it.
     * The plugin has manage the dependencies, e.g. to add or remove the markers if the tree order
     * changes.
     * This function should only be used, if the marker depends on runtime values.
     * @param item the item to which the marker is added
     * @param marker @see getTreeItemMarker
     * @param isDependency if true, the item is marked as a dependency to another marked item (usually
     * of another tree)
     */
    void
    addMarker( TreeItem*             item,
               const TreeItemMarker* marker,
               bool                  isDependency = false );

    /** removes the given marker from the given tree item */
    void
    removeMarker( TreeItem*             item,
                  const TreeItemMarker* marker );

    /** removes all plugin managed marker which have been set by this plugin */
    void
    removeMarker();

    /** removes all plugin managed marker from the given tree which have been set by this plugin */
    void
    removeMarker( TreeType type );

    /** returns a new tree item marker, which can be used to marks tree items with
     * different background color or other attributes, which can be chosen by the user
     * @param label text for the color legend
     * @param icons list of pixmaps of ascending size. The best fitting icon will be shrunk to tree item height
     * and displayed for each marked item.
     */
    const TreeItemMarker*
    getTreeItemMarker( const QString&        label,
                       const QList<QPixmap>& icons = QList<QPixmap>( ) );

    /** Returns order of tabs, default order is METRICWIDGET, CALLWIDGET, SYSTEMWIDGET */
    const QList<DisplayType>&
    getOrder() const;

    /** Returns the currenly selected value modus */
    ValueModus
    getValueModus() const;

    // --- functions to handle selections  ---------------------------------

    /** Returns last selected item in the given tree @param type METRICTREE, CALLTREE, CALLFLAT, SYSTEMTREE */
    TreeItem*
    getSelection( TreeType type ) const;

    /** Returns selected items in the given tree  @param type METRICTREE, CALLTREE, CALLFLAT, SYSTEMTREE */
    const QList<TreeItem*>&
    getSelections( TreeType type ) const;

    /** Returns true, if the currenly selected metric item uses integer values */
    bool
    intMetricSelected() const;

    /** Selects the given item.
     *  @param item The tree item to select
     *  @param add If add is false, all previously selected items are deselected. If
     *  add is true and the item already is selected, it becomes deselected
     */
    void
    selectItem( TreeItem* item,
                bool      add );

    // --- functions for menus, statusbar, toolbar  ---------------------------------

    /** Inserts a menu item to the context menu of the treeWidget of the given type and returns the corresponding action.
     *  The action is automatically deleted after the context menu is closed.
     * @param type METRICTREE, CALLTREE, CALLFLAT, SYSTEMTREE
     * @param menuItemText the label of the created context menu item
     */
    QAction*
    addContextMenuItem( TreeType       type,
                        const QString& menuItemText );

    /** Adds a submenu with the plugin label to the plugin menu and returns a pointer to the created QMenu.
     */
    QMenu*
    enablePluginMenu();

    /** Adds the given toolbar below the menu bar */
    void
    addToolBar( QToolBar* toolbar );

    /** Removes the previously added toolbar */
    void
    removeToolBar( QToolBar* toolbar );

    /** Writes the given message to the status line at the bottom of the cube window */
    void
    setMessage( const QString& str,
                MessageType    type = Information );

    // --- system tree related function ---------------------------------

    /** Returns the index of the currenly selected subset of the system tree. */
    int
    getActiveSubsetIndex() const;

    /** Returns the contents of the currenly selected subset of the system tree. */
    const QList<TreeItem*>&
    getActiveSubset() const;

    /** Sets the currenly selected subset of the system tree to the given index. */
    void
    setActiveSubset( int index );

    /** Returns a list of all names of the defined subsets for the system tree. */
    QStringList
    getSubsetLabelList() const;

    // --- end system tree related function ---------------------------------

    // --- functions which are only available for TabInterface subclasses -------------------------

    /**
     * adds a tab to METRICTAB, CALLTAB or SYSTEMTAB
     */
    void
    addTab( DisplayType   type,
            TabInterface* tab,
            int           addAtIndex = -1 );

    /** Enables or disables the given tab. If disabled, the tab gets inactive and greyed out */
    void
    enableTab( TabInterface* tab,
               bool          enabled );

    /** Clears the value widget, the widget only shows an empty area
     */
    void
    clearValueWidget();

    /** Updates the value widget which shows three lines.
     * @param minValue the minimum value
     * @param maxValue the maximum value
     * @param selectedValue the currently selected value or the aggragation of selected values
     * @param minAbsValue the minimum absolute value
     * @param maxAbsValue the maximum absolute value
     * @param absValue
     * @param intType true, if integer values should be shown
     * @param userDefinedMinMaxValues
     * @param _mean the mean of all values
     * @param _variance the variance of all values
     */
    void
    updateValueWidget( double minValue,
                       double maxValue,
                       double selectedValue,
                       double minAbsValue,
                       double maxAbsValue,
                       double absValue,
                       bool   intType,
                       bool   userDefinedMinMaxValues,
                       double _mean,
                       double _variance );

    /** Updates the value widget but doesn't display the absolute value line (current modus is absolute value) */
    void
    updateValueWidget( double minValue,
                       double maxValue,
                       double selectedValue,
                       bool   intType,
                       bool   userDefinedMinMaxValues,
                       double _mean,
                       double _variance );

    /** Updates the value widget if no current selected value exists, or is undefined */
    void
    updateValueWidget( double minValue,
                       double maxValue,
                       bool   intType,
                       bool   userDefinedMinMaxValues,
                       double _mean,
                       double _variance );

    // --- end tab related functions ----

    /** allows the plugin to save and load settings */
    void
    addSettingsHandler( SettingsHandler* s );

    /** adds a colormap to the list of available colormaps from which the user can choose the active one */
    void
    addColorMap( ColorMap* map );

    // --- access global settings ----

    /** sets a global value
     * @param name identifier of the value
     * @param value the value which can be of any type
     * @param notifyMyself if true, the signal globalValueChanged is also sent to the caller
     * of this method. Causes infinite recursion, if called inside the slot connected to
     * globalValueChanged. */
    void
    setGlobalValue( const QString&  name,
                    const QVariant& value,
                    bool            notifyMyself = false );

    /**
     * Retreives the global value of the given identifier name.
     */
    QVariant
    getGlobalValue( const QString& name ) const;


    /** Calculates a color corresponding to the value parameter having minValue at color
     * position 0.0 and maxValue at color position 1.0 on the color scale.
     * If whiteForZero is set to true, the zero value is assigned the color white.
     */
    QColor
    getColor( double value,
              double minValue,
              double maxValue,
              bool   whiteForZero = true ) const;

    /**
     * formatNumber writes the given value into a string using the given format. If
     * integerType is true, the value is formatted without decimal places.
     * @param format FORMAT_TREES for treeWidgets and FORMAT_DEFAULT for all other widgets
     */
    QString
    formatNumber( double          value,
                  PrecisionFormat format,
                  bool            integerType = false ) const;

    /**
     * Convenience function, equivalent to formatNumber(double value, PrecisionFormat format, bool integerType = false)
     * with PrecisionFormat = FORMAT_DEFAULT
     */
    QString
    formatNumber( double value,
                  bool   integerType = false ) const;

    // --- end access global settings ----

    // --- update the trees after plugin has changed data  ----

    /** recalculates the values of the items of all trees */
    void
    recalculateTreeItems();

    /** Updates the items of all trees, if their values (cube::Values) have been changed by a plugin and
        notifies all tabs that values have been changed. */
    void
    updateTreeItems();

    /** Updates the properties (label, color, font...) of the given tree. This method has to be called if
        tree items have been marked */
    void
    updateTreeView( TreeType treeType );

    // --- end update the trees after plugin has been changed data  ----

    /** returns a stream to write debug messages; only active if -verbose command line option is set */
    QTextStream&
    debug();

    /** return statistical information about the given tree */
    void
    getStatisticValues( TreeType type,
                        double&  minValue,
                        double&  maxValue,
                        double&  selectedValue,
                        double&  minAbsValue,
                        double&  maxAbsValue,
                        double&  absValue,
                        double&  mean,
                        double&  varianceSqrt ) const;

    /** shows a dialog to let the user set the minimum and maximum value for coloring */
    void
    setUserDefinedMinMaxValues( TreeType type );

    /** returns true, if user defined values are set */
    bool
    getUserDefinedMinValues( TreeType type,
                             double&  min,
                             double&  max ) const;

private:
    friend class PluginManager; // to allow PluginManager to emit signals of PluginServices directly

    PluginServices( PluginManager* pm,
                    int            index );
    ~PluginServices();
    Tree*
    getTree( TreeType type ) const;
    void
    resizeTreeItemMarker( int iconHeight );

    /** Closes all tabs of the current plugin. */
    void
    closeTabs();

    TabWidget* currentTabWidget;            // TabWidget, in which the plugin has placed it tabs


    PluginManager*        pm;
    int                   index;            // index of current plugin in the PluginList and plugin menu
    QList<TabInterface* > currentTabs;      // all tabs of the current plugin/service
    SettingsHandler*      settingsHandler;  // required if the plugin wants to  save and load settings

    QList<TreeItem*>       markedItemList;  // items that are currenly marked by this plugin
    QList<TreeItemMarker*> markerList;      // all marker that the current plugin has requested
};

#endif // PLUGINSERVICES_H
