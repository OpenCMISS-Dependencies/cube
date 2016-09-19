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



#include "SystemBoxPlot.h"
#include "PluginManager.h"
#include <qdebug.h>
#include <algorithm>
#include <cmath>

using namespace std;

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( SystemBoxPlotPlugin, SystemBoxPlot );
#endif

/** set a version number, the plugin with the highest version number will be loaded */
void
SystemBoxPlot::version( int& major, int& minor, int& bugfix ) const
{
    major  = 1;
    minor  = 0;
    bugfix = 0;
}

QString
SystemBoxPlot::getHelpText() const
{
    return "The Boxplot plugin shows the distribution of values in the system tree."
    " It starts with the smallest value (minimum) and ends with the largest value (maximum)."
    " The bottom and top of the box represent the lower quartile (Q1) and the upper quartile (Q3)."
    " The bold line represents the median (Q2) and the dashed line represents the mean value."
    " To zoom into the boxplot, select an area with the mouse."
    " To zoom out, click with the middle mouse button in the drawing.";
}

// plugin name
QString
SystemBoxPlot::name() const
{
    return "System Boxplot Plugin";
}

/**
   creates a bot plot which displays the values calculated by SystemTreeWidget
 */

SystemBoxPlot::SystemBoxPlot()
{
    boxplot = 0;
}

SystemBoxPlot::~SystemBoxPlot()
{
    delete boxplot;
}


bool
SystemBoxPlot::cubeOpened( PluginServices* service )
{
    this->service = service;

    boxplot = new BoxPlot( 0 );
    boxplot->setWhatsThis(
        "This Boxplot shows the distribution of values in the system tree."
        " It starts with the smallest value (minimum) and ends with the largest value (maximum)."
        " The bottom and top of the box represent the lower quartile (Q1) and the upper quartile (Q3)."
        " The bold line represents the median (Q2) and the dashed line represents the mean value."
        " To zoom into the boxplot, select an area with the mouse."
        " To zoom out, click with the middle mouse button in the drawing."
        );

    scrollArea = new ScrollArea( 0 );
    scrollArea->setWidget( boxplot );
    scrollArea->setWidgetResizable( true );
    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    container = new QSplitter( Qt::Vertical );
    container->addWidget( scrollArea );

    subsetCombo = new QComboBox();
    container->addWidget( subsetCombo );

    // set lower splitter element to minimum size (1 pixel => replaced by minimumSize())
    QList<int> sizeList;
    sizeList << container->size().height() << 1;
    container->setSizes( sizeList );

    connect( service, SIGNAL( orderHasChanged( const QList<DisplayType> & ) ),
             this, SLOT( orderHasChanged( const QList<DisplayType> & ) ) );

    service->addTab( SYSTEM, this );
    return true;
}

QWidget*
SystemBoxPlot::widget()
{
    return container;
}

QString
SystemBoxPlot::label() const
{
    return "BoxPlot";
}

void
SystemBoxPlot::valuesChanged()
{
    StatisticalInformation item = calculateStatistics( false );
    StatisticalInformation abs  = calculateStatistics( true );

    double start;
    if ( service->getValueModus() == ABSOLUTE )
    {
        start = item.minimum;
    }
    else
    {
        start = 0.0;
    }

    // fill box plot with calculated data
    boxplot->Set( item, abs, start );

    updateValueWidget();

    updateSubsetCombo(); // visited count may have changed
}

QIcon
SystemBoxPlot::icon() const
{
    return QIcon( ":images/boxplot-icon.png" );
}

// calculate statistical values
StatisticalInformation
SystemBoxPlot::calculateStatistics( bool absolute )
{
    int    theCount;
    double theSum;
    double theMean;
    double theVariance;
    double theMin;
    double theMax;
    double theMedian;
    double theQ25;
    double theQ75;

    vector<double> values;

    theSum = 0.;
    double value;
    foreach( TreeItem * item, service->getActiveSubset() )
    {
        if ( item->isLeaf() ) // single-threaded processes or thread
        {
            if ( absolute )
            {
                value = item->getAbsoluteValue();
            }
            else
            {
                value = item->getValue();
            }

            values.push_back( value );
            theSum += value;
        }
    }

    theCount = values.size();

    if ( theCount == 0 )
    {
        theCount = 0;
        boxplot->Reset();
        return StatisticalInformation( 0, 0, 0, 0, 0, 0, 0, 0, 0 ); // not enough values to display
    }
    theMean = theSum / values.size();

    theVariance = 0.;
    for ( unsigned i = 0; i < values.size(); i++ )
    {
        theVariance += ( values.at( i ) - theMean ) * ( values.at( i ) - theMean );
    }
    if ( values.size() > 0 )
    {
        theVariance /= ( values.size() );
    }

    sort( values.begin(), values.end() );
    theMin    = *values.begin();
    theMax    = *( values.end() - 1 );
    theMedian = *( values.begin() + values.size() / 2 );
    theQ25    = *( values.begin() + values.size() / 4 );
    theQ75    = *( values.begin() + values.size() * 3 / 4 );

    return StatisticalInformation( theCount, theSum, theMean, theMin, theQ25, theMedian, theQ75, theMax, theVariance );
}

/**
   updates the value widget below the tab widget
 */
void
SystemBoxPlot::updateValueWidget()
{
    vector<double>         values;
    StatisticalInformation item;

    if ( service->getValueModus() == ABSOLUTE )
    {
        item = calculateStatistics( true );
    }
    else
    {
        item = calculateStatistics( false );
    }

    /* from SystemTopologyWidget */
    // in case the topology is empty or if no items are selected, just
    // clear the value widget
    if ( item.count == 0 )
    {
        service->clearValueWidget();
    }
    else
    {
        double sigma = sqrt( item.variance );
        double displayedValue;

        if ( service->getValueModus() == ABSOLUTE )
        {
            bool intType = service->intMetricSelected();

            displayedValue = item.mean;
            // in the absolute value mode display the mean value (with the min/max values)
            service->updateValueWidget( item.minimum, item.maximum, displayedValue, intType, false, item.mean, sigma );
        }
        else
        {
            bool intType = false;
            displayedValue = 100; // show +/- sigma
            // in value modes other that the absolute value mode display the
            // current minvalue/value/maxvalue plus absolute
            // minvalue/value/maxvalue
            service->updateValueWidget( 0.0, 100.0, displayedValue,
                                        item.minimum, item.maximum, displayedValue, intType, false, item.mean, sigma );
        }
    }
}

void
SystemBoxPlot::orderHasChanged( const QList<DisplayType>& order )
{
    bool enabled = order.at( 0 ) != SYSTEM; // disable boxplot, if system widget is first
    service->enableTab( this, enabled );
}

void
SystemBoxPlot::setActive( bool active )
{
    if ( active )
    {
        updateSubsetCombo();
        valuesChanged();
    }
}

/** Updates the labels of the subset combo and select the active subset. No signals are emited.
 */
void
SystemBoxPlot::updateSubsetCombo()
{
    subsetCombo->disconnect();
    subsetModel.setStringList( service->getSubsetLabelList() );
    subsetCombo->setModel( &subsetModel );
    subsetCombo->setCurrentIndex( service->getActiveSubsetIndex() );
    connect( subsetCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( subsetChanged( int ) ) );
}

void
SystemBoxPlot::subsetChanged( int idx )
{
    service->setActiveSubset( idx );
    valuesChanged();
    updateSubsetCombo(); // update number of elements for "Visited"
}
