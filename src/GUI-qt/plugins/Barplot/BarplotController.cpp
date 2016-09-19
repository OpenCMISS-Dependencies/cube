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



#include "BarplotController.h"
#include "BarplotArea.h"

BarsPlotAreaController::BarsPlotAreaController( QWidget* parent ) :
    AbstractPlotAreaController( parent )
{
    view = new BarPlotArea( parent );
    AbstractPlotAreaController::setConnections();
}

void
BarsPlotAreaController::updateData( QList<CubeDataItem> &data )
{
    ( ( BarPlotArea* )view )->setDataRef( data );
    view->repaint();
}
void
BarsPlotAreaController::reset()
{
    ( ( BarPlotArea* )view )->reset();
    view->repaint();
}
/*void
BarsPlotAreaController::setDisplayStatus(bool newDisplayStatus)
{
    ( ( BarPlotArea* )view )->setDisplayStatus(newDisplayStatus);
}*/
