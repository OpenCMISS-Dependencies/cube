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

#include "CallTreeView.h"
#include "CallTree.h"
#include "Globals.h"
#include "PluginManager.h"

using namespace std;

CallTreeView::CallTreeView( TreeModelInterface* model,
                            const QString&      tabLabel ) : TreeView( model, tabLabel )
{
    createContextMenuActions();
    flatTreeView             = 0;
    flatTreeSelectionChanged = false;
}

/**
 * called, if user selects an item in the flat tree view
 */
void
CallTreeView::callFlatItemSelected()
{
    flatTreeSelectionChanged = true;
}

/** called if the call tree tab becomes active */
void
CallTreeView::setActive( bool active )
{
    /* check, if user has clicked on an item in the flat view */
    if ( !flatTreeView )
    {
        flatTreeView = Globals::getTabManager()->getView( CALLFLAT );
        if ( flatTreeView )
        {
            connect( flatTreeView, SIGNAL( clicked( QModelIndex ) ), this, SLOT( callFlatItemSelected() ) );
        }
    }

    /* Select all entries in the call tree, that have the same label as the selected entries
       in the flat tree.
       If nothing is clicked in the flat tree, the selection of the call tree remains unchanged.
     */
    if ( active )
    {
        if ( flatTreeSelectionChanged )
        {
            Tree* flat = Globals::getTabManager()->getTree( CALLFLAT );

            bool                   deselect = true;
            const QList<TreeItem*> selected = flat->getSelectionList();
            foreach( TreeItem * item, selected )
            {
                this->selectAll( item->getName(), deselect );
                deselect = false;
            }
        }
        emit recalculateRequest( getTree() );
    }
    else // user activates call flat view => check if user selects items in flat tree
    {
        flatTreeSelectionChanged = false;
    }
}

/**
 * creates context menu items for call trees
 */
void
CallTreeView::fillContextMenu()
{
    foreach( QAction * action, callContextMenuHash.values() ) // enable all context menu actions
    {
        action->setEnabled( true );
    }

    CallTree* tree       = static_cast<CallTree*> ( modelInterface->getTree() );
    bool      isLoopRoot = contextMenuItem && ( contextMenuItem == tree->getLoopRootItem() || contextMenuItem == tree->getAggregatedRootItem() );

    if ( isLoopRoot )
    {
        if ( contextMenuItem == tree->getAggregatedRootItem() )
        {
            contextMenu->addAction( callContextMenuHash.value( ShowIterations ) );
        }
        else
        {
            contextMenu->addAction( callContextMenuHash.value( HideIterations ) );
        }
        contextMenu->addSeparator();
    }

    contextMenu->addAction( contextMenuHash.value( TreeItemInfo ) );
    contextMenu->addAction( contextMenuHash.value( Documentation ) );

    if ( !isLoopRoot )
    {
        contextMenu->addAction( callContextMenuHash.value( SetAsLoop ) );
    }

    contextMenu->addSeparator();
    contextMenu->addAction( contextMenuHash.value( ExpandMenu ) );

    QMenu* hidingMenu = contextMenu->addMenu( tr( "Hiding" ) );
    hidingMenu->setWhatsThis( "Hide subtrees." );
    hidingMenu->addAction( contextMenuHash.value( DynamicHiding ) );
    hidingMenu->addAction( contextMenuHash.value( NoHiding ) );

    QString cutText = multipleContext ? tr( "Cut selected call tree items" ) : tr( "Cut call tree" );
    QMenu*  cutMenu = contextMenu->addMenu( cutText );
    cutMenu->setWhatsThis( "Cut selected subtree." );
    cutMenu->addAction( callContextMenuHash.value( Reroot ) );
    cutMenu->addAction( callContextMenuHash.value( Prune ) );
    cutMenu->addAction( callContextMenuHash.value( SetAsLeaf ) );
    contextMenu->addSeparator();

    contextMenu->addAction( contextMenuHash.value( FindItems ) );
    contextMenu->addAction( contextMenuHash.value( ClearFound ) );
    contextMenu->addAction( contextMenuHash.value( SortingMenu ) );
    contextMenu->addSeparator();
    contextMenu->addAction( contextMenuHash.value( UserDefinedMinMax ) );
    contextMenu->addSeparator();
    contextMenu->addAction( contextMenuHash.value( CopyClipboard ) );
    contextMenu->addSeparator();

    if ( !contextMenuItem || contextIsInvalid )
    {
        cutMenu->setEnabled( false );
    }
    if ( !contextMenuItem || multipleContext )
    {
        callContextMenuHash.value( HideIterations )->setEnabled( false );
        callContextMenuHash.value( ShowIterations )->setEnabled( false );
        callContextMenuHash.value( SetAsLoop )->setEnabled( false );
        callContextMenuHash.value( Reroot )->setEnabled( false );
    }
}

void
CallTreeView::createContextMenuActions()
{
    QAction* action = new QAction( this );
    action->setIcon( QIcon( QPixmap::fromImage( QImage( ":images/iterations_small.png" ) ) ) );
    action->setIconVisibleInMenu( true );
    action->setStatusTip( tr( "Shows summary or detailed list of iterations" ) );
    action->setWhatsThis( "Show summary or detailed list of iterations" );
    action->setText( "Show iterations" );
    connect( action, SIGNAL( triggered() ), this, SLOT( showIterations() ) );
    callContextMenuHash.insert( ShowIterations, action );

    action = new QAction( this );
    action->setIcon( QIcon( QPixmap::fromImage( QImage( ":images/iterations_small.png" ) ) ) );
    action->setIconVisibleInMenu( true );
    action->setStatusTip( tr( "Shows summary or detailed list of iterations" ) );
    action->setWhatsThis( "Shows summary or detailed list of iterations" );
    action->setText( "Hide iterations" );
    connect( action, SIGNAL( triggered() ), this, SLOT( hideIterations() ) );
    callContextMenuHash.insert( HideIterations, action );

    action = new QAction( tr( "Set as loop" ), this );
    action->setStatusTip( tr( "Handle the current item as loop and its children as iterations." ) );
    action->setWhatsThis( tr( "Handle the current item as loop and its children as iterations." ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( setAsLoop() ) );
    callContextMenuHash.insert( SetAsLoop, action );

    // Replaces the root of the selected callpath by its subtree. Selected callpath becomes a new root.
    action = new QAction( tr( "&Set as root" ), this );
    action->setStatusTip( tr( "Replaces the root element of the tree of the selected callpath by its subtree." ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( onRerootItem() ) );
    action->setWhatsThis( "For call trees. Removes all elements of the selected tree above the selected item. Selected item becomes a new root element." );
    callContextMenuHash.insert( Reroot, action );

    // Removes element and its subtree from the parent element. Prune
    action = new QAction( tr( "&Prune element" ), this );
    action->setStatusTip( tr( "Removes selected element and its subtree." ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( onPruneItem() ) );
    action->setWhatsThis( "For call tree. Removes selected element from the list of the children ot its parent. Pruned element contributes to exclusive value of its parent." );
    callContextMenuHash.insert( Prune, action );

    // Removes all children of its element, but element stays.
    action = new QAction( tr( "&Set as leaf" ), this );
    action->setStatusTip( tr( "Removes all children and their subtrees from the element." ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( onSetAsLeaf() ) );
    action->setWhatsThis( "For call trees. Removes all children and their subtrees from the element. Removed children contribute to the exclusive calue it the selected item." );
    callContextMenuHash.insert( SetAsLeaf, action );
}

void
CallTreeView::showIterations()
{
    CallTree* tree = static_cast<CallTree*> ( modelInterface->getTree() );
    tree->showIterations();
    emit recalculateRequest( tree );
}

void
CallTreeView::hideIterations()
{
    CallTree* tree = static_cast<CallTree*> ( modelInterface->getTree() );
    tree->hideIterations();
    emit recalculateRequest( tree );
}

void
CallTreeView::setAsLoop()
{
    CallTree* tree = static_cast<CallTree*> ( modelInterface->getTree() );
    tree->setAsLoop( contextMenuItem );

    PluginManager::getInstance()->genericUserAction( LoopMarkAction );
    updateValues();
}

void
CallTreeView::onRerootItem()
{
    if ( contextMenuItem->isTopLevelItem() )
    {
        return; // already root item
    }

    CallTree* tree = static_cast<CallTree*> ( modelInterface->getTree() );
    tree->setItemAsRoot( contextMenuItem );

    Globals::getTabManager()->reinit();
    updateValues();
}


// prune element
void
CallTreeView::onPruneItem()
{
    assert( !contextIsInvalid );

    if ( contextMenuItem->isTopLevelItem() )
    {
        Globals::setStatusMessage( "action is not allowed for call tree root" );
        return;
    }

    CallTree* tree = static_cast<CallTree*> ( modelInterface->getTree() );

    if ( multipleContext )
    {
        const QList<TreeItem*> selectedItems = tree->getSelectionList();
        foreach( TreeItem * item, selectedItems )
        {
            tree->pruneItem( item );
        }
    }
    else
    {
        tree->pruneItem( contextMenuItem );
    }

    // select 1st element, if nothing is selected
    if ( selectionModel()->selectedRows().size() == 0 )
    {
        QModelIndex idx  = modelInterface->getModel()->index( 0, 0 );
        TreeItem*   item = modelInterface->getTreeItem( idx );

        selectItem( item, QItemSelectionModel::Select );
    }
}


// set as leaf
void
CallTreeView::onSetAsLeaf()
{
    assert( !contextIsInvalid );
    CallTree* tree = static_cast<CallTree*> ( modelInterface->getTree() );

    if ( multipleContext )
    {
        const QList<TreeItem*> selectedItems = tree->getSelectionList();
        foreach( TreeItem * item, selectedItems )
        {
            tree->setItemAsLeaf( item );
        }
    }
    else
    {
        tree->setItemAsLeaf( contextMenuItem );
    }

    // select this leaf element, if nothing is selected
    if ( selectionModel()->selectedRows().size() == 0 )
    {
        selectItem( contextMenuItem, QItemSelectionModel::Select );
    }
}


// this slot displays the location of the clicked item's callee
//
void
CallTreeView::onLocation()
{
    TreeItem* item = contextMenuItem;
    location( item, false );
}

void
CallTreeView::onLocationCallee()
{
    TreeItem* item = contextMenuItem;
    location( item, true );
}

// display the location of a call or region item
//
void
CallTreeView::location( TreeItem* item, bool takeCallee )
{
    QString mod;
    int     line  = -1;
    int     line2 = -1;
    QString lineStr, line2Str;

    if ( !item->getCubeObject() )
    {
        return;
    }

    if ( item->getType() == CALLITEM )
    {
        if ( !takeCallee )
        {
            cube::Cnode* cnode = static_cast<cube::Cnode*>( item->getCubeObject() );
            mod  = QString::fromStdString( cnode->get_mod() );
            line = cnode->get_line();
        }
        else
        {
            cube::Cnode*  cnode  = static_cast<cube::Cnode*>( item->getCubeObject() );
            cube::Region* region = cnode->get_callee();
            mod   = QString::fromStdString( region->get_mod() );
            line  = region->get_begn_ln();
            line2 = region->get_end_ln();
        }
    }
    else if ( item->getType() == REGIONITEM )
    {
        cube::Region* region = static_cast<cube::Region*>( item->getCubeObject() );
        mod   = QString::fromStdString( region->get_mod() );
        line  = region->get_begn_ln();
        line2 = region->get_end_ln();
    }

    if ( mod.isEmpty() )
    {
        mod = "Undefined";
    }
    if ( line == -1 )
    {
        lineStr = "Undefined";
    }
    else
    {
        lineStr = QString::number( line );
    }
    if ( line2 == -1 )
    {
        line2Str = "Undefined";
    }
    else
    {
        line2Str = QString::number( line2 );
    }


    QString text;
    text.append( "\nModule:          " );
    text.append( mod );
    text.append( "\n\nStarts at line:  " );
    text.append( lineStr );
    if ( modelInterface->getTree()->getTreeType() == CALLFLAT || takeCallee )
    {
        text.append( "\n\nEnds at line:    " );
        text.append( line2Str );
    }
    text.append( "\n" );

    Globals::setStatusMessage( text, Information );
}
// end of location()

QString
CallTreeView::getContextDescription( TreeItem* item )
{
    cube::Cnode* cnode = static_cast<cube::Cnode* >( item->getCubeObject() );
    if ( !cnode )
    {
        return "";
    }
    cube::Region* region = cnode->get_callee();

    std::string descr = "Region name:    " + region->get_name() + "\n";
    descr = descr + "Mangled name:   " + region->get_mangled_name() + "\n\n";

    descr = descr + region->get_descr() + "\n";

    stringstream sstr;
    sstr << cnode->get_id();
    string _str_cnode_id;
    sstr >> _str_cnode_id;

    stringstream sstr1;
    string       _str_beg_ln;
    if ( region->get_begn_ln() != -1 )
    {
        sstr1 << region->get_begn_ln();
        sstr1 >> _str_beg_ln;
    }
    else
    {
        _str_beg_ln = "undefined";
    }

    stringstream sstr2;
    string       _str_end_ln;
    if ( region->get_end_ln() != -1 )
    {
        sstr2 << region->get_end_ln();
        sstr2 >> _str_end_ln;
    }
    else
    {
        _str_end_ln = "undefined";
    }
    descr = descr + "Call path ID:   " + _str_cnode_id + "\n";
    descr = descr + "Beginning line:   " + _str_beg_ln + "\n";
    descr = descr + "Ending line:   " + _str_end_ln + "\n";
    descr = descr + "Paradigm:   " + region->get_paradigm() + "\n";
    descr = descr + "Role:   " + region->get_role() + "\n";
    descr = descr + "Source file:   " + region->get_mod() + "\n";
    descr = descr + "Url:   " + region->get_url() + "\n";
    // descr = descr + "Description:   " + _region->get_descr();

    QString callerMod = QString::fromStdString( cnode->get_mod() );
    if ( callerMod.isEmpty() )
    {
        descr = descr + "Caller info:   not available";
    }
    // todo write caller info

    return QString( descr.c_str() );
}
