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




#include "CubeRegion.h"

#include "CallTree.h"

#include "TreeView.h"
#include "CubeMetric.h"
#include "AggregatedTreeItem.h"
#include "TreeModel.h"

using namespace std;
using namespace cube;

CallTree::CallTree()
{
    treeType             = CALLTREE;
    iterationsAreHidden_ = false;
    loopRoot             = 0;
    aggregatedLoopRoot   = 0;
}

CallTree::~CallTree()
{
    unsetLoop(); // delete merged iterations
}

QString
CallTree::getLabel()
{
    if ( !cube )
    {
        return "not yet initialized";
    }
    return ( cube->get_calltree_title() != "" ) ? cube->get_calltree_title().c_str() : "Call tree";
}

TreeItem*
CallTree::createTree()
{
    TreeItem* top = new TreeItem( QString( "Invisible call tree root" ), CALLITEM, 0 );

    createItems<cube::Cnode>( top, cube->get_root_cnodev(), CALLITEM );

    /** check it an item is defined as a loop ("CLUSTER ROOT CNODE ID" is set) */
    QString nrs = QString( cube->get_attr( "CLUSTER ROOT CNODE ID" ).c_str() );
    if ( nrs.length() > 0 )
    {
        TreeItem* item    = 0;
        uint      cnodeID = nrs.toUInt();
        foreach( TreeItem * it, treeItemHash.values() )
        {
            uint itemCnode = it->getCubeObject()->get_id();
            if ( itemCnode == cnodeID )
            {
                item = it;
                break;
            }
        }
        setAsLoop( item );
    }

    return top;
}

QString
CallTree::getItemName( cube::Vertex* vertex )
{
    std::string name = ( static_cast<cube::Cnode* > ( vertex ) )->get_callee()->get_name();

    if ( ( static_cast<cube::Cnode* > ( vertex ) )->get_num_parameters().size() != 0 )
    {
        char buffer[ 100 ];
        memset( buffer, 0, 100 );

        std::vector<std::pair<std::string, double> > params = ( static_cast<cube::Cnode* > ( vertex ) )->get_num_parameters();

        name += "(";
        for ( unsigned i = 0; i < params.size() - 1; i++ )
        {
            sprintf( buffer, "%g", params[ i ].second );
            name += params[ i ].first + "=" + std::string( buffer ) + ", ";
            memset( buffer, 0, 100 );
        }
        if ( params.size() >= 1 )
        {
            sprintf( buffer, "%g", params[ params.size() - 1 ].second );
            name += params[ params.size() - 1 ].first + "=" + std::string( buffer );
            memset( buffer, 0, 100 );
        }
        name += ")";
    }
    if ( ( static_cast<cube::Cnode* > ( vertex ) )->get_str_parameters().size() != 0 )
    {
        std::vector<std::pair<std::string, std::string> > params = ( static_cast<cube::Cnode* > ( vertex ) )->get_str_parameters();

        name += "[";
        for ( unsigned i = 0; i < params.size() - 1; i++ )
        {
            name += params[ i ].first + "=" + params[ i ].second + ", ";
        }
        if ( params.size() >= 1 )
        {
            name += params[ params.size() - 1 ].first + "=" + params[ params.size() - 1 ].second;
        }
        name += "]";
    }


    return QString( name.c_str() );
}


// -----------------------------------------------------------------
// ---------------- iteration handling --------------------------
// -----------------------------------------------------------------

#define LOOP_MARKER " (loop)"

/** The given item is set as a loop. This allows to hide its iterations.
 */
void
CallTree::setAsLoop( TreeItem* item )
{
    assert( item );
    if ( item == aggregatedLoopRoot || item == loopRoot )
    {
        return;
    }

    if ( item->children.size() == 0 )
    {
        return;
    }

    unsetLoop(); // unset previously set loop
    loopRoot           = item;
    aggregatedLoopRoot = mergeIterations( loopRoot );

    int iterations = aggregatedLoopRoot->iterations.size();
    aggregatedLoopRoot->name.append( " (" + QString::number( iterations ) + " iterations)" );

    loopRoot->name.append( LOOP_MARKER );
    loopRoot->label.append( LOOP_MARKER );
}

void
CallTree::unsetLoop()
{
    if ( loopRoot != 0 )
    {
        if ( iterationsAreHidden_ )
        {
            showIterations();
        }

        QString& str1 = loopRoot->name;
        int      idx  = str1.lastIndexOf( LOOP_MARKER );
        if ( idx > 0 )
        {
            str1.remove( idx, QString( LOOP_MARKER ).length() ); // remove marker, TODO use icon
        }
        QString& str2 = loopRoot->label;
        idx = str2.lastIndexOf( LOOP_MARKER );
        if ( idx > 0 )
        {
            str2.remove( idx, QString( LOOP_MARKER ).length() ); // remove marker, TODO use icon
        }

        deleteMergedIterations( aggregatedLoopRoot );
        aggregatedLoopRoot = 0;
        loopRoot           = 0;
    }
}


/**
   Hides the iteration of the currenly defined loop.
 */
void
CallTree::hideIterations()
{
    if ( !loopRoot )
    {
        return;
    }
    iterationsAreHidden_ = true;

    // replace loop structure with aggregated structure
    treeModel->replaceSubtree( loopRoot, aggregatedLoopRoot );

    aggregatedLoopRoot->select();
    aggregatedLoopRoot->setExpanded( true );

    updateTreeItemList();
}

void
CallTree::showIterations()
{
    iterationsAreHidden_ = false;

    // replace aggregated loop structure with iterations
    treeModel->replaceSubtree( aggregatedLoopRoot, loopRoot );

    loopRoot->select();
    loopRoot->setExpanded( true );

    updateTreeItemList();
}

bool
CallTree::iterationsAreHidden()
{
    return iterationsAreHidden_;
}

TreeItem*
CallTree::getLoopRootItem()
{
    return loopRoot;
}

AggregatedTreeItem*
CallTree::getAggregatedRootItem()
{
    return aggregatedLoopRoot;
}

/**
   Treats the current item as root of a loop and merges its children. References from the merged iterations
   to the single iterations and vice versa are inserted.
 */
AggregatedTreeItem*
CallTree::mergeIterations( TreeItem* loopRoot )
{
    AggregatedTreeItem* aggregatedRoot = new AggregatedTreeItem( loopRoot->getDepth(), loopRoot );
    aggregatedRoot->cubeObject = loopRoot->cubeObject;
    aggregatedRoot->parentItem = loopRoot->parentItem;

    foreach( TreeItem * iteration, loopRoot->getChildren() )
    {
        aggregatedRoot->iterations.append( static_cast<Cnode*> ( iteration->getCubeObject() ) );
    }

    mergeIterations( aggregatedRoot, loopRoot->getChildren() );

    return aggregatedRoot;
}


/**
 * @brief mergeIterations recursively merges iteration items with the same name into
 * new items and appends them to newParent.
 * Only items of the same tree depth which have the same name are merged.
 */
void
CallTree::mergeIterations( TreeItem* newParent, const QList<TreeItem*>& iterations )
{
    QHash<QString, AggregatedTreeItem*> hash; // name -> aggregated item

    // find all children of iterations, eleminate those with same name and append a copy to newparent
    foreach( TreeItem * iteration, iterations )
    {
        foreach( TreeItem * child, iteration->getChildren() )
        {
            if ( !hash.contains( child->getName() ) )
            {
                AggregatedTreeItem* newChild = new AggregatedTreeItem( newParent->getDepth() + 1, child );

                newParent->addChild( newChild );

                hash.insert( child->name, newChild );
            }
            AggregatedTreeItem* aggregated = hash.value( child->name );
            aggregated->iterations.append( static_cast<cube::Cnode*> ( child->cubeObject ) );
        }
    }

    // if newly added children of newParent have children, call this method recursively
    foreach( TreeItem * item, newParent->getChildren() )
    {
        QString childName = item->name;

        QList<TreeItem*> listOfChildren;
        foreach( TreeItem * iteration, iterations )
        {
            foreach( TreeItem * child, iteration->getChildren() )
            {
                if ( child->name == childName )
                {
                    listOfChildren.push_back( child );
                }
            }
        }
        if ( listOfChildren.size() > 0 )
        {
            mergeIterations( item, listOfChildren );
        }
    }
}

/**
   delete the merged iterations and all its children recursively
 */
void
CallTree::deleteMergedIterations( TreeItem* aggregated )
{
    foreach( TreeItem * item, aggregated->getChildren() ) // for all aggregated items
    {
        deleteMergedIterations( item );
        delete item;
    }
}


/**
 * updates the vector Tree::treeItems by exchanging loop iterations with aggregated iterations and vice versa
 */
void
CallTree::updateTreeItemList()
{
    if ( (  iterationsAreHidden_ && treeItems.contains( aggregatedLoopRoot ) ) ||
         ( !iterationsAreHidden_ && treeItems.contains( loopRoot ) ) )
    {
        return; // already up to date
    }

    QList<TreeItem*> oldItems;
    QList<TreeItem*> newItems;

    if ( iterationsAreHidden_ )
    {
        oldItems.append( loopRoot );
        newItems.append( aggregatedLoopRoot );
    }
    else
    {
        oldItems.append( aggregatedLoopRoot );
        newItems.append( loopRoot );
    }

    // recursively remove all items of oldItems from Tree::treeItems
    while ( !oldItems.empty() )
    {
        TreeItem* item = oldItems.takeFirst();
        treeItems.removeOne( item );
        if ( item->getChildren().size() > 0 )
        {
            oldItems.append( item->getChildren() );
        }
    }

    // recursively add all items of newItems from Tree::treeItems
    while ( !newItems.empty() )
    {
        TreeItem* item = newItems.takeFirst();
        treeItems.append( item );
        if ( item->getChildren().size() > 0 )
        {
            newItems.append( item->getChildren() );
        }
    }
}

// -----------------------------------------------------------------
// ---------------- end iterations ------ --------------------------
// -----------------------------------------------------------------



// -----------------------------------------------------------------
// ---------------- calculation functions --------------------------
// -----------------------------------------------------------------

// compute the current value for a CALL tree widget item
// if the call tree is on LEFT
//
void
CallTree::computeValue( list_of_metrics& mv, TreeItem* item )
{
    if ( item->isAggregatedRootItem() && item->isExpanded() )
    {
        list_of_sysresources empty;
        computeValueHiddenIterationsFirstLevel( mv, empty, item ); // compute root
        computeValueHiddenIterations( mv, empty, item );
        return;
    }

    cube::Cnode* cnode = ( static_cast<cube::Cnode* > ( item->getCubeObject() ) );

    item->setValues( cube->get_sev_adv( mv, cnode, cube::CUBE_CALCULATE_INCLUSIVE ),
                     cube->get_sev_adv( mv, cnode, cube::CUBE_CALCULATE_EXCLUSIVE ) );

    if ( item->isExpanded() )
    {
        foreach( TreeItem * child, item->getChildren() )
        {
            computeValue( mv, child );
        }
    }
}

// compute the current value for CallTree item
// if the calltree is the last one
//
void
CallTree::computeValue( list_of_metrics& mv, list_of_sysresources& sv, TreeItem* item )
{
    if ( item->isExpanded() && item->isAggregatedRootItem() )
    {
        computeValueHiddenIterations( mv, sv, item );
        return;
    }

    cube::Cnode* cnode = ( static_cast<cube::Cnode* > ( item->getCubeObject() ) );

    item->setValues( cube->get_sev_adv( mv, sv, cnode, cube::CUBE_CALCULATE_INCLUSIVE ),
                     cube->get_sev_adv( mv, sv, cnode, cube::CUBE_CALCULATE_EXCLUSIVE ) );

    if ( item->isExpanded() )
    {
        foreach( TreeItem * child, item->getChildren() )
        {
            computeValue( mv, sv, child );
        }
    }
}

/**
 * @brief CallTree::computeValueHiddenIterationsFirstLevel
 * If iterations are merged, the tree level with the iteration items is removed. The iterations items
 * may have exclusive values, which have to be added to the exclusive value of its parent.
 */
void
CallTree::computeValueHiddenIterationsFirstLevel( list_of_metrics& mv, list_of_sysresources& sv, TreeItem* item )
{
    cube::Cnode*   cnode = static_cast<cube::Cnode* > ( item->getCubeObject() );
    list_of_cnodes cnodesExcl;

    // add root item to cnode list
    cnode_pair cp;
    cp.first  = cnode;
    cp.second = CUBE_CALCULATE_EXCLUSIVE;
    cnodesExcl.push_back( cp );

    // add iterations (first loop level) sto cnode list
    foreach( TreeItem * iteration, loopRoot->getChildren() )
    {
        cp.first = static_cast<cube::Cnode* > ( iteration->getCubeObject() );
        cnodesExcl.push_back( cp );
    }

    if ( sv.size() == 0 ) // METRIC tree is on left
    {
        cube::Value* total = cube->get_sev_adv( mv, cnode, cube::CUBE_CALCULATE_INCLUSIVE );
        cube::Value* own   = cube->get_sev_adv( mv, cnodesExcl );
        item->setValues( total, own );
    }
    else // CALL tree is last one
    {
        cube::Value* total = cube->get_sev_adv( mv, sv, cnode, cube::CUBE_CALCULATE_INCLUSIVE );
        cube::Value* own   = cube->get_sev_adv( mv, cnodesExcl, sv );
        item->setValues( total, own );
    }
}

/**
 * @brief CallTree::computeValueHiddenIterations calculates values for all children of a subtree with hidden iterations.
 * For each item to calculate, the corresponding items of the hidden iterations calculated.
 * @param item TreeItem whose children will contain the merged values of all corresponding iterations
 */
void
CallTree::computeValueHiddenIterations( list_of_metrics& mv, list_of_sysresources& sv, TreeItem* item )
{
    // calculate values for all visible elements in "hide iterations" mode
    foreach( TreeItem * citem, item->getChildren() )
    {
        assert( citem->isAggregatedLoopItem() );
        AggregatedTreeItem* compactChild = static_cast<AggregatedTreeItem*>( citem );
        list_of_cnodes      cnodesIncl;
        list_of_cnodes      cnodesExcl;

        // push all iterations of the current item into calculation list
        foreach( cube::Cnode * cnode, compactChild->getIterations() )
        {
            cnode_pair cp;
            cp.first  = cnode;
            cp.second = cube::CUBE_CALCULATE_INCLUSIVE;
            cnodesIncl.push_back( cp );
            cp.second = CUBE_CALCULATE_EXCLUSIVE;
            cnodesExcl.push_back( cp );
        }

        if ( sv.size() == 0 ) // METRIC tree is on left
        {
            cube::Value* total = cube->get_sev_adv( mv, cnodesIncl );
            cube::Value* own   = cube->get_sev_adv( mv, cnodesExcl );
            compactChild->setValues( total, own );
        }
        else // CALL tree is last one
        {
            cube::Value* total = cube->get_sev_adv( mv, cnodesIncl, sv );
            cube::Value* own   = cube->get_sev_adv( mv, cnodesExcl, sv );
            compactChild->setValues( total, own );
        }

        // also calculate values for all expanded children of item
        if ( compactChild->isExpanded() )
        {
            computeValueHiddenIterations( mv, sv, compactChild );
        }
    }
}

/**
 * @brief getSelectedFromHiddenIterations returns a list of items of the hidden iterations which correspond
 * to the selected merged item
 * @param iterations a list of hidden iterations, starting with tree level after root
 * @param selected path to the selected item, contains the names of each ancestor of the selected item,
 * starting with the level after root
 * @return list with the selected item for each iteration where an item with the same name at the same tree
 * level is found
 */
QList<TreeItem*>
getSelectedFromHiddenIterations( const QList<TreeItem*>& iterations, QStringList& selected )
{
    QList<TreeItem*> nextLevel;
    QList<TreeItem*> ret;

    QString selectedName = selected.takeFirst();
    foreach( TreeItem * iter, iterations )
    {
        foreach( TreeItem * child, iter->getChildren() )
        {
            if ( child->getName() == selectedName ) // found selected item in different iteration
            {
                nextLevel.push_back( child );
            }
        }
    }
    if ( !selected.empty() )
    {
        ret = getSelectedFromHiddenIterations( nextLevel, selected );
    }
    else
    {
        ret = nextLevel;
    }
    return ret;
}

// compute all absolute values for this tree's visible items
//
void
CallTree::computeBasicValues( const QList<Tree*>& leftTrees, TreeItem* top )
{
    if ( !top )
    {
        top = this->top;
    }

    // set all values of all items to 0.0
    initializeValues();

    if ( leftTrees.size() == 0 )
    {
        return;                       // situation  Calltree, XXX, XXX -> Calltree doesn't show anything
    }
    else if ( leftTrees.size() == 1 ) // call tree is in the middle
    {
        if ( leftTrees[ 0 ]->getTreeType() == SYSTEMTREE )
        {
            return;  // situation  System, Calltree, Metric -> calltree doesn't show anything
        }
        else // situation  Metric, Calltree, System -> standard
        {
            list_of_metrics mv;

            Tree*                   metricTree  = leftTrees[ 0 ];
            const QList<TreeItem*>& sel_metrics = metricTree->getSelectionList();

            foreach( TreeItem * item, sel_metrics )
            {
                metric_pair mp;
                mp.first  = static_cast<cube::Metric*> ( item->getCubeObject() );
                mp.second = ( item->isExpanded() ) ? CUBE_CALCULATE_EXCLUSIVE : CUBE_CALCULATE_INCLUSIVE;
                if ( item->getChildren().size() == 0 )
                {
                    mp.second = CUBE_CALCULATE_INCLUSIVE;
                }
                mv.push_back( mp );
            }
            foreach( TreeItem * child, top->getChildren() )
            {
                computeValue( mv, child );
            }
        }
    }
    else if ( leftTrees.size() == 2 )  // situation System, Metric , Calltree or situation Metric, System, Calltree
    {
        Tree* systemTree = ( leftTrees[ 0 ]->getTreeType() == SYSTEMTREE ) ? leftTrees[ 0 ] : leftTrees[ 1 ];
        Tree* metricTree = ( leftTrees[ 0 ]->getTreeType() != SYSTEMTREE ) ? leftTrees[ 0 ] : leftTrees[ 1 ];

        list_of_metrics      mv;
        list_of_sysresources sv;

        QList<TreeItem*> sel_metrics = metricTree->getSelectionList();
        foreach( TreeItem * item, sel_metrics )
        {
            metric_pair mp;
            mp.first  = static_cast<cube::Metric*> ( item->getCubeObject() );
            mp.second = ( item->isExpanded() ) ? cube::CUBE_CALCULATE_EXCLUSIVE : cube::CUBE_CALCULATE_INCLUSIVE;
            if ( item->getChildren().size() == 0 )
            {
                mp.second = CUBE_CALCULATE_INCLUSIVE;
            }
            mv.push_back( mp );
        }
        QList<TreeItem*> sel_systemnodes = systemTree->getSelectionList();
        foreach( TreeItem * item, sel_systemnodes )
        {
            sysres_pair sp;
            sp.first  = static_cast<cube::Sysres*> ( item->getCubeObject() );
            sp.second = ( item->isExpanded() ) ? cube::CUBE_CALCULATE_EXCLUSIVE : cube::CUBE_CALCULATE_INCLUSIVE;
            sv.push_back( sp );
        }

        foreach( TreeItem * child, top->getChildren() )
        {
            computeValue( mv, sv, child );
        }
    }
}
// -----------------------------------------------------------------
// ---------------- end calculation functions ----------------------
// -----------------------------------------------------------------


TreeItem*
CallTree::getTreeItem( uint32_t cnodeId )
{
    foreach( TreeItem * item, treeItems )
    {
        cube::Cnode* cnode = static_cast<cube::Cnode*> ( item->getCubeObject() );
        if ( cnode->get_id() == cnodeId )
        {
            return item;
        }
    }
    return 0;
}


/** sets current item as call tree root item
 */
void
CallTree::setItemAsRoot( TreeItem* newRoot )
{
    lastSelection = newRoot;
    selectionList.clear();
    selectionList.append( lastSelection );

    // set item properties according to root position
    cube->reroot_cnode( static_cast<cube::Cnode* > ( newRoot->getCubeObject() ) );
    newRoot->setDepth( 0 );

    TreeItem* oldRoot = top->getChildren().first();

    treeModel->replaceSubtree( oldRoot, newRoot );

    deleteItem( oldRoot );
}

/** removes the given item from the tree
 */
void
CallTree::pruneItem( TreeItem* item )
{
    cube::Cnode* cnode =  static_cast<cube::Cnode* > ( item->getCubeObject() );
    treeModel->removeSubtree( item );
    cube->prune_cnode( cnode );
}

/** removes the children of the item from the tree
 */
void
CallTree::setItemAsLeaf( TreeItem* item )
{
    cube::Cnode* cnode =  static_cast<cube::Cnode* > ( item->getCubeObject() );
    cube->set_cnode_as_leaf( cnode );
    treeModel->removeChildren( item );
}
