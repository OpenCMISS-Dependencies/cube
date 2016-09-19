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



#include <cmath>


#include "CubeRegion.h"


#include "MetricTree.h"
#include "Globals.h"
#include "TreeView.h"
#include "AggregatedTreeItem.h"

using namespace std;
using namespace cube;

MetricTree::MetricTree()
{
    treeType = METRICTREE;
}

QString
MetricTree::getLabel()
{
    if ( !cube )
    {
        return "not yet initialized";
    }
    return ( cube->get_metrics_title() != "" ) ? cube->get_metrics_title().c_str() : "Metric tree";
}

TreeItem*
MetricTree::createTree()
{
    TreeItem* top = new TreeItem( QString(), METRICITEM, 0 );

    createItems<cube::Metric>( top, cube->get_root_metv(), METRICITEM );

    return top;
}

/** return false, if the item is invalid and should not be added to the tree */
bool
MetricTree::itemIsValid( cube::Vertex* vertex )
{
    // if the metric item is VOID, it will not inserted into the tree
    cube::Metric* metric     = ( cube::Metric* )vertex;
    bool          voidValued = false;
    while ( true )
    {
        if ( std::strcmp( metric->get_val().c_str(), "VOID" ) == 0 )
        {
            voidValued = true;
            break;
        }
        metric = metric->get_parent();
        if ( metric == NULL )
        {
            break;
        }
    }
    return !voidValued;
}

QString
MetricTree::getItemName( cube::Vertex* vertex )
{
    return QString( ( ( cube::Metric* )vertex )->get_disp_name().c_str() );
}

double
MetricTree::getMaxValue( TreeItem* item ) const
{
    assert( maxValueIndex.size() > 0 );
    return maxValues.at( maxValueIndex.value( item ) );
}

// -----------------------------------------------------------------
// ---------------- calculation functions --------------------------
// -----------------------------------------------------------------


// special case:
// compute the current value for a METRIC tree widget item
// if the metric tree is on LEFT
//
void
MetricTree::computeValue( TreeItem* item )
{
    cube::Metric* metric = static_cast<cube::Metric*>( item->cubeObject );

    item->setValues( cube->get_sev_adv( metric, cube::CUBE_CALCULATE_INCLUSIVE ),
                     cube->get_sev_adv( metric, cube::CUBE_CALCULATE_EXCLUSIVE ) );

    if ( item->isExpanded() )
    {
        foreach( TreeItem * child, item->getChildren() )
        {
            computeValue( child );
        }
    }
}

// special case:
// compute the current value for a METRIC tree widget item
// if the metric tree is on center and calltree is on left
//
void
MetricTree::computeValue( list_of_cnodes& cv,  TreeItem* item )
{
    cube::Metric* metric = static_cast<cube::Metric*>( item->cubeObject );

    item->setValues( cube->get_sev_adv( cv, metric, cube::CUBE_CALCULATE_INCLUSIVE ),
                     cube->get_sev_adv( cv, metric, cube::CUBE_CALCULATE_EXCLUSIVE )
                     );

    if ( item->isExpanded() )
    {
        foreach( TreeItem * child, item->getChildren() )
        {
            computeValue( cv, child );
        }
    }
}

// special case:
// compute the current value for a METRIC tree widget item
// if the metric tree is on center and calltree is on left
//
void
MetricTree::computeValue( list_of_regions& rv,  TreeItem* item )
{
    cube::Metric* metric = static_cast<cube::Metric*>( item->cubeObject );

    item->setValues( cube->get_sev_adv( rv, metric, cube::CUBE_CALCULATE_INCLUSIVE ),
                     cube->get_sev_adv( rv, metric, cube::CUBE_CALCULATE_EXCLUSIVE )
                     );

    if ( item->isExpanded() )
    {
        foreach( TreeItem * child, item->getChildren() )
        {
            computeValue( rv, child );
        }
    }
}


// special case:
// compute the current value for a METRIC tree widget item
// if the metric tree is on center and calltree is on left
//
void
MetricTree::computeValue( list_of_sysresources& sv,  TreeItem* item )
{
    cube::Metric* metric = static_cast<cube::Metric*>( item->cubeObject );

    item->setValues( cube->get_sev_adv( sv, metric, cube::CUBE_CALCULATE_INCLUSIVE ),
                     cube->get_sev_adv( sv, metric, cube::CUBE_CALCULATE_EXCLUSIVE )
                     );

    if ( item->isExpanded() )
    {
        foreach( TreeItem * child, item->getChildren() )
        {
            computeValue( sv, child );
        }
    }
}


// special case:
// compute the current value for a METRIC tree widget item
// if the metric tree is on center and calltree is on left
//
void
MetricTree::computeValue( list_of_regions& rv, list_of_sysresources& sv,  TreeItem* item )
{
    cube::Metric* metric = static_cast<cube::Metric*> ( item->cubeObject );

    item->setValues( cube->get_sev_adv( rv, sv, metric, cube::CUBE_CALCULATE_INCLUSIVE ),
                     cube->get_sev_adv( rv, sv, metric, cube::CUBE_CALCULATE_EXCLUSIVE )
                     );


    if ( item->isExpanded() )
    {
        foreach( TreeItem * child, item->getChildren() )
        {
            computeValue( sv, child );
        }
    }
}


// special case:
// compute the current value for a METRIC tree widget item
// if the metric tree is on center and calltree is on left
//
void
MetricTree::computeValue( list_of_cnodes& cv, list_of_sysresources& sv,  TreeItem* item )
{
    cube::Metric* metric = static_cast<cube::Metric*>( item->cubeObject );

    item->setValues( cube->get_sev_adv( cv, sv, metric, cube::CUBE_CALCULATE_INCLUSIVE ),
                     cube->get_sev_adv( cv, sv, metric, cube::CUBE_CALCULATE_EXCLUSIVE )
                     );

    if ( item->isExpanded() )
    {
        foreach( TreeItem * child, item->getChildren() )
        {
            computeValue( cv, sv, child );
        }
    }
}



// compute all absolute values for this tree's items
//
void
MetricTree::computeBasicValues( const QList<Tree*>& leftTrees, TreeItem* top )
{
    if ( !top )
    {
        top = this->top;
    }

    //     cout << " SDFFSDG " << endl;
    list_of_regions      rv;
    list_of_cnodes       cv;
    list_of_sysresources sv;
    list_of_metrics      mv;

    bool unset_regions = false;  // / marks, that region was set as "subroutines" and have to be unset after the calculation

    // set all values of all items to 0.0
    initializeValues();

    if ( leftTrees.size() == 0 )     // metric tree is left
    {
        QList<TreeItem*> sortedList; // sort tree items: selected items last
        foreach( TreeItem * item, top->getChildren() )
        {
            if ( !item->isSelected() )
            {
                sortedList.append( item );
            }
        }
        foreach( TreeItem * item, top->getChildren() )
        {
            if ( item->isSelected() )
            {
                sortedList.append( item );
            }
        }

        foreach( TreeItem * item, sortedList )
        {
            computeValue( item );
            if ( !item->isSelected() )
            {
                cube::Metric* metric = static_cast<cube::Metric*>( item->cubeObject );
                if ( Globals::isExperimental() )
                {
                    cube->dropAllRowsInMetric( metric );
                    qDebug() << "drop metric " << item->getLabel();
                }
            }
        }
    }
    else if ( leftTrees.size() == 1 )
    {
        if ( leftTrees[ 0 ]->getTreeType() == SYSTEMTREE  )
        {
            Tree*            systemTree      = leftTrees[ 0 ];
            QList<TreeItem*> sel_systemnodes = systemTree->getSelectionList();
            foreach( TreeItem * item, sel_systemnodes )
            {
                sysres_pair sp;
                sp.first  = static_cast<cube::Sysres*> ( item->getCubeObject() );
                sp.second = ( item->isExpanded() ) ? cube::CUBE_CALCULATE_EXCLUSIVE : cube::CUBE_CALCULATE_INCLUSIVE;
                sv.push_back( sp );
            }
            foreach( TreeItem * item, top->getChildren() )
            {
                computeValue( sv, item );
            }
        }
        else // CALLTREE or CALLFLAT
        {
            Tree* callTree = leftTrees[ 0 ];
            if ( callTree->getTreeType() == CALLFLAT )
            {
                QList<TreeItem*> selected = callTree->getSelectionList();
                foreach( TreeItem * item, selected )
                {
                    region_pair rp;
                    rp.first =   ( cube::Region* )( item->getCubeObject() );
                    if ( rp.first == NULL )
                    {
                        unset_regions = true;
                        cube::Region* reg = ( cube::Region* )( item->getParent()->getCubeObject() );
                        reg->set_as_subroutines();
                        rp.first = reg;
                    }

                    rp.second = ( item->isExpanded() ) ? cube::CUBE_CALCULATE_EXCLUSIVE : cube::CUBE_CALCULATE_INCLUSIVE;
                    rv.push_back( rp );
                }
                foreach( TreeItem * item, top->getChildren() )
                {
                    computeValue( rv, item );
                }
            }
            else // CALLTREE
            {
                QList<TreeItem*> sel_callnodes = callTree->getSelectionList();
                foreach( TreeItem * treeItem, sel_callnodes )
                {
                    cnode_pair cp;
                    cp.second = ( treeItem->isExpanded() ) ? cube::CUBE_CALCULATE_EXCLUSIVE : cube::CUBE_CALCULATE_INCLUSIVE;

                    if ( !treeItem->isAggregatedLoopItem() ) // single item
                    {
                        cp.first = ( cube::Cnode* )( treeItem->cubeObject );
                        cv.push_back( cp );
                    }
                    else // aggregated loop item
                    {
                        AggregatedTreeItem* item = static_cast<AggregatedTreeItem*>( treeItem );
                        foreach( cube::Cnode * cnode, item->getIterations() )
                        {
                            cp.first = cnode;
                            cv.push_back( cp );
                        }
                    }
                }

                foreach( TreeItem * item, top->getChildren() )
                {
                    computeValue( cv, item );
                }
            }
        }
    }
    else if ( leftTrees.size() == 2 )
    {
        Tree* systemTree = ( leftTrees[ 0 ]->getTreeType() == SYSTEMTREE ) ? leftTrees[ 0 ] : leftTrees[ 1 ];
        Tree* callTree   = ( leftTrees[ 0 ]->getTreeType() != SYSTEMTREE ) ? leftTrees[ 0 ] : leftTrees[ 1 ];

        QList<TreeItem*> sel_systemnodes = systemTree->getSelectionList();
        foreach( TreeItem * item, sel_systemnodes )
        {
            sysres_pair sp;
            sp.first  = static_cast<cube::Sysres* > ( item->getCubeObject() );
            sp.second = ( item->isExpanded() ) ? cube::CUBE_CALCULATE_EXCLUSIVE : cube::CUBE_CALCULATE_INCLUSIVE;
            sv.push_back( sp );
        }

        if ( callTree->getTreeType() == CALLFLAT )
        {
            foreach( TreeItem * item, callTree->getSelectionList() )
            {
                region_pair rp;
                rp.first =   ( cube::Region* )item->getCubeObject();
                if ( rp.first == NULL )
                {
                    unset_regions = true;
                    cube::Region* reg = ( cube::Region* )( item->getParent()->getCubeObject() );
                    reg->set_as_subroutines();
                    rp.first = reg;
                }

                rp.second = ( item->isExpanded() ) ? cube::CUBE_CALCULATE_EXCLUSIVE : cube::CUBE_CALCULATE_INCLUSIVE;
                rv.push_back( rp );
            }
            foreach( TreeItem * item, top->getChildren() )
            {
                computeValue( rv, sv, item );
            }
        }
        else // CALLTREE
        {
            QList<TreeItem*> sel_callnodes = callTree->getSelectionList();

            foreach( TreeItem * treeItem, sel_callnodes )
            {
                cnode_pair cp;
                cp.second = ( treeItem->isExpanded() ) ? cube::CUBE_CALCULATE_EXCLUSIVE : cube::CUBE_CALCULATE_INCLUSIVE;

                if ( treeItem->cubeObject ) // single item
                {
                    cp.first = ( cube::Cnode* )( treeItem->cubeObject );
                    cv.push_back( cp );
                }
                else // aggregated loop item
                {
                    AggregatedTreeItem* item = static_cast<AggregatedTreeItem*>( treeItem );
                    foreach( cube::Cnode * cnode, item->getIterations() )
                    {
                        cp.first = cnode;
                        cv.push_back( cp );
                    }
                }
            }

            foreach( TreeItem * item, top->getChildren() )
            {
                computeValue( cv, sv, item );
            }
        }
    }

    if ( unset_regions )
    {
        for ( list_of_regions::iterator riter = rv.begin(); riter != rv.end(); ++riter )
        {
            ( *riter ).first->unset_as_subroutines();
        }
    }
}

// end of computeBasicValues()

void
MetricTree::computeMaxValues()
{
    double roundThreshold = Globals::getRoundThreshold( FORMAT_TREES );
    double value;
    double maxValue = 100.0;

    maxValues.clear();
    maxValueIndex.clear();

    // for metric trees we compute the max values for each root's subtree
    QList<TreeItem*> subTree;

    for ( unsigned i = 0; i < ( unsigned )top->getChildren().size(); i++ )
    {
        subTree.clear();
        subTree.append( top->getChildren().at( i ) );
        maxValue = top->getChildren().at( i )->totalValue;

        for ( int j = 0; j < subTree.size(); j++ )
        {
            maxValueIndex.insert( subTree[ j ], i );
            foreach( TreeItem * child, subTree[ j ]->getChildren() )
            {
                subTree.append( child );
            }
            getMax( subTree[ j ], value );
            // we save max absolute value (for the case of differences of cube)
            if ( fabs( maxValue ) < fabs( value ) )
            {
                maxValue = value;
            }
        }

        if ( fabs( maxValue ) <= fabs( roundThreshold ) )
        {
            maxValue = 0.0;
        }
        maxValues.append( maxValue );
    }
} // end of computeMaxValues()


TreeItem*
MetricTree::getTreeItem( std::string metricId )
{
    foreach( TreeItem * item, this->getItems() )
    {
        cube::Metric* metric = static_cast<cube::Metric*>( item->cubeObject );
        if ( metric->get_uniq_name() == metricId )
        {
            return item;
        }
    }
    return 0;
}





// return the name if the ith external metric item
//
double
MetricTree::getExternalReferenceValue( const QString& name )
{
    return externalMetricValue.value( name, 0.0 );
}


// compute the needed external values from the "cubeExternal"
// database for the external percentage value modus
//
void
MetricTree::computeExternalReferenceValues( cube::Cube* cubeExternal )
{
    externalMetricValue.clear();

    const std::vector<cube::Metric*>& metricVec = cubeExternal->get_metv();
    const std::vector<cube::Cnode*>&  callVec   = cubeExternal->get_cnodev();
    const std::vector<cube::Thread*>& threadVec = cubeExternal->get_thrdv();

    // calculate inclusive values for all metric root items
    for ( int i = 0; i < ( int )metricVec.size(); i++ )
    {
        // take the next metric item
        cube::Metric* metric     = metricVec[ i ];
        QString       metricName = QString::fromStdString( metric->get_uniq_name() );

        // compute external metric values
        double totalValue = 0.0;
        for ( int j = 0; j < ( int )callVec.size(); j++ )
        {
            for ( int k = 0; k < ( int )threadVec.size(); k++ )
            {
                totalValue += cube->get_sev( metric, callVec[ j ], threadVec[ k ] );
            }
        }
        externalMetricValue[ metricName ] = totalValue;
    }
}
