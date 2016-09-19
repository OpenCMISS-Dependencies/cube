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


#include "AggregatedTreeItem.h"
#include "CallTree.h"


/*
 * creates a IterationTreeItem with the data of item and the given depth
 */
AggregatedTreeItem::AggregatedTreeItem( int depth, TreeItem* item ) : TreeItem( item->getName(), item->getType(), 0 )
{
    this->name          = item->getName();
    this->hidden        = item->isHidden();
    this->expanded      = item->isExpanded();
    this->tree          = item->getTree();
    this->textExpanded  = "- " + item->getName();
    this->textCollapsed = "- " + item->getName();

    // todo selected, hidden, visible
    this->depth = depth;
}

const QList<cube::Cnode*>&
AggregatedTreeItem::getIterations() const
{
    return iterations;
}

bool
AggregatedTreeItem::isAggregatedRootItem() const
{
    CallTree* tree = static_cast<CallTree*> ( getTree() );
    return this == tree->getAggregatedRootItem();
}
