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


#include "TreeModelProxy.h"
#include "TreeItem.h"

TreeModelProxy::TreeModelProxy( TreeModel* model ) : tree( model->getTree() ), sourceModel( model )
{
    // setDynamicSortFilter(true);
    setSourceModel( sourceModel );
    filterIsActive = false;
    comparator     = 0;
}

QAbstractItemModel*
TreeModelProxy::getModel()
{
    return this;
}

Tree*
TreeModelProxy::getTree() const
{
    return tree;
}

TreeItem*
TreeModelProxy::getTreeItem( const QModelIndex& idx ) const
{
    QModelIndex sidx = mapToSource( idx );
    return static_cast<TreeItem*>( sidx.internalPointer() );
}

void
TreeModelProxy::setFoundItems( const QModelIndexList& indexList )
{
    QModelIndexList sourceList;

    foreach( QModelIndex idx, indexList )
    {
        idx = mapToSource( idx );
        sourceList.append( idx );
    }
    sourceModel->setFoundItems( sourceList );
}

void
TreeModelProxy::clearFoundItems()
{
    sourceModel->clearFoundItems();
}

QModelIndex
TreeModelProxy::find( TreeItem* searchItem ) const
{
    QModelIndex idx = sourceModel->find( searchItem );

    return mapFromSource( idx );
}

QModelIndexList
TreeModelProxy::find( QRegExp regExp ) const
{
    QModelIndexList srcList = sourceModel->find( regExp );
    QModelIndexList found;

    foreach( QModelIndex idx, srcList )
    {
        idx = mapFromSource( idx );
        found.append( idx );
    }

    return found;
}

// -------------------------------------------------------------------------------------------

/**
 * @brief TreeModelProxy::filterAcceptsRow
 * returns if the item at souceRow of the sourceParent item of the source model should be visible
 */
bool
TreeModelProxy::filterAcceptsRow( int sourceRow, const QModelIndex& sourceParent ) const
{
    if ( !filterIsActive )
    {
        return true;
    }
    else if ( sourceParent.isValid() )
    {
        QModelIndex idx  = sourceModel->index( sourceRow, DATA_COL, sourceParent );
        TreeItem*   item =  sourceModel->getTreeItem( idx );
        return !item->isHidden();
    }
    return true; // 1. call has invalid indices for all tree elements
}

/**
 * allows sorting of the items of the model
 */
bool
TreeModelProxy::lessThan( const QModelIndex& left, const QModelIndex& right ) const
{
    if ( !comparator )
    {
        return true;
    }
    TreeItem* item1 = static_cast<TreeItem*>( left.internalPointer() );
    TreeItem* item2 = static_cast<TreeItem*>( right.internalPointer() );
    return comparator->compare( item1, item2 );
}

/**
 * @brief TreeModelProxy::setFilter
 * @param threshold percentual threshold, all values below are hidden
 * @param dynamic if true, the threshold is evaluated for new item values
 */
void
TreeModelProxy::setFilter( double threshold, bool dynamic )
{
    ( void )dynamic; // todo static filtering
    filterIsActive  = true;
    filterThreshold = threshold;
    tree->hideMinorValues( filterThreshold );
    this->invalidateFilter();
}

void
TreeModelProxy::disableFilter()
{
    filterIsActive = false;
    this->invalidateFilter();
    tree->unhideItems();
}

void
TreeModelProxy::setComparator( Comparator* c )
{
    comparator = c;
}

void
TreeModelProxy::updateValues()
{
    if ( filterIsActive )
    {
        tree->hideMinorValues( filterThreshold );
        this->invalidateFilter();                        // required to restart filtering after values have been changed
    }
    emit( dataChanged( QModelIndex(), QModelIndex() ) ); // all values have been changed
}
