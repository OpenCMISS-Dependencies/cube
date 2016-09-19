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



#include "AbstractPlotAreaController.h"

AbstractPlotAreaController::AbstractPlotAreaController(QWidget *parent) : QObject(parent)
{
}

void AbstractPlotAreaController::setConnections()
{
    QObject::connect(view, SIGNAL(mouseIsClicked(QMouseEvent*)), this, SIGNAL(mouseIsClicked(QMouseEvent*)));
}

AbstractPlotArea* AbstractPlotAreaController::getView()
{
    return view;
}

void AbstractPlotAreaController::updateVerticalMajorMeshLines(QList<int> VMajorNotches)
{
    view->setVMajorNotches(VMajorNotches);
    view->repaint();
}

void AbstractPlotAreaController::updateVerticalMinorMeshLines(QList<int> VMinorNotches)
{
    view->setVMinorNotches(VMinorNotches);
    view->repaint();
}

void AbstractPlotAreaController::updateHorizontalMajorMeshLines(QList<int> HMajorNotches)
{
    view->setHMajorNotches(HMajorNotches);
    view->repaint();

}

void AbstractPlotAreaController::updateHorizontalMinorMeshLines(QList<int> HMinorNotches)
{
    view->setHMinorNotches(HMinorNotches);
    view->repaint();

}

void AbstractPlotAreaController::updateMouseAxisHorizontalAlignment(plotWidget::Alignment alignment)
{
    view->setMouseAxisHorizontalAlignment(alignment);
    view->repaint();
}

void AbstractPlotAreaController::updateMouseAxisVerticalAlignment(plotWidget::Alignment alignment)
{
    view->setMouseAxisVerticalAlignment(alignment);
    view->repaint();
}

void AbstractPlotAreaController::setLegendVisible(bool isLegendVisible)
{
    view->setLegendVisible(isLegendVisible);
    view->repaint();
}

void AbstractPlotAreaController::updateLegendXAlignment(plotWidget::Alignment newXAlignment)
{
    view->setLegendXAlignment(newXAlignment);
    view->repaint();
}

void AbstractPlotAreaController::updateLegendYAlignment(plotWidget::Alignment newYAlignment)
{
    view->setLegendYAlignment(newYAlignment);
    view->repaint();
}

void AbstractPlotAreaController::setLegendBackgroundSolid(bool isLegendBackgroundSolid)
{
    view->setLegendBackgroundSolid(isLegendBackgroundSolid);
    view->repaint();

}
