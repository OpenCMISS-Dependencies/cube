/****************************************************************************
**  CUBE        http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 1998-2016                                                **
**  Forschungszentrum Juelich GmbH, Juelich Supercomputing Centre          **
**                                                                         **
**  Copyright (c) 2009-2015                                                **
**  German Research School for Simulation Sciences GmbH,                   **
**  Laboratory for Parallel Programming                                    **
**                                                                         **
**  This software may be modified and distributed under the terms of       **
**  a BSD-style license.  See the COPYING file in the package base         **
**  directory for details.                                                 **
****************************************************************************/


#include "Heatmap.h"
#include "PluginServices.h"
#include <QVBoxLayout>
#include <QtPlugin>


#include <QString>
#include <iostream>
#include <string.h>
using namespace std;

/**  If the plugin doesn't load, the most likely reason is an undefined reference.
 * - go into the build-backend/ directory of the cube sources
 * - export CHECK_PLUGIN=$PREFIX/plugins/libDemoPlugin.so
 * - call "make check-plugin"
 */

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( HeatmapPlugin, Heatmap );
#endif

Heatmap::Heatmap() : tabNotPresent( true )
{
}

/******************************************************************************************************
* start of CubePlugin implementation
******************************************************************************************************/

/** set a version number, the plugin with the highest version number will be loaded */
void
Heatmap::version( int& major, int& minor, int& bugfix ) const
{
    major  = 1;
    minor  = 0;
    bugfix = 0;
}

QString
Heatmap::getHelpText() const
{
    return "";
}

/**
 * returns the unique plugin name
 */
QString
Heatmap::name() const
{
    return "Heatmap Plugin";
}

/**
 * starting point of the plugin
 */
bool
Heatmap::cubeOpened( PluginServices* service )
{
    this->service = service;

    tabNotPresent       = true;
    widget_             = new QWidget();
    HeatmapWidgetLayout = new QGridLayout( widget_ );
    widget_->setLayout( HeatmapWidgetLayout );
    widget_->layout()->setSpacing( 0 );

    // add tab to the system tab widget with the label this->label() and content this->widget()
    // service->addTab( SYSTEM, this );

    // add menu item and submenu to the Plugin Menu
    QMenu*   menu       = service->enablePluginMenu();
    QAction* menuAction = menu->addAction( "Heatmap Settings" );
    // connect( menuAction, SIGNAL( triggered() ), this, SLOT( menuItemIsSelected() ) );
    // service->addPluginMenu("Heatmap Submenu (without elements)");


    // registered plugins may load and save settings
    service->addSettingsHandler( this );

    // react on signals emitted by PluginServices
    connect( service, SIGNAL( globalValueChanged( QString ) ),
             this, SLOT( globalValueChanged( QString ) ) );

    connect( service, SIGNAL( orderHasChanged( const QList<DisplayType> & ) ),
             this, SLOT( orderHasChanged( const QList<DisplayType> & ) ) );

    connect( service, SIGNAL( genericUserAction( UserAction ) ),
             this, SLOT( markItemAsLoop( UserAction ) ) );


    // ________________________________________________________________
    cubeReader = new CubeRead( service->getCube() );
    if ( cubeReader->getCubeHasItr() != false )
    {
        addTabToGUI();
    }
    dataProvider_H = new DataProvider( cubeReader );

    // /create controllers
    heatMapPlotAreaController        = new HeatMapPlotAreaController( widget() );
    heatMapHorizontalRulerController = new HorizontalRulerController( widget() );
    heatMapVerticalRulerController   = new VerticalRulerController( widget() );
    // / Setting heatmap ruler limits
    heatMapVerticalRulerController->setLimits( 0, dataProvider_H->getProcessesCount() );
    heatMapHorizontalRulerController->setLimits( 0, dataProvider_H->getIterationsCount() );
    // / Setting heatmap ruler widgets default notches count
    heatMapHorizontalRulerController->setMinorNotchesCount( plotWidget::DEFAULT_HEAT_WIDGET_H_MINOR_NOTCHES_COUNT );
    heatMapHorizontalRulerController->setMajorNotchesCount( plotWidget::DEFAULT_HEAT_WIDGET_H_MAJOR_NOTCHES_COUNT );
    heatMapVerticalRulerController->setMinorNotchesCount( plotWidget::DEFAULT_HEAT_WIDGET_V_MINOR_NOTCHES_COUNT );
    heatMapVerticalRulerController->setMajorNotchesCount( plotWidget::DEFAULT_HEAT_WIDGET_V_MAJOR_NOTCHES_COUNT );
    // / create heatmap setting menu
    heatMapCustomizationDialog = new HeatmapCustomizationzation( true, plotWidget::DEFAULT_HEAT_WIDGET_H_MAJOR_NOTCHES_COUNT, plotWidget::DEFAULT_HEAT_WIDGET_H_MINOR_NOTCHES_COUNT,
                                                                 true, plotWidget::DEFAULT_HEAT_WIDGET_V_MAJOR_NOTCHES_COUNT, plotWidget::DEFAULT_HEAT_WIDGET_V_MINOR_NOTCHES_COUNT,
                                                                 widget() );

// /save image
    heatMapSaveController = new ImageSaverController( heatMapPlotAreaController, heatMapHorizontalRulerController, heatMapVerticalRulerController );


    contextMenu          = new QMenu( heatMapPlotAreaController->getView() );
    saveImageContextMenu = contextMenu->addAction( "Save Image ..." );

    QObject::connect( menuAction, SIGNAL( triggered() ), heatMapCustomizationDialog, SLOT( show() ) );

    // /Adding controllers view to the main view
    setWidgetNorth( heatMapHorizontalRulerController->getView() );
    setWidgetWest( heatMapVerticalRulerController->getView() );
    setWidgetCenter( heatMapPlotAreaController->getView() );

    // / Setting mouse lines alignment for heat map plot controller
    heatMapPlotAreaController->updateMouseAxisHorizontalAlignment( plotWidget::Left );
    heatMapPlotAreaController->updateMouseAxisVerticalAlignment( plotWidget::Top );

    // / Connect heat map area with updates to the mesh lines positions coming from noth vertical and horizontal rulers

    QObject::connect( heatMapVerticalRulerController, SIGNAL( majorNotchesLocationsChanged( QList<int>) ), heatMapPlotAreaController, SLOT( updateHorizontalMajorMeshLines( QList<int>) ) );
    QObject::connect( heatMapVerticalRulerController, SIGNAL( minorNotchesLocationsChanged( QList<int>) ), heatMapPlotAreaController, SLOT( updateHorizontalMinorMeshLines( QList<int>) ) );
    QObject::connect( heatMapHorizontalRulerController, SIGNAL( majorNotchesLocationsChanged( QList<int>) ), heatMapPlotAreaController, SLOT( updateVerticalMajorMeshLines( QList<int>) ) );
    QObject::connect( heatMapHorizontalRulerController, SIGNAL( minorNotchesLocationsChanged( QList<int>) ), heatMapPlotAreaController, SLOT( updateVerticalMinorMeshLines( QList<int>) ) );

    QObject::connect( dataProvider_H, SIGNAL( changeCalibrateStatus( bool ) ), heatMapVerticalRulerController, SLOT( setCalibrateStatus( bool ) ) );
    QObject::connect( dataProvider_H, SIGNAL( changeCalibrateStatus( bool ) ), heatMapHorizontalRulerController, SLOT( setCalibrateStatus( bool ) ) );


    QObject::connect( dataProvider_H, SIGNAL( heatMapUpdated( QVector<QVector<double> >, double, double, PluginServices* ) ), heatMapPlotAreaController, SLOT( setData( QVector<QVector<double> >, double, double, PluginServices* ) ) );
    QObject::connect( dataProvider_H, SIGNAL( heatMapDisable() ), heatMapPlotAreaController, SLOT( reset() ) );
    QObject::connect( dataProvider_H, SIGNAL( vLimitsHaveChangedHeatmap( int, int ) ), heatMapVerticalRulerController, SLOT( setLimits( int, int ) ) );
    QObject::connect( dataProvider_H, SIGNAL( hLimitsHaveChangedHeatmap( int, int ) ), heatMapHorizontalRulerController, SLOT( setLimits( int, int ) ) );

    QObject::connect( heatMapPlotAreaController, SIGNAL( mouseIsClicked( QMouseEvent* ) ), this, SLOT( heatmapAreaMouseCoordEventHandler( QMouseEvent* ) ) );
    QObject::connect( heatMapPlotAreaController->getView(), SIGNAL( customContextMenuRequested( const QPoint & ) ), this, SLOT( ShowContextMenu( const QPoint & ) ) );
    QObject::connect( saveImageContextMenu, SIGNAL( triggered() ), heatMapSaveController, SLOT( saveImage() ) );





    // / Setting the heat map customization controller events with the corresponding views
    QObject::connect( heatMapCustomizationDialog, SIGNAL( setHMajorStepsCount( int ) ), heatMapHorizontalRulerController, SLOT( setMajorNotchesCount( int ) ) );
    QObject::connect( heatMapCustomizationDialog, SIGNAL( setHMajorFixedInterval( int ) ), heatMapHorizontalRulerController, SLOT( setMajorNotchesInterval( int ) ) );
    QObject::connect( heatMapCustomizationDialog, SIGNAL( setHMinorStepsCount( int ) ), heatMapHorizontalRulerController, SLOT( setMinorNotchesCount( int ) ) );
    QObject::connect( heatMapCustomizationDialog, SIGNAL( setVMajorStepsCount( int ) ), heatMapVerticalRulerController, SLOT( setMajorNotchesCount( int ) ) );
    QObject::connect( heatMapCustomizationDialog, SIGNAL( setVMajorFixedInterval( int ) ), heatMapVerticalRulerController, SLOT( setMajorNotchesInterval( int ) ) );
    QObject::connect( heatMapCustomizationDialog, SIGNAL( setVMinorStepsCount( int ) ), heatMapVerticalRulerController, SLOT( setMinorNotchesCount( int ) ) );
    //  QObject::connect( heatMapCustomizationDialog, SIGNAL( setMinColor( QColor ) ), heatMapPlotAreaController, SLOT( setColorMin( QColor ) ) );
    //   QObject::connect( heatMapCustomizationDialog, SIGNAL( setMaxColor( QColor ) ), heatMapPlotAreaController, SLOT( setColorMax( QColor ) ) );


    return true; // initialisation is ok => plugin should be shown
}

void
Heatmap::cubeClosed()
{
    delete widget_;
}

/******************************************************************************************************
* start of TabInterface implementation
******************************************************************************************************/

/**
 * returns the tab label of the created system tab
 */

QString
Heatmap::label() const
{
    return "Heatmap";
}

/** returns the icon which will be placed left to the tab label */
QIcon
Heatmap::icon() const
{
    return QIcon( ":/icon.png" );
}

/**
 * returns widget that will be placed into the tab
 */
QWidget*
Heatmap::widget()
{
    return widget_;
}

/** This method is called, if the selected tree item has changed, the tree structure
 *  has changed or if an item is expanded or collapsed */
void
Heatmap::valuesChanged()
{
}

/**
 * @brief HeatmapPlugin::setActive is called when the tab gets activated or deactivated by selecting another tab
 * The tab related Heatmap elements should only react on signals, if the tab is active. For that reason the
 * signals are disconnected if another tab becomes active.
 */
void
Heatmap::setActive( bool active )
{
    if ( active )
    {
        connect( service, SIGNAL( treeItemIsSelected( TreeType, TreeItem* ) ),
                 this, SLOT( treeItemIsSelected( TreeType, TreeItem* ) ) );

        QString txt = service->getSelection( METRICTREE )->getName();

        dataProvider_H->getMetricNames(); // to fill metricnames array of dataprovider

        TreeItem* calltreeItem = service->getSelection( CALLTREE );
        if ( !calltreeItem->isAggregatedLoopItem() && !calltreeItem->isAggregatedRootItem() )
        {
            dataProvider_H->DisableHeatMap();
            return;
        }

        AggregatedTreeItem*        aggregatedItem = static_cast<AggregatedTreeItem*> ( calltreeItem );
        const QList<cube::Cnode*>& clist          = aggregatedItem->getIterations();

        dataProvider_H->setMetricName( txt );
        if ( service->getSelection( CALLTREE )->isExpanded() == true )
        {
            dataProvider_H->generateHeatMapIteration( clist, cube::CUBE_CALCULATE_EXCLUSIVE, service );
        }
        else
        {
            dataProvider_H->generateHeatMapIteration( clist, cube::CUBE_CALCULATE_INCLUSIVE, service );
        }
    }
    else
    {
        service->disconnect( service, SIGNAL( treeItemIsSelected( TreeType, TreeItem* ) ) );
    }
}

/** Returns the minimal width that is necessary to display all informations.
    This value is used in the menu item "Display->Optimize width".
 */
QSize
Heatmap::sizeHint() const
{
    return QSize( 100, 100 );
}

void
Heatmap::valueModusChanged( ValueModus modus )
{
    ( void )modus;
}

// ******************************************************************************************************


/******************************************************************************************************
* implementation of slots to handle signals of PluginServices
******************************************************************************************************/

/**
 * @brief HeatmapPlugin::loadSettings loads settings from global setting file
 * the plugin should create a group with its name as group label
 */
void
Heatmap::loadGlobalSettings( QSettings& settings )
{
    ( void )settings; // to suppres warning about not being used....
}

/**
   Saves settings system specific file, for UNIX systems to $HOME/.config/FZJ/Cube.conf
 */
void
Heatmap::saveGlobalSettings( QSettings& settings )
{
    int numCalls = settings.value( "HeatmapPluginCalls", 0 ).toInt(); // 0 is default, if no value exists
    settings.setValue( "HeatmapPluginCalls", ++numCalls );
}
QString
Heatmap::settingName()
{
    return "heatmapPlugin";
}

void
Heatmap::treeItemIsSelected( TreeType type, TreeItem* item )
{
    // use formatNumber() to format value according to user settings in Display->Precision
    QString txt = item->getName() + " " + service->formatNumber( item->getValue() );

    TreeItem* calltreeItem = ( type == CALLTREE ? item : service->getSelection( CALLTREE ) );

    if ( !calltreeItem->isAggregatedLoopItem() && !calltreeItem->isAggregatedRootItem() )
    {
        dataProvider_H->DisableHeatMap();
        return;
    }

    AggregatedTreeItem*        aggregatedItem = static_cast<AggregatedTreeItem*> ( calltreeItem );
    const QList<cube::Cnode*>& clist          = aggregatedItem->getIterations();

    dataProvider_H->setMetricName( type == METRICTREE ? item->getName() : service->getSelection( METRICTREE )->getName() );
    if ( item->isExpanded() == true )
    {
        dataProvider_H->generateHeatMapIteration( clist, cube::CUBE_CALCULATE_EXCLUSIVE, service );
    }
    else
    {
        dataProvider_H->generateHeatMapIteration( clist, cube::CUBE_CALCULATE_INCLUSIVE, service );
    }
}

/**
 * called, if the user has selected the menu item "Display->Dimension Order"
 */
void
Heatmap::orderHasChanged( const QList<DisplayType>& order )
{
    // example: disable Heatmap plugin, if system widget is first
    bool enabled = order.at( 0 ) != SYSTEM;
    service->enableTab( this, enabled );
}

/**
 * @brief HeatmapPlugin::globalValueChanged is called if the value of a global variable has changed. In this
 * example, the value of "testVal" is incremented if the plugin menu item is selected.
 */
void
Heatmap::globalValueChanged( const QString& name )
{
    ( void )name;
}

// ___________ define by Reyhaneh
void
Heatmap::setWidgetNorth( QWidget* WidgetNorth )
{
    WidgetNorth->setFixedHeight( plotWidget::DEFAULT_RULER_THICKNESS_NORTH );
    HeatmapWidgetLayout->addWidget( WidgetNorth, 0, 1 );
}
void
Heatmap::setWidgetWest( QWidget* WidgetWest )
{
    WidgetWest->setFixedWidth( plotWidget::DEFAULT_RULER_THICKNESS_WEST );
    HeatmapWidgetLayout->addWidget( WidgetWest, 1, 0 );
}
void
Heatmap::setWidgetCenter( QWidget* WidgetCenter )
{
    WidgetCenter->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    HeatmapWidgetLayout->addWidget( WidgetCenter, 1, 1 );
}
void
Heatmap::heatmapAreaMouseCoordEventHandler( QMouseEvent* mouseEvent )
{
    // / 1. get the x-coordinate from horizontal ruler (integer)
    int processId = heatMapVerticalRulerController->pixelCoordToRulerCoord( mouseEvent->y(), true );
    // / 2. get the y-coordinate from vertical ruler (integer)
    int iterationId = heatMapHorizontalRulerController->pixelCoordToRulerCoord( mouseEvent->x() );

    if ( processId == -1 || iterationId == -1
         || processId == dataProvider_H->getThreadsCount() || iterationId == dataProvider_H->getIterationsCount() )
    {
        return;
    }

    QString status = "Thread: " + QString::number( processId ) + " Iteration: " + QString::number( iterationId );
    // / 3. get the value at x and y
    status += " Value: " + QString::number( heatMapPlotAreaController->getData( processId, iterationId ), 'f', 8 );
    // / 4. send a message to the permanent status-bar of our view
    QToolTip::showText( mouseEvent->globalPos(), status, widget_ );
}
void
Heatmap::ShowContextMenu( const QPoint& pos )
{
    if ( heatMapPlotAreaController->getView()->getDataToDisplay() == true )
    {
        contextMenu->popup( heatMapPlotAreaController->getView()->mapToGlobal( pos ) );
    }
}
void
Heatmap::markItemAsLoop( UserAction type )
{
    if ( type == LoopMarkAction )
    {
        addTabToGUI();
    }
}

void
Heatmap::addTabToGUI()
{
    if ( tabNotPresent )
    {
        service->addTab( SYSTEM, this, 2 );
        tabNotPresent = false;
    }
}
