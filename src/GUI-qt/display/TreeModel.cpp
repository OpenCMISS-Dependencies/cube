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


// @see http://qt-project.org/doc/qt-4.8/modelview.html

#include <QtGui>
#include "TreeItem.h"
#include "TreeModel.h"
#include "Globals.h"
#include "TreeItemMarker.h"

TreeModel::TreeModel( Tree* tree )
{
    this->tree = tree;
}

TreeModel::~TreeModel()
{
}


// --- begin QAbstractItemModel functions --------------------------
int
TreeModel::columnCount( const QModelIndex& ) const
{
    return 1;
}

/**
 * @brief TreeModel::data
 * @param index tree index of the item to be displayed
 * @param role type of element to be displayed
 * @return the data to display for a tree item
 */
QVariant
TreeModel::data( const QModelIndex& index, int role ) const
{
    if ( !index.isValid() )
    {
        return QVariant();
    }

    if ( role == Qt::DisplayRole )
    {
        TreeItem* item = static_cast<TreeItem*>( index.internalPointer() );
        return item->getLabel();
    }
    else if ( role == Qt::BackgroundRole )
    {
        if ( foundItems.contains( index ) )
        {
            return QBrush( Qt::yellow );
        }
        else if ( foundParents.contains( index ) )
        {
            return QBrush( QColor( Qt::yellow ).lighter( 170 ) );
        }
    }
    else if ( role == TreeItemRole )
    {
        TreeItem* item = static_cast<TreeItem*>( index.internalPointer() );
        return qVariantFromValue( ( void* )item );
    }
    return QVariant();
}

Qt::ItemFlags
TreeModel::flags( const QModelIndex& index ) const
{
    if ( !index.isValid() )
    {
        return 0;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex
TreeModel::index( int row, int column, const QModelIndex& parent ) const
{
    if ( !hasIndex( row, column, parent ) )
    {
        return QModelIndex();
    }

    TreeItem* parentItem;

    if ( !parent.isValid() )
    {
        parentItem = tree->getRootItem();
    }
    else
    {
        parentItem = static_cast<TreeItem*>( parent.internalPointer() );
    }

    TreeItem* childItem = parentItem->child( row );
    if ( childItem )
    {
        return createIndex( row, column, childItem );
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex
TreeModel::parent( const QModelIndex& index ) const
{
    if ( !index.isValid() )
    {
        return QModelIndex();
    }

    TreeItem* childItem  = static_cast<TreeItem*>( index.internalPointer() );
    TreeItem* parentItem = childItem->getParent();

    if ( parentItem == tree->getRootItem() )
    {
        return QModelIndex();
    }

    return createIndex( parentItem->row(), DATA_COL, parentItem );
}

int
TreeModel::rowCount( const QModelIndex& parent ) const
{
    if ( parent.column() > 0 ) // model has only one column
    {
        return 0;
    }

    TreeItem* parentItem;
    if ( !parent.isValid() )
    {
        parentItem = tree->getRootItem();
    }
    else
    {
        parentItem = static_cast<TreeItem*>( parent.internalPointer() );
    }

    return parentItem->getChildren().size();
}
// --- end QAbstractItemModel functions --------------------------


// -- begin additional model manipulation methods ---------------------------------------
void
TreeModel::updateValues()
{
    emit( dataChanged( QModelIndex(), QModelIndex() ) );
}

/** Removes the children of given item from the model and the tree. The children are deleted afterwards. */
void
TreeModel::removeChildren( TreeItem* item )
{
    QModelIndex idx = find( item );
    beginRemoveRows( idx, 0, rowCount( idx ) );
    tree->deleteChildren( item );
    endRemoveRows();
}

/** Removes the given item from the model and the tree. The item is deleted afterwards. */
void
TreeModel::removeSubtree( TreeItem* item )
{
    QModelIndex idx    = find( item );
    QModelIndex parent = idx.parent();
    int         row    = idx.row();

    beginRemoveRows( parent, row, row );
    tree->deleteChildren( item );
    tree->deleteItem( item );
    endRemoveRows();
}

/**
 * Replaces oldItem with newItem in tree and model and notifies the view.
 */
void
TreeModel::replaceSubtree( TreeItem* oldItem, TreeItem* newItem )
{
    TreeItem*         parent   = oldItem->getParent();
    QList<TreeItem*>& children = parent->children;

    QModelIndex idx       = find( parent );
    int         row       = find( oldItem ).row();
    int         listIndex = children.indexOf( oldItem );

    beginRemoveRows( idx, row, row );
    children.removeAt( listIndex );
    endRemoveRows();

    beginInsertRows( idx, row, row );
    children.insert( listIndex, newItem );
    newItem->parentItem = oldItem->parentItem;
    endInsertRows();
}

/** Adds newItem to the model and the tree. If parent is null, the item is added as top level item  */
void
TreeModel::addSubtree( TreeItem* newItem, TreeItem* parent )
{
    if ( !parent )
    {
        parent = tree->getRootItem();
    }
    QModelIndex idx = find( parent );
    int         row = rowCount( idx );
    beginInsertRows( idx, row, row );

    parent->addChild( newItem );

    endInsertRows();
}



// --------------------------------------------------------------------------------------
// -- begin TreeModelInterface ----------------------------------------------------------------

QAbstractItemModel*
TreeModel::getModel()
{
    return this;
}

Tree*
TreeModel::getTree() const
{
    return tree;
}

TreeItem*
TreeModel::getTreeItem( const QModelIndex& idx ) const
{
    return static_cast<TreeItem*>( idx.internalPointer() );
}

void
TreeModel::clearFoundItems()
{
    foundItems.clear();
    foundParents.clear();
}

QModelIndex
TreeModel::find( TreeItem* searchItem ) const
{
    QList<QModelIndex> list;
    QModelIndex        root = QModelIndex();
    list.append( root );

    TreeItem*   item = 0;
    QModelIndex idx;

    while ( !list.isEmpty() )
    {
        QModelIndex parent = list.takeFirst();
        int         rows   = rowCount( parent );
        for ( int i = 0; i < rows; ++i )
        {
            idx  = index( i, DATA_COL, parent );
            item = static_cast<TreeItem*>( idx.internalPointer() );
            if ( item == searchItem ) // found
            {
                list.clear();
                break;
            }
            list.append( idx ); // check children
        }
    }
    if ( item != searchItem )
    {
        idx = QModelIndex();
    }
    return idx;
}

/**
 * Marks the itenms of the given list. The items of the list become invalid after updateValues is called
 */
void
TreeModel::setFoundItems( const QModelIndexList& value )
{
    foundItems = value;
    QSet<QModelIndex> parents;

    foreach( QModelIndex idx, foundItems )
    {
        QModelIndex parentIdx = parent( idx );
        while ( parentIdx.isValid() )
        {
            parents.insert( parentIdx );
            parentIdx = parent( parentIdx );
        }
    }

    foundParents = parents.toList();
    updateValues();
}

QModelIndexList
TreeModel::find( QRegExp regExp ) const
{
    if ( !regExp.isValid() )
    {
        Globals::setStatusMessage( "Warning: no valid regular expression", Warning );
        return QModelIndexList();
    }

    QList<QModelIndex> list;
    QList<QModelIndex> found;
    QModelIndex        root = QModelIndex();

    list.append( root );
    while ( !list.isEmpty() )
    {
        QModelIndex parent = list.takeFirst();
        int         rows   = rowCount( parent );
        for ( int i = 0; i < rows; ++i )
        {
            QModelIndex idx = index( i, DATA_COL, parent );

            TreeItem* item = static_cast<TreeItem*>( idx.internalPointer() );
            if ( regExp.indexIn( item->getLabel() ) != -1 ) // found
            {
                found.append( idx );
            }
            list.append( idx ); // check children
        }
    }
    return found;
}
