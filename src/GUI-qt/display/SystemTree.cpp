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


#include "SystemTree.h"
#include "CallTree.h"
#include "TreeView.h"
#include <QDebug>

using namespace cube;
using namespace std;

SystemTree::SystemTree()
{
    treeType = SYSTEMTREE;
}

QString
SystemTree::getLabel()
{
    return "System tree";
}

/**
 * creates a system tree
 * The first level of the system tree consists of items of type SYSTEMTREENODEITEM. The children
 * of this level can be also SYSTEMTREENODEITEM.
 * The last two levels of the tree are items of type LOCATIONGROUPITEM which have children of
 * type LOCATION.
 */
TreeItem*
SystemTree::createTree()
{
    TreeItem*                  top = new TreeItem( QString(), SYSTEMTREENODEITEM, 0 );
    std::vector<cube::Vertex*> vec;

    // insert root systen tree nodes and its children recursively
    createItems( top, cube->get_root_stnv(), SYSTEMTREENODEITEM );

    // insert LOCATIONGROUP items but not its children of type LOCATION
    QList<TreeItem*> groups;
    createItems( top, cube->get_location_groupv(), LOCATIONGROUPITEM, &groups, false );

    // insert LOCATION items
    createItems( top, cube->get_locationv(), LOCATIONITEM );

    // for single-threaded system trees : hide the threads
    bool singleThreaded = true;
    foreach( TreeItem * group, groups ) // for all items of type LOCATIONGROUPITEM
    {
        if ( group->getChildren().size() > 1 )
        {
            singleThreaded = false;
            break;
        }
    }
    if ( singleThreaded )
    {
        foreach( TreeItem * group, groups )
        {
            if ( group->getChildren().size() == 1 )
            {
                TreeItem* child = group->child( 0 );
                group->removeChild( child );
                treeItemHash.remove( child->getCubeObject() );
            }
        }
    }
    // end handling of single-threaded trees

    return top;
}

QString
SystemTree::getItemName( cube::Vertex* vertex )
{
    return QString( ( ( cube::Sysres* )vertex )->get_name().c_str() );
}









// -----------------------------------------------------------------
// ---------------- calculation functions --------------------------
// -----------------------------------------------------------------


void
SystemTree::computeValue( list_of_metrics& mv,  TreeItem* item )
{
    cube::Sysres* sysres = static_cast<cube::Sysres*> ( item->getCubeObject() );

    item->setValues( cube->get_sev_adv( mv, sysres, cube::CUBE_CALCULATE_INCLUSIVE ),
                     cube->get_sev_adv( mv, sysres, cube::CUBE_CALCULATE_EXCLUSIVE ) );


    foreach( TreeItem * child, item->getChildren() )
    {
        computeValue( mv, child );
    }
}

// special case:
// compute the current value for a Systemtree widget item
// if the systen tree is the last panel
//
void
SystemTree::computeValue( list_of_metrics& mv, list_of_cnodes& cv,  TreeItem* item )
{
    cube::Sysres* sysres = static_cast<cube::Sysres*> ( item->getCubeObject() );

    Value* inclusive = cube->get_sev_adv( mv, cv, sysres, cube::CUBE_CALCULATE_INCLUSIVE );
    Value* exclusive = 0;
    if ( item->getChildren().size() == 0 ) // optimization: leafs only have one value
    {
        exclusive = inclusive->copy();
    }
    else
    {
        exclusive = cube->get_sev_adv( mv, cv, sysres, cube::CUBE_CALCULATE_EXCLUSIVE );
    }

    item->setValues( inclusive, exclusive );

    // item->setValues( cube->get_sev_adv( mv, cv, sysres, cube::CUBE_CALCULATE_INCLUSIVE ),
    //                 cube->get_sev_adv( mv, cv, sysres, cube::CUBE_CALCULATE_EXCLUSIVE ) );


    foreach( TreeItem * child, item->getChildren() )
    {
        computeValue( mv, cv, child );
    }
}


// special case:
// compute the current value for a Systemtree widget item
// if the systen tree is in the center
//
void
SystemTree::computeValue( list_of_metrics& mv, list_of_regions& rv,  TreeItem* item )
{
    cube::Sysres* sysres = static_cast<cube::Sysres*> ( item->getCubeObject() );

    item->setValues( cube->get_sev_adv( mv, rv, sysres, cube::CUBE_CALCULATE_INCLUSIVE ),
                     cube->get_sev_adv( mv, rv, sysres, cube::CUBE_CALCULATE_EXCLUSIVE ) );

    foreach( TreeItem * child, item->getChildren() )
    {
        computeValue( mv, rv, child );
    }
}



// compute all absolute values for this tree's items
//
void
SystemTree::computeBasicValues( const QList<Tree*>& leftTrees, TreeItem* top )
{
    list_of_regions      rv;
    list_of_cnodes       cv;
    list_of_sysresources sv;
    list_of_metrics      mv;

    if ( !top )
    {
        top = this->top;
    }

    bool unset_regions = false; // /  marks, that some regions got status "subroutines" and  have to be reset after calculation;
    // set all values of all items to 0.0
    initializeValues();


    if ( leftTrees.size() == 0 )
    {
        return;                       // situation, systemTree, XXX, XXX -> Systemtree doesn't show anything
    }
    else if ( leftTrees.size() == 1 ) // systemTree is in the middle
    {
        if ( ( leftTrees[ 0 ]->getTreeType() == CALLTREE ) || ( leftTrees[ 0 ]->getTreeType() == CALLFLAT ) )
        {
            return;                                             // situation, Calltree, System, Metric -> systemtree doesnt show anything
        }
        else if ( leftTrees[ 0 ]->getTreeType() == METRICTREE ) // situation  Metric, System, Calltree -> standard
        {
            Tree*                   metricTree  = leftTrees[ 0 ];
            const QList<TreeItem*>& sel_metrics = metricTree->getSelectionList();

            foreach( TreeItem * sel, sel_metrics )
            {
                metric_pair mp;
                mp.first  = static_cast<cube::Metric*> ( sel->getCubeObject() );
                mp.second = ( sel->isExpanded() ) ? cube::CUBE_CALCULATE_EXCLUSIVE : cube::CUBE_CALCULATE_INCLUSIVE;
                mv.push_back( mp );
            }

            foreach( TreeItem * child, top->getChildren() )
            {
                computeValue( mv, child );
            }
        }
    }
    else if ( leftTrees.size() == 2 ) // situation  Metric, Calltree, System  or Calltree, Metrictree, System
    {
        Tree* metricTree = ( leftTrees[ 0 ]->getTreeType() == METRICTREE ) ? leftTrees[ 0 ] : leftTrees[ 1 ];
        Tree* callTree   = ( leftTrees[ 0 ]->getTreeType() != METRICTREE ) ? leftTrees[ 0 ] : leftTrees[ 1 ];

        const QList<TreeItem*>& sel_metrics = metricTree->getSelectionList();
        foreach( TreeItem * sel, sel_metrics )
        {
            metric_pair mp;
            mp.first  = static_cast<cube::Metric*> ( sel->getCubeObject() );
            mp.second = ( sel->isExpanded() ) ? cube::CUBE_CALCULATE_EXCLUSIVE : cube::CUBE_CALCULATE_INCLUSIVE;
            mv.push_back( mp );
        }

        if ( callTree->getTreeType() == CALLFLAT )
        {
            QList<TreeItem*> sel_regions = callTree->getSelectionList();
            foreach( TreeItem * selected, sel_regions )
            {
                region_pair rp;
                rp.first =  static_cast<cube::Region*> ( selected->getCubeObject() );
                if ( rp.first == NULL )
                {
                    unset_regions = true;
                    cube::Region* reg = static_cast<cube::Region*> ( selected->getParent()->getCubeObject() );
                    reg->set_as_subroutines();
                    rp.first = reg;
                }
                rp.second = ( selected->isExpanded() ) ? cube::CUBE_CALCULATE_EXCLUSIVE : cube::CUBE_CALCULATE_INCLUSIVE;
                rv.push_back( rp );
            }
            foreach( TreeItem * child, top->getChildren() )
            {
                computeValue( mv, rv, child );
            }
        }
        else
        {                                  // use selected values of CALLTREE
            const QList<TreeItem*>& sel_call = callTree->getSelectionList();
            fillCnodeList( sel_call, cv ); // also handles aggregated loops

            foreach( TreeItem * child, top->getChildren() )
            {
                computeValue( mv, cv, child );
            }
        } // active call tree
    }     // end SystemTab is last tab


    if ( unset_regions )
    {
        for ( list_of_regions::iterator riter = rv.begin(); riter != rv.end(); ++riter )
        {
            ( *riter ).first->unset_as_subroutines();
        }
    }




    // calculate minimum and maximum value for each level of the tree for peer distribution value mode
    {
        double         tmp = 0.0;
        vector<double> min_vector;
        vector<double> max_vector;
        vector<bool>   depth_seen;
        foreach( TreeItem * item, getItems() )
        {
            size_t depth = item->getDepth();

            if ( min_vector.size() <= depth )
            {
                min_vector.resize( depth + 1 );
            }
            if ( max_vector.size() <= depth )
            {
                max_vector.resize( depth + 1 );
            }
            while ( depth_seen.size() <= depth )
            {
                depth_seen.push_back( false );
            }

            tmp = item->totalValue;

            if ( !depth_seen[ depth ] )
            {
                depth_seen[ depth ] = true;
                min_vector[ depth ] = tmp;
                max_vector[ depth ] = tmp;
            }
            else
            {
                min_vector[ depth ] = ( min_vector[ depth ] > tmp ) ? tmp : min_vector[ depth ];
                max_vector[ depth ] = ( max_vector[ depth ] < tmp ) ? tmp : max_vector[ depth ];
            }
        }

        foreach( TreeItem * item, getItems() )
        {
            int depth = item->getDepth();
            item->minValue = min_vector[ depth ];
            item->maxValue = max_vector[ depth ];
        }
    }
}
// end of computeBasicValues()

/**
 * adds the items of the given QList to list_of_cnodes. If the items are aggregated loop entries,
 * the nodes from whom the items are aggregated are added.
 */
void
SystemTree::fillCnodeList( QList<TreeItem*> items, list_of_cnodes& cv )
{
    foreach( TreeItem * treeItem, items )
    {
        cnode_pair cp;
        cp.second = ( treeItem->isExpanded() ) ? cube::CUBE_CALCULATE_EXCLUSIVE : cube::CUBE_CALCULATE_INCLUSIVE;

        if ( treeItem->isAggregatedRootItem() && treeItem->isExpanded() )
        {
            cp.first = static_cast<cube::Cnode*> ( treeItem->getCubeObject() );
            cv.push_back( cp );

            // add exclusive values of all hidden children of the root loop item
            cp.second = CUBE_CALCULATE_EXCLUSIVE;

            CallTree* callTree = static_cast<CallTree*>( treeItem->getTree() );
            foreach( TreeItem * iteration, callTree->getLoopRootItem()->getChildren() )
            {
                cp.first = static_cast<cube::Cnode*> ( iteration->cubeObject );
                cv.push_back( cp );
            }
        }
        else if ( !treeItem->isAggregatedLoopItem() ) // single item or collapsed root loop item
        {
            cp.first = static_cast<cube::Cnode*> ( treeItem->getCubeObject() );
            cv.push_back( cp );
        }
        else // aggregated loop item
        {
            AggregatedTreeItem* item = static_cast<AggregatedTreeItem*>( treeItem );
            foreach( Cnode * cnode, item->getIterations() )
            {
                cp.first = cnode;
                cv.push_back( cp );
            }
        }
    }
}

/**
 * @brief compute the value of one element
 */
double
SystemTree::computeValue( TreeItem* metricItem, cube::CalculationFlavour metricState,
                          TreeItem* callItem, cube::CalculationFlavour callState,
                          TreeItem* systemItem, cube::CalculationFlavour systemState )
{
    double value = 0.;

    list_of_regions      rv;
    list_of_cnodes       cv;
    list_of_sysresources sv;
    list_of_metrics      mv;

    // situation  Metric, Calltree, System  or Calltree, Metrictree, System
    {
        metric_pair mp;
        mp.first  = static_cast<cube::Metric*> ( metricItem->getCubeObject() );
        mp.second = metricState;
        mv.push_back( mp );

        if ( callItem->type == REGIONITEM )
        {
            region_pair rp;
            rp.first  = static_cast<cube::Region*> ( callItem->getCubeObject() );
            rp.second = callState;
            rv.push_back( rp );
        }
        else
        {   // call tree item
            if ( callItem->isAggregatedLoopItem() || ( callItem->isAggregatedRootItem() && callItem->isExpanded() ) )
            {
                QList<TreeItem*> callList;
                callList.append( callItem );
                fillCnodeList( callList, cv ); // also handles aggregated loops
            }
            else
            {
                cnode_pair cp;
                cp.first  = static_cast<cube::Cnode*> ( callItem->getCubeObject() );
                cp.second = callState;
                cv.push_back( cp );
            }
            value = cube->get_sev( mv, cv, ( cube::Sysres* )systemItem->cubeObject, systemState );
        }
    }
    return value;
}

/**
 * @brief getVisitedItems
 * @return all threads with visited > 0
 */
QList<TreeItem*>
SystemTree::getVisitedItems()
{
    QList<TreeItem*> visited;

    Tree* metricTree = treeHash.value( METRICTREE );
    Tree* callTree   = treeHash.value( CALLTREE );
    if ( !callTree->isActive() )
    {
        callTree = treeHash.value( CALLFLAT );
    }

    TreeItem* visitedMetric = 0;
    foreach( TreeItem * item, metricTree->getItems() )
    {
        if ( item->getName() == "Visits" )
        {
            visitedMetric = item;
            break;
        }
    }

    double val = 0;
    foreach( TreeItem * item, getItems() )
    {
        if ( ( item->type == LOCATIONGROUPITEM && item->getChildren().size() == 0 )
             || ( item->type == LOCATIONITEM ) )
        {
            foreach( TreeItem * callItem, callTree->getSelectionList() )
            {
                cube::CalculationFlavour callTreeState;
                if ( !callItem->isExpanded() )
                {
                    callTreeState =  cube::CUBE_CALCULATE_INCLUSIVE;
                }
                else
                {
                    callTreeState = CUBE_CALCULATE_EXCLUSIVE;
                }

                val = this->computeValue( visitedMetric, cube::CUBE_CALCULATE_INCLUSIVE,
                                          callItem, callTreeState,
                                          item, cube::CUBE_CALCULATE_INCLUSIVE );
                if ( val > 0 )
                {
                    break; // at least one visit
                }
            }
            if ( val > 0 )
            {
                visited.append( item );
            }
        }
    }
    return visited;
}


TreeItem*
SystemTree::getTreeItem( uint32_t systemId )
{
    foreach( TreeItem * item, treeItems )
    {
        cube::Sysres* sysres = static_cast<cube::Sysres*> ( item->getCubeObject() );
        if ( sysres->get_sys_id() == systemId )
        {
            return item;
        }
    }
    return 0;
}
