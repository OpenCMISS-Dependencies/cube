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


#ifndef CallTree_H
#define CallTree_H

#include "Tree.h"
#include "AggregatedTreeItem.h"

class CallTree : public Tree
{
public:
    CallTree();
    ~CallTree();

    virtual QString
    getLabel();

    TreeItem*
    getTreeItem( uint32_t cnodeId );

    // ----  iteration handling
    /** define current tree item as root item of a loop */
    void
    setAsLoop( TreeItem* item );

    /** hide iterations, show aggregated subtree */
    void
    hideIterations();

    /** show all iterations */
    void
    showIterations();

    /** true, if iterations are aggregated */
    bool
    iterationsAreHidden();

    /** return the root item of the current loop */
    TreeItem*
    getLoopRootItem();

    /** return the root item of the aggregated loop */
    AggregatedTreeItem*
    getAggregatedRootItem();

    // ----  end iteration handling

    void
    setItemAsRoot( TreeItem* item );
    void
    pruneItem( TreeItem* item );
    void
    setItemAsLeaf( TreeItem* item );

protected:
    virtual TreeItem*
    createTree();

    virtual QString
    getItemName( cube::Vertex* vertex );

    virtual void
    computeBasicValues( const QList<Tree*>& leftTrees,
                        TreeItem*           top );

private:
    /* calltree can aggregate loop iterations */
    TreeItem*           loopRoot;           // item which is handled as a root of a loop and whose children are iterations
    AggregatedTreeItem* aggregatedLoopRoot; // root of the aggregated items of the currenly defined loop
    bool                iterationsAreHidden_;

    void
    updateTreeItemList();

    void
    computeValue( cube::list_of_metrics& mv,
                  TreeItem*              item );
    void
    computeValue( cube::list_of_metrics&      mv,
                  cube::list_of_sysresources& sv,
                  TreeItem*                   item );

    void
    computeValueHiddenIterations( cube::list_of_metrics&      mv,
                                  cube::list_of_sysresources& sv,
                                  TreeItem*                   item );
    void
    computeValueHiddenIterationsFirstLevel( cube::list_of_metrics&      mv,
                                            cube::list_of_sysresources& sv,
                                            TreeItem*                   item );

    // ---- iteration handling ----
    void
    unsetLoop();

    /** returns true, if the item is the root item of an aggregated loop (with hidden items) */
    void
    deleteMergedIterations( TreeItem* iter );
    void
    mergeIterations( TreeItem*               newParent,
                     const QList<TreeItem*>& iterations );
    AggregatedTreeItem*
    mergeIterations( TreeItem* loopRoot );

    // ---- end iteration handling ----
};

#endif // CallTree_H
