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



#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>

#include "TabWidget.h"
#include "ValueWidget.h"
#include "TreeView.h"
#include "Globals.h"
#include "TabInterface.h"
#include "Constants.h"
#include "PluginManager.h"

TabWidget::TabWidget( DisplayType type )
{
    // general initializations
    valueModus = ABSOLUTE;
    this->type = type;

    tabWidget   = new QTabWidget( this );
    valueWidget = new ValueWidget( this );
    valueCombo  = new QComboBox( this );
    valueCombo->setWhatsThis( "Each tree view has its own value mode combo, a drop-down menu above the tree, where it is possible to change the way the severity values are displayed.\n\nThe default value mode is the Absolute value mode. In this mode, as explained below, the severity values from the Cube file are displayed. However, sometimes these values may be hard to interpret, and in such cases other value modes can be applied. Basically, there are three categories of additional value modes.\n\n- The first category presents all severities in the tree as percentage of a reference value. The reference value can be the absolute value of a selected or a root node from the same tree or in one of the trees on the left-hand-side. For example, in the Own root percent value mode the severity values are presented as percentage of the own root's (inclusive) severity value. This way you can see how the severities are distributed within the tree. The value modes 2-8 below fall into this category.\n\nAll nodes of trees on the left-hand-side of the metric tree have undefined values. (Basically, we could compute values for them, but it would sum up the severities over all metrics, that have different meanings and usually even different units, and thus those values would not have much expressiveness.) Since we cannot compute percentage values based on undefined reference values, such value modes are not supported. For example, if the call tree is on the left-hand-side, and the metric tree is in the middle, then the metric tree does not offer the Call root percent mode.\n\n- The second category is available for system trees only, and shows the distribution of the values within hierarchy levels. E.g., the Peer percent value mode displays the severities as percentage of the maximal value on the same hierarchy depth. The value modes 9-10 fall into this category.\n\n- Finally, the External percent value mode relates the severity values to severities from another external Cube file.\n\nDepending on the type and position of the tree, the following value modes may be available:\n\n1) Absolute (default)\n2) Metric root percent\n3) Metric selection percent\n4) Call root percent\n5) Call selection percent\n6) System root percent\n7) System selection percent\n8) Own root percent\n9) Peer percent\n10) Peer distribution\n11) External percent" );

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setMargin( LAYOUT_MARGIN );
    layout->setSpacing( LAYOUT_SPACING );
    layout->addWidget( valueCombo );
    layout->addWidget( tabWidget );
    layout->addWidget( valueWidget );

    setLayout( layout );

    tabWidget->setMovable( true );
    tabWidget->setUsesScrollButtons( true );
    tabWidget->setIconSize( QSize( 20, 20 ) );
}

TabWidget::~TabWidget()
{
}

/**
 * @brief TabWidget::initialize is called after a cube file has been loaded
 * @param order
 */
void
TabWidget::initialize( const QList<DisplayType>& order )
{
    valueCombo->setCurrentIndex( 0 );
    connect( tabWidget, SIGNAL( currentChanged( int ) ), this, SLOT( onCurrentChanged( int ) ) );

    tabHash.clear();
    treeHash.clear();

    setOrder( order );
}

void
TabWidget::setOrder( const QList<DisplayType>& order )
{
    this->order = order;
    updateValueCombo();
}

// called when files are closed;
// clear the tab, no data is shown
//
void
TabWidget::cubeClosed()
{
    tabWidget->disconnect(); // disconnect all slots

    // remove tab elements
    while ( tabWidget->count() > 0 )
    {
        tabWidget->removeTab( 0 );
    }
    valueWidget->clear();
}

void
TabWidget::addPluginTab( TabInterface* tc, int addAtIndex )
{
    this->addTab( tc, addAtIndex );
    if ( addAtIndex != -1 )
    {
        pluginWidgets.insert( addAtIndex, tc->widget() );
    }
    else
    {
        pluginWidgets.append( tc->widget() );
    }
}

void
TabWidget::removePluginTab( TabInterface* tc )
{
    int index = tabWidget->indexOf( tc->widget() );
    tabWidget->removeTab( index );
    tabHash.remove( tc->widget() );
}

int
TabWidget::addTab( TabInterface* tc, int addAtIndex )
{
    tabHash.insert( tc->widget(), tc );
    if ( addAtIndex == -1 )
    {
        return tabWidget->addTab( tc->widget(), tc->icon(), tc->label() );
    }
    else
    {
        return tabWidget->insertTab( addAtIndex, tc->widget(), tc->icon(), tc->label() );
    }
}

int
TabWidget::addTreeTab( TreeView* treeView )
{
    treeHash.insert( treeView->widget(), treeView );
    return addTab( treeView );
}

TreeView*
TabWidget::getActiveTreeView()
{
    TreeView* treeView = 0;
    if ( treeHash.size() == 1 ) // only one tree in this tab
    {
        treeView = treeHash.values().first();
    }
    else // CALLTREE of CALLFLAT
    {
        foreach( TreeView * view, treeHash.values() )
        {
            if ( view == currentTabInterface() )
            {
                treeView = view;
                break;
            }
        }
    }
    return treeView;
}

TabInterface*
TabWidget::currentTabInterface() const
{
    return tabHash.value( tabWidget->currentWidget() );
}

TabInterface*
TabWidget::tabInterface( int idx ) const
{
    return tabHash.value( tabWidget->widget( idx ) );
}

/**************** get methods ******************/

// return the current value modus, see constants.h for the ValueModus type
//
ValueModus
TabWidget::getValueModus()
{
    return valueModus;
}

ValueWidget*
TabWidget::getValueWidget()
{
    return valueWidget;
}

DisplayType
TabWidget::getType() const
{
    return type;
}

/**********************************/
/** updates the value widget with current tree values */
void
TabWidget::updateValueWidget()
{
    double minValue;
    double maxValue;
    double selectedValue;
    double minAbsValue;
    double maxAbsValue;
    double absValue;
    double mean;
    double varianceSqrt;
    Tree*  tree                    = getActiveTreeView()->getTree();
    bool   userDefinedMinMaxValues = tree->hasUserDefinedMinMaxValues();
    bool   wasOk                   = tree->getStatisticValues( minValue, maxValue, selectedValue,
                                                               minAbsValue, maxAbsValue, absValue,
                                                               mean, varianceSqrt );

    TreeItem* item    = tree->getLastSelection();
    bool      intType = item ? item->isIntegerType() : false;

    valueWidget->clear();
    if ( !wasOk )
    {
        valueWidget->update( minValue, maxValue, intType, userDefinedMinMaxValues, mean, varianceSqrt );
    }
    else if ( tree->getValueModus() == ABSOLUTE )
    {
        valueWidget->update( minValue, maxValue, absValue, intType, userDefinedMinMaxValues, mean, varianceSqrt );
    }
    else
    {
        valueWidget->update( minValue, maxValue, selectedValue,
                             minAbsValue, maxAbsValue, absValue,
                             intType, userDefinedMinMaxValues,
                             mean, varianceSqrt );
    }
}

// Sets the value modus to the given index of valueCombo. See constants.h for the ValueModus type.
// Notifies all its tabs about the change.
void
TabWidget::setValueModus( int )
{
    if ( tabWidget->count() == 0 )
    {
        return;
    }

    this->valueModus = static_cast<ValueModus> ( valueCombo->itemData( valueCombo->currentIndex() ).toInt() );

    if ( valueModus == EXTERNAL )
    {
        emit externalValueModusSelected( this ); // first check if external value modus is valid
    }
    else
    {
        setValueModus( valueModus );
    }
}

void
TabWidget::setValueModus( ValueModus modus )
{
    if ( tabWidget->count() == 0 )
    {
        return;
    }

    int idx = valueCombo->findData( modus );
    valueCombo->setCurrentIndex( idx == -1 ? 0 : idx );

    TabInterface* current = currentTabInterface();

    // always notify SystemTreeWidget, because following tabs may refer on its values
    if ( type == SYSTEM )
    {
        TreeView* systemTree = this->getActiveTreeView();
        if ( current != systemTree )
        {
            systemTree->valueModusChanged( valueModus );
        }
    }

    current->valueModusChanged( valueModus );
    valuesChanged();
}

bool
TabWidget::isOrder( DisplayType left, DisplayType right )
{
    return order.indexOf( right ) > order.indexOf( left );
}

void
TabWidget::updateValueCombo()
{
    valueCombo->disconnect();

    ValueModus selectedModus = static_cast<ValueModus> ( valueCombo->itemData( valueCombo->currentIndex() ).toInt() );
    valueCombo->clear();

    valueCombo->addItem( tr( ABSOLUTE_NAME ), ABSOLUTE );

    if ( type == SYSTEM )
    {
        if ( isOrder( METRIC, SYSTEM ) )
        {
            valueCombo->addItem( tr( OWNROOT_NAME ), OWNROOT );
            valueCombo->addItem( tr( METRICROOT_NAME ), METRICROOT );
            valueCombo->addItem( tr( METRICSELECTED_NAME ), METRICSELECTED );
            valueCombo->addItem( tr( PEER_NAME ), PEER );
            valueCombo->addItem( tr( PEERDIST_NAME ), PEERDIST );
        }
        if ( order.indexOf( METRIC ) == 0 )
        {
            if ( isOrder( CALL, SYSTEM ) )
            {
                valueCombo->addItem( tr( CALLROOT_NAME ), CALLROOT );
                valueCombo->addItem( tr( CALLSELECTED_NAME ), CALLSELECTED );
            }
        }
    }
    else if ( type == CALL )
    {
        if ( isOrder( METRIC, CALL ) )
        {
            valueCombo->addItem( tr( OWNROOT_NAME ), OWNROOT );
            valueCombo->addItem( tr( METRICROOT_NAME ), METRICROOT );
            valueCombo->addItem( tr( METRICSELECTED_NAME ), METRICSELECTED );
        }
        if ( order.indexOf( METRIC ) == 0 )
        {
            if ( isOrder( SYSTEM, CALL ) )
            {
                valueCombo->addItem( tr( SYSTEMROOT_NAME ), SYSTEMROOT );
                valueCombo->addItem( tr( SYSTEMSELECTED_NAME ), SYSTEMSELECTED );
            }
        }
    }
    else // type == METRICTAB
    {
        valueCombo->addItem( tr( OWNROOT_NAME ), OWNROOT );
    }

    if ( order.indexOf( METRIC ) == 0 )
    {
        valueCombo->addItem( tr( EXTERNAL_NAME ), EXTERNAL );
    }

    // set selected item to previously selected if available
    int idx = valueCombo->findData( selectedModus );
    valueCombo->setCurrentIndex( idx == -1 ? 0 : idx );

    connect( valueCombo, SIGNAL( currentIndexChanged( int ) ),
             this, SLOT( setValueModus( int ) ) );
}

// update the view of the current tab element
//
/**
 * @brief TabWidget::valuesChanged
 * Notifies the currently visible tab, that the tree values have changed.
 */
void
TabWidget::valuesChanged()
{
    if ( tabWidget->count() > 0 )
    {
        // save for all inactive tabs, that values have been changed -> onCurrentChanged
        foreach( TabInterface * ti, tabHash.values() )
        {
            changeInfo.insert( ti, ti == currentTabInterface() ? false : true );
        }

        TabInterface* current = currentTabInterface();

        // always notify SystemTree first, because following tabs may refer on its values
        if ( type == SYSTEM )
        {
            TreeView* systemTree = this->getActiveTreeView();
            if ( current != systemTree )
            {
                systemTree->valuesChanged();
            }
        }

        // notify active tab
        current->valuesChanged();
    }
    updateValueWidget();
}

/** signal currentChanged() is emitted when the visible tab element has changed;
   that signal gets connected to this slot;
   parameter: index is the new index of the current tab element
 */
void
TabWidget::onCurrentChanged( int index )
{
    valueWidget->clear();

    for ( int i = 0; i < tabWidget->count(); i++ )
    {
        if ( i != index )
        {
            tabInterface( i )->setActive( false );
        }
        else
        {
            bool changedValues = changeInfo.value( tabInterface( i ) );
            if ( changedValues ) // only call, if tree values have been changes while tab has been inactive
            {
                tabInterface( i )->valuesChanged();
                changeInfo.insert( tabInterface( i ), false );
            }
            tabInterface( i )->setActive( true );
            updateValueWidget();
        }
    }

    TreeView* view = dynamic_cast<TreeView*> ( tabInterface( index )->widget() );
    if ( view )
    {
        TreeType type = view->getTree()->getTreeType();
        PluginManager::getInstance()->tabActivated( type );
    }
}

QSize
TabWidget::sizeHint() const
{
    // use sizehint of currenly visible tab, default implementation uses the maximum of all tabs
    QSize hint;
    if ( currentTabInterface() )
    {
        hint = currentTabInterface()->widget()->sizeHint();
    }
    else
    {
        hint = QSize();
    }
    hint.setWidth( hint.width() + 2 * LAYOUT_MARGIN + 2 * LAYOUT_SPACING );
    return hint;
}

/**
   enables/disables the given tab
 */
void
TabWidget::enableTab( TabInterface* tab, bool enabled )
{
    int index = tabWidget->indexOf( tab->widget() );
    if ( index >= 0 )
    {
        tabWidget->setTabEnabled( index, enabled );
    }
}


/***************** settings ***********************/
/*
   //save tab settings
   //
   void
   TabWidget::saveSettings( QSettings& settings )
   {
    QString groupName = typeName();

    settings.beginGroup( groupName );

    // save settings for each tree
    foreach( TreeWidget * tree, treeHash.values() )
    {
        tree->saveSettings( settings );
    }

    //miscellaneous
    settings.setValue( "currentIndex", currentIndex() );
    settings.setValue( "selectionSyntax", selectionSyntax );
    settings.endGroup();
   }

   //load tab settings
   //
   bool
   TabWidget::loadSettings( QSettings& settings )
   {
    QString groupName = typeName();

    settings.beginGroup( groupName );

    // load settings for each tree
    foreach( TreeWidget * tree, treeHash.values() )
    {
        tree->loadSettings( settings );
    }

    //miscellaneous
    setCurrentIndex( settings.value( "currentIndex", 0 ).toInt() );
    selectionSyntax = ( SelectionSyntax )( settings.value( "selectionSyntax", 0 ).toInt() );
    settings.endGroup();

    valuesChanged();

    return true;
   }

 */
/************ miscellaneous **************/
