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


#ifndef CALLTREEITEM_H
#define CALLTREEITEM_H

#include "TreeItem.h"

/**
 * tree item of a aggregated loop
 * only used by CallTree
 */
class AggregatedTreeItem : public TreeItem
{
    friend class CallTree;

public:
    /**
     * returns the items, the current item is aggregated from
     * - for the root element of the loop, all hidden child iterations are returned
     * - for elements inside the loop, all its iterations are returned
     */
    const QList<cube::Cnode*>&
    getIterations() const;

    bool
    isAggregatedRootItem() const;

private:
    // ----- hidden loop iterations (only for call tree items)
    QList<cube::Cnode*> iterations; // list of current element in all iterations

    AggregatedTreeItem( int depth, TreeItem* item );
};

#endif // CALLTREEITEM_H
