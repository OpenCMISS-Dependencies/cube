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


#ifndef SYSTEMTREE_H
#define SYSTEMTREE_H

#include "Tree.h"
#include "CubeMetric.h"
#include "CubeRegion.h"
#include "CubeSysres.h"


class SystemTree : public Tree
{
public:
    SystemTree();

    virtual QString
    getLabel();

    QList<TreeItem*>
    getVisitedItems();

    TreeItem*
    getTreeItem( uint32_t systemId );

protected:
    TreeItem*
    createTree();

    virtual void
    computeBasicValues( const QList<Tree*>& leftTrees,
                        TreeItem*           top );

    virtual QString
    getItemName( cube::Vertex* vertex );

private:
    void
    computeValue( cube::list_of_metrics& mv,
                  TreeItem*              item );
    void
    computeValue( cube::list_of_metrics& mv,
                  cube::list_of_cnodes&  cv,
                  TreeItem*              item );
    void
    computeValue( cube::list_of_metrics& mv,
                  cube::list_of_regions& rv,
                  TreeItem*              item );
    double
    computeValue( TreeItem*                metricItem,
                  cube::CalculationFlavour metricState,
                  TreeItem*                callItem,
                  cube::CalculationFlavour callState,
                  TreeItem*                systemItem,
                  cube::CalculationFlavour systemState );
    void
    fillCnodeList( QList<TreeItem*>      items,
                   cube::list_of_cnodes& cv );
};

#endif // METRICTREE_H
