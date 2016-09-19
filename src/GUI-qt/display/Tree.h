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


#ifndef TREE_H
#define TREE_H
#include <assert.h>
#include <QtCore>
#include "Cube.h"
#include "CubeMetric.h"
#include "Constants.h"
#include "CubeVertex.h"
#include "TreeItem.h"

class TreeModel;

class Tree : public QObject
{
    Q_OBJECT

public:
    Tree();
    ~Tree();

    /** creates the tree structure from the given cube */
    void
    initialize( cube::Cube* cube );

    /** returns the tree label */
    virtual QString
    getLabel() = 0;

    /** computes the values for all tree items */
    void
    computeValues( const QList<Tree*>& left );

    /** uses the computed values to generate the labels and colors */
    void
    updateItems();

    /** returns the top level item of the tree. This item is an empty invisible item
     *  whose children are displayed.
     */
    TreeItem*
    getRootItem() const;

    /** returns the type of the tree METRICTREE, CALLTREE, CALLFLAT, SYSTEMTREE */
    TreeType
    getTreeType() const;

    /** returns true, if all tree items have integer values */
    bool
    hasIntegerType() const;

    /** returns all tree items in arbitrary order */
    const QList<TreeItem*>&
    getItems() const;

    /** returns an interface to the tree data which can be used by Qt TreeViews */
    TreeModel*
    getModel() const;

    /** Returns the maximum value of this tree. For metric trees, the maximum depends on the root element
        of the given item, for all other trees, the item is ignored */
    virtual double
    getMaxValue( TreeItem* item = 0 ) const;

    /** returns the reference value (100%) to calculate the value of each item in the current value modus */
    double
    getValueModusReferenceValue() const;

    bool
    getStatisticValues( double& minValue,
                        double& maxValue,
                        double& selectedValue,
                        double& minAbsValue,
                        double& maxAbsValue,
                        double& absValue,
                        double& mean,
                        double& varianceSqrt ) const;


    /** provides the tree with the data of the other trees */
    void
    setTrees( QList<Tree*> trees );

    // ----- interaction with the view ----

    /** returns the current value modus, which the user has selected in the GUI */
    ValueModus
    getValueModus() const;

    /** called by the view, if the user has changed the value modus */
    void
    valueModusChanged( ValueModus modus );

    void
    setActive( bool active );

    bool
    isActive() const;

    /** notifies the view that the given item has been expanded */
    void
    expandItem( TreeItem* item,
                bool      expand );

    /** notifies the registered view that the given item has been selected */
    void
    selectItem( TreeItem* item,
                bool      addToSelection = false );

    /** notifies the registered view that the given item has been deselected */
    void
    deselectItem( TreeItem* item );

    /** returns the most recently selected item */
    TreeItem*
    getLastSelection() const;

    /** returns the items, which have been selected in the view */
    const QList<TreeItem*>&
    getSelectionList() const;

    /* used by the view to set the information about the selected item */
    void
    setLastSelection( TreeItem* value );
    void
    setSelectionList( const QList<TreeItem*>& value );

    cube::Cube*
    getCube() const;

    void
    deleteChildren( TreeItem* item );
    void
    deleteItem( TreeItem* item );

    /** Adds newItem to the tree and the model. If parent is null, the item is added as top level item  */
    void
    addSubtree( TreeItem* newItem,
                TreeItem* parent = 0 );

    /** Removes the given item from the tree and the model. The item is deleted afterwards. */
    void
    removeSubtree( TreeItem* item );

    /** @brief Tree::hideMinorValues sets all tree items to hidden whose values are less than threshold
     *  @param threshold threshold in percent of the maximum value */
    void
    hideMinorValues( double threshold );

    void
    unhideItems();

    bool
    hasUserDefinedMinMaxValues() const;
    double
    getUserDefinedMinValue() const;
    double
    getUserDefinedMaxValue() const;
    void
    unsetUserDefinedMinMaxValues();
    void
    setUserDefinedMinMaxValues( double minValue,
                                double maxValue );

signals:
    void
    itemExpanded( TreeItem* item,
                  bool      expand );

    void
    itemSelected( TreeItem* item,
                  bool      addToSelection = false );

    void
    itemDeselected( TreeItem* item );

protected:
    // ============================================================================================

    cube::Cube*                     cube;
    TreeType                        treeType;        // METRICTREE, CALLTREE, CALLFLAT or SYSTEMTREE
    TreeItem*                       top;             // invisible top level tree item (root)
    QHash<cube::Vertex*, TreeItem*> treeItemHash;    // hash to find the tree item for a given Vertex
    QList<TreeItem*>                treeItems;       // values of treeItemHash
    bool                            integerType;     // true, if metric has integer type
    double                          maxValue;        // the maximum value of the tree

    QHash<TreeType, Tree*> treeHash;                 // reference to the four available trees
    ValueModus             valueModus;               // currently selected value modus
    double                 valueModusReferenceValue; // calculated reference value for current value modus
    bool                   active;                   // true, if corresponding view is visible (tab is selected)
    QList<TreeItem*>       selectionList;            // all currently selected items
    TreeItem*              lastSelection;            // the most recently selected item

    TreeModel* treeModel;                            // simple model: Tree->QAbstractItemModel

    // ============================================================================================

    virtual void
    computeBasicValues( const QList<Tree*>& left,
                        TreeItem*           top = 0 ) = 0;

    virtual TreeItem*
    createTree() = 0;

    virtual bool
    itemIsValid( cube::Vertex* vertex );

    virtual QString
    getItemName( cube::Vertex* vertex ) = 0;

    virtual void
    computeMaxValues();

    void
    getMax( TreeItem* item,
            double&   value );

    void
    initializeValues();


    /**
       creates tree structure with Tree::top as (empty) top level element from a list of Vertex. It the vertex
       has children, all children will be added recursively.
     * @param top top level element of the tree
     * @param vec list of vertex which will be added to the tree
     * @param itemType METRICITEM, CALLITEM, REGIONITEM, SYSTEMTREENODEITEM, LOCATIONGROUPITEM, LOCATIONITEM
     * @param newItems if this list exists, it is filled with the created items
     */
    template <typename T>
    void
    createItems( TreeItem* top, const std::vector<T*>& vec, const TreeItemType itemType,
                 QList<TreeItem*>* newItems = 0, bool recursive = true )
    {
        top->tree = this;

        foreach( cube::Vertex * vertex, vec )
        {
            if ( !itemIsValid( vertex ) )
            {
                continue;
            }
            QString name = getItemName( vertex );

            TreeItem* parent = 0;
            if ( vertex->get_parent() == NULL ) // top-level item without parent
            {
                parent = top;
            }
            else // items with parent
            {
                parent = treeItemHash.value( vertex->get_parent() );
            }
            TreeItem* item = new TreeItem( name, itemType, vertex );
            assert( parent != 0 );
            parent->addChild( item );

            treeItemHash.insert( vertex, item );
            if ( newItems )
            {
                newItems->append( item );
            }

            if ( recursive && vertex->num_children() > 0 )
            {
                createItems<cube::Vertex>( top, vertex->get_children(), itemType, newItems );
            }
        } // foreach vertex
    }     // end of createItems()

private:
    double userMinValue;                  // user defined minimum value
    double userMaxValue;                  // user defined maximum value
    bool   userDefinedMinMaxValues;

    double
    computeReferenceValue( ValueModus valueModus );
};

#endif // TREE_H
