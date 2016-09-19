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




#include "HeatmapController.h"
#include "HeatmapPlotArea.h"


HeatMapPlotAreaController::HeatMapPlotAreaController( QWidget* parent ) : AbstractPlotAreaController( parent )
{

    view = new HeatMapPlotArea( parent );
    AbstractPlotAreaController::setConnections();
}

void
HeatMapPlotAreaController::setData( QVector<QVector<double> > data, double dataMin, double dataMax,PluginServices* service ) const
{
    ( ( HeatMapPlotArea* )view )->setDataMin( dataMin );
    ( ( HeatMapPlotArea* )view )->setDataMax( dataMax );
    ( ( HeatMapPlotArea* )view )->setData( data );
     ( ( HeatMapPlotArea* )view )->setPluginService(service);
    view->repaint();
}
void
HeatMapPlotAreaController::setDataColor(QVector<QVector< QColor> > tempDataColor)
{
    ( ( HeatMapPlotArea* )view )->setDataColor(tempDataColor);
}

double
HeatMapPlotAreaController::getData( int processId, int iterationId ) const
{
    return ( ( HeatMapPlotArea* )view )->getData( processId, iterationId );
}

void
HeatMapPlotAreaController::reset()
{
    ( ( HeatMapPlotArea* )view )->reset();
    view->repaint();
}
void
HeatMapPlotAreaController::setLegendItemsCount( int legendItemsCount )
{
    ( ( HeatMapPlotArea* )view )->setLegendItemsCount( legendItemsCount );
    view->repaint();
}

int
HeatMapPlotAreaController::getLegendItemsCount() const
{
    return ( ( HeatMapPlotArea* )view )->getLegendItemsCount();
}

/*void
HeatMapPlotAreaController::setColorMin( QColor min )
{
    ( ( HeatMapPlotArea* )view )->setDataMinColor( min );
}

void
HeatMapPlotAreaController::setColorMax( QColor max )
{
    ( ( HeatMapPlotArea* )view )->setDataMaxColor( max );
}*/

