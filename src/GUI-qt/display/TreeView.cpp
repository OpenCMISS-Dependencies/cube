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



#include <QAction>
#include <QRegExp>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QApplication>
#include <QDialog>
#include <QInputDialog>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QScrollBar>
#include "CubeRegion.h"

#include "TreeView.h"
#include "Tree.h"
#include "CallTree.h"
#include "TreeModel.h"
#include "TreeItem.h"
#include "PluginManager.h"
#include "Globals.h"
#include "HelpBrowser.h"
#include "Environment.h"
#include "InfoWidget.h"
#include "TreeModelProxy.h"

#include "HmiInstrumentation.h"

using namespace std;

#define TREE_COLUMN 0

InfoWidget* TreeView::infoWidget = 0;

TreeView::TreeView( TreeModelInterface* model,
                    const QString&      tabLabel ) : QTreeView()
{
    helpBrowser          = 0;
    this->icon_          = QIcon();
    this->tabLabel       = tabLabel;
    this->modelInterface = model;
    this->delegate       = new TreeItemDelegate();
    this->setModel( model->getModel() );
    this->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    this->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    this->setSelectionMode( QAbstractItemView::ExtendedSelection );
    this->header()->hide();

    this->setItemDelegate( delegate );

    connect( this, SIGNAL( expanded( QModelIndex ) ), this, SLOT( itemExpanded( QModelIndex ) ) );
    connect( this, SIGNAL( collapsed( QModelIndex ) ), this, SLOT( itemCollapsed( QModelIndex ) ) );

    // context menu
    this->setContextMenuPolicy( Qt::CustomContextMenu );
    connect( this, SIGNAL( customContextMenuRequested( const QPoint & ) ),
             this, SLOT( onCustomContextMenu( const QPoint & ) ) );
    contextMenuItem = 0;
    contextMenu     = 0;
    generateContextActions();
    // end context menu

    this->setUniformRowHeights( true ); // improves performance

    // set the column width to fit the contents and set correct scrollbars
    this->header()->setStretchLastSection( false );
#if QT_VERSION >= 0x050000
    this->header()->setSectionResizeMode( QHeaderView::ResizeToContents );
#else
    this->header()->setResizeMode( QHeaderView::ResizeToContents );
#endif
}

TreeView::~TreeView()
{
    foreach( QAction * action, pluginActions )
    {
        delete action;
    }
    delete contextMenu;
    delete delegate;
}

bool
TreeView::selectionIsValid( QList<TreeItem*>&, TreeItem* )
{
    return true;
}

/** overwrite mousePressEvent to disable selection of tree items with the right mouse button
 * right mouse button should only pop up the context menu
 */
void
TreeView::mousePressEvent( QMouseEvent* event )
{
    if ( event->button() != Qt::RightButton )
    {
        // also ignore mouse clicks on white space (index is not valid)
        QModelIndex index = this->indexAt( event->pos() );
        if ( index.isValid() )
        {
            QTreeView::mousePressEvent( event );
        }
    }
}

// ----------- public slots --------------------
/** selects the given item in model and view */
void
TreeView::selectItem( TreeItem* item, bool addToSelection )
{
    QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::Select;
    if ( !addToSelection )
    {
        flags |= QItemSelectionModel::Clear;
    }
    QPersistentModelIndex index = QPersistentModelIndex( modelInterface->find( item ) );
    selectionModel()->select( QItemSelection( index, index ), flags );
}

/**
 * @brief TreeView::selectionChanged is a virtual function from QTreeView, that is connected to the selectionChanged signal
 * of QItemSelectionModel.
 * @param iselected the new selection
 * @param ideselected the previous selection
 */
void
TreeView::selectionChanged( const QItemSelection& iselected, const QItemSelection& ideselected )
{
    START_TRANSITION();

    QTreeView::selectionChanged( iselected, ideselected );
    QModelIndexList allSelected = this->selectedIndexes(); // all selected items
    QModelIndexList selected    = iselected.indexes();     // recently selected items
    QModelIndexList deselected  = ideselected.indexes();

    if ( allSelected.isEmpty() )  // at least one item has to be selected -> don't allow to deselect solely selected item
    {
        QModelIndex index = deselected.last();
        selectionModel()->blockSignals( true );
        selectionModel()->select( QItemSelection( index, index ), QItemSelectionModel::Select );
        selectionModel()->blockSignals( false );
        allSelected.append( index );
        return;
    }

    if ( Globals::isExperimental() && getTree()->getTreeType() == METRICTREE )
    {
        foreach( QModelIndex idx, deselected )
        {
            TreeItem*     item   = modelInterface->getTreeItem( idx );
            cube::Metric* metric = static_cast<cube::Metric*>( item->cubeObject );
            getTree()->getCube()->dropAllRowsInMetric( metric );
            qDebug() << "drop unselected metric " << item->getLabel();
        }
    }

    // update status line with selection info
    if ( selected.size() > 0 )
    {
        TreeItem* item = modelInterface->getTreeItem( allSelected.last() );
        Globals::setStatusMessage( QString( "Selected \"" ).append( item->getName() ).append( "\"" ) );
    }
    else if ( deselected.size() > 0 )
    {
        TreeItem* item = modelInterface->getTreeItem( deselected.last() );
        Globals::setStatusMessage( QString( "Deselected \"" ).append( item->getName() ).append( "\"" ) );
    }

    bool invalidSelection = false;
    // check if selection is valid, otherwise unselect recently selected item
    // invalid selections are for example metrics with different units
    if ( selected.size() > 0 && allSelected.size() > 1 )
    {
        QList<TreeItem*> oldSelections = getTree()->getSelectionList();
        foreach( QModelIndex index, selected )
        {
            TreeItem* newSelection = modelInterface->getTreeItem( index );
            if ( !selectionIsValid( oldSelections, newSelection ) )
            {
                invalidSelection = true;
                selected.removeOne( index );
                allSelected.removeOne( index );
                selectionModel()->blockSignals( true );
                selectionModel()->select( QItemSelection( index, index ), QItemSelectionModel::Deselect ); // emits selectionChanged
                selectionModel()->blockSignals( false );
            }
            else
            {
                oldSelections.append( newSelection );
            }
        }
    }

    QList<TreeItem*> selectedList;
    foreach( QModelIndex listIndex, allSelected )
    {
        TreeItem* item = modelInterface->getTreeItem( listIndex );
        selectedList.append( item );
        // update info widget, if visible
        if ( infoWidget && infoWidget->isVisible() )
        {
            updateInfoWidget( item );
        }
    }

    // update internal list of selected items: deselect all items and select all items which are selected in the view
    getTree()->setSelectionList( selectedList );

    emit selectionChanged( getTree() ); // notify tab manager to recalculate tree values

    // notify plugins
    if ( !invalidSelection )
    {
        TreeItem* item = modelInterface->getTreeItem( allSelected.last() );
        getTree()->setLastSelection( item );
        PluginManager::getInstance()->treeItemIsSelected( item->getTree()->getTreeType(), item );
    }

    END_TRANSITION();
}

QSize
TreeView::sizeHint() const
{
    // return width of the tree drawing + scollbar width
    int scrollBarWidth = this->verticalScrollBar()->width();
    return QSize( sizeHintForColumn( TREE_COLUMN ) + scrollBarWidth, 1 );
}

void
TreeView::paintEvent( QPaintEvent* event )
{
    // set TreeItemMarker icons to correct height
    QFontMetrics metrics = QFontMetrics( font() );
    int          height  = metrics.ascent() + metrics.descent();
    PluginManager::getInstance()->resizeTreeItemMarker( height );

    QTreeView::paintEvent( event );
}

void
TreeView::deselectItem( TreeItem* item )
{
    QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::Deselect;

    selectItem( item, flags );
}

void
TreeView::expandItem( TreeItem* item, bool expand )
{
    QModelIndex index = modelInterface->find( item );
    setExpanded( index, expand );
}

// ----------- end public slots --------------------

void
TreeView::valueModusChanged( ValueModus modus )
{
    getTree()->valueModusChanged( modus );
    modelInterface->updateValues();
}

void
TreeView::setActive( bool active )
{
    if ( active )
    {
        emit recalculateRequest( getTree() );
    }
}

Tree*
TreeView::getTree() const
{
    return modelInterface->getTree();
}

/**
 * called if the user has expanded an item
 * @param index
 */
void
TreeView::itemExpanded( QModelIndex index )
{
    START_TRANSITION();
    setCursor( Qt::WaitCursor );
    TreeItem* item = modelInterface->getTreeItem( index );
    item->setExpandedStatus( true );
    emit itemExpanded( item, true );
    setCursor( Qt::ArrowCursor );

    END_TRANSITION();
}

void
TreeView::itemCollapsed( QModelIndex parentIndex )
{
    TreeItem* item = modelInterface->getTreeItem( parentIndex );

    selectionModel()->blockSignals( true ); // don't emit signal for each deselection

    bool            childIsSelected = false;
    QModelIndexList indexList;
    // recursively deselects all children of the collapsed item
    indexList.append( parentIndex );
    while ( !indexList.isEmpty() )
    {
        QModelIndex index = indexList.takeFirst();
        if ( selectionModel()->isSelected( index ) ) // deselect selected children
        {
            childIsSelected = true;
            selectionModel()->select( QItemSelection( index, index ), QItemSelectionModel::Deselect );
        }
        // check children
        int count = model()->rowCount( index );
        for ( int i = 0; i < count; ++i )
        {
            indexList.append( index.child( i, TREE_COLUMN ) );
        }
    }
    selectionModel()->blockSignals( false );
    // select collapsed item if at least one child has been selected
    if ( childIsSelected ) // select subtree root, if tree is collapsed and at least one child is selected
    {
        selectionModel()->select( QItemSelection( parentIndex, parentIndex ), QItemSelectionModel::Select );
    }
    item->setExpandedStatus( false );
    emit itemExpanded( item, false );
}

void
TreeView::setIcon( const QIcon& icon )
{
    icon_ = icon;
}

QWidget*
TreeView::widget()
{
    return this;
}

QString
TreeView::label() const
{
    return tabLabel;
}

QIcon
TreeView::icon() const
{
    return icon_;
}

void
TreeView::updateValues()
{
    modelInterface->updateValues();
}


static QList<QVariant>
convertIndexToQVariant( QModelIndex idx )
{
    QList<QVariant> list;
    do
    {
        list.prepend( idx.row() );
        idx = idx.parent();
    }
    while ( idx.isValid() );

    return list;
}

QModelIndex
TreeView::convertQVariantToIndex( QList<QVariant> list )
{
    QModelIndex elem;
    foreach( QVariant row, list )
    {
        elem = model()->index( row.toInt(), DATA_COL, elem );
    }
    return elem;
}

// ============= settings ===========================
void
TreeView::loadExperimentSettings( QSettings& settings )
{
    if ( getTree()->getTreeType() == CALLTREE )
    {
        QModelIndex elem = convertQVariantToIndex( settings.value( "loopRootItem" ).toList() );

        if ( elem.isValid() )
        {
            TreeItem* loop = modelInterface->getTreeItem( elem );
            if ( loop )
            {
                CallTree* tree = static_cast<CallTree*> ( getTree() );
                tree->setAsLoop( loop );
                bool hidden = settings.value( "iterationsAreHidden" ).toBool();
                if ( hidden )
                {
                    tree->hideIterations();
                }
            }
        }
    }
    // traverse the tree to get the expanded items
    QList<QVariant> expandedItems = settings.value( "expandedItems" ).toList();
    foreach( QVariant var, expandedItems )
    {
        QModelIndex elem = convertQVariantToIndex( var.toList() );
        if ( elem.isValid() ) // select the item, if valid
        {
            expand( elem );
        }
    }

    // traverse the tree to get the selected items
    QList<QVariant> selectedItems = settings.value( "selectedItems" ).toList();
    QItemSelection  itemSelection;
    foreach( QVariant var, selectedItems )
    {
        QModelIndex elem = convertQVariantToIndex( var.toList() );
        if ( elem.isValid() ) // select the item, if valid
        {
            QItemSelection sel = QItemSelection( elem, elem );
            itemSelection.merge( sel, QItemSelectionModel::Select );
        }
    }
    if ( !itemSelection.isEmpty() )
    {
        selectionModel()->select( itemSelection, QItemSelectionModel::ClearAndSelect );
    }
}

void
TreeView::saveExperimentSettings( QSettings& settings )
{
    QAbstractItemModel* imodel = model();

    if ( getTree()->getTreeType() == CALLTREE )
    {
        CallTree* tree   = static_cast<CallTree*> ( getTree() );
        bool      hidden = tree->iterationsAreHidden();
        TreeItem* loop   = hidden ? tree->getAggregatedRootItem() : tree->getLoopRootItem();
        if ( loop )
        {
            QModelIndex loopIndex = modelInterface->find( loop );
            settings.setValue( "loopRootItem", QVariant( convertIndexToQVariant( loopIndex ) ) );
            settings.setValue( "iterationsAreHidden", hidden );
        }
    }

    // save expanded items
    QList<QVariant>    expandedItems;
    QList<QModelIndex> list;
    QModelIndex        root = imodel->parent( imodel->index( 0, DATA_COL ) );
    QModelIndex        idx;
    list.append( root );
    while ( !list.isEmpty() )
    {
        QModelIndex parent = list.takeFirst();
        int         rows   = imodel->rowCount( parent );
        for ( int i = 0; i < rows; ++i )
        {
            idx = imodel->index( i, DATA_COL, parent );
            if ( isExpanded( idx ) )
            {
                expandedItems.append( QVariant( convertIndexToQVariant( idx ) ) );
                list.append( idx ); // check children
            }
        }
        settings.setValue( "expandedItems", QVariant( expandedItems ) );
    }
    // end save expanded items

    // save selected items
    QList<QVariant> selectedItems;
    foreach( QModelIndex idx, selectedIndexes() )
    {
        selectedItems.append( QVariant( convertIndexToQVariant( idx ) ) );
    }
    settings.setValue( "selectedItems", QVariant( selectedItems ) );
}

// / ============= end settings =======================

// / ============= begin context menu =======================

static QString
getUrl( TreeItem* item )
{
    // we must distinguish on the item type to call the right get method
    if ( item->getCubeObject() == NULL )
    {
        return QString( "" );
    }


    if ( item->getType() == METRICITEM )
    {
        return QString::fromStdString( ( ( cube::Metric* )( item->getCubeObject() ) )->get_url() );
    }
    else if ( item->getType() == CALLITEM )
    {
        return QString::fromStdString( ( ( cube::Cnode* )( item->getCubeObject() ) )->get_callee()->get_url() );
    }
    else if ( item->getType() == REGIONITEM && item->getCubeObject() != NULL )
    {
        return QString::fromStdString( ( ( cube::Region* )( item->getCubeObject() ) )->get_url() );
    }

    return QString();
}

void
TreeView::insertPluginMenuItem( QAction* pluginAction )
{
    pluginActions.append( pluginAction );
}

/**
 * @brief TreeView::updateContextMenu enables or disables menu items depending on
 * the item the context menu is called on
 */
void
TreeView::updateContextMenu()
{
    QString description = getContextDescription( contextMenuItem );
    contextMenuHash.value( TreeItemInfo )->setEnabled( description.isEmpty() ? false : true );

    QString url = getUrl( contextMenuItem ).trimmed();
    contextMenuHash.value( Documentation )->setEnabled( url.isEmpty() ? false : true );
}

void
TreeView::onCustomContextMenu( const QPoint& point )
{
    contextMenu     = new QMenu();
    contextMenuItem = 0;
    connect( contextMenu, SIGNAL( destroyed() ), this, SLOT( contextMenuClosed() ) );

    QModelIndex index = this->indexAt( point );
    contextMenuItem = modelInterface->getTreeItem( index );

    contextIsInvalid = false; // true, if a child and its parent are selected
    multipleContext  = false; // true, if the contextMenuItem is part of more than one selected items

    // check if context menu item is part of a group of selected items and if the selection is valid
    const QList<TreeItem*> selectedItems = getTree()->getSelectionList();
    foreach( TreeItem * sel, selectedItems )
    {
        if ( !contextIsInvalid )
        {
            TreeItem* parent = sel;
            while ( ( parent = parent->getParent() ) )
            {
                if ( selectedItems.contains( parent ) )
                {
                    contextIsInvalid = true;
                }
            }
        }
        if ( sel == contextMenuItem && selectedItems.size() > 1 )
        {
            multipleContext = true;
        }
    }

    if ( contextMenuItem ) // enable all generic context menu items
    {
        foreach( QAction * action, contextMenuHash.values() )
        {
            action->setEnabled( true );
        }
    }
    else // disable all generic context menu items that depend on choosen item
    {
        foreach( QAction * action, contextMenuHash.values() )
        {
            action->setEnabled( false );
        }
        // enable context menu actions which don't depend on a choosen item
        contextMenuHash.value( ExpandAll )->setEnabled( true );
        contextMenuHash.value( ExpandMenu )->setEnabled( true );
        contextMenuHash.value( ExpandMarked )->setEnabled( true );
        contextMenuHash.value( CollapseAll )->setEnabled( true );
        contextMenuHash.value( FindItems )->setEnabled( true );
        contextMenuHash.value( ClearFound )->setEnabled( true );
        contextMenuHash.value( CopyClipboard )->setEnabled( true );
        contextMenuHash.value( SortingMenu )->setEnabled( true );
        contextMenuHash.value( SortByName )->setEnabled( true );
        contextMenuHash.value( SortByInclusiveValue )->setEnabled( true );
        contextMenuHash.value( SortByExclusiveValue )->setEnabled( true );
        contextMenuHash.value( DisableSorting )->setEnabled( true );
        contextMenuHash.value( DynamicHiding )->setEnabled( true );
        contextMenuHash.value( NoHiding )->setEnabled( true );
    } // end context free

    fillContextMenu();

    if ( contextMenuItem )
    {
        updateContextMenu();

        // add plugin menu items
        PluginManager::getInstance()->contextMenuIsShown( getTree()->getTreeType(), contextMenuItem );
        foreach( QAction * pluginAction, pluginActions )
        {
            contextMenu->addAction( pluginAction );
        }
    }

    // QContextMenuEvent * event = new  QContextMenuEvent( QContextMenuEvent::Mouse ,point,this->mapToGlobal( point ) );

    // QApplication::postEvent(this,event);

    // show context menu
    contextMenu->exec( this->mapToGlobal( point ) );
    contextMenu->deleteLater();
}

void
TreeView::contextMenuClosed()
{
    // todo check if all menu items in fillContextMenu are deleted (if contextMenu is owner of the actions)

    // remove and delete previous context menu entries
    foreach( QAction * action, contextMenu->actions() )
    {
        contextMenu->removeAction( action );
        delete action;
    }
    contextMenu->clear();
    pluginActions.clear();
    contextMenu     = 0; // used deleteLater to free memory
    contextMenuItem = 0;
}

void
TreeView::fillContextMenu()
{
    contextMenu->addAction( contextMenuHash.value( TreeItemInfo ) );
    contextMenu->addSeparator();
    contextMenu->addAction( contextMenuHash.value( ExpandMenu ) );
    contextMenu->addSeparator();
    contextMenu->addAction( contextMenuHash.value( FindItems ) );
    contextMenu->addAction( contextMenuHash.value( ClearFound ) );
    contextMenu->addAction( contextMenuHash.value( SortingMenu ) );
    contextMenu->addSeparator();
    contextMenu->addAction( contextMenuHash.value( UserDefinedMinMax ) );
    contextMenu->addSeparator();
    contextMenu->addAction( contextMenuHash.value( CopyClipboard ) );
    contextMenu->addSeparator();
}

// / ============= begin context menu actions  =======================

/**
 * The action which are created here are used to build the context menu. Some of the actions are used for
 * all trees.
 */
void
TreeView::generateContextActions()
{
    // create an action for showing the short metric info of the clicked item,
    // by default disabled, it will be enabled for items for which the info is defined
    QAction* action = new QAction( tr( "Info" ), this );
    action->setStatusTip( tr( "Shows a short description of the clicked item" ) );
    action->setWhatsThis( "For all trees (for call trees under \"Called region\"). Gives some short information about the clicked node. Disabled if you did not click over a node or if no information is available for the clicked node." );
    connect( action, SIGNAL( triggered() ), this, SLOT( onInfo() ) );
    contextMenuHash.insert( TreeItemInfo, action );

    // ===================================================================================
    // create an action for expanding all items in the tree
    QAction* expandAllAct = new QAction( tr( "Expand all" ), this );
    expandAllAct->setStatusTip( tr( "Expand all nodes" ) );
    expandAllAct->setWhatsThis( "For all trees. Expands all nodes in the tree." );
    connect( expandAllAct, SIGNAL( triggered() ), this, SLOT( onExpandAll() ) );
    contextMenuHash.insert( ExpandAll, expandAllAct );

    // create an action for expanding all items in the subtree of the clicked item
    QAction* expandClickedAct = new QAction( tr( "Expand subtree" ), this );
    expandClickedAct->setStatusTip( tr( "Expand all nodes in the clicked subtree" ) );
    expandClickedAct->setWhatsThis( "For all trees.  Enabled only if you clicked above a node. Expands all nodes in the subtree of the clicked node (inclusively the clicked node)." );
    connect( expandClickedAct, SIGNAL( triggered() ), this, SLOT( onExpandSubtree() ) );
    contextMenuHash.insert( ExpandClicked, expandClickedAct );

    // create an action for expanding the clicked item and
    // recursively expanding the child having the largest value,
    // starting from the clicked item untill a leaf item is reached
    QAction* expandLargestAct = new QAction( tr( "Expand largest" ), this );
    expandLargestAct->setStatusTip( tr( "Expand path with largest nodes below clicked" ) );
    expandLargestAct->setWhatsThis( "For all trees.  Enabled only if you clicked above a node. Starting at the clicked node, expands its child with the largest inclusive value (if any), and continues recursively with that child until it finds a leaf. It is recommended to collapse all nodes before using this function in order to be able to see the path along the largest values." );
    connect( expandLargestAct, SIGNAL( triggered() ), this, SLOT( onExpandLargest() ) );
    contextMenuHash.insert( ExpandLargest, expandLargestAct );

    QAction* expandMarked = new QAction( tr( "Expand marked" ), this );
    expandMarked->setStatusTip( tr( "Expand all pathes with marked nodes" ) );
    expandMarked->setWhatsThis( "For all trees. Expands all child which are marked, and continues recursively with that child until it finds a leaf." );
    connect( expandMarked, SIGNAL( triggered() ), this, SLOT( onExpandMarked() ) );
    contextMenuHash.insert( ExpandMarked, expandMarked );

    // create an action for collapsing all items in the tree
    QAction* collapseAllAct = new QAction( tr( "Collapse all" ), this );
    collapseAllAct->setStatusTip( tr( "Collapse all nodes" ) );
    collapseAllAct->setWhatsThis( "For all trees. Collapses all nodes in the tree." );
    connect( collapseAllAct, SIGNAL( triggered() ), this, SLOT( onCollapseAll() ) );
    contextMenuHash.insert( CollapseAll, collapseAllAct );

    // create an action for collapsing all items in the subtree of the clicked item
    QAction* collapseClickedAct = new QAction( tr( "Collapse subtree" ), this );
    collapseClickedAct->setStatusTip( tr( "Collapse all nodes in the clicked subtree" ) );
    collapseClickedAct->setWhatsThis( "For all trees.  Enabled only if you clicked above a node. It collapses all nodes in the subtree of the clicked node (inclusively the clicked node)." );
    connect( collapseClickedAct, SIGNAL( triggered() ), this, SLOT( onCollapseSubtree() ) );
    contextMenuHash.insert( CollapseClicked, collapseClickedAct );
    // --------------------- end expand/collapse

    // creates an action for marking all items whose name contains a user-defined string
    QAction* findAct = new QAction( tr( "&Find items" ), this );
    findAct->setStatusTip( tr( "Marks all visible items containing the given text" ) );
    //  findAct->setShortcut(tr("Ctrl+F"));
    findAct->setWhatsThis( "For all trees. Opens a dialog to get a text input from the user. If the user called the context menu over an item, the default text is the name of the clicked item, otherwise it is the last text which was searched for.\n\nThe function marks by a yellow background all non-hidden nodes whose names contain the given text, and by a light yellow background all collapsed nodes whose subtree contains such a non-hidden node. The current found node, that is initialized to the first found node, is marked by a distinguishable yellow hue." );
    connect( findAct, SIGNAL( triggered() ), this, SLOT( onFindItems() ) );
    contextMenuHash.insert( FindItems, findAct );

    // removes the markings made by findAct
    QAction* removeFindAct = new QAction( tr( "Clear found items" ), this );
    removeFindAct->setStatusTip( tr( "Clears background of all items" ) );
    removeFindAct->setWhatsThis( "For all trees. Removes the background markings of the preceding find functions." );
    connect( removeFindAct, SIGNAL( triggered() ), this, SLOT( onUnmarkItems() ) );
    contextMenuHash.insert( ClearFound, removeFindAct );

    QMenu* expandMenu = new QMenu( tr( "Expand/collapse" ) );
    expandMenu->setWhatsThis( "Collapse or expand the tree." );
    expandMenu->addAction( contextMenuHash.value( ExpandAll ) );
    expandMenu->addAction( contextMenuHash.value( ExpandClicked ) );
    expandMenu->addAction( contextMenuHash.value( ExpandLargest ) );
    expandMenu->addAction( contextMenuHash.value( ExpandMarked ) );
    expandMenu->addSeparator();
    expandMenu->addAction( contextMenuHash.value( CollapseAll ) );
    expandMenu->addAction( contextMenuHash.value( CollapseClicked ) );
    contextMenuHash.insert( ExpandMenu, expandMenu->menuAction() );

    // ====================================================================================
    // create an action for copying the text of the selected item to the
    // clipboard
    QAction* copyToClipboardAct = new QAction( tr( "Copy to clipboard" ), this );
    copyToClipboardAct->setStatusTip( tr( "Copies the selected node name to the clipboard" ) );
    copyToClipboardAct->setWhatsThis( "For all trees. Copies the text of the selected node to the clipboard." );
    connect( copyToClipboardAct, SIGNAL( triggered() ), this, SLOT( onCopyToClipboard() ) );
    contextMenuHash.insert( CopyClipboard, copyToClipboardAct );

    // create an action for dynamically hiding subtrees with minor values
    QAction* dynamicHidingAct = new QAction( tr( "Dynamic hiding" ), this );
    dynamicHidingAct->setCheckable( true );
    dynamicHidingAct->setStatusTip( tr( "Dynamically hide nodes with minor total values" ) );
    dynamicHidingAct->setWhatsThis( "Not available for metric trees. This menu item activates dynamic hiding. All currently hidden nodes get shown. You are asked to define a percentage threshold between 0.0 and 100.0. All nodes whose color position on the color scale (in percent) is below this threshold get hidden. As default value, the color percentage position of the clicked node is suggested, if you right-clicked over a node. If not, the default value is the last threshold. The hiding is called dynamic, because upon value changes (caused for example by changing the node selection) hiding is re-computed for the new values. With other words, value changes may change the visibility of the nodes.\n\n During dynamic hiding, for expanded nodes with some hidden children and for nodes with all of its children hidden, their displayed (exclusive) value includes the hidden children's inclusive value. After this sum we display in brackets the percentage of the hidden children's value in it." );
    connect( dynamicHidingAct, SIGNAL( triggered() ), this, SLOT( onDynamicHiding() ) );
    contextMenuHash.insert( DynamicHiding, dynamicHidingAct );

    // create an action for showing all hidden items
    QAction* noHidingAct = new QAction( tr( "No hiding" ), this );
    noHidingAct->setCheckable( true );
    noHidingAct->setStatusTip( tr( "Swith off hiding and show all hidden items" ) );
    noHidingAct->setWhatsThis( "Not available for metric trees. This menu item deactivates any hiding, and shows all hidden nodes." );
    connect( noHidingAct, SIGNAL( triggered() ), this, SLOT( onDisableHiding() ) );
    contextMenuHash.insert( NoHiding, noHidingAct );

    noHidingAct->setChecked( true );
    QActionGroup* hidingGroup = new QActionGroup( this );
    hidingGroup->addAction( dynamicHidingAct );
    hidingGroup->addAction( noHidingAct );

    // --- info actions
    // create an action for showing the online info for the region of the clicked item,
    // by default disabled, it will be enabled for items for which the url is defined
    action = new QAction( tr( "Documentation" ), this );
    action->setStatusTip( tr( "Shows the documentation of the clicked item" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( onShowDocumentation() ) );
    action->setWhatsThis( "For metric trees, flat call profiles, and call trees. Shows some (usually more extensive) online description for the callee of the clicked node. Disabled if no node is clicked or if no online information is available." );
    contextMenuHash.insert( Documentation, action );

    TreeModelProxy* model = dynamic_cast<TreeModelProxy*> ( this->model() );
    if ( model ) // Sorting is only available for models that inherit from TreeModelProxy
    {
        QActionGroup* sortingGroup = new QActionGroup( this );
        QMenu*        sortMenu     = new QMenu( tr( "Sort tree items..." ) );
        contextMenuHash.insert( SortingMenu, sortMenu->menuAction() );

        action = new QAction( tr( "Sort by name (ascending)" ), this );
        copyToClipboardAct->setStatusTip( tr( "" ) );
        copyToClipboardAct->setWhatsThis( tr( "" ) );
        connect( action, SIGNAL( triggered() ), this, SLOT( onSortByName() ) );
        contextMenuHash.insert( SortByName, action );
        sortMenu->addAction( action );
        sortingGroup->addAction( action );
        action->setCheckable( true );

        action = new QAction( tr( "Sort by inclusive value (descending)" ), this );
        connect( action, SIGNAL( triggered() ), this, SLOT( onSortByInclusiveValue() ) );
        contextMenuHash.insert( SortByInclusiveValue, action );
        sortMenu->addAction( action );
        sortingGroup->addAction( action );
        action->setCheckable( true );

        action = new QAction( tr( "Sort by exclusive value (descending)" ), this );
        connect( action, SIGNAL( triggered() ), this, SLOT( onSortByExclusiveValue() ) );
        contextMenuHash.insert( SortByExclusiveValue, action );
        sortMenu->addAction( action );
        sortingGroup->addAction( action );
        action->setCheckable( true );

        action = new QAction( tr( "Disable sorting" ), this );
        copyToClipboardAct->setStatusTip( tr( "" ) );
        copyToClipboardAct->setWhatsThis( tr( "" ) );
        connect( action, SIGNAL( triggered() ), this, SLOT( onDisableSorting() ) );
        contextMenuHash.insert( DisableSorting, action );
        sortMenu->addAction( action );
        sortingGroup->addAction( action );
        action->setCheckable( true );
        action->setChecked( true );
    }
    // ====================================================================================

    action = new QAction( tr( "Min/max values" ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( onMinMaxValues() ) );
    action->setStatusTip( tr( "Sets user-defined minimal/maximal values that should correspond to the color extremes." ) );
    action->setWhatsThis( tr( "Sets user-defined minimal/maximal values that should correspond to the color extremes." ) );
    contextMenuHash.insert( UserDefinedMinMax, action );
}

/**
 * creates dialog to allow the user to enter a search string
 */
class FindDialog : public QDialog
{
public:
    FindDialog( QWidget* parent,
                bool     enableSelection ) : QDialog( parent )
    {
        this->setWindowTitle( "Find" );
        QLabel* label = new QLabel( tr( "Regular expression to search for:" ) );
        lineEdit = new QLineEdit;
        label->setBuddy( lineEdit );

        selectCheckBox = new QCheckBox( tr( "select items" ) );

        QPushButton* findButton = new QPushButton( tr( "&Find" ) );
        findButton->setDefault( true );

        QPushButton* cancelButton = new QPushButton( tr( "&Cancel" ) );
        cancelButton->setDefault( true );

        QDialogButtonBox* buttonBox = new QDialogButtonBox( Qt::Horizontal );
        buttonBox->addButton( findButton, QDialogButtonBox::ActionRole );
        buttonBox->addButton( cancelButton, QDialogButtonBox::ActionRole );

        QVBoxLayout* topLayout = new QVBoxLayout;
        this->setLayout( topLayout );
        topLayout->addWidget( label );
        topLayout->addWidget( lineEdit );

        if ( enableSelection )
        {
            selectCheckBox->setChecked( true );
            topLayout->addWidget( selectCheckBox );
        }
        topLayout->addWidget( buttonBox );

        connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
        connect( findButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    }
    QString
    getText() const
    {
        return lineEdit->text();
    }
    void
    setText( const QString& txt )
    {
        lineEdit->setText( txt );
    }
    bool
    selectItems()
    {
        return selectCheckBox->isChecked();
    }
private:
    QCheckBox* selectCheckBox;
    QLineEdit* lineEdit;
};


void
TreeView::onExpandAll()
{
    getTree()->getRootItem()->setExpandedStatus( true, true );
    expandAll();
    emit itemExpanded( getTree()->getRootItem(), true );
}

/**
 * Recursively expands or collapses the given item and all its children in this view.
 * If the item is collapsed and children of the item are selected, the item gets selected
 * and the children get deselected.
 * @param expand if true, expands items, else it collapses them
 */
void
TreeView::expandSubtree( TreeItem* item, bool expand )
{
    this->blockSignals( true );             // don't emit signal for each subitem expand event
    selectionModel()->blockSignals( true ); // don't emit signal for each deselection

    bool            childIsSelected = false;
    QModelIndex     subtree         = modelInterface->find( item );
    QModelIndexList indexList;
    indexList.append( subtree );
    while ( !indexList.isEmpty() )
    {
        QModelIndex index = indexList.takeFirst();
        this->setExpanded( index, expand );
        // if tree is collapsed, deselect all selected children and select subtree root
        if ( !expand && selectionModel()->isSelected( index ) )
        {
            childIsSelected = true;
            selectionModel()->select( QItemSelection( index, index ), QItemSelectionModel::Deselect );
        }
        // expand children
        int count = model()->rowCount( index );
        for ( int i = 0; i < count; ++i )
        {
            indexList.append( index.child( i, TREE_COLUMN ) );
        }
    }
    this->blockSignals( false );
    selectionModel()->blockSignals( false );
    if ( !expand && childIsSelected ) // select subtree root, if tree is collapsed and at least one child is selected
    {
        selectionModel()->select( QItemSelection( subtree, subtree ), QItemSelectionModel::Deselect );
    }
}

void
TreeView::onCollapseAll()
{
    // set items in model collapsed
    getTree()->getRootItem()->setExpandedStatus( false, true );
    // set items in view collapsed
    collapseAll();
    // recalculate value (if selected recalculate values of right trees)
    emit itemExpanded( getTree()->getRootItem(), false );
}

void
TreeView::onExpandSubtree()
{
    QList<TreeItem*> selectedItems;
    if ( multipleContext )
    {
        selectedItems = getTree()->getSelectionList();
    }
    else
    {
        selectedItems.append( contextMenuItem );
    }

    foreach( TreeItem * item, selectedItems )
    {
        item->setExpandedStatus( true, true );
        expandSubtree( item, true );
        emit itemExpanded( item, true );
    }
}

/**
 * recursively expands the children with the largest values in view and model
 */
void
TreeView::expandLargest( TreeItem* item )
{
    // QPersistantModelIndex is required because ModelIndex becomes invalid after item has been expanded
    QPersistentModelIndex index = QPersistentModelIndex( modelInterface->find( item ) );

    this->setExpanded( index, true ); // set expanded in view
    item->setExpandedStatus( true );  // set expanded in model

    int count = 1;
    while ( count > 0 )
    {
        TreeItem*   maxItem = 0;
        QModelIndex maxIndex;

        count = model()->rowCount( index );
        for ( int i = 0; i < count; ++i )
        {
            QModelIndex childIndex = index.child( i, TREE_COLUMN );
            TreeItem*   item       = modelInterface->getTreeItem( childIndex );
            if ( !maxItem || item->valueCollapsed > maxItem->valueCollapsed )
            {
                maxItem  = item;
                maxIndex = childIndex;
            }
        }
        // expand largest child
        index = QPersistentModelIndex( maxIndex );
        if ( maxItem )
        {
            this->setExpanded( maxIndex, true );
            maxItem->setExpandedStatus( true );
        }
    }
}

void
TreeView::onExpandLargest()
{
    QList<TreeItem*> selectedItems;
    if ( multipleContext )
    {
        selectedItems = getTree()->getSelectionList();
    }
    else
    {
        selectedItems.append( contextMenuItem );
    }

    foreach( TreeItem * item, selectedItems )
    {
        expandLargest( item );
        emit itemExpanded( item, true );
    }
}


void
TreeView::onExpandMarked()
{
    const QList<TreeItem*>& items = getTree()->getItems();
    foreach( TreeItem * item, items )
    {
        if ( item->getMarkerList().size() > 0 )
        {
            TreeItem* parent = item;
            emit      itemExpanded( parent, true );
            do
            {
                QPersistentModelIndex index = QPersistentModelIndex( modelInterface->find( parent ) );
                this->setExpanded( index, true );  // set expanded in view
                parent->setExpandedStatus( true ); // set expanded in model
                parent = parent->getParent();
            }
            while ( parent->getParent() );
        }
    }
}

void
TreeView::onCollapseSubtree()
{
    QList<TreeItem*> selectedItems;
    if ( multipleContext )
    {
        selectedItems = getTree()->getSelectionList();
    }
    else
    {
        selectedItems.append( contextMenuItem );
    }

    foreach( TreeItem * subtree, selectedItems )
    {
        // set items in model collapsed
        subtree->setExpandedStatus( false, true );
        // set items in view collapsed
        expandSubtree( subtree, false );
        // deselect all child items, select parent item
        selectItem( subtree, true );
        // recalculate value (if selected recalculate values of right trees)
        emit itemExpanded( subtree, false );
    }
}

void
TreeView::onCopyToClipboard()
{
    /** recursively find all selected items */
    QString          text;
    QList<TreeItem*> treeItems;
    treeItems.append( getTree()->getRootItem() );
    while ( !treeItems.isEmpty() )
    {
        TreeItem* item = treeItems.first();
        treeItems.removeFirst();
        if ( item->isSelected() )
        {
            text += QString( 2 * item->getDepth(), ' ' ) + item->getLabel() + "\n";
        }
        QListIterator<TreeItem*> it( item->getChildren() );
        it.toBack();
        while ( it.hasPrevious() )
        {
            treeItems.prepend( it.previous() );
        }
    }

    QApplication::clipboard()->setText( text );
}

void
TreeView::onFindItems()
{
    // pop up a dialog to enter the expression to be searched for
    TreeItem*   selected        = contextMenuItem;
    bool        enableSelection = this->getTree()->getTreeType() != METRICTREE;
    FindDialog* findDialog      = new FindDialog( this, enableSelection );

    if ( selected )
    {
        findDialog->setText( QRegExp::escape( selected->getName() ) );
    }
    bool    ok          = findDialog->exec();
    QString text        = findDialog->getText();
    bool    selectItems = findDialog->selectItems();
    if ( !ok || text.isEmpty() )
    {
        return;
    }

    QRegExp regExp( text );
    if ( !regExp.isValid() )
    {
        Globals::setStatusMessage( "The entered string is not a valid regular expression!", Warning );
        return;
    }

    // search for the regular expression in the tree
    QModelIndexList found = modelInterface->find( QRegExp( text ) );

    // select the found items, if desired
    if ( selectItems )
    {
        // selection will make the items of QModelIndex invalid => save as QPersisentModelIndex
        // before selection starts
        QList<QPersistentModelIndex> toSelect;
        foreach( QModelIndex idx, found )
        {
            toSelect.append( QPersistentModelIndex( idx ) );
        }

        this->blockSignals( true ); // don't emit signal for each selection event
        this->clearSelection();
        foreach( QPersistentModelIndex idx, toSelect )
        {
            selectionModel()->select( QItemSelection( idx, idx ), QItemSelectionModel::Select );
        }
        this->blockSignals( false );
        emit selectionChanged( getTree() );
    }
    // mark the items, items of the list are invalid after this call
    modelInterface->setFoundItems( found );
}

/**
 * @brief TreeView::selectAll selects all items with the given item name and expands their parents
 * @param itemName the item name to search for
 * @param clear if true, all previous selections are cleared
 */
void
TreeView::selectAll( const QString& itemName, bool clear )
{
    QModelIndexList found = modelInterface->find( QRegExp( itemName ) );

    // selection will make the items of QModelIndex invalid => save as QPersisentModelIndex
    // before selection starts
    QList<QPersistentModelIndex> toSelect;
    foreach( QModelIndex idx, found )
    {
        toSelect.append( QPersistentModelIndex( idx ) );
    }

    this->blockSignals( true ); // don't emit signal for each selection event

    if ( clear )
    {
        this->clearSelection();
    }
    foreach( QPersistentModelIndex idx, toSelect )
    {
        selectionModel()->select( QItemSelection( idx, idx ), QItemSelectionModel::Select );
        // expand parent items
        while ( ( idx = idx.parent() ).isValid() )
        {
            this->setExpanded( idx, true );  // set expanded in view
            TreeItem* item = modelInterface->getTreeItem( idx );
            item->setExpandedStatus( true ); // set expanded in model
        }
    }
    this->blockSignals( false );
    emit selectionChanged( getTree() );
}

void
TreeView::onUnmarkItems()
{
    modelInterface->setFoundItems( QModelIndexList() );
}

/* shows a window with information about the item on which the context menu is called */
void
TreeView::onInfo()
{
    if ( multipleContext )
    {
        const QList<TreeItem*> selectedItems = getTree()->getSelectionList();
        foreach( TreeItem * item, selectedItems )
        {
            updateInfoWidget( item );
        }
    }
    else
    {
        updateInfoWidget( contextMenuItem );
    }
    infoWidget->raise();
}

void
TreeView::updateInfoWidget( TreeItem* item )
{
    QString description = getContextDescription( item );
    if ( !description.isEmpty() )
    {
        if ( !infoWidget )
        {
            infoWidget = new InfoWidget( Globals::getMainWindow() );
        }
        infoWidget->showText( item->getName(), description );
    }
}

void
TreeView::onShowDocumentation()
{
    TreeItem* item = contextMenuItem;

    if ( item == NULL ) // if called from help menu, show information about the recently selected item
    {
        item = getTree()->getLastSelection();
    }

    QApplication::setOverrideCursor( Qt::WaitCursor );
    if ( !helpBrowser )
    {
        helpBrowser = new HelpBrowser( tr( "Documentation" ), this );
    }

    // Check whether HTTP access to online documentation should be disabled
    bool        no_http = env_str2bool( getenv( "CUBE_DISABLE_HTTP_DOCS" ) );
    bool        success = false;
    cube::Cube* cube    = item->getTree()->getCube();

    const std::vector<std::string>&     mirrors = cube->get_mirrors();
    vector<std::string>::const_iterator it      = mirrors.begin();

    QList<QUrl> mirrorList;
    while ( !success && it != mirrors.end() )
    {
        QString url = getUrl( item );
        url.replace( QString( "@mirror@" ), QString::fromStdString( *it ) );
        if ( no_http && ( url.startsWith( "http://" ) || url.startsWith( "https://" ) ) )
        {
            ++it;

            continue;
        }
        if ( url.trimmed().length() > 0 )
        {
            mirrorList.append( QUrl( url ) );
        }
        ++it;
    }
    helpBrowser->showUrl( mirrorList, QString( "No accessible mirror found" ) );
    QApplication::restoreOverrideCursor();
}


// ------------------ hiding/sorting -------------------------------------------------------------
// ------------------ only available if the used TreeModel is a QSortFilterProxyModel ------------



void
TreeView::onDynamicHiding()
{
    TreeModelProxy* model = dynamic_cast<TreeModelProxy*> ( this->model() );
    if ( model )
    {
        bool   ok;
        double newThreshold = getHidingThreshold( contextMenuItem, ok );

        if ( ok )
        {
            hidingThreshold = newThreshold;
            model->setFilter( hidingThreshold );
        }
    }
}

void
TreeView::onDisableHiding()
{
    TreeModelProxy* model = dynamic_cast<TreeModelProxy*> ( this->model() );
    if ( model )
    {
        model->disableFilter();
    }
}

double
TreeView::getHidingThreshold( TreeItem* item, bool& ok )
{
    double clickedValue;
    double defaultThreshold;

    // first determine a default (suggested) threshold for hiding; this
    // threshold is a percentual value: we will hide items whose colors
    // are under this percentage on the color scale

    if ( item != NULL )
    {
        // get the current value of the clicked item
        clickedValue = item->getValue( &ok );
        if ( ok )
        {
            // take the absolute value
            clickedValue = fabs( clickedValue );

            // if the clickedValue of the current node is near by 0, then set it to 0
            if ( clickedValue <= Globals::getRoundThreshold( FORMAT_TREES ) )
            {
                clickedValue = 0.0;
            }
            // get values corresponding to the minimal and maximak colors
            double min = 0.0;
            double max = 0.0;

            if ( getTree()->hasUserDefinedMinMaxValues() )
            {
                min = getTree()->getUserDefinedMinValue();
                max = getTree()->getUserDefinedMaxValue();
            }
            else
            {
                min = 0.0;
                if ( getTree()->getValueModus() == ABSOLUTE )
                {
                    max = getTree()->getMaxValue( item );
                }
                else
                {
                    max = 100.0;
                }
            }

            // the default threshold will be the percentual position of the
            // clicked value between the min and max values
            if ( max - min == 0.0 )
            {
                defaultThreshold = 0.0;
            }
            else
            {
                defaultThreshold = 100.0 * ( clickedValue - min ) / ( max - min );
                if ( defaultThreshold < 0.0 )
                {
                    defaultThreshold = 0.0;
                }
                else if ( defaultThreshold > 100.0 )
                {
                    defaultThreshold = 100.0;
                }
                else
                {
                    defaultThreshold += 1e-17; // todo where is this value defined ?????
                }
            }
        }
        else
        {
            defaultThreshold = hidingThreshold;
        }
    }
    else
    {
        defaultThreshold = hidingThreshold;
    }


    // get a user-defined threshold, using defaultThreshold as default
    double newThreshold;
    int    decimals = Globals::getRoundNumber( FORMAT_TREES );
    newThreshold = QInputDialog::getDouble( this, "Dynamic hiding",
                                            "Dynamically hide nodes below (less or equal) this percentage on the color scale:",
                                            defaultThreshold, 0.0, 100.0, decimals, &ok );
    return newThreshold;
}

void
TreeView::onSortByName()
{
    TreeModelProxy* model = dynamic_cast<TreeModelProxy*> ( this->model() );
    if ( model )
    {
        model->setComparator( new NameComparator() );
        // model->sort( -1 );
        model->sort( TREE_COLUMN );
    }
}

void
TreeView::onSortByInclusiveValue()
{
    TreeModelProxy* model = dynamic_cast<TreeModelProxy*> ( this->model() );
    if ( model )
    {
        model->setComparator( new InclusiveValueComparator() );
        // model->sort( -1 );
        model->sort( TREE_COLUMN );
    }
}

void
TreeView::onSortByExclusiveValue()
{
    TreeModelProxy* model = dynamic_cast<TreeModelProxy*> ( this->model() );
    if ( model )
    {
        model->setComparator( new ExclusiveValueComparator() );
        // model->sort( -1 );
        model->sort( TREE_COLUMN );
    }
}

void
TreeView::onDisableSorting()
{
    TreeModelProxy* model = dynamic_cast<TreeModelProxy*> ( this->model() );
    if ( model )
    {
        model->setComparator( 0 );
        model->sort( -1 ); // returns to the sort order of the underlying source model.
    }
}

void
TreeView::onMinMaxValues()
{
    bool   ok = true;
    double minValue, maxValue;
    bool   updateView = false;

    // ask the user if user-defined minimal and maximal values should be used
    QMessageBox::StandardButton answer =
        QMessageBox::question( this, "Min/max values", "Use user-defined minimal and maximal values for coloring?",
                               QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes );

    // if the user canceled the input then do nothing
    if ( answer == QMessageBox::Cancel )
    {
        return;
    }

    // if the user de-activated user-defined min/max values for coloring,
    // then re-display with the default coloring
    else if ( answer == QMessageBox::No )
    {
        if ( getTree()->hasUserDefinedMinMaxValues() )
        {
            getTree()->unsetUserDefinedMinMaxValues();
            updateView = true;
        }
    }
    else // user defines min/max values activated
    {
        // if the user activated user-defined coloring, then ask for the minimal and maximal values
        // for the minimal and maximal colors;
        // ok gets false if the user canceled the input, in this case do nothing

        bool   userMinMax = getTree()->hasUserDefinedMinMaxValues();
        double min        = userMinMax ? getTree()->getUserDefinedMinValue() : 0.0;
        minValue = QInputDialog::getDouble( this, "Minimal value", "Minimal value for coloring:", min, 0.0, 1e+15, 15, &ok );

        if ( ok )
        {
            assert( getTree()->getTreeType() != METRICTREE );
            double max = userMinMax ? getTree()->getUserDefinedMaxValue() : getTree()->getMaxValue();
            maxValue = QInputDialog::getDouble( this, "Maximal value", "Maximal value for coloring:",
                                                max, 0.0, 1e+15, 15, &ok );
        }

        if ( ok )
        {
            getTree()->setUserDefinedMinMaxValues( minValue, maxValue );
            userMinValue = minValue;
            userMaxValue = maxValue;
            updateView   = true;
        }
    }
    if ( updateView )
    {
        getTree()->updateItems();           // update colors
        emit selectionChanged( getTree() ); // used to update the value widget
    }
}
// end of onMinMaxValues()

// / ============= end context menu actions  =======================


// ==================================================================================================================
//  class TreeItemDelegate which paints each tree item
// ==================================================================================================================

/**
 * @brief TreeItemDelegate::sizeHint return width of the item to draw
 */
QSize
TreeItemDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    // qDebug() << "TreeItemDelegate::sizeHint" << index.data().toString() << option.rect.size() << option.fontMetrics.boundingRect( option.rect, Qt::TextDontClip, index.data().toString() ).size();
    QSize size = option.fontMetrics.boundingRect( option.rect, Qt::TextDontClip, index.data().toString() ).size();

    int iconSize = 10;
    int pad1     = 2;
    int pad2     = option.fontMetrics.height() / 2;
    int textX    = iconSize + pad1 + pad2; // start position of text = width of left icon + paddings
    size.setWidth( size.width() + textX );

    return size;
}

/** draws the tree item marker for a given tree item */
void
TreeItemDelegate::paintMarker( QPainter* painter, const QRect& textRect, TreeItem* item ) const
{
    QRect rect = textRect.adjusted( 0, 0, 0, -1 );

    // item with marker -> fill rectangle with marker color
    foreach( const TreeItemMarker * marker, item->markerList )
    {
        if ( marker->isColorVisible() )
        {
            painter->fillRect( rect, marker->getAttributes().getColor() );
            rect.adjust( 0, 0, -4, -2 );
        }
    }

    rect = textRect.adjusted( 0, 0, 0, -1 );

    bool markedAsParent = false;
    // item is parent of a marked item => draw rectangle
    if ( !item->isExpanded() && !item->parentMarkerList.isEmpty() )
    {
        const TreeItemMarker* usedMarker = 0;
        // Use marker color to outline parent item. Use black, if more than one marker is set
        foreach( const TreeItemMarker * marker, item->parentMarkerList )
        {
            if ( marker->isColorVisible() )
            {
                if ( !usedMarker || usedMarker == marker )
                {
                    painter->setPen( marker->getAttributes().getColor() );
                    usedMarker = marker;
                }
                else
                {
                    painter->setPen( Qt::black );
                    break;
                }
                markedAsParent = true;
            }
        }
        if ( markedAsParent )
        {
            painter->drawRect( rect );
            painter->setPen( Qt::black );
        }
    }

    // mark items as dependency
    const TreeItemMarker* usedMarker = 0;
    foreach( const TreeItemMarker * marker, item->dependencyMarkerList )
    {
        if ( marker->isColorVisible() || !marker->getIcon().isNull() )
        {
            if ( markedAsParent )
            {
                rect = textRect.adjusted( 1, 1, -1, -2 );
            }

            painter->setPen( Qt::white );
            painter->drawRect( rect );
            QPen pen = painter->pen();
            pen.setStyle( Qt::DashLine );
            if ( !usedMarker || usedMarker == marker ) // outline with marker color
            {
                pen.setColor( marker->getAttributes().getColor() );
                usedMarker = marker;
            }
            else // more than one marker => mark with black dash outline
            {
                pen.setColor( Qt::black );
            }
            painter->setPen( pen );
            painter->drawRect( rect );
        }
        painter->setPen( Qt::black );
    }

    // === draw optional right icon(s)
    if ( !item->markerList.isEmpty() )
    {
        int posX = rect.x() + rect.width() + iconPad;
        foreach( const TreeItemMarker * marker, item->markerList )
        {
            const QPixmap& icon = marker->getIcon();
            if ( !icon.isNull() )
            {
                int padY = ( rect.height() - icon.height() ) / 2;
                padY = ( rect.height() - icon.height() ) / 2;
                painter->drawPixmap( posX, rect.y() + padY, icon );
                posX += icon.width();
            }
        }
    }
}

/**
 * @brief TreeItemDelegate::paint paints the item with the given index
 */
void
TreeItemDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    int iconSize = option.fontMetrics.ascent() - 1;
    int pad      = option.fontMetrics.height() / 2; // space right of the icon
    int textX    = iconSize + iconPad + pad;        // start position of text = width of left icon + paddings

    // textRect = bounding rectangle of the item text and its icons
    QRect textRect = option.fontMetrics.boundingRect( option.rect, Qt::TextDontClip, index.data().toString() );
    textRect.setWidth( textRect.width() + textX );

    QBrush brush = qvariant_cast<QBrush> ( index.data( Qt::BackgroundRole ) );
    painter->fillRect( textRect, brush );

    TreeItem* item = ( TreeItem* )qvariant_cast<void*> ( index.data( TreeItemRole ) );

    paintMarker( painter, textRect, item );

    // set text/foreground color
    bool noMarkersInTree = item->getTree()->getRootItem()->parentMarkerList.size() == 0;
    bool gray            = TreeItemMarker::isGrayedOut() && item->getMarkerList().size() == 0 &&
                           item->parentMarkerList.size() == 0 && !noMarkersInTree;
    QColor textColor = gray ? Qt::lightGray : option.palette.text().color();

    // draw selected items: set background and foreground color
    if ( option.state & QStyle::State_Selected )
    {
        painter->fillRect( textRect, option.palette.highlight() );
        textColor = option.palette.highlightedText().color();
    }
    // else if ( option.state & QStyle::State_HasFocus )
    else
    {
        painter->setPen( option.palette.text().color() );
    }

    // === draw left icon

    QRect   rect = option.rect;
    QPixmap pix  = createIcon( item, iconSize, gray );
    int     padY = ( rect.height() - pix.height() ) / 2;
    painter->drawPixmap( rect.x() + iconPad, rect.y() + padY, pix );

    // === draw item text
    rect.adjust( textX, 0, 0, 0 );

    painter->setPen( textColor );
    painter->drawText( rect, Qt::TextDontClip, index.data().toString() );
}

QPixmap
TreeItemDelegate::createIcon( TreeItem* item, int iconSize, bool gray ) const
{
    QPixmap pix( iconSize, iconSize );
    QColor  fillColor   = item->getColor();
    QColor  borderColor = Qt::black;
    if ( gray )
    {
        borderColor = Qt::gray;
        fillColor.setAlpha( fillColor.alpha() / 8 );
    }
    pix.fill( fillColor );

    QPainter painter( &pix );
    painter.setPen( borderColor );
    painter.drawRect( 0, 0, iconSize - 1, iconSize - 1 );

    double prec = Globals::getRoundThreshold( FORMAT_TREES );
    if ( item->getValue() >= prec )
    {
        // for positive values draw a raised color icon
        painter.setPen( borderColor );
        painter.drawLine( iconSize - 2, 1, iconSize - 2, iconSize - 2 );
        painter.drawLine( 1, iconSize - 2, iconSize - 2, iconSize - 2 );

        painter.setPen( Qt::white );
        painter.drawLine( 1, 1, 1, iconSize - 2 );
        painter.drawLine( 1, 1, iconSize - 2, 1 );
    }
    else if ( item->getValue() < -prec )
    {
        // for negative values draw a deepened color icon
        painter.setPen( borderColor );
        painter.drawLine( 1, 1, 1, iconSize - 2 );
        painter.drawLine( 1, 1, iconSize - 2, 1 );

        painter.setPen( Qt::white );
        painter.drawLine( 1, iconSize - 2, iconSize - 2, iconSize - 2 );
        painter.drawLine( iconSize - 2, 1, iconSize - 2, iconSize - 2 );
    }
    return pix;
}

// ==================================================================================================================
