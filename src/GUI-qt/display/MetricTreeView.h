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


#ifndef METRICTREEVIEW_H
#define METRICTREEVIEW_H

#include "TreeView.h"

/**
 * MetricTreeView extends the default tree view with new context menu items
 */
class MetricTreeView : public TreeView
{
    Q_OBJECT
public:
    MetricTreeView( TreeModelInterface* modelInterface,
                    const QString&      tabLabel );

protected:
    virtual void
    fillContextMenu();

    virtual QString
    getContextDescription( TreeItem* item );

    bool
    selectionIsValid( QList<TreeItem*>& oldItems,
                      TreeItem*         newItem );

private:
    enum MetricTreeContextMenuAction { MetricFullInfo, CreateDerivatedChildMetric,
                                       CreateDerivatedRootMetric, EditDerivatedMetric, RemoveMetric };
    QHash <MetricTreeContextMenuAction, QAction*> metricContextMenuHash; // contains all tree specific context menu actions
    void
    createContextMenuActions();
    void
    createDerivatedMetric( bool asChild );

private slots:
    void
    onCreateDerivatedChildMetric();
    void
    onCreateDerivatedRootMetric();
    void
    onRemoveMetric();
    void
    onEditDerivatedMetric();
};

#endif // METRICTREEVIEW_H
