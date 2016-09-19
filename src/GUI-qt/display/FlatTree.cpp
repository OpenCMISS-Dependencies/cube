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


#include "FlatTree.h"
#include "CubeMetric.h"
#include "CubeRegion.h"
#include "TreeView.h"

using namespace cube;

FlatTree::FlatTree()
{
    treeType = CALLFLAT;
}

QString
FlatTree::getLabel()
{
    return "Flat view";
}

QString
FlatTree::getItemName( cube::Vertex* vertex )
{
    return QString( ( ( cube::Region* )vertex )->get_name().c_str() );
}

/**
 * @brief FlatTree::createTree
 * creates a flat tree that has only one level of items.
 */
TreeItem*
FlatTree::createTree()
{
    TreeItem* top = new TreeItem( QString(), REGIONITEM, 0 );

    createItems<cube::Region>( top, cube->get_regv(), REGIONITEM );

    // create a Subroutine subitem, if the item has any callees
    foreach( TreeItem * item, top->getChildren() )
    {
        cube::Vertex* region = item->getCubeObject();
        if ( hasCallee( region ) )
        {
            TreeItem* child = new TreeItem( "Subroutines", REGIONITEM, 0 );
            item->addChild( child );
            treeItemHash.insertMulti( 0, child );
        }
    }

    return top;
}

/** Returns true, if the region parameter has any callee in the call tree.
    Called to determine a region item should have "subroutines" child item
 */
bool
FlatTree::hasCallee( cube::Vertex* region )
{
    const std::vector<cube::Cnode*>& cnodeVector = cube->get_cnodev();
    for ( unsigned i = 0; i < cnodeVector.size(); i++ )
    {
        if ( cnodeVector[ i ] != NULL )
        {
            if ( cnodeVector[ i ]->get_caller() == region )
            {
                return true;
            }
        }
    }
    return false;
}



// -----------------------------------------------------------------
// ---------------- calculation functions --------------------------
// -----------------------------------------------------------------



// special case:
// compute the current value for a METRIC tree widget item
// if the metric tree is on LEFT
//
void
FlatTree::computeValue( list_of_metrics& mv, TreeItem* item )
{
    cube::Region* region = static_cast<cube::Region*> ( item->getCubeObject() );
    if ( region == NULL )
    {
        region = static_cast<cube::Region*> ( item->getParent()->getCubeObject() );
        region->set_as_subroutines();
    }

    item->setValues( cube->get_sev_adv( mv, region, cube::CUBE_CALCULATE_INCLUSIVE ),
                     cube->get_sev_adv( mv, region, cube::CUBE_CALCULATE_EXCLUSIVE ) );

    region->unset_as_subroutines();

    if ( item->isExpanded() )
    {
        foreach( TreeItem * child, item->getChildren() )
        {
            computeValue( mv, child );
        }
    }
}


// special case:
// compute the current value for calltreewidget item
// if the calltree is the last one
//
void
FlatTree::computeValue( list_of_metrics& mv, list_of_sysresources& sv, TreeItem* item )
{
    cube::Region* region = static_cast<cube::Region*> ( item->getCubeObject() );
    if ( region == NULL )
    {
        region = static_cast<cube::Region*> ( item->getParent()->getCubeObject() );
        region->set_as_subroutines();
    }

    item->setValues( cube->get_sev_adv( mv, sv, region, cube::CUBE_CALCULATE_INCLUSIVE ),
                     cube->get_sev_adv( mv, sv, region, cube::CUBE_CALCULATE_EXCLUSIVE ) );

    region->unset_as_subroutines();

    if ( item->isExpanded() )
    {
        foreach( TreeItem * child, item->getChildren() )
        {
            computeValue( mv, sv, child );
        }
    }
}


// compute all absolute values for this tree's items
//
void
FlatTree::computeBasicValues( const QList<Tree*>& leftTrees, TreeItem* top )
{
    if ( !top )
    {
        top = this->top;
    }

    // set all values of all items to 0.0
    initializeValues();

    if ( leftTrees.size() == 0 )
    {
        return;                       // situation Calltree, XXX, XXX -> Calltree doesn't show anything
    }
    else if ( leftTrees.size() == 1 ) // CallTree is in the middle
    {
        if ( leftTrees[ 0 ]->getTreeType() == SYSTEMTREE )
        {
            return; // situation System, Calltree, Metric -> calltree doesn't show anything
        }
        else
        {
            list_of_metrics         mv;
            Tree*                   metricTree  = leftTrees[ 0 ];
            const QList<TreeItem*>& sel_metrics = metricTree->getSelectionList();

            foreach( TreeItem * item, sel_metrics )
            {
                metric_pair mp;
                mp.first  = static_cast<cube::Metric*> ( item->getCubeObject() );
                mp.second = ( item->isExpanded() ) ? CUBE_CALCULATE_EXCLUSIVE : CUBE_CALCULATE_INCLUSIVE;
                mv.push_back( mp );
            }
            foreach( TreeItem * child, top->getChildren() )
            {
                computeValue( mv, child );
            }
        }
    }
    else if ( leftTrees.size() == 2 )  // situation System, Metric, Flattree or situation Metric, System, Flattree
    {
        list_of_metrics      mv;
        list_of_sysresources sv;

        Tree* systemTree = ( leftTrees[ 0 ]->getTreeType() == SYSTEMTREE ) ? leftTrees[ 0 ] : leftTrees[ 1 ];
        Tree* metricTree = ( leftTrees[ 0 ]->getTreeType() != SYSTEMTREE ) ? leftTrees[ 0 ] : leftTrees[ 1 ];

        const QList<TreeItem*>& sel_metrics = metricTree->getSelectionList();

        foreach( TreeItem * item, sel_metrics )
        {
            metric_pair mp;
            mp.first  = static_cast<cube::Metric*> ( item->getCubeObject() );
            mp.second = ( item->isExpanded() ) ? CUBE_CALCULATE_EXCLUSIVE : CUBE_CALCULATE_INCLUSIVE;
            mv.push_back( mp );
        }

        const QList<TreeItem*>& sel_systemnodes = systemTree->getSelectionList();
        foreach( TreeItem * item, sel_systemnodes )
        {
            sysres_pair sp;
            sp.first  = static_cast<cube::Sysres*> ( item->getCubeObject() );
            sp.second = ( item->isExpanded() ) ? CUBE_CALCULATE_EXCLUSIVE : CUBE_CALCULATE_INCLUSIVE;
            sv.push_back( sp );
        }
        foreach( TreeItem * child, top->getChildren() )
        {
            computeValue( mv, sv, child );
        }
    }
}
