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


#ifndef TREEITEMDATA_H
#define TREEITEMDATA_H
#include "CubeVertex.h"
#include "CubeValues.h"
#include "CubeCnode.h"
#include "Constants.h"
#include <QColor>

class Tree;
class TreeItemMarker;
class TabManager;
class TreeItemMarker;

class TreeItem
{
public:
    // allow all trees to modify its items, but don't allow plugins
    friend class Tree;
    friend class MetricTree;
    friend class SystemTree;
    friend class CallTree;
    friend class FlatTree;
    friend class AggregatedTreeItem;
    friend class TreeModel;

    friend class TabManager;
    friend class TreeView;
    friend class PluginServices;
    friend class TreeItemDelegate;

    TreeItem( const QString& name,
              TreeItemType   type,
              cube::Vertex*  cube );
    virtual
    ~TreeItem();

    // ---- getter ----------------------------------------
    /** returns the absolute value of the item */
    double
    getAbsoluteValue() const;

    /** @brief returns the value for the selected value modus.
     * @param ok If ok is given, it returns if the value is valid.
     * For some metrics (e.g. metrics which are building a minimum) no valid exclusive value
     * exists. The zero value for a minimum metrics is the maximum negative value. */
    double
    getValue( bool* ok = 0 ) const;

    double
    getOwnValue() const;

    const cube::Value*
    getOwnValueObject() const;

    /** returns the inclusive value */
    double
    getTotalValue() const;

    const cube::Value*
    getTotalValueObject() const;

    /** returns the tree level of the current item */
    int
    getDepth() const; // check usage

    bool
    isCalculated() const;
    bool
    isExpanded() const;
    bool
    isHidden() const;
    bool
    isSelected() const;

    /** returns the corresponding cube Vertex object or NULL, if the item is aggregated */
    cube::Vertex*
    getCubeObject() const;

    /** returns the name of the item */
    const QString&
    getName() const;

    /** returns the label of the item with consists of its name and the item value */
    const QString&
    getLabel() const;

    /** returns the color, which is used as icon the corresponding tree widget */
    const QColor&
    getColor() const;

    /** returns the parent of the current item */
    TreeItem*
    getParent() const;

    /** true, if the item has no children */
    bool
    isLeaf() const;

    /** returns true, if the item is the root item of an aggregated loop */
    virtual bool
    isAggregatedRootItem() const;

    /** returns true, if the item is an aggregation from all loop iteration */
    bool
    isAggregatedLoopItem() const;

    /** returns a list of all children of the current item */
    const QList<TreeItem*>&
    getChildren() const;

    TreeItemType
    getType() const;

    /** @brief Searches recursively for all children of the item which are leafs.
     *  @returns list of leaf items */
    QList<TreeItem*>
    getLeafs() const;

    /** expands the current item or collapses it (expand = false) */
    void
    setExpanded( bool expand );

    /** selects the current item
     *  @param addToSelection if false, all previously selected item are deselected */
    void
    select( bool addToSelection = false );

    /** deselects the current item */
    void
    deselect();

    /** updates the label and color of the item according to its value */
    void
    updateItem();

    /** returns true, if the item is an item of the first visible tree level */
    bool
    isTopLevelItem() const;

    /** returns the visible root of the current item */
    const TreeItem*
    getTopLevelItem() const;

    /** returns a list of TreeItemMarkers which have been set for the current item */
    const QList<const TreeItemMarker*>&
    getMarkerList() const;

    /** if item is a call tree item or a flat tree item, the corresponding source file and the
        line number information is returned */
    void
    getSourceInfo( QString& filename,
                   int&     startLine,
                   int&     endLine );

    /** returns true, if the item in the current value modus should be displayed in integer format */
    bool
    isIntegerType() const;

protected:
    // the type of the node;
    // needed to distinguish how to handle nodes
    TreeItemType type;

    QString name;
    QString label; // text of the item in the QTreeView

    // the corresponding cube object
    cube::Vertex* cubeObject;

    // tree structure
    Tree*            tree; // the tree the item is part of
    TreeItem*        parentItem;
    QList<TreeItem*> children;
    int              depth; // depth of the current item in the tree

    // values attached to the item
    double    ownValue;
    double    totalValue;
    TreeItem* rootItem;

    bool         singleValue;
    cube::Value* ownValue_adv;
    cube::Value* totalValue_adv;

    bool calculated; // true, if a value has been calculated

    // item values/colors for the current state; note that valueExpanded
    // can differ from ownValue if the item has hidden children, in which
    // case the total values of the hidden children are added up to the own value
    double  valueExpanded, valueCollapsed;
    QColor  colorExpanded, colorCollapsed;
    QString textExpanded, textCollapsed;

    /* view status */
    bool                         expanded;
    bool                         hidden;
    bool                         selected;
    bool                         visible;
    QList<const TreeItemMarker*> markerList;             // markers for current item
    QList<const TreeItemMarker*> parentMarkerList;       // markers for child items; used to mark parent if child is not expanded
    QList<const TreeItemMarker*> dependencyMarkerList;   // markers to show a dependency to another marked item

    // the minimal and maximal peer values for system tree items
    double minValue;
    double maxValue;

    // ============================================================================

    // returns the tree, the item is part of
    Tree*
    getTree() const;

    // function to build the tree
    void
    addChild( TreeItem* item );
    void
    removeChild( TreeItem* item );
    void
    setValues( cube::Value* total,
               cube::Value* own );
    double
    getValue( ValueModus modus,
              bool&      ok ) const;

    // end function to build the tree

    void
    setMarker( const TreeItemMarker* marker,
               bool                  isDependencyMarker = false,
               bool                  markParents = true );
    void
    removeMarker( const TreeItemMarker* marker );

private:
    // setter to be called by the view to update the status of the item
    void
    setExpandedStatus( bool expanded,
                       bool recursive = false );
    void
    setSelectionStatus( bool selected = true );

    void
    setHidden( bool hidden );

    // functions to build TreeModel
    TreeItem*
    child( int row ) const;
    int
    row() const;

    // end functions for TreeModel

    void
    setDepth( int value );
};

#endif // TREEITEMDATA_H
