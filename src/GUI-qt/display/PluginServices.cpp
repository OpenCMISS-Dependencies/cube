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


#include <QDebug>
#include <QStatusBar>

#include <assert.h>
#include "CubeServices.h"
#include "PluginServices.h"
#include "PluginManager.h"
#include "TabInterface.h"
#include "MetricTree.h"
#include "CallTree.h"
#include "SystemTree.h"
#include "ValueWidget.h"
#include "Globals.h"
#include "TreeView.h"
#include "SystemTreeView.h"
#include "TabWidget.h"
#include "CubePlugin.h"
#include "PluginList.h"


using namespace std;


PluginServices::PluginServices( PluginManager* pm,
                                int            index )
{
    this->pm               = pm;
    this->index            = index;
    this->currentTabWidget = 0;
    this->settingsHandler  = 0;
}

PluginServices::~PluginServices()
{
    removeMarker();

    foreach( TreeItemMarker * marker, markerList )
    {
        pm->tabManager->removeStaticMarker( marker );
        delete marker;
    }
}

void
PluginServices::addTab( DisplayType type, TabInterface* tab, int addAtIndex )
{
    assert( pm->tabManager != NULL );

    if ( type == SYSTEM )
    {
        currentTabWidget = pm->tabManager->getTab( type );
        currentTabWidget->addPluginTab( tab, addAtIndex );
        if ( addAtIndex != -1 )
        {
            currentTabs.insert( addAtIndex, tab );
        }
        else
        {
            currentTabs.append( tab );
        }
        CubePlugin* plugin = PluginList::getCubePlugin( index );
        tab->widget()->setWhatsThis( plugin->getHelpText() );
    }
    else
    {
        qDebug() << "plugins for METRICTAB and CALLTAB are not implemented yet";
    }
}

void
PluginServices::enableTab( TabInterface* tab, bool enabled )
{
    if ( currentTabWidget ) // if the plugin hasn't (yet) created a tab, this call is ignored
    {
        currentTabWidget->enableTab( tab, enabled );
    }
}

void
PluginServices::closeTabs()
{
    foreach( TabInterface * ti, currentTabs )
    {
        currentTabWidget->removePluginTab( ti );
    }
}

void
PluginServices::clearValueWidget()
{
    if ( !currentTabWidget )
    {
        qDebug() << "Warning: clearValueWidget is only available for tabs";
        return;
    }
    currentTabWidget->getValueWidget()->clear();
}


void
PluginServices::updateValueWidget( double minValue, double maxValue, double selectedValue, double minAbsValue, double maxAbsValue, double absValue, bool intType, bool userDefinedMinMaxValues, double _mean, double _variance )
{
    if ( !currentTabWidget )
    {
        qDebug() << "Warning: updateValueWidget is only available for tabs";
        return;
    }
    currentTabWidget->getValueWidget()->update( minValue, maxValue, selectedValue, minAbsValue, maxAbsValue, absValue, intType, userDefinedMinMaxValues, _mean, _variance );
}

void
PluginServices::updateValueWidget( double minValue, double maxValue, double selectedValue, bool intType, bool userDefinedMinMaxValues, double _mean, double _variance )
{
    if ( !currentTabWidget )
    {
        qDebug() << "Warning: updateValueWidget is only available for tabs";
        return;
    }
    currentTabWidget->getValueWidget()->update( minValue, maxValue, selectedValue, intType, userDefinedMinMaxValues, _mean, _variance );
}

void
PluginServices::updateValueWidget( double minValue, double maxValue, bool intType, bool userDefinedMinMaxValues, double _mean, double _variance )
{
    if ( !currentTabWidget )
    {
        qDebug() << "Warning: updateValueWidget is only available for tabs";
        return;
    }
    currentTabWidget->getValueWidget()->update( minValue, maxValue, intType, userDefinedMinMaxValues, _mean, _variance );
}

void
PluginServices::addSettingsHandler( SettingsHandler* s )
{
    settingsHandler = s;
}

void
PluginServices::addColorMap( ColorMap* map )
{
    pm->colormaps.insert( this, map );
}

QAction*
PluginServices::addContextMenuItem( TreeType type, const QString& menuItemText )
{
    assert( pm->cube );

    Tree*     tree = getTree( type );
    TreeView* view = pm->tabManager->getView( type );

    QAction* newItem = new QAction( menuItemText, tree );
    view->insertPluginMenuItem( newItem );

    return newItem;
}


QMenu*
PluginServices::enablePluginMenu()
{
    assert( pm->cube );

    assert( index >= 0 );
    QMenu* parentMenu = pm->pluginMenuList.at( index );
    parentMenu->menuAction()->setVisible( true );

    return parentMenu;
}

void
PluginServices::addToolBar( QToolBar* toolbar )
{
    assert( pm->cube );
    pm->mainWindow->addToolBar( toolbar );
    pm->toolBarHash.insert( this, toolbar );
    toolbar->setVisible( true );
}

void
PluginServices::removeToolBar( QToolBar* toolbar )
{
    assert( pm->cube );
    pm->mainWindow->removeToolBar( toolbar );
    pm->toolBarHash.remove( this, toolbar );
}

cube::Cube*
PluginServices::getCube() const
{
    return pm->cube;
}

const QString&
PluginServices::getCubeFileName() const
{
    return pm->cubeFileName;
}

QString
PluginServices::getCubeBaseName() const
{
    assert( pm->cube );
    return QString( cube::services::get_cube_name( getCubeFileName().toStdString() ).c_str() );
}

QString
PluginServices::getStatName() const
{
    string cubeFileName = getCube()->get_cubename();
    string statFileName = getCube()->get_statistic_name();

    string statisticsFileName = "";
    if ( statFileName != "" )
    {
        statisticsFileName =  cube::services::dirname( cubeFileName ) + "/" +  statFileName;
    }
    else
    {
        statisticsFileName = cube::services::get_cube_name( cubeFileName ) + ".stat";
    }

    return QString::fromStdString( statisticsFileName );
}

QWidget*
PluginServices::getParentWidget() const
{
    return pm->mainWindow;
}

const QList<DisplayType>&
PluginServices::getOrder() const
{
    return pm->order;
}

TreeItem*
PluginServices::getSelection( TreeType type ) const
{
    assert( pm->cube );
    Tree* tree = pm->tabManager->getTree( type );

    return tree->getLastSelection();
}

const QList<TreeItem*>&
PluginServices::getSelections( TreeType type ) const
{
    assert( pm->cube );
    Tree* tree = pm->tabManager->getTree( type );
    return tree->getSelectionList();
}

TreeItem*
PluginServices::getMetricTreeItem( std::string metricId ) const
{
    assert( pm->cube );
    MetricTree* tree = static_cast<MetricTree*> ( getTree( METRICTREE ) );
    return tree->getTreeItem( metricId );
}

TreeItem*
PluginServices::getCallTreeItem( uint32_t cnodeId ) const
{
    assert( pm->cube );
    CallTree* tree = static_cast<CallTree*> ( getTree( CALLTREE ) );
    return tree->getTreeItem( cnodeId );
}

TreeItem*
PluginServices::getSystemTreeItem( uint32_t sysId ) const
{
    assert( pm->cube );
    SystemTree* tree = static_cast<SystemTree*> ( getTree( SYSTEMTREE ) );
    return tree->getTreeItem( sysId );
}

const QList<TreeItem*>&
PluginServices::getActiveSubset() const
{
    assert( pm->cube );
    SystemTreeView* view = static_cast<SystemTreeView*> ( pm->tabManager->getView( SYSTEMTREE ) );
    return view->getActiveSubset();
}
int
PluginServices::getActiveSubsetIndex() const
{
    assert( pm->cube );
    SystemTreeView* view = static_cast<SystemTreeView*> ( pm->tabManager->getView( SYSTEMTREE ) );
    return view->getActiveSubsetIndex();
}
void
PluginServices::setActiveSubset( int index )
{
    assert( pm->cube );
    SystemTreeView* view = static_cast<SystemTreeView*> ( pm->tabManager->getView( SYSTEMTREE ) );
    return view->setActiveSubset( index );
}
QStringList
PluginServices::getSubsetLabelList() const
{
    assert( pm->cube );
    SystemTreeView* view = static_cast<SystemTreeView*> ( pm->tabManager->getView( SYSTEMTREE ) );
    return view->getSubsetLabelList();
}

const QList<TreeItem* >&
PluginServices::getTreeItems( TreeType type ) const
{
    assert( pm->cube );
    return getTree( type )->getItems();
}

void
PluginServices::addMarker( const TreeItemMarker* marker, TreeItem* metric, TreeItem* call, TreeItem* system )
{
    pm->tabManager->addStaticMarker( marker, metric, call, system );
}

void
PluginServices::addMarker( TreeItem* item, const TreeItemMarker* marker, bool isDependency )
{
    item->setMarker( marker, isDependency );
    markedItemList.append( item );
}

void
PluginServices::removeMarker( TreeItem* item, const TreeItemMarker* marker )
{
    item->removeMarker( marker );
    markedItemList.removeOne( item );
}

void
PluginServices::removeMarker( TreeType type )
{
    foreach( TreeItem * item, markedItemList )
    {
        if ( item->getTree()->getTreeType() == type )
        {
            foreach( TreeItemMarker * marker, markerList )
            {
                item->removeMarker( marker );
                markedItemList.removeOne( item );
            }
        }
    }
}

void
PluginServices::removeMarker()
{
    foreach( TreeItem * item, markedItemList )
    {
        foreach( TreeItemMarker * marker, markerList )
        {
            item->removeMarker( marker );
        }
    }
    markedItemList.clear();
}

void
PluginServices::resizeTreeItemMarker( int iconHeight )
{
    foreach( TreeItemMarker * marker, markerList )
    {
        marker->setSize( iconHeight );
    }
}

const TreeItemMarker*
PluginServices::getTreeItemMarker( const QString& label, const QList<QPixmap>& icons )
{
    TreeItemMarker* marker = new TreeItemMarker( label, icons );
    markerList.append( marker );
    return marker;
}

ValueModus
PluginServices::getValueModus() const
{
    assert( currentTabWidget ); // only valid for tabs

    return currentTabWidget->getValueModus();
}


bool
PluginServices::intMetricSelected() const
{
    assert( pm->cube );
    foreach( TreeItem * item, getSelections( METRICTREE ) )
    {
        if ( !static_cast<cube::Metric*>( item->getCubeObject() )->isIntegerType() )
        {
            return false;
        }
    }
    return true;
}


/**
 * @brief PluginServices::selectItem
 * @param item the tree item which is selected/deselected
 * @param add if false, the previously selected items are deselected
 */
void
PluginServices::selectItem( TreeItem* newSelection, bool add )
{
    assert( pm->cube );
    Tree* tree     = newSelection->getTree();
    bool  deselect = false;

    if ( add ) // check if one previously implicitly (collapsed parent) selected element has been deselected
    {
        // check if selected element is child of a selected and collapsed parent
        TreeItem* parentCollapsed = 0;
        TreeItem* item            = newSelection->getParent();
        while ( item != 0 ) // get uppermost selected and collapsed parent node
        {
            if ( !item->isExpanded() && item->isSelected() )
            {
                parentCollapsed = item;
                break;
            }
            item = item->getParent();
        }
        // deselect previously implicitly selected item
        if ( parentCollapsed != 0 )
        {
            deselect = true;
            parentCollapsed->setExpanded( true );
            // All children off a selected collapsed parent item are implicitly selected.
            // If such a child gets deselected, its parents have to be expanded and
            // deselected, and it's siblings have to be selected.
            item = newSelection;
            while ( item != parentCollapsed )
            {
                item->setExpanded( true );

                // select all siblings of the deselected element
                foreach( TreeItem * sibling, item->getParent()->getChildren() )
                {
                    if ( sibling != item )
                    {
                        sibling->select( true );
                        sibling->setExpanded( false );
                    }
                }
                item = item->getParent();
            }
            // expand and deselect the parent item
            parentCollapsed->deselect();
        }

        // deselect previously explicitly selected item
        if ( tree->getSelectionList().contains( newSelection ) )
        {
            newSelection->deselect();
            deselect = true;
        }
    }
    else // deselect all previously selected items and select the new one
    {
        newSelection->select( false );
    }

    if ( !deselect ) // item has been selected
    {
        QString selection = newSelection->getLabel();
        setMessage( QString( "Selected \"" ).append( selection ) );

        TreeItem* item = newSelection->getParent();
        while ( item != NULL )
        {
            if ( !item->isExpanded() )
            {
                item->setExpanded( true );
            }
            item = item->getParent();
        }
        newSelection->select( true );
    }
}

void
PluginServices::setMessage( const QString& str, MessageType type )
{
    Globals::setStatusMessage( str, type );
}


QColor
PluginServices::getColor( double value, double minValue, double maxValue, bool whiteForZero ) const
{
    return Globals::getColor( value, minValue, maxValue, whiteForZero );
}


QString
PluginServices::formatNumber( double value, PrecisionFormat format, bool integerType ) const
{
    return Globals::formatNumber( value, integerType, format );
}

QString
PluginServices::formatNumber( double value, bool integerType ) const
{
    return Globals::formatNumber( value, integerType, FORMAT_DEFAULT );
}

// For the calltree and systemtree, TopLevelItems actually refer to the visible root nodes in CUBE.
const QList<TreeItem*>
PluginServices::getTopLevelItems( TreeType type ) const
{
    assert( pm->cube );
    return getTree( type )->getRootItem()->getChildren();
}

void
PluginServices::setGlobalValue( const QString& name, const QVariant& value, bool notifyMyself )
{
    pm->setGlobalValue( name, value, notifyMyself ? 0 : this );
}

QVariant
PluginServices::getGlobalValue( const QString& name ) const
{
    return pm->globalValues.value( name );
}



void
PluginServices::recalculateTreeItems()
{
    pm->tabManager->reinit();
}

void
PluginServices::updateTreeItems()
{
    TreeType treeType[] = { METRICTREE, CALLTREE, CALLFLAT, SYSTEMTREE };

    for ( int i = 0; i < 4; i++ )
    {
        Tree*                   tree  = getTree( treeType[ i ] );
        const QList<TreeItem*>& items = tree->getItems();
        foreach( TreeItem * item, items )
        {
            item->setValues( item->totalValue_adv, item->ownValue_adv );
        }
        updateTreeView( treeType[ i ] );
    }
}

void
PluginServices::updateTreeView( TreeType type )
{
    assert( pm->cube );
    TreeView* view = pm->tabManager->getView( type );
    return view->updateValues();
}

QTextStream&
PluginServices::debug()
{
    CubePlugin*  plugin = PluginList::getCubePlugin( index );
    QTextStream& stream = Globals::debug( plugin->name() );
    return stream;
}

void
PluginServices::getStatisticValues( TreeType type,
                                    double& minValue, double& maxValue, double& selectedValue,
                                    double& minAbsValue, double& maxAbsValue, double& absValue,
                                    double& mean, double& varianceSqrt ) const
{
    getTree( type )->getStatisticValues( minValue, maxValue, selectedValue, minAbsValue,
                                         maxAbsValue, absValue, mean, varianceSqrt );
}

bool
PluginServices::getUserDefinedMinValues( TreeType type, double& min, double& max ) const
{
    Tree* tree = getTree( type );
    min = tree->getUserDefinedMinValue();
    max = tree->getUserDefinedMaxValue();
    return tree->hasUserDefinedMinMaxValues();
}

void
PluginServices::setUserDefinedMinMaxValues( TreeType type )
{
    pm->tabManager->getView( type )->onMinMaxValues();
}

// --- private ---

Tree*
PluginServices::getTree( TreeType type ) const
{
    assert( pm->cube );

    return pm->tabManager->getTree( type );
}

// --- end private ---
