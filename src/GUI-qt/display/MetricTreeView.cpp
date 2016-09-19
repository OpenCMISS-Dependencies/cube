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


#include "MetricTreeView.h"
#include "CallTree.h"
#include "Globals.h"
#include "NewDerivatedMetricWidget.h"

MetricTreeView::MetricTreeView( TreeModelInterface* model,
                                const QString&      tabLabel ) : TreeView( model, tabLabel )
{
    createContextMenuActions();
}

/**
 * @brief MetricTreeView::selectionIsValid checks if newItem can be added to the current selection
 * @param oldItems previously selected tree items
 * @param newItem item that should become selected
 * @return true, if the item can be added to the current selection
 */
bool
MetricTreeView::selectionIsValid( QList<TreeItem*>& oldItems, TreeItem* newItem )
{
    bool    sameUnit = true;
    QString first_uom, second_uom;
    for ( int i = 0; i < oldItems.size() && sameUnit; i++ )
    {
        const TreeItem* oldItem = oldItems.at( i )->getTopLevelItem();
        first_uom  = QString::fromStdString( ( ( cube::Metric* )( oldItem->getCubeObject() ) )->get_uom() );
        second_uom = QString::fromStdString( ( ( cube::Metric* )( newItem->getCubeObject() ) )->get_uom() );
        if ( first_uom != second_uom )
        {
            sameUnit = false;
            break;
        }
    }
    if ( !sameUnit )
    {
        QString message( "Multiple metric selection is possible only if the unit of measurement is compatible. \nIn this case \"" );
        message += first_uom;
        message += "\" is incompatible with \"";
        message += second_uom;
        message += "\"";
        Globals::setStatusMessage( message, Error );
        return false;
    }

    bool sameRoot = true;
    for ( int i = 0; i < oldItems.size() && sameRoot; i++ )
    {
        const TreeItem* first  = oldItems.at( i )->getTopLevelItem();
        const TreeItem* second = newItem->getTopLevelItem();
        if ( first != second )
        {
            sameRoot = false;
            break;
        }
    }

    if ( !sameRoot )
    {
        Globals::setStatusMessage( "Be careful. Metrics with different roots might be incompatible for operation \"+\".", Warning );
    }
    return true;
}

/**
 * creates context menu items for call trees
 */
void
MetricTreeView::fillContextMenu()
{
    foreach( QAction * action, metricContextMenuHash.values() ) // enable all metric context menu actions
    {
        action->setEnabled( true );
    }

    contextMenu->addAction( contextMenuHash.value( TreeItemInfo ) );
    contextMenu->addAction( contextMenuHash.value( Documentation ) );

    contextMenu->addSeparator();
    contextMenu->addAction( contextMenuHash.value( ExpandMenu ) );

    contextMenu->addAction( contextMenuHash.value( FindItems ) );
    contextMenu->addAction( contextMenuHash.value( ClearFound ) );
    contextMenu->addAction( contextMenuHash.value( SortingMenu ) );
    contextMenu->addSeparator();
    contextMenu->addAction( contextMenuHash.value( CopyClipboard ) );
    contextMenu->addSeparator();

    QMenu* editMetricMenu    = contextMenu->addMenu( tr( "Edit metric..." ) );
    QMenu* derivedMetricMenu = editMetricMenu->addMenu( tr( "Create derived metric..." ) );
    derivedMetricMenu->addAction( metricContextMenuHash.value( CreateDerivatedChildMetric ) );
    derivedMetricMenu->addAction( metricContextMenuHash.value( CreateDerivatedRootMetric ) );
    editMetricMenu->addAction( metricContextMenuHash.value( EditDerivatedMetric ) );
    editMetricMenu->addAction( metricContextMenuHash.value( RemoveMetric ) );

    if ( multipleContext )
    {
        editMetricMenu->setEnabled( false );
    }
    if ( !contextMenuItem )
    {
        metricContextMenuHash.value( CreateDerivatedChildMetric )->setEnabled( false );
        metricContextMenuHash.value( EditDerivatedMetric )->setEnabled( false );
        metricContextMenuHash.value( RemoveMetric )->setEnabled( false );
    }
}

void
MetricTreeView::createContextMenuActions()
{
    // ============ derived metrics
    // create an action for creation of the derived metric
    QAction* action = new QAction( tr( "as a child" ), this );
    action->setStatusTip( tr( "Creates a derived metric as a child of selected metric." ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( onCreateDerivatedChildMetric() ) );
    action->setWhatsThis( "Creates a derived metric as a child of selected metric. Values of this metric are calculated as an arithmetcal expression of different constants and references to another existing metrics. Derived metrics support only DOUBLE values." );
    metricContextMenuHash.insert( CreateDerivatedChildMetric, action );

    // create an action for creation of the derived metric
    action = new QAction( tr( "as a root" ), this );
    action->setStatusTip( tr( "Creates a top level derived metric." ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( onCreateDerivatedRootMetric() ) );
    action->setWhatsThis( "Creates a derived metric as a top level metric. Values of this metric are calculated as an arithmetcal expression of different constants and references to another existing metrics. Derived metrics support only DOUBLE values." );
    metricContextMenuHash.insert( CreateDerivatedRootMetric, action );

    // exit the expression of the derived metric
    action = new QAction( tr( "Edit derived metric..." ), this );
    action->setStatusTip( tr( "Shows the online description of the clicked item" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( onEditDerivatedMetric() ) );
    action->setWhatsThis( "Creates a derived metric as a child of selected metric. Values of this metric are calculated as an arithmetcal expression of different constants and references to another existing metrics. Derived metrics support only DOUBLE values." );
    metricContextMenuHash.insert( EditDerivatedMetric, action );

    // create an action for showing the online metric info of the clicked item,
    // by default disabled, it will be enabled for items for which the url is defined
    action = new QAction( tr( "Remove metric..." ), this );
    action->setStatusTip( tr( "Removes whole subtree of metrics from the cube" ) );
    connect( action, SIGNAL( triggered() ), this, SLOT( onRemoveMetric() ) );
    metricContextMenuHash.insert( RemoveMetric, action );

    action->setWhatsThis( "Removes whole subtree of metrics from the cube" );
}

void
MetricTreeView::onCreateDerivatedChildMetric()
{
    createDerivatedMetric( true );
}

void
MetricTreeView::onCreateDerivatedRootMetric()
{
    createDerivatedMetric( false );
}

void
MetricTreeView::onRemoveMetric()
{
    TreeItem* item = contextMenuItem;

    const TreeItem* root = modelInterface->getTree()->getRootItem();
    if ( root->getChildren().size() > 1 )
    {
        TreeItem *newSelection = item != root->getChildren().at( 0 ) ? root->getChildren().at( 0 ) : root->getChildren().at( 1 );
        selectItem( newSelection, false );
        modelInterface->getTree()->removeSubtree( item );
    }
    else
    {
        Globals::setStatusMessage("MetricTree: at least one metric is required");
    }
}

void
MetricTreeView::onEditDerivatedMetric()
{
    TreeItem*     item   = contextMenuItem;
    cube::Metric* metric = static_cast<cube::Metric*> ( item->getCubeObject() );
    cube::Cube*   cube   = modelInterface->getTree()->getCube();

    NewDerivatedMetricWidget* edit_metric_widget = new NewDerivatedMetricWidget( cube, metric, NULL, this );
    edit_metric_widget->exec();
    delete edit_metric_widget;
    emit recalculateRequest( getTree() );
}

void
MetricTreeView::createDerivatedMetric( bool asChild )
{
    TreeItem*     item   = contextMenuItem;
    cube::Metric* metric = NULL;
    if ( item != NULL && asChild )
    {
        metric = static_cast<cube::Metric*> ( item->getCubeObject() );
    }

    cube::Cube*               cube                     = modelInterface->getTree()->getCube();
    NewDerivatedMetricWidget* create_new_metric_widget = new NewDerivatedMetricWidget( cube, NULL, metric,  this );
    create_new_metric_widget->exec();

    cube::Metric* newMetric = create_new_metric_widget->get_created_metric();
    if ( newMetric != NULL ) // add metric to the metric tree only, if one was created
    {
        QString   name    = QString::fromStdString( newMetric->get_disp_name() );
        TreeItem* newItem = new TreeItem( name, METRICITEM, newMetric );

        if ( item == NULL || asChild == false ) // create top level metrc
        {
            modelInterface->getTree()->addSubtree( newItem );
        }
        else
        {
            modelInterface->getTree()->addSubtree( newItem, item );
        }
    }
    delete create_new_metric_widget;
    emit recalculateRequest( getTree() );
}


QString
MetricTreeView::getContextDescription( TreeItem* item )
{
    cube::Metric* _met    = static_cast<cube::Metric* >( item->getCubeObject() );
    std::string   content = _met->get_descr();
    content = content + "\n\n";

    // store the description text in the string "descr";
    // we get the description from the cube object "cubeObject" of the clicked item,
    // where we have to distinguish on its type
    std::string title;

    std::string uniq_name;
    std::string disp_name;
    std::string dtype;
    std::string uom;
    std::string val;
    std::string url;
    std::string kind;
    std::string descr;
    std::string cubepl_expression;
    std::string cubepl_init_expression;
    title = "No information";

    uniq_name              = _met->get_uniq_name();
    disp_name              = _met->get_disp_name();
    dtype                  = _met->get_dtype();
    uom                    = _met->get_uom();
    val                    = _met->get_val();
    url                    = _met->get_url();
    descr                  = _met->get_descr();
    kind                   = _met->get_metric_kind();
    cubepl_expression      = _met->get_expression();
    cubepl_init_expression = _met->get_init_expression();


    content = content +
              "Display name :  " + disp_name + "\n" +
              "Unique name :  " + uniq_name + "\n" +
              "Data type :  " + dtype + "\n" +
              "Unit of measurement :  " + uom + "\n" +
              "Value :  " + val + "\n" +
              "URL :  " + url + "\n" +
              "Kind of values :  " + kind;

    if ( !( cubepl_expression.compare( "" ) == 0 ) )
    {
        content = content + "\n" +  "CubePL expression :  " +  cubepl_expression;
    }
    if ( !( cubepl_init_expression.compare( "" ) == 0 ) )
    {
        content = content + "\n" +  "CubePL Init expression :  " +  cubepl_init_expression;
    }
    return QString( content.c_str() );
}
