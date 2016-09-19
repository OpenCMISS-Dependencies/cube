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


#include "SystemTopology.h"
#include "SystemTopologyWidget.h"
#include "CubeMetric.h"
#include "CubeCartesian.h"
#include "SystemTopologyToolBar.h"
#include <assert.h>

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( SystemTopologyPlugin, SystemTopology );
#endif

/** sets a version number, the plugin with the highest version number will be loaded */
void
SystemTopology::version( int& major, int& minor, int& bugfix ) const
{
    major  = 1;
    minor  = 0;
    bugfix = 0;
}

/**
 * returns the unique plugin name
 */
QString
SystemTopology::name() const
{
    return "System Topology Plugin";
}

QString
SystemTopology::getHelpText() const
{
    return "";
}

void
SystemTopology::cubeClosed()
{
    delete topologyToolBar;
    foreach( QWidget * widget, widgetList )
    {
        delete widget;
    }
    widgetList.clear();
}

bool
SystemTopology::cubeOpened( PluginServices* service )
{
    this->topologyToolBar = 0;
    cube::Cube* cube          = service->getCube();
    unsigned    numTopologies = cube->get_cartv().size();
    if ( numTopologies == 0 )
    {
        return false;
    }

    this->service         = service;
    this->topologyToolBar = new SystemTopologyToolBar( service );

    whiteForZero             = true;
    lineType                 = BLACK_LINES;
    antialiasing             = false;
    showUnusedTopologyPlanes = true;
    tabInfoVisible           = 0;
    tabInfoCount             = 0;

    service->addToolBar( topologyToolBar );
    QMenu* menu = service->enablePluginMenu();
    addTopologyMenu( menu );

    for ( unsigned i = 0; i < numTopologies; i++ )
    {
        SystemTopologyWidget* systemTopologyWidget = new SystemTopologyWidget( this, topologyToolBar, i );
        widgetList.append( systemTopologyWidget );
    }

    service->addSettingsHandler( this );

    return true;
}

PluginServices*
SystemTopology::getService()
{
    return service;
}

// since the topology tool bar can be hidden from the tool bar itself, without using the menu for that,
// and I did not find any signal signalling this event,
// if the menu is to be shown, I update the status of the topology tool bar menu
//
void
SystemTopology::updateToolBarMenu()
{
    if ( topologyToolBar->isHidden() )
    {
        hideToolbarAct->setChecked( true );
    }
    else if ( topologyToolBar->toolButtonStyle() == Qt::ToolButtonTextOnly )
    {
        textAct->setChecked( true );
    }
    else
    {
        iconAct->setChecked( true );
    }
}


void
SystemTopology::addTopologyMenu( QMenu* topologyMenu )
{
    topologyMenu->setStatusTip( tr( "Ready" ) );

    QMenu* topologyColoringMenu = topologyMenu->addMenu( tr( "Item coloring" ) );
    topologyColoringMenu->setStatusTip( tr( "Ready" ) );
    topologyColoringMenu->setWhatsThis( "This menu item offers a choice how zero-valued system nodes should be colored in the topology display. The two offered options are either to use white or to use white only if all system leaf values are zero and use the minimal color otherwise." );

    white1Act = new QAction( tr( "color for zero: white" ), this );
    white1Act->setStatusTip( tr( "Use white as color for zero values in the topology display" ) );
    connect( white1Act, SIGNAL( triggered() ), this, SLOT( whiteOn() ) );
    white1Act->setCheckable( true );
    white1Act->setChecked( true );
    topologyColoringMenu->addAction( white1Act );
    white1Act->setWhatsThis( "Defines that always the white color should be used to color zero-valued items in topologies." );

    white2Act = new QAction( tr( "color for zero: white only if all zero" ), this );
    white2Act->setStatusTip( tr( "Use white as color for zero values in the topology display only if all values are zero" ) );
    connect( white2Act, SIGNAL( triggered() ), this, SLOT( whiteOff() ) );
    white2Act->setCheckable( true );
    white2Act->setChecked( false );
    topologyColoringMenu->addAction( white2Act );
    white2Act->setWhatsThis( "Defines that the white color should be used to color zero-valued items in topologies only if all system leaf values are zero, and the minimal color should be used otherwise." );

    QActionGroup* whiteActionGroup = new QActionGroup( this );
    whiteActionGroup->setExclusive( true );
    whiteActionGroup->addAction( white1Act );
    whiteActionGroup->addAction( white2Act );

    QMenu* topologyLineStyleMenu = topologyMenu->addMenu( tr( "Line coloring" ) );
    topologyLineStyleMenu->setStatusTip( tr( "Ready" ) );
    topologyLineStyleMenu->setWhatsThis( "Allows to define the color of the lines in topology painting. Available colors are black, gray, white, or no lines." );

    blackLinesAct = new QAction( tr( "Black" ), this );
    blackLinesAct->setStatusTip( tr( "Lines in topology should be painted black." ) );
    connect( blackLinesAct, SIGNAL( triggered() ), this, SLOT( blackLines() ) );
    blackLinesAct->setCheckable( true );
    blackLinesAct->setChecked( true );
    blackLinesAct->setWhatsThis( "Sets the color of the lines in topology painting to black." );
    topologyLineStyleMenu->addAction( blackLinesAct );

    grayLinesAct = new QAction( tr( "Gray" ), this );
    grayLinesAct->setStatusTip( tr( "Lines in topology should be painted gray." ) );
    connect( grayLinesAct, SIGNAL( triggered() ), this, SLOT( grayLines() ) );
    grayLinesAct->setCheckable( true );
    grayLinesAct->setChecked( false );
    grayLinesAct->setWhatsThis( "Sets the color of the lines in topology painting to gray." );
    topologyLineStyleMenu->addAction( grayLinesAct );

    whiteLinesAct = new QAction( tr( "White" ), this );
    whiteLinesAct->setStatusTip( tr( "Lines in topology should be painted white." ) );
    connect( whiteLinesAct, SIGNAL( triggered() ), this, SLOT( whiteLines() ) );
    whiteLinesAct->setCheckable( true );
    whiteLinesAct->setChecked( false );
    whiteLinesAct->setWhatsThis( "Sets the color of the lines in topology painting to white." );
    topologyLineStyleMenu->addAction( whiteLinesAct );

    noLinesAct = new QAction( tr( "No lines" ), this );
    noLinesAct->setStatusTip( tr( "Lines in topology should not be painted." ) );
    connect( noLinesAct, SIGNAL( triggered() ), this, SLOT( noLines() ) );
    noLinesAct->setCheckable( true );
    noLinesAct->setChecked( false );
    noLinesAct->setWhatsThis( "Defines that there should be no framing lines for the items in topologies." );
    topologyLineStyleMenu->addAction( noLinesAct );

    QActionGroup* linesActionGroup = new QActionGroup( this );
    // only one element in this group can be checked
    linesActionGroup->setExclusive( true );
    linesActionGroup->addAction( blackLinesAct );
    linesActionGroup->addAction( grayLinesAct );
    linesActionGroup->addAction( whiteLinesAct );
    linesActionGroup->addAction( noLinesAct );

    QMenu* toolbarMenu = topologyMenu->addMenu( tr( "Toolbar" ) );
    toolbarMenu->setStatusTip( tr( "Ready" ) );
    connect( toolbarMenu, SIGNAL( aboutToShow() ), this, SLOT( updateToolBarMenu() ) );
    toolbarMenu->setWhatsThis( "This menu item allows to specify if the tool bar's buttons should be labeled by icons, by a text description, or if the tool bar should be hidden. See also help on the tool bar itself." );

    textAct = new QAction( tr( "Text" ), this );
    textAct->setStatusTip( tr( "Toolbar icons should be labeled with text" ) );
    connect( textAct, SIGNAL( triggered() ), topologyToolBar, SLOT( setButtonsToText() ) );
    // connect( textAct, SIGNAL( triggered() ), topologyToolBar, SLOT( show() ) );
    textAct->setCheckable( true );
    textAct->setChecked( false );
    textAct->setWhatsThis( "Here you can specify that the tool bar's buttons should be labeled by text descriptions. See also help on the tool bar itself." );
    toolbarMenu->addAction( textAct );

    iconAct = new QAction( tr( "Icon" ), this );
    iconAct->setStatusTip( tr( "Toolbar icons should be labeled with icons" ) );
    connect( iconAct, SIGNAL( triggered() ), topologyToolBar, SLOT( setButtonsToIcon() ) );
    // connect( iconAct, SIGNAL( triggered() ), topologyToolBar, SLOT( show() ) );
    iconAct->setCheckable( true );
    iconAct->setChecked( true );
    iconAct->setWhatsThis( "Here you can specify that the tool bar's buttons should be labeled by icons. See also help on the tool bar itself." );
    toolbarMenu->addAction( iconAct );

    hideToolbarAct = new QAction( tr( "Hide" ), this );
    hideToolbarAct->setStatusTip( tr( "Hide toolbar" ) );
    connect( hideToolbarAct, SIGNAL( triggered() ), topologyToolBar, SLOT( hide() ) );
    hideToolbarAct->setCheckable( true );
    hideToolbarAct->setChecked( false );
    hideToolbarAct->setWhatsThis( "Here you can hide the tool bar. See also help on the tool bar itself." );
    toolbarMenu->addAction( hideToolbarAct );

    QActionGroup* toolbarActionGroup = new QActionGroup( this );
    toolbarActionGroup->setExclusive( true );
    toolbarActionGroup->addAction( textAct );
    toolbarActionGroup->addAction( iconAct );
    toolbarActionGroup->addAction( hideToolbarAct );

    topologyMenu->addSeparator();

    emptyAct = topologyMenu->addAction( tr( "Show also unused hardware in topology" ) );
    emptyAct->setCheckable( true );
    emptyAct->setChecked( true );
    emptyAct->setStatusTip( tr( "Show also planes of unused hardware in the topology display" ) );
    connect( emptyAct, SIGNAL( triggered() ), this, SLOT( toggleUnused() ) );
    emptyAct->setWhatsThis( "If this menu item is not checked, unused topology planes, i.e., planes whose grid elements don't have any processes/threads assigned to, are hidden. Unused plane elements, if not hidden, are colored gray. See also help on the topologies themselves." );

    antialiasingAct = topologyMenu->addAction( tr( "Topology antialiasing" ) );
    antialiasingAct->setCheckable( true );
    antialiasingAct->setChecked( false );
    antialiasingAct->setStatusTip( tr( "Activate/deactivate antialiasing for rendering in topology" ) );
    antialiasingAct->setWhatsThis( "If this menu item is checked, antialiasing is used when painting lines in the topologies. See also help on the topologies themselves." );
    connect( antialiasingAct, SIGNAL( triggered() ), this, SLOT( toggleAntialiasing() ) );

    focusPlaneAct = topologyMenu->addAction( tr( "Zoom into current plane" ) );
    focusPlaneAct->setCheckable( true );
    focusPlaneAct->setChecked( true );
    focusPlaneAct->setStatusTip( tr( "Activate/deactivate focus on current plane" ) );
    focusPlaneAct->setWhatsThis( "If this menu item is checked, the plane distance is increased to show "
                                 "the current plane completely. The neighbor planes also get additional space." );
    connect( focusPlaneAct, SIGNAL( triggered( bool ) ), this, SLOT( enableFocus( bool ) ) );
}

void
SystemTopology::enableFocus( bool f )
{
    foreach( SystemTopologyWidget * widget, widgetList )
    {
        widget->getTransform()->setFocusEnabled( f );
        widget->getTransform()->redraw();
    }
}


QString
SystemTopology::settingName()
{
    return "systemTopology";
}

// save tab settings
//
void
SystemTopology::saveGlobalSettings( QSettings& settings )
{
    // save topology toolbar style
    settings.setValue( "toolbarstyle", topologyToolBar->toolButtonStyle() );
    settings.setValue( "toolbarHidden", topologyToolBar->isHidden() );

    settings.setValue( "lineType", lineType );
    settings.setValue( "whiteForZero", whiteForZero );

    settings.setValue( "emptyActChecked", emptyAct->isChecked() );
    settings.setValue( "antialiasingActChecked", antialiasingAct->isChecked() );
}

void
SystemTopology::loadGlobalSettings( QSettings& settings )
{
    LineType lineType = ( LineType )( settings.value( "lineType", 0 ).toInt() );
    setLineType( lineType );

    whiteForZero = settings.value( "whiteForZero", true ).toBool();

    // how zero-valued items in the topology widget should be colored
    // (menu Display/Coloring/Topology coloring)
    if ( getWhiteForZero() && !white1Act->isChecked() )
    {
        white1Act->setChecked( true );
    }
    else if ( !getWhiteForZero() && !white2Act->isChecked() )
    {
        white2Act->setChecked( true );
    }
    // colors for lines in the topology display
    // (menu Display/Coloring/Topology line coloring)
    if ( lineType == BLACK_LINES && !blackLinesAct->isChecked() )
    {
        blackLinesAct->setChecked( true );
    }
    else if ( lineType == GRAY_LINES && !grayLinesAct->isChecked() )
    {
        grayLinesAct->setChecked( true );
    }
    else if ( lineType == WHITE_LINES && !whiteLinesAct->isChecked() )
    {
        whiteLinesAct->setChecked( true );
    }
    else if ( lineType == NO_LINES && !noLinesAct->isChecked() )
    {
        noLinesAct->setChecked( true );
    }

    // restore the status of the actions under Display/Topology
    if ( settings.value( "emptyActChecked", true ).toBool() != emptyAct->isChecked() )
    {
        emptyAct->trigger();
    }
    if ( settings.value( "antialiasingActChecked", false ).toBool() != antialiasingAct->isChecked() )
    {
        antialiasingAct->trigger();
    }

    // update topology toolbar style
    int style = settings.value( "toolbarstyle", Qt::ToolButtonIconOnly ).toInt();
    topologyToolBar->setToolButtonStyle( ( Qt::ToolButtonStyle )style );
    if ( settings.value( "toolbarHidden", false ).toBool() )
    {
        topologyToolBar->hide();
    }
}

void
SystemTopology::saveExperimentSettings( QSettings& settings )
{
    foreach( SystemTopologyWidget * widget, widgetList )
    {
        widget->saveExperimentSettings( settings );
    }
}

void
SystemTopology::loadExperimentSettings( QSettings& settings )
{
    foreach( SystemTopologyWidget * widget, widgetList )
    {
        widget->loadExperimentSettings( settings );
    }
}

// defines if zero values in topologies should be represented by the
// color white or by the minimal color;
// this slot is connected to the actions in the menu
// Display/Coloring/Topology coloring

void
SystemTopology::whiteOff()
{
    whiteForZero = false;

    foreach( SystemTopologyWidget * widget, widgetList )
    {
        widget->updateColors();
    }
}
void
SystemTopology::whiteOn()
{
    whiteForZero = true;

    foreach( SystemTopologyWidget * widget, widgetList )
    {
        widget->updateColors();
    }
}

// set line coloring in topology widgets;
// connected to the actions in the menu Display/Coloring/Topology line coloring

void
SystemTopology::setLineType( LineType lineType )
{
    this->lineType = lineType;
    foreach( SystemTopologyWidget * widget, widgetList )
    {
        widget->setLineType( lineType );
    }
}
void
SystemTopology::blackLines()
{
    setLineType( BLACK_LINES );
}
void
SystemTopology::grayLines()
{
    setLineType( GRAY_LINES );
}
void
SystemTopology::whiteLines()
{
    setLineType( WHITE_LINES );
}
void
SystemTopology::noLines()
{
    setLineType( NO_LINES );
}


// define if lines in topologies should be drawn with antialiasing
//
void
SystemTopology::setAntialiasing( bool value )
{
    antialiasing = value;
    foreach( SystemTopologyWidget * widget, widgetList )
    {
        widget->valuesChanged();
    }
}

// toggle modus if lines in topologies should be drawn with antialiasing
void
SystemTopology::toggleAntialiasing()
{
    antialiasing = !antialiasing;
    foreach( SystemTopologyWidget * widget, widgetList )
    {
        widget->valuesChanged();
    }
}

// change the modus if unused topology planes should be displayed
void
SystemTopology::toggleUnused()
{
    showUnusedTopologyPlanes = !showUnusedTopologyPlanes;
    foreach( SystemTopologyWidget * widget, widgetList )
    {
        widget->toggleUnused();
    }
}


// return if unused topology planes should be displayed
//
bool
SystemTopology::getShowUnusedTopologyPlanes()
{
    return showUnusedTopologyPlanes;
}

// return if lines in topology display should be antialiased
//
bool
SystemTopology::getAntialiasing()
{
    return antialiasing;
}

// return the line style for topologies
// (black, gray, white or no lines, see constants.h for the LineType type)
//
LineType
SystemTopology::getLineType()
{
    return lineType;
}

// tell how zero values in topologies are colored
// (white or minimal color)
//
bool
SystemTopology::getWhiteForZero()
{
    return whiteForZero;
}

// set how zero values in topologies should be colored
// (white or minimal color)
//
void
SystemTopology::setWhiteForZero( bool whiteForZero )
{
    this->whiteForZero = whiteForZero;
}

void
SystemTopology::updateTopologyVisibility( int visible )
{
    tabInfoVisible += visible ? 1 : 0;
    tabInfoCount++;
    if ( tabInfoCount == widgetList.size() ) // all tabs have called this method
    {
        if ( tabInfoVisible == 0 )           // none of the topologies is visible
        {
            topologyToolBar->disableTopologyButtons();
        }
        else
        {
            topologyToolBar->enableTopologyButtons();
        }
        tabInfoVisible = 0;
        tabInfoCount   = 0;
    }
}
