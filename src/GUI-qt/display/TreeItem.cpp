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



#include <assert.h>
#include <math.h>
#include <QDebug>

#include "CubeRegion.h"

#include "PluginManager.h"
#include "Globals.h"
#include "Tree.h"
#include "TreeView.h"
#include "TreeItem.h"
#include "AggregatedTreeItem.h"
#include "MetricTree.h"
#include "TreeItemMarker.h"

TreeItem::TreeItem( const QString& name,
                    TreeItemType   type,
                    cube::Vertex*  cube )
{
    this->name       = name;
    this->label      = name;
    this->type       = type;
    this->cubeObject = cube;

    calculated = false;

    ownValue   = 0.0;
    totalValue = 0.0;
    rootItem   = NULL;

    ownValue_adv   = NULL;
    totalValue_adv = NULL;
    singleValue    = true;

    expanded = false;
    hidden   = false;
    selected = false;
    visible  = ( parentItem == NULL );

    textExpanded   = "";
    textCollapsed  = "";
    valueExpanded  = 0.0;
    valueCollapsed = 0.0;
    colorExpanded  = Qt::white;
    colorCollapsed = Qt::white;

    parentItem = 0;
    depth      = 0;
    tree       = 0;

    // the minimal and maximal peer values for system tree items
    minValue = 0.0;
    maxValue = 0.0;
}

TreeItem::~TreeItem()
{
    if ( ownValue_adv != NULL )
    {
        ownValue_adv->Free();
    }
    if ( totalValue_adv != NULL )
    {
        totalValue_adv->Free();
    }
}

int
TreeItem::getDepth() const
{
    return depth;
}

bool
TreeItem::isCalculated() const
{
    return calculated;
}

bool
TreeItem::isExpanded() const
{
    return expanded;
}

Tree*
TreeItem::getTree() const
{
    return tree;
}

cube::Vertex*
TreeItem::getCubeObject() const
{
    return cubeObject;
}


TreeItem*
TreeItem::child( int row ) const
{
    return children.value( row );
}

const QList<TreeItem*>&
TreeItem::getChildren() const
{
    return children;
}

void
TreeItem::addChild( TreeItem* item )
{
    assert( !item->isSelected() );
    assert( item->parentItem == NULL || item->parentItem == this );

    children.append( item );

    item->parentItem = this;
    item->depth      = this->getDepth() + 1;
    item->tree       = this->getTree();
}

bool
TreeItem::isHidden() const
{
    return this->hidden;
}

void
TreeItem::setHidden( bool hidden )
{
    this->hidden = hidden;
}

bool
TreeItem::isSelected() const
{
    return this->selected;
}

const QString&
TreeItem::getName() const
{
    return name;
}


const QColor&
TreeItem::getColor() const
{
    return expanded ? colorExpanded : colorCollapsed;
}


bool
TreeItem::isIntegerType() const
{
    if ( tree->getValueModus() != ABSOLUTE )
    {
        return false;
    }
    if ( tree->getTreeType() == METRICTREE )
    {   // each item has different type
        return ( static_cast<cube::Metric*> ( cubeObject ) )->isIntegerType();
    }
    else
    {   //  all tree items have the same type
        return tree->hasIntegerType();
    }
}

bool
TreeItem::isTopLevelItem() const
{
    return !( getParent() && getParent()->getParent() );
}

const TreeItem*
TreeItem::getTopLevelItem() const
{
    const TreeItem* item = this;
    while ( !item->isTopLevelItem() ) // get top level parent item
    {
        item = item->getParent();
    }
    return item;
}


void
TreeItem::getSourceInfo( QString& filename, int& startLine, int& endLine )
{
    cube::Region* region = 0;
    startLine = -1;
    endLine   = -1;
    filename.clear();

    if ( !getCubeObject() )
    {
        return;
    }

    if ( type == CALLITEM )
    {
        cube::Cnode* cnode = static_cast<cube::Cnode*>( getCubeObject() );
        region = static_cast<cube::Region*>( cnode->get_callee() );
    }
    else if ( type == REGIONITEM )
    {
        region = static_cast<cube::Region*>( getCubeObject() );
    }

    if ( region )
    {
        std::string str = region->get_mod();
        filename  = QString::fromStdString( region->get_mod() ).trimmed();
        startLine = region->get_begn_ln();
        endLine   = region->get_end_ln();
    }
    if ( ( filename == "MPI" ) || ( filename == "INTERNAL" ) || ( filename ==  "OMP" )  || ( filename == "PTHREAD" ) ||
         ( filename == "CUDA" ) || ( filename ==  "OPENCL" ) )
    {
        filename = "";
    }
}

void
TreeItem::setMarker( const TreeItemMarker* marker, bool isDependencyMarker, bool markParents )
{
    if ( isDependencyMarker )
    {
        if ( !dependencyMarkerList.contains( marker ) )
        {
            dependencyMarkerList.append( marker );
        }
    }
    else
    {
        markerList.append( marker );
    }

    if ( markParents )
    {
        TreeItem* parent = this->getParent();
        while ( parent )
        {
            if ( !parent->parentMarkerList.contains( marker ) )
            {
                parent->parentMarkerList.append( marker );
            }
            parent = parent->getParent();
        }
    }
}

void
TreeItem::removeMarker( const TreeItemMarker* marker )
{
    bool found = markerList.removeOne( marker );
    if ( found )
    {
        TreeItem* parent = this->getParent();
        while ( parent )
        {
            found  = parent->parentMarkerList.removeOne( marker );
            parent = found ? parent->getParent() : 0;
        }
    }

    dependencyMarkerList.removeOne( marker );
}

const QList<const TreeItemMarker*>&
TreeItem::getMarkerList() const
{
    return markerList;
}

double
TreeItem::getAbsoluteValue() const
{
    return expanded ? valueExpanded : valueCollapsed;
}

double
TreeItem::getValue( bool* ok ) const
{
    bool   ok_;
    double ret = getValue( getTree()->getValueModus(), ok_ );
    if ( ok )
    {
        *ok = ok_;
    }
    return ret;
}

double
TreeItem::getOwnValue() const
{
    return ownValue;
}

const cube::Value*
TreeItem::getOwnValueObject() const
{
    return ownValue_adv;
}

double
TreeItem::getTotalValue() const
{
    return totalValue;
}

const cube::Value*
TreeItem::getTotalValueObject() const
{
    return totalValue_adv;
}

/* functions for use in the corresponding model TreeModel */

const QString&
TreeItem::getLabel() const
{
    return label;
}

TreeItem*
TreeItem::getParent() const
{
    return parentItem;
}

int
TreeItem::row() const
{
    if ( parentItem )
    {
        return parentItem->children.indexOf( const_cast<TreeItem*>( this ) );
    }

    return 0;
}

TreeItemType
TreeItem::getType() const
{
    return type;
}

// private function for use in trees after new values have been calculated
void
TreeItem::setValues( cube::Value* total, cube::Value* own )
{
    if ( totalValue_adv != total && totalValue_adv != NULL )
    {
        totalValue_adv->Free();
    }
    if ( ownValue_adv != own && ownValue_adv != NULL )
    {
        ownValue_adv->Free();
    }

    totalValue_adv = total;
    ownValue_adv   = own;
    totalValue     = ( totalValue_adv != NULL ) ? totalValue_adv->getDouble() : 0.;
    ownValue       = ( ownValue_adv != NULL ) ? ownValue_adv->getDouble() : 0.;
    valueCollapsed = totalValue;
    valueExpanded  = ownValue;
    singleValue    = ( ownValue_adv != NULL ) ? ownValue_adv->singleValue() : true;
    calculated     = true;
}

bool
TreeItem::isLeaf() const
{
    return getChildren().count() == 0;
}

bool
TreeItem::isAggregatedLoopItem() const
{
    return this->cubeObject == 0;
}

bool
TreeItem::isAggregatedRootItem() const
{
    return false;
}

QList<TreeItem*>
TreeItem::getLeafs() const
{
    QList<TreeItem*> leafs;

    QList<TreeItem*> children = getChildren();
    while ( !children.isEmpty() )
    {
        TreeItem* item = children.last();
        children.removeLast();
        if ( item->getChildren().size() == 0 )
        {
            leafs.append( item );
        }
        else
        {
            foreach( TreeItem * child, item->getChildren() )
            {
                children.append( child );
            }
        }
    }

    return leafs;
}

// ======== private ===================================================================
//

void
TreeItem::removeChild( TreeItem* item )
{
    assert( item->getParent() == this );

    children.removeOne( item );
    item->parentItem = NULL;
}

/**
 * @brief returns the value of this item adapted to the given value modus
 */
double
TreeItem::getValue( ValueModus valueModus, bool& ok ) const
{
    ok = true;
    cube::Value* valueObj = ( expanded ? this->ownValue_adv : this->totalValue_adv );

    if ( !valueObj )
    {
        ok = false;
        return 0.;
    }

    // if the metric tab is on the right, then the value is undefined
    DisplayType        currentDisplay = Globals::getTabManager()->getDisplayType( getTree()->getTreeType() );
    QList<DisplayType> order          = Globals::getTabManager()->getOrder();
    foreach( DisplayType display, order )
    {
        if ( display == METRIC ) // metric is on the left or current is metric
        {
            break;
        }
        else if ( display == currentDisplay ) // metric is on the right of current one
        {
            ok = false;
            return 0;
        }
    }

    if ( valueObj->getDouble() != 0 && valueObj->isZero() )
    {
        /* For some metrics (e.g. metrics which are building a minimum) no valid exclusive value exists.
         * The zero value for a minimum metrics is the maximum negative value. In this case, the value
         * should not be displayed and be marked as invalid.
         */
        ok = false;
        return 0;
    }

    double roundThreshold = Globals::getRoundThreshold( FORMAT_TREES );

    // get the absolute value
    double value = valueObj->getDouble();

    if ( tree->getTreeType() == SYSTEMTREE && expanded && children.size() > 0 )
    {
        ok = false; // no exclusive values for system tree
        return 0;
    }

    // compute the value in the current modus
    if ( valueModus != ABSOLUTE )
    {
        double referenceValue = 0.0;

        if ( valueModus == OWNROOT )
        {
            referenceValue = this->rootItem->totalValue;
        }
        else if ( valueModus == PEER )
        {
            referenceValue = this->maxValue;
        }
        else if ( type == METRICITEM && valueModus == EXTERNAL )
        {
            MetricTree*     metricTree = static_cast<MetricTree*> ( tree );
            const TreeItem* item       = getTopLevelItem();
            cube::Metric*   metric     = static_cast<cube::Metric* > ( item->getCubeObject() );
            QString         metricName = QString::fromStdString( metric->get_uniq_name() );

            referenceValue = metricTree->getExternalReferenceValue( metricName );
        }
        else // only one reference value for the complete tree
        {
            referenceValue = tree->getValueModusReferenceValue();
        }

        if ( ok )
        {
            if ( valueModus != PEERDIST )
            {
                if ( referenceValue == 0.0 )
                {
                    if ( value != 0.0 )
                    {
                        ok = false;
                    }
                }
                else
                {
                    value = 100.0 * value / referenceValue;
                }
            }
            else // peer distribution
            {
                double referenceValue1 = this->maxValue;
                double referenceValue2 = this->minValue;
                double diff            = ( referenceValue1 - referenceValue2 );
                if ( diff == 0.0 )
                {
                    if ( value - referenceValue2 != 0 )
                    {
                        ok = false;
                    }
                    else
                    {
                        value = 0.0;
                    }
                }
                else
                {
                    value = 100.0 * ( value - referenceValue2 ) / diff;
                }
            }
        }
    }

    // round to 0.0 if very close to it
    if ( value <= roundThreshold && value >= -roundThreshold )
    {
        value = 0.0;
    }

    return value;
}

/*
 * @brief updateItem has to be called if the value of an tree item has been changed.
 * It sets the item label and its color.
 */
void
TreeItem::updateItem()
{
    // todo check ok/ok2
    bool ok = true;

    // get the value of the item in the current modus;
    // if the value is undefined, ok gets false
    double value = getValue( tree->getValueModus(), ok );

    bool   ok2            = true;
    int    hiddenChildren = 0;
    double hiddenValue    = 0.0;
    double childValue     = 0.0;
    if ( ok )
    {
        // add up hidden children's value to the own value
        for ( int i = 0; i < this->getChildren().size(); i++ )
        {
            if ( this->child( i )->isHidden() )
            {
                hiddenChildren++;
                if ( ok2 )
                {
                    childValue = child( i )->getValue( tree->getValueModus(), ok2 );
                }
                if ( ok2 )
                {
                    hiddenValue += childValue;
                }
            }
        }
    }

    if ( expanded )
    {
        value += hiddenValue;
    }

    // ---- sets the label of the item which will be displayed in the tree view
    QString text = "";
    if ( !ok && ( !expanded || hiddenChildren == 0 || !ok2 ) )
    {
        text.append( "-" );
    }
    else // value exists
    {
        if ( singleValue )
        {
            text.append( Globals::formatNumber( value, isIntegerType(), FORMAT_TREES ) );
        }
        else
        {
            cube::Value* valueObj = ( expanded ? this->ownValue_adv : this->totalValue_adv );
            text.append( valueObj->getString().c_str() );
        }
    }
    text.append( " " );
    text.append( this->name );

    // add units to the labels of the top level items of the metric tree
    if ( isTopLevelItem() && type == METRICITEM && tree->getValueModus() == ABSOLUTE )
    {
        cube::Metric* met = static_cast<cube::Metric*> ( this->cubeObject );
        QString       unit( met->get_uom().c_str() );

        if ( met->get_parent() == NULL )
        {
            if ( unit.size() > 0 )
            {
                text.append( " (" );
                text.append( unit ); // add the unit of measurement
                text.append( ")" );
            }
        }
    }


    // ----- write info about hidden children's value
    if ( hiddenChildren > 0 && ( expanded || ( this->getChildren().size() == hiddenChildren ) ) )
    {
        text.append( " (" );
        if ( ok || ( expanded && hiddenChildren > 0 && ok2 ) )
        {
            if ( value == 0.0 )
            {
                if ( hiddenValue != 0.0 )
                {
                    ok2 = false;
                }
            }
            else
            {
                hiddenValue = 100.0 * hiddenValue / value;
            }

            if ( ok2 )
            {
                text.append( Globals::formatNumber( hiddenValue, false, FORMAT_TREES ) );
                text.append( "%" );
            }
            else
            {
                text.append( "-" );
            }
        }
        else
        {
            text.append( "-" );
        }
        text.append( ")" );
    }

    this->label = text;

    // set the colors for the item
    QColor color = Globals::getColor( 0, 0, 0 );
    if ( !ok && ( !expanded || hiddenChildren == 0 || !ok2 ) )
    {
        color = Globals::getColor( 0, 0, 0 );
    }
    else if ( !tree->hasUserDefinedMinMaxValues() )
    {
        double min = 0.0;
        double max = 0.0;
        if ( tree->getValueModus() == ABSOLUTE )
        {
            max = tree->getMaxValue( this );
        }
        else
        {
            max = 100.0;
        }
        // in the case, calculated min is greater than max in its ablut value, we swap them here
        if ( fabs( min ) > fabs( max ) )
        {
            double tmp = min;
            min = max;
            max = tmp;
        }

        // coloring is only according to the absolut values of the min and max.
        color = Globals::getColor( fabs( value ), fabs( min ), fabs( max ) );
    }
    else // userDefinedMinMaxValues
    {
        // coloring is only according to the absolut values of the min and max.
        color = Globals::getColor( fabs( value ),
                                   fabs( tree->getUserDefinedMinValue() ),
                                   fabs( tree->getUserDefinedMaxValue() ) );
    }

    if ( expanded )
    {
        colorExpanded = color;
    }
    else
    {
        colorCollapsed = color;
    }
    /* if ( this->getTree()->getTreeType() == METRICTREE )
       {
         qDebug() << "TreeItem::updateItem" << this->getName() << this->getLabel();
       }*/
}
// end of displayItem()

void
TreeItem::setExpanded( bool expand )
{
    tree->expandItem( this, expand );
}

void
TreeItem::select( bool addToSelection )
{
    tree->selectItem( this, addToSelection );
}

void
TreeItem::deselect()
{
    tree->deselectItem( this );
}

// ---- methods which are called by the view, if user has made selections

void
TreeItem::setExpandedStatus( bool expanded, bool recursive )
{
    this->expanded = expanded;

    if ( recursive )
    {
        foreach( TreeItem * item, this->getChildren() )
        {
            item->setExpandedStatus( expanded, true );
        }
    }
}

void
TreeItem::setSelectionStatus( bool selected )
{
    this->selected = selected;
}


// sets the depth of this items and all its children recursively
void
TreeItem::setDepth( int value )
{
    this->depth = value;

    QList<TreeItem*> list;
    list.append( this );
    while ( !list.isEmpty() )
    {
        TreeItem* item = list.takeLast();
        foreach( TreeItem * child, item->getChildren() )
        {
            child->depth = item->depth + 1;
            list.append( child );
        }
    }
}
