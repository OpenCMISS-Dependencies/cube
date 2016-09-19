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


#include <QVBoxLayout>
#include <QtPlugin>
#include "DemoPlugin.h"
#include "PluginServices.h"

/**
 * If the plugin doesn't load, start cube with -verbose to get detailed information.
 * If the error message "Plugin verification data mismatch" is printed, check if the same compiler
 * and Qt version have been used.
 */

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( DemoPlugin, DemoPlugin );
#endif

/******************************************************************************************************
* start of CubePlugin implementation
******************************************************************************************************/

DemoPlugin::DemoPlugin() {
  // The constructor should be empty, use cubeOpened to initialize. If Qt widgets or
  // signals/slots are used in constructor, they have to be deleted in destructor, 
  // otherwise cube may crash if the plugin is unloaded.
}

/** sets a version number, the plugin with the highest version number will be loaded */
void
DemoPlugin::version( int& major, int& minor, int& bugfix ) const
{
    major  = 1;
    minor  = 0;
    bugfix = 0;
}

/**
 * returns the unique plugin name
 */
QString
DemoPlugin::name() const
{
    return "Demo Plugin";
}

QString
DemoPlugin::getHelpText() const
{
    return "Just a simple demo plugin";
}

/**
 * starting point of the plugin
 */
bool
DemoPlugin::cubeOpened( PluginServices* service )
{
    this->service = service;

    widget_ = new QWidget();
    qlabel_ = new QLabel( "demo example" );
    QVBoxLayout* layout = new QVBoxLayout();
    widget_->setLayout( layout );
    layout->addWidget( qlabel_ );

    // add tab to the system tab widget with the label this->label() and content this->widget()
    service->addTab( SYSTEM, this );

    // add menu item and submenu to the Plugin Menu
    QMenu*   menu       = service->enablePluginMenu();
    QAction* menuAction = menu->addAction( "Demo Menu Item" );
    connect( menuAction, SIGNAL( triggered() ), this, SLOT( menuItemIsSelected() ) );

    // registered plugins may load and save settings
    service->addSettingsHandler( this );

    // react on signals emitted by PluginServices
    connect( service, SIGNAL( contextMenuIsShown( TreeType, TreeItem* ) ),
             this, SLOT( contextMenuIsShown( TreeType, TreeItem* ) ) );

    connect( service, SIGNAL( globalValueChanged( QString ) ),
             this, SLOT( globalValueChanged( QString ) ) );

    connect( service, SIGNAL( orderHasChanged( const QList<DisplayType> & ) ),
             this, SLOT( orderHasChanged( const QList<DisplayType> & ) ) );

    connect( service, SIGNAL( genericUserAction( UserAction ) ),
             this, SLOT( checkLoop( UserAction ) ) );

    defineTreeItemMarker();

    return true; // initialisation is ok => plugin should be shown
}

void
DemoPlugin::cubeClosed()
{
    delete widget_;
    markerList.clear();
}

/******************************************************************************************************
* start of TabInterface implementation
******************************************************************************************************/

/**
 * returns the tab label of the created system tab
 */
QString
DemoPlugin::label() const
{
    return "Demo Plugin Label";
}

/** returns the icon which will be placed left to the tab label */
QIcon
DemoPlugin::icon() const
{
    return QIcon( ":/icon.png" );
}

/**
 * returns widget that will be placed into the tab
 */
QWidget*
DemoPlugin::widget()
{
    return widget_;
}


/**
 * @brief DemoPlugin::setActive is called when the tab gets activated or deactivated by selecting another tab
 * The tab related demo elements should only react on signals, if the tab is active. For that reason the
 * signals are disconnected if another tab becomes active.
 */
void
DemoPlugin::setActive( bool active )
{
    if ( active )
    {
        connect( service, SIGNAL( treeItemIsSelected( TreeType, TreeItem* ) ),
                 this, SLOT( treeItemIsSelected( TreeType, TreeItem* ) ) );

        QString                                                    txt = service->getSelection( METRICTREE )->getName();
        service->debug() << "setActive: last selected metric: " << txt << endl;
    }
    else
    {
        service->disconnect( service, SIGNAL( treeItemIsSelected( TreeType, TreeItem* ) ) );
    }
}

/** This method is called, if the selected tree item has changed, the tree structure
 *  has changed or if an item is expanded or collapsed */
void
DemoPlugin::valuesChanged()
{
    // debug messages are written to console and to the extended status bar (click on status bar) if cube is started with -verbose
    service->debug() << "valuesChanged" << endl;
}


/** Returns the minimal width that is necessary to display all informations.
    This value is used in the menu item "Display->Optimize width".
 */
QSize
DemoPlugin::sizeHint() const
{
    return QSize( 100, 100 );
}

void
DemoPlugin::valueModusChanged( ValueModus modus )
{
    service->debug() << "valueModusChanged " << modus << endl;
}

// ******************************************************************************************************


/******************************************************************************************************
* implementation of slots to handle signals of PluginServices
******************************************************************************************************/

/**
 * @brief DemoPlugin::loadSettings loads settings from global setting file
 * the plugin should create a group with its name as group label
 */
void
DemoPlugin::loadGlobalSettings( QSettings& settings )
{
    int numCalls = settings.value( "demoPluginCalls", 0 ).toInt();

    service->debug() << "loadSettings: " << numCalls << endl;
}

/**
   Saves settings system specific file, for UNIX systems to $HOME/.config/FZJ/Cube.conf
 */
void
DemoPlugin::saveGlobalSettings( QSettings& settings )
{
    int numCalls = settings.value( "DemoPluginCalls", 0 ).toInt(); // 0 is default, if no value exists
    settings.setValue( "DemoPluginCalls", ++numCalls );
}

QString
DemoPlugin::settingName()
{
    return "demoPlugin";
}

/**
 * @brief DemoPlugin::contextMenuIsShown is called, if the user clicks with right mouse button on a tree
 * item.
 */
void
DemoPlugin::contextMenuIsShown( TreeType type, TreeItem* item )
{
    contextItem = item;
    contextTreeType = type;

    QAction*    contextAction = service->addContextMenuItem( type, "demo context menu item (no action)" );
    QString     text;
    QTextStream st( &text );
    st << "context for tree type " << type <<  " label  " << item->getName();
    connect( contextAction, SIGNAL( triggered() ), new DemoSlot( text ), SLOT( print() ) );

    for ( int idx = 0; idx < markerList.size() / 2; idx++ )
    {
        contextAction = service->addContextMenuItem( type, "add dynamic marker " + QString::number( idx ) );
        contextAction->setProperty( "index", idx );
        connect( contextAction, SIGNAL( triggered() ), this, SLOT( setDynamicMarker() ) );
    }
    for ( int idx = markerList.size() / 2; idx < markerList.size(); idx++ )
    {
        contextAction = service->addContextMenuItem( type, "add static marker " + QString::number( idx ) );
        contextAction->setProperty( "index", idx );
        connect( contextAction, SIGNAL( triggered() ), this, SLOT( setStaticMarker() ) );
    }
    service->setMessage( "context menu is shown" ); // write the message to status bar
}

/** - marks a single item which doesn't depend on selections in other trees or 
    - marks a single item if runtime values fit. These dependencies are managed by the plugin. The plugin has to set and delete
      its dynamic markers manually e.g. if the tree order or the values change.
*/
void
DemoPlugin::setDynamicMarker()
{
    int index = sender()->property( "index" ).toInt();
    service->addMarker( contextItem, markerList.at( index ) );
    service->updateTreeView( contextTreeType );
}

/** This example for static marker usage sets a static marker for the recently selected item tuple (metric, call 
 *  and system tree selection).
 *  A static marker depends on the selections of the other trees. The marker is only visible, if the dependencies 
 *  left to the item are selected. The dependencies left to the marked items are marked with black dashed lines.
 *  If an item with NULL value is given, the rightmost item left to this item is marked. E.g. the statistic plugin
 *  doesn't have information about the system tree, so the last argument is NULL.
 *      
 *  Further examples: StatisticPlugin, LaunchPlugin
*/
void
DemoPlugin::setStaticMarker()
{
    int index = sender()->property( "index" ).toInt();

    // set example marker which consists of the 3 items of the current selection
    // dependencies are automatically marked
    service->addMarker( markerList.at( index ),
                              service->getSelection( METRICTREE ),
                              service->getSelection( CALLTREE ),
                              service->getSelection( SYSTEMTREE ) );
}

/** defines a number of tree items markers, which are used to mark tree items with different background colour and
    icons */
void
DemoPlugin::defineTreeItemMarker()
{
    QStringList images;
    images << ":images/left_small.png" << ":images/right_small.png" << ":images/up_small.png" << ":images/down_small.png";
    for ( int i = 0; i < images.size(); i++ )
    {
        QList<QPixmap> icons;
        icons.append( QPixmap( images.at( i ) ) );
        markerList.append( service->getTreeItemMarker( "marker" + QString::number( i ), icons ) );
    }
}

void
DemoPlugin::treeItemIsSelected( TreeType, TreeItem* item )
{
    // use formatNumber() to format value according to user settings in Display->Precision
    QString txt = item->getName() + " " + service->formatNumber( item->getValue() );

    qlabel_->setText( txt );
    service->debug() << "treeItemIsSelected " << txt << item->getCubeObject() << endl;

    /* handle special case: item is a loop in the call tree, which may be aggregated */
    if ( item->isAggregatedLoopItem() )
    {
        QList<cube::Cnode*> iterations = ( static_cast<AggregatedTreeItem*> ( item ) )->getIterations();
        int                 min        = std::min( 10, iterations.size() );
        if ( min > 0 )
        {
            service->debug() << "selected loop item: cnode_ids of first 10 iterations:" << endl;
        }
        for ( int i = 0; i < min; i++ )
        {
            cube::Cnode*        cnode = iterations.at( i );
            service->debug() << cnode->get_id() << endl;
        }
    }
    if ( item->getCubeObject() == 0 )
    {
        service->debug() << "selected aggregated item" << item->getName() << endl;
    }
    else
    {
        cube::Cnode*                                    cnode = static_cast<cube::Cnode*> ( item->getCubeObject() );
        service->debug() << "selected single item: " << cnode->get_id() << endl;
    }
}

/**
 * called, if the user has selected the menu item "Display->Dimension Order"
 */
void
DemoPlugin::orderHasChanged( const QList<DisplayType>& order )
{
    service->debug() << "orderHasChanged user has changed the dimension order" << endl;

    // example: disable demo plugin, if system widget is first
    bool enabled = order.at( 0 ) != SYSTEM;
    service->enableTab( this, enabled );
}

/**
 * @brief DemoPlugin::globalValueChanged is called if the value of a global variable has changed. In this
 * example, the value of "testVal" is incremented if the plugin menu item is selected.
 */
void
DemoPlugin::globalValueChanged( const QString& name )
{
    service->debug() << "globalValueChanged " << name << service->getGlobalValue( "testVal" ).toString() << endl;
}

/**
 * slot, which is called if "Demo Menu Item" is selected
 */
void
DemoPlugin::menuItemIsSelected()
{
    service->debug() << "menu item is selected and global value incremented" << endl;

    // increment global value "testVal", which can also used by other plugins
    int val = service->getGlobalValue( "testVal" ).toInt();
    service->setGlobalValue( "testVal", QVariant( ++val ), true ); // true = notify myself
}

void
DemoPlugin::checkLoop( UserAction type )
{
    if ( type == LoopMarkAction )
    {
        service->debug() << "user has marked tree item as loop" << endl;
    }
}
