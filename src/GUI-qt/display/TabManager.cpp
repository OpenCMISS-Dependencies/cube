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



#include "TabManager.h"

#include <QLabel>
#include "Tree.h"
#include "MetricTree.h"
#include "CallTree.h"
#include "FlatTree.h"
#include "SystemTree.h"

#include "TreeModelProxy.h"
#include "TreeView.h"
#include "MetricTreeView.h"
#include "CallTreeView.h"
#include "FlatTreeView.h"
#include "SystemTreeView.h"
#include "PluginManager.h"

TabManager::TabManager( QWidget* parent ) :
    QSplitter( parent )
{
    isInitialized = false;

    DisplayType tabType[] = { METRIC, CALL, SYSTEM };
    for ( int i = 0; i < DISPLAYS; i++ )
    {
        TabWidget* tab = new TabWidget( tabType[ i ] );
        this->addWidget( tab );
        tabWidgets.append( tab );
        order.append( tabType[ i ] );
    }

    typeHash.insert( METRICTREE, METRIC );
    typeHash.insert( CALLTREE, CALL );
    typeHash.insert( CALLFLAT, CALL );
    typeHash.insert( SYSTEMTREE, SYSTEM );
}


void
TabManager::cubeOpened( cube::Cube* cube )
{
    for ( int i = 0; i < typeHash.keys().size(); i++ )
    {
        trees.append( 0 );
        views.append( 0 );
    }

    // set the tab order
    foreach( DisplayType tabType, order )
    {
        tabWidgets[ tabType ]->initialize( order );
    }

    // create trees and corresponding views
    foreach( TreeType treeType, typeHash.keys() )
    {
        createTreeAndView( treeType, cube );
    }

    // create tabs
    foreach( TreeType treeType, typeHash.keys() )
    {
        DisplayType tabType = typeHash.value( treeType );
        tabWidgets[ tabType ]->addTreeTab( views[ treeType ] );
    }

    // set tree information
    foreach( TreeType treeType, typeHash.keys() )
    {
        Tree* tree = trees[ treeType ];
        tree->setTrees( trees ); // give each tree information about the other trees
    }
    // initial selection will be done in TabManager::initialize
}


/**
 * @brief TabManager::reinit
 * recalculates the values of all trees and updates the display
 */
void
TabManager::reinit()
{
    DisplayType first = order[ 0 ];
    // values of the first tree have to be calculated, as they are not calculated on selection
    trees[ first ]->computeValues( QList<Tree*>() );

    // select the items of the tree on the left => recalculate all trees on the right
    treeItemSelected( getActiveTree( first ) );

    foreach( TreeView * view, views )
    {
        view->update();
    }
}

void
TabManager::cubeClosed()
{
    this->setVisible( false ); // ensure that no repaint events are sent any more

    foreach( TabWidget * tabWidget, tabWidgets )
    {
        tabWidget->cubeClosed();
    }
    foreach( TreeView * view, views )
    {
        delete view;
    }
    foreach( Tree * tree, trees )
    {
        delete tree;
    }
    foreach( QAbstractItemModel * model, proxyModels )
    {
        delete model;
    }

    views.clear();
    trees.clear();
    proxyModels.clear();

    this->setVisible( true );
    isInitialized = false;
}

QList<DisplayType>
TabManager::getOrder() const
{
    return order;
}

void
TabManager::setOrder( const QList<DisplayType>& value )
{
    if ( value == order )
    {
        return; // order didn't change
    }
    this->order = value;

    // reorder widgets
    foreach( DisplayType tabType, order )
    {
        tabWidgets[ tabType ]->setOrder( order );
        this->addWidget( tabWidgets[ tabType ] );
    }
    if ( trees.size() > 0 )
    {
        // set values according to new position
        reinit();

        PluginManager::getInstance()->setTabOrder( order ); // notify plugins
    }
}

/**
 * returns the TabWidget which corresponds to the given TabWidgetType
 * (METRICTAB, CALLTAB, SYSTEMTAB)
 */
TabWidget*
TabManager::getTab( DisplayType tabType )
{
    return tabWidgets[ tabType ];
}

/**
 * returns the tree which corresponds to the given TreeType
 * (METRICTREE, CALLTREE, CALLFLAT, SYSTEMTREE)
 */
Tree*
TabManager::getTree( TreeType type )
{
    return trees[ type ];
}

/**
 * returns the TreeView which corresponds to the given TreeType
 * (METRICTREE, CALLTREE, CALLFLAT, SYSTEMTREE)
 */
TreeView*
TabManager::getView( TreeType type )
{
    return views[ type ];
}

void
TabManager::initialize()
{
    // check that at least one tree item is selected, otherwise select first
    foreach( TreeType treeType, typeHash.keys() )
    {
        Tree*     tree = trees[ treeType ];
        TreeView* view = views[ treeType ];

        if ( tree->getSelectionList().size() == 0 )                       // nothing selected
        {
            TreeItem* first = tree->getRootItem()->getChildren().first();
            view->selectItem( first, false );
        }
    }

    // connect signals after all selections have been done
    foreach( TreeType treeType, typeHash.keys() )
    {
        Tree*     tree = trees[ treeType ];
        TreeView* view = views[ treeType ];

        connect( tree, SIGNAL( itemSelected( TreeItem*, bool ) ), view, SLOT( selectItem( TreeItem*, bool ) ) );
        connect( tree, SIGNAL( itemDeselected( TreeItem* ) ), view, SLOT( deselectItem( TreeItem* ) ) );
        connect( tree, SIGNAL( itemExpanded( TreeItem*, bool ) ), view, SLOT( expandItem( TreeItem*, bool ) ) );
        connect( view, SIGNAL( selectionChanged( Tree* ) ), this, SLOT( treeItemSelected( Tree* ) ) );
        connect( view, SIGNAL( itemExpanded( TreeItem*, bool ) ), this, SLOT( treeItemExpanded( TreeItem*, bool ) ) );
        connect( view, SIGNAL( recalculateRequest( Tree* ) ), this, SLOT( recalculateTree( Tree* ) ) );
    }

    reinit();  // calculate tree data and display it

    isInitialized = true;
}

// -------------- settings -------------------
void
TabManager::loadExperimentSettings( QSettings& settings )
{
    settings.beginGroup( "TabManager" );

    /* load initial selections */
    foreach( TreeType treeType, typeHash.keys() )
    {
        Tree*     tree = trees[ treeType ];
        TreeView* view = views[ treeType ];
        settings.beginGroup( tree->getLabel().remove( ' ' ) );
        view->loadExperimentSettings( settings );
        settings.endGroup();
    }

    /* update order and activate initial selections */
    QList<DisplayType> sorder;
    QString            orderStr = settings.value( "TabOrder", "0 1 2" ).toString();
    foreach( QString s, orderStr.split( " ", QString::SkipEmptyParts ) )
    {
        sorder.append( static_cast<DisplayType> ( s.toInt() ) );
    }
    if ( order != sorder )
    {
        setOrder( sorder );
    }

    settings.endGroup(); // TabManager
}

void
TabManager::saveExperimentSettings( QSettings& settings )
{
    settings.beginGroup( "TabManager" );

    QString orderStr;
    foreach( uint i, order )
    {
        orderStr += QString::number( static_cast<DisplayType> ( i ) ) + " ";
    }
    settings.setValue( "TabOrder", orderStr );

    foreach( TreeType treeType, typeHash.keys() )
    {
        // select first item, or item given by preferences
        Tree*     tree = trees[ treeType ];
        TreeView* view = views[ treeType ];

        settings.beginGroup( tree->getLabel().remove( ' ' ) );
        view->saveExperimentSettings( settings );
        settings.endGroup();
    }
    settings.endGroup(); // TabManager
}

QString
TabManager::settingName()
{
    return "TabManager";
}

// -------------- end settings ----------------

/**
 *  returns the active tree of the given tab
 */
Tree*
TabManager::getActiveTree( DisplayType tabType )
{
    return tabWidgets[ tabType ]->getActiveTreeView()->getTree();
}

DisplayType
TabManager::getDisplayType( TreeType treeType )
{
    return typeHash[ treeType ];
}

void
TabManager::updateTreeItemProperties()
{
    foreach( Tree * tree, trees )
    {
        tree->updateItems();
    }
    foreach( TreeView * view, views )
    {
        view->updateValues();
    }
}

void
TabManager::updateTreeItems()
{
    foreach( TreeView * view, views )
    {
        if ( view != NULL )
        {
            view->updateValues();
        }
    }
}

/**
 * @brief TabManager::getNeighborTrees
 * @param left is filled with trees left of the tree current
 * @param right is filled with trees right of the tree current
 */
void
TabManager::getNeighborTrees( QList<Tree*>& left, QList<Tree*>& right, Tree* current )
{
    int  idx   = 0;
    bool found = false;

    for ( idx = 0; idx < order.size(); idx++ )
    {
        Tree* tree = getActiveTree( order[ idx ] );

        if ( tree == current )
        {
            found = true;
        }
        else
        {
            if ( !found )
            {
                left.append( tree );
            }
            else
            {
                right.append( tree );
            }
        }
    }
}

// todo: also recalculate only required subtree
void
TabManager::recalculateTree( Tree* tree )
{
    QList<Tree*> left;
    QList<Tree*> right;
    getNeighborTrees( left, right, tree );

    if ( !left.empty() )
    {
        Tree* leftTree = left.last();
        treeItemSelected( leftTree );
    }
    else
    {
        reinit();
    }
}

/**
 * recalculates the values of all trees right of the selected one
 */
void
TabManager::treeItemSelected( Tree* selectedTree )
{
    QList<Tree*> left;
    QList<Tree*> right;
    getNeighborTrees( left, right, selectedTree );

    DisplayType tabType = typeHash.value( selectedTree->getTreeType() );
    tabWidgets[ tabType ]->updateValueWidget(); // display the selected value

    /* call computeValues for all trees right of the selected one */
    left.append( selectedTree );
    foreach( Tree * tree, right )
    {
        // qDebug() << "compute tree" << tree->getLabel(); QTime t; t.restart();

        tree->computeValues( left ); // recalculate the values of the tree
        left.append( tree );
        // qDebug() << "compute values in ms:" << tree->getLabel() << t.restart();

        tabType = typeHash.value( tree->getTreeType() );
        tabWidgets[ tabType ]->valuesChanged();
    }

    activateStaticMarker();

    foreach( Tree * tree, right )
    {
        TreeView* view = views[ tree->getTreeType() ];
        view->updateValues(); // informs the the model (proxy) and the view about the changed data
    }
}

/**
 * Calculates the expanded values. If the expanded item is selected, the trees values right of the
 * current one have to be recalculated.
 * @param expanded if true, the item has been expanded, otherwise it has been collapsed
 */
void
TabManager::treeItemExpanded( TreeItem* item, bool expanded )
{
    // qDebug() << "TabManager::treeItemExpanded" << item->getName() << expanded;
    Tree*        current = item->getTree();
    QList<Tree*> left;
    QList<Tree*> right;
    getNeighborTrees( left, right, current );

    // calculate expanded values
    if ( expanded )
    {
        current->computeValues( left );
    }
    else // update the collapsed root item(s)
    {
        if ( item->getParent() == 0 ) // invisible root item
        {
            foreach( TreeItem * child, item->getChildren() )
            child->updateItem();
        }
        else
        {
            item->updateItem();
        }
    }

    // if expanded item is also selected, update the trees right of the selected item
    // if item is invisible root item also update the tree
    if ( item->isSelected() || item->getParent() == 0 )
    {
        treeItemSelected( item->getTree() );
    }

    // update view and check if items have to be hidden
    TreeView* view = views[ current->getTreeType() ];
    view->updateValues();
}

/**
 * Creates a tree and its view. The tree is added to the list TabManager::trees, its
 * view is added to TabManager::views.
 * @param type
 * @param cube
 */
void
TabManager::createTreeAndView( TreeType type, cube::Cube* cube )
{
    Tree*     tree = 0;
    TreeView* view = 0;
    QString   iconFile;

    TreeModelProxy* proxyModel = 0;
    switch ( type )
    {
        case METRICTREE:
        {
            tree = new MetricTree();
            tree->initialize( cube );
            proxyModel = new TreeModelProxy( tree->getModel() ); // use proxy model for sorting
            // view = new TreeView( tree->getModel(), tree->getLabel() ); // use standard model
            view     = new MetricTreeView( proxyModel, tree->getLabel() );
            iconFile = ":images/tree-icon.png";
            break;
        }
        case CALLTREE:
        {
            CallTree* callTree = new CallTree();
            tree = callTree;
            tree->initialize( cube );
            callTree->hideIterations();
            proxyModel = new TreeModelProxy( tree->getModel() );
            view       = new CallTreeView( proxyModel, tree->getLabel() );
            iconFile   = ":images/tree-icon.png";
            break;
        }
        case CALLFLAT:
        {
            tree = new FlatTree();
            tree->initialize( cube );
            proxyModel = new TreeModelProxy( tree->getModel() );
            view       = new FlatTreeView( proxyModel, tree->getLabel() );
            iconFile   = ":images/list-icon.png";
            break;
        }
        case SYSTEMTREE:
        {
            tree = new SystemTree();
            tree->initialize( cube );
            proxyModel = new TreeModelProxy( tree->getModel() );
            view       = new SystemTreeView( proxyModel, tree->getLabel() );
            iconFile   = ":images/tree-icon.png";
            break;
        }
    }
    if ( proxyModel ) // save model for deletation
    {
        this->proxyModels.append( proxyModel );
    }

    view->setIcon( QIcon( iconFile ) );

    trees[ type ] = tree;
    views[ type ] = view;
}

void
TabManager::setTreeFont( const QFont& font, const QColor& color )
{
    foreach( TreeView * view, views )
    {
        if ( view )
        {
            view->setFont( font );

            QPalette palette = view->palette();
            palette.setColor( QPalette::Highlight, color );
            view->setPalette( palette );
        }
    }
}

void
TabManager::updateColors()
{
    if ( isInitialized )
    {
        foreach( TabWidget * tab, tabWidgets )
        {
            tab->valuesChanged(); // notify plugin tabs
        }
    }
}

QList<TabWidget*>
TabManager::getTabWidgets()
{
    return tabWidgets;
}

void
TabManager::addStaticMarker( const TreeItemMarker* marker, TreeItem* metric, TreeItem* call, TreeItem* system )
{
    if ( markerHash.contains( marker ) )
    {
        MarkerTripletList& list = markerHash[ marker ];
        list.addTriplet( metric, call, system );
    }
    else
    {
        MarkerTripletList list;
        list.addTriplet( metric, call, system );
        markerHash.insert( marker, list );
    }
}

void
TabManager::removeStaticMarker( const TreeItemMarker* marker )
{
    foreach( QList<TreeItem*> triplet, markerHash.value( marker ) )
    foreach( TreeItem * item, triplet )
    if ( item )
    {
        item->removeMarker( marker );
    }
    markerHash.remove( marker );
}

void
TabManager::activateStaticMarker()
{
    // delete all static marker
    foreach( const TreeItemMarker * marker, markerHash.keys() )
    foreach( QList<TreeItem*> triplet, markerHash.value( marker ) )
    foreach( TreeItem * item, triplet )
    if ( item )
    {
        item->removeMarker( marker );
    }

    // set marker
    foreach( const TreeItemMarker * marker, markerHash.keys() )
    {
        const MarkerTripletList& list = markerHash.value( marker );
        for ( int tabNr = 0; tabNr < order.size(); tabNr++ )
        {
            DisplayType tabType = order[ tabNr ];
            foreach( QList<TreeItem*> triplet, list )
            {
                TreeItem* currentItem = triplet.at( tabType );
                if ( currentItem )
                {
                    // check if all triplet parts left of the current tab are selected
                    bool setMarker = true;
                    for ( int leftTab = 0; leftTab < tabNr; leftTab++ )
                    {
                        Tree*     tree     = tabWidgets[ order[ leftTab ] ]->getActiveTreeView()->getTree();
                        TreeItem* selected = tree->getSelectionList().first();
                        TreeItem* marked   = triplet.at( order[ leftTab ] );
                        if ( !marked || marked != selected )
                        {
                            setMarker = false;
                            break;
                        }
                    }
                    if ( setMarker )
                    {
                        bool rightMarkerExists = false;
                        for ( int rightTab = tabNr + 1; rightTab < order.size(); rightTab++ )
                        {
                            if ( triplet.at( order[ rightTab ] ) != 0 )
                            {
                                rightMarkerExists = true;
                                break;
                            }
                        }
                        if ( rightMarkerExists )
                        {
                            currentItem->setMarker( marker, true ); // mark only as dependency
                        }
                        else
                        {
                            currentItem->setMarker( marker );
                        }
                    }
                }
            } // all marker triplet
        }     // all tabs
    }
}
