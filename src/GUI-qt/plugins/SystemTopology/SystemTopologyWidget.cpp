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



#include "SystemTopologyWidget.h"
#include "SystemTopologyToolBar.h"
#include "ScrollArea.h"

#include "CubeCartesian.h"

#include <QScrollBar>
#include <QSettings>
#include <QDebug>
#include <assert.h>

#include "SystemTopology.h"
#include "SystemTopologyView.h"
#include "SystemTopologyDrawing.h"
// #include "SystemTopologyGL.h"
#include "SystemTopologyToolBar.h"
#include "ScrollArea.h"


/**
   controls several widgets:
   TopologyDimensionBar which allows to reorder the dimensions
   SystemTopologyView which draws the topology
 */

/*************** constructor / destructor *************************/

SystemTopologyWidget::SystemTopologyWidget( SystemTopology*        sys,
                                            SystemTopologyToolBar* bar,
                                            unsigned               topologyId ) : QSplitter( Qt::Vertical )

{
    this->sys        = sys;
    this->topologyId = topologyId;
    topologyToolBar  = bar;
    service          = sys->getService();
    firstVisible     = true;
    selectMode       = FOLD;
    cube::Cube* cube = service->getCube();

    // initialize topologyName, which is used as tab label
    topologyName = ( cube->get_cartv() ).at( topologyId )->get_name().c_str();
    if ( topologyName == "" )
    {
        topologyName.append( "Topology " );
        topologyName.append( QString::number( topologyId ) );
    }

    // add current widget as a system tab
    service->addTab( SYSTEM, this ); // has to be called after constructor has been finished

    data         = new SystemTopologyData( sys, topologyId );
    dimensionBar = ( TopologyDimensionBar* )getDimensionSelectionBar( cube, data );
    data->setFoldingDimensions( dimensionBar->getFoldingVector() );
    data->initialize( cube );

    transform = new SystemTopologyViewTransform( data );
    view      = new SystemTopologyDrawing( data, transform, topologyToolBar );
    // view = new SystemTopologyGL(data);

    topologyToolBar->reinit();

    // put topology drawing inside a scroll area
    scroll = new ScrollArea( this );
    scroll->setWidget( view );
    connect( scroll, SIGNAL( resized( QSize ) ), this, SLOT( setSize( QSize ) ) );
    QScrollBar* sbh = scroll->horizontalScrollBar();
    QScrollBar* sbv = scroll->verticalScrollBar();
    connect( sbh, SIGNAL( valueChanged( int ) ), this, SLOT( hscroll( int ) ) );
    connect( sbv, SIGNAL( valueChanged( int ) ), this, SLOT( vscroll( int ) ) );
    this->addWidget( scroll );

    // put topology dimension toolbar into scrollPane and put it below the topology scroll area
    QScrollArea* scrollDim = new QScrollArea();
    this->addWidget( scrollDim );
    scrollDim->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scrollDim->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scrollDim->setFrameStyle( QFrame::NoFrame );
    scrollDim->setMinimumHeight( dimensionBar->minimumSizeHint().height() );
    scrollDim->setMaximumHeight( dimensionBar->minimumSizeHint().height() );
    scrollDim->setWidget( dimensionBar );
    long ndims = ( cube->get_cartv() ).at( this->topologyId )->get_ndims();
    if ( ndims <= 3 ) // minimize dimension selection bar
    {
        QList<int> sizeList;
        sizeList << 1 << 0;
        this->setSizes( sizeList );
    }

    connect( view, SIGNAL( selectItem( TreeItem*, bool ) ), this, SLOT( selectItem( TreeItem*, bool ) ) );
    connect( view, SIGNAL( scrollTo( int, int ) ), scroll, SLOT( scrollTo( int, int ) ) );

    connect( data, SIGNAL( dataChanged() ), view, SLOT( updateDrawing() ) );
    connect( data, SIGNAL( viewChanged() ), view, SLOT( updateDrawing() ) );
    connect( data, SIGNAL( rescaleRequest() ), transform, SLOT( rescale() ) );

    connect( transform, SIGNAL( rescaleRequest() ), view, SLOT( rescaleDrawing() ) );
    connect( transform, SIGNAL( viewChanged() ), view, SLOT( updateDrawing() ) );
    connect( transform, SIGNAL( zoomChanged( double ) ), view, SLOT( changeZoom( double ) ) );
    connect( transform, SIGNAL( xAngleChanged( int ) ), view, SLOT( setXAngle( int ) ) );
    connect( transform, SIGNAL( yAngleChanged( int ) ), view, SLOT( setYAngle( int ) ) );
    connect( transform, SIGNAL( positionChanged( int, int ) ), view, SLOT( move( int, int ) ) );

    connect( service, SIGNAL( treeItemIsSelected( TreeType, TreeItem* ) ), this, SLOT( handleSelection( TreeType, TreeItem* ) ) );
}

SystemTopologyWidget::~SystemTopologyWidget()
{
    delete data;
    delete view;
    delete transform;
    delete scroll;
}

/**
 * @brief returns the widget that will be placed in SystemTabWidget
 */
QWidget*
SystemTopologyWidget::widget()
{
    return this;
}

void
SystemTopologyWidget::vscroll( int val )
{
    assert( view != 0 );
    view->scrolledTo( scroll->horizontalScrollBar()->value(), val );
}

void
SystemTopologyWidget::hscroll( int val )
{
    assert( view != 0 );
    view->scrolledTo( val, scroll->verticalScrollBar()->value() );
}

QIcon
SystemTopologyWidget::icon() const
{
    return QIcon( ":images/topology-icon.png" );
}

QString
SystemTopologyWidget::label() const
{
    return topologyName;
}

/**************************** miscellaneous **********************/


/** create toolbar to select dimensions to display
 */
QWidget*
SystemTopologyWidget::getDimensionSelectionBar( cube::Cube* cube, SystemTopologyData* data )
{
    const std::vector<long>&        dims  = cube->get_cart( topologyId )->get_dimv();
    const std::vector<std::string>& names = cube->get_cart( topologyId )->get_namedims();

    TopologyDimensionBar* dimBar = new TopologyDimensionBar( dims, names );

    connect( dimBar, SIGNAL( selectedDimensionsChanged( std::vector<long>) ), data, SLOT( selectedDimensionsChanged( std::vector<long>) ) );
    connect( dimBar, SIGNAL( foldingDimensionsChanged( std::vector<std::vector<int> >) ), data, SLOT( foldingDimensionsChanged( std::vector<std::vector<int> >) ) );
    connect( dimBar, SIGNAL( splitLengthChanged( int ) ), data, SLOT( splitLengthChanged( int ) ) );

    return dimBar;
}

void
SystemTopologyWidget::handleSelection( TreeType type, TreeItem* item )
{
    if ( type == SYSTEMTREE )
    {
        topologyToolBar->setRecentSelection( item );
    }
}

/**
   marks the given item as selected in System tree
   @param add if true, adds the item to the previously selected items, otherwise deselects previous selections
 */
void
SystemTopologyWidget::selectItem( TreeItem* item, bool add )
{
    service->selectItem( item, add );

    data->updateSelection();
}


// -----------------------------------------------------------------------

// updates the value widget below the tab widget ; todo: move to controller class
void
SystemTopologyWidget::updateValueWidget()
{
    // in case the topology is empty , just clear the value widgt
    if ( data->getDim( 0 ) == 0 || data->getDim( 1 ) == 0 || data->getDim( 2 ) == 0 )
    {
        service->clearValueWidget();
    }
    else
    {
        double _sigma    = 0.0;
        double _variance = 0.0;
        double _mean     = 0.0;

        double tmp = 0.;

        unsigned N = 0;
        foreach( TreeItem * item, service->getTreeItems( SYSTEMTREE ) )
        {
            if ( item->isLeaf() )
            {
                tmp    = item->getAbsoluteValue();
                _mean += tmp;
                N++;
            }
        }

        _mean = ( N != 0 ) ? _mean / N : 0.;

        foreach( TreeItem * item, service->getTreeItems( SYSTEMTREE ) )
        {
            if ( item->isLeaf() )
            {
                tmp        = item->getAbsoluteValue();
                _variance += ( _mean - tmp ) * ( _mean - tmp );
            }
        }
        _sigma = ( N != 0 ) ? sqrt( _variance / N ) : 0.;

        bool   nothingSelected = true;
        double value           = 0.0;
        double absoluteValue   = 0.0;
        foreach( TreeItem * item, service->getSelections( SYSTEMTREE ) )
        {
            if ( item->isLeaf() )
            {
                nothingSelected = false;
                absoluteValue  +=  item->getAbsoluteValue();
                if ( service->getValueModus() != ABSOLUTE )
                {
                    value += item->getValue();
                }
            }
        }

        bool   userDefinedMinMaxValues;
        double userMinValue, userMaxValue;
        userDefinedMinMaxValues = service->getUserDefinedMinValues( SYSTEMTREE, userMinValue, userMaxValue );

        // compute value for minimal color
        double min;
        if ( userDefinedMinMaxValues )
        {
            min = userMinValue;
        }
        else if ( service->getValueModus() == ABSOLUTE )
        {
            min = data->getMinValue();
        }
        else
        {
            min = 0.0;
        }

        // compute value for minimal color
        double max;
        if ( userDefinedMinMaxValues )
        {
            max = userMaxValue;
        }
        else if ( service->getValueModus() == ABSOLUTE )
        {
            max = data->getMaxValue();
        }
        else
        {
            max = 100.0;
        }

        bool intType = service->intMetricSelected();
        if ( nothingSelected )
        {
            service->updateValueWidget( min, max, intType, userDefinedMinMaxValues, _mean, _sigma );
        }
        else if ( service->getValueModus() == ABSOLUTE )
        {
            // in the absolute value mode display the absolute value (with
            // the min/max values)
            service->updateValueWidget( min, max, absoluteValue, intType, userDefinedMinMaxValues, _mean, _sigma );
        }
        else
        {
            // in value modes other that the absolute value mode display the
            // current minvalue/value/maxvalue plus absolute
            // minvalue/value/maxvalue
            service->updateValueWidget( 0.0, 100.0, value, data->getMinAbsValue(), data->getMaxAbsValue(), absoluteValue, intType, userDefinedMinMaxValues, _mean, _sigma );
        }
    }
}


/****************** slots for menuitems / TabWidget ***************************/

void
SystemTopologyWidget::rescale()
{
    transform->rescale( false );
}

// set the line type for topologies: black, gray, white, or no lines
void
SystemTopologyWidget::setLineType( LineType lineType )
{
    data->setLineType( lineType );
}

// toggle the state if unused planes should be displayed or not
void
SystemTopologyWidget::toggleUnused()
{
    data->toggleUnused();
}

void
SystemTopologyWidget::valuesChanged()
{
    assert( view != 0 );
    data->updateColors();
    view->updateDrawing();
    updateValueWidget();
}

void
SystemTopologyWidget::updateColors()
{
    assert( view != 0 );
    data->updateColors();
    view->updateDrawing();
    updateValueWidget();
}

/************************ settings ***************************/

// save topology settings
//
void
SystemTopologyWidget::saveExperimentSettings( QSettings& settings )
{
    transform->saveSettings( settings, topologyId );
    dimensionBar->saveSettings( settings, topologyId );
}

bool
SystemTopologyWidget::loadExperimentSettings( QSettings& settings )
{
    dimensionBar->loadSettings( settings, topologyId );
    transform->loadSettings( settings, topologyId );
    return true;
}

void
SystemTopologyWidget::setActive( bool active )
{
    if ( active )
    {
        topologyToolBar->setTopologyWidget( this );

        valuesChanged();

        // if tab becomes visible for the 1st time, rescale the drawing to fit into the widget
        if ( firstVisible )
        {
            transform->rescale( true );
            firstVisible = false;
        }
    }
    sys->updateTopologyVisibility( active );
}

void
SystemTopologyWidget::setSize( const QSize& size )
{
    view->setSize( size );
}
