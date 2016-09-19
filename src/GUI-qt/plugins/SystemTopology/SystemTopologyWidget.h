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


// Controller

#ifndef _SYSTEMTOPOLOGYWIDGET_H
#define _SYSTEMTOPOLOGYWIDGET_H

#include <QScrollArea>
#include <QSplitter>
#include <vector>

#include "Cube.h"
#include "SystemTopologyData.h"
#include "SystemTopologyView.h"
#include "SystemTopologyViewTransform.h"
#include "TopologyDimensionBar.h"

class ScrollArea;
class QSettings;
class SystemTopology;
class SystemTopologyToolBar;

class SystemTopologyWidget : public QSplitter, public TabInterface
{
    Q_OBJECT

signals:
    void
    xAngleChanged( int angle );
    void
    yAngleChanged( int angle );

private slots:
    void
    vscroll( int val );
    void
    hscroll( int val );
    void
    handleSelection( TreeType type,
                     TreeItem*      item );

public slots:
    void
    selectItem( TreeItem* item,
                bool      add );

public:
    SystemTopologyWidget( SystemTopology*        sys,
                          SystemTopologyToolBar* bar,
                          unsigned               topologyId );
    ~SystemTopologyWidget();

    QWidget*
    widget();

    //re-compute the colors
    void
    updateColors();

    //remove all data
    void
    cleanUp();

    //rescale the topology sizes such that the topology fits into the scroll widget's window
    void
    rescale();

    //save and load topology settings
    void
    saveExperimentSettings( QSettings& settings );
    bool
    loadExperimentSettings( QSettings& settings );

    //update the value widget
    void
    updateValueWidget();

    //set the color for the lines framing processes/threads in the topology
    void
    setLineType( LineType lineType );

    //toggle the flag if unused planes should be painted or not
    void
    toggleUnused();

    SystemTopologyViewTransform*
    getTransform()
    {
        return transform;
    }
    SystemTopologyData*
    getData()
    {
        return data;
    }
    // TabInterface interface
    QIcon
    icon() const;

    // TabInterface implementation
    QString
    label() const;

    //update the colors and repaint
    void
    valuesChanged();
    void
    setActive( bool active );

private slots:
    void
    setSize( const QSize& size );

private:
    QWidget*
    getDimensionSelectionBar( cube::Cube*         cube,
                              SystemTopologyData* data );

    typedef enum { SELECT, FOLD } Mode;
    Mode selectMode;                         // if > 2 dimensions: call multiDimFold or multiDimSelect

    // widgets created in this class
    TopologyDimensionBar* dimensionBar;
    ScrollArea*           scroll;

    SystemTopologyData*          data;
    SystemTopologyView*          view;
    SystemTopologyViewTransform* transform;

    // information from caller
    PluginServices*        service;
    SystemTopologyToolBar* topologyToolBar;
    SystemTopology*        sys;

    unsigned topologyId;
    QString  topologyName;

    bool firstVisible;
};

#endif
