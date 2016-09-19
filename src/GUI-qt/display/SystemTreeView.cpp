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


#include <QInputDialog>
#include <QLineEdit>
#include "SystemTreeView.h"
#include "SystemTree.h"
#include "Globals.h"

SystemTreeView::SystemTreeView( TreeModelInterface* model,
                                const QString&      tabLabel ) : TreeView( model, tabLabel )
{
    subsetCombo = new QComboBox();
    subsetCombo->setModel( &subsetModel );

    subsetCombo->setWhatsThis(
        tr( "The Boxplot uses the currently selected subset of threads when determining its statistics."
            " Other defined subsets can be chosen from the combobox menu, such as \"All\" threads or"
            " \"Visited\" threads for only threads that visited the currently selected callpath."
            " Additional subsets can be defined from the System Tree with the \"Define subset\" context menu"
            " using the currently slected threads via multiple selection (control + left mouseclick)"
            " or with the \"Find items\" context menu selection option." ) );

    splitter = new QSplitter( Qt::Vertical );
    splitter->addWidget( this );
    splitter->addWidget( subsetCombo );

    QList<int> sizeList;
    sizeList << splitter->size().height() << 1;
    splitter->setSizes( sizeList );

    initializeCombo();
    createContextMenuActions();

    connect( this, SIGNAL( definedSubsetsChanged( const QString & ) ),
             this, SLOT( fillSubsetCombo( const QString & ) ) );
}

void
SystemTreeView::setActive( bool active )
{
    if ( active )
    {
        connect( subsetCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( updateSubsetMarks() ) );
        connect( subsetCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( updateSubsetModel() ) );
    }
    else
    {
        subsetCombo->disconnect();
    }
    modelInterface->getTree()->setActive( active );
}

void
SystemTreeView::valuesChanged()
{
    updateSubsetModel();
    updateSubsetMarks();
}

/**
   mark the items of the subset in the tree
 */
void
SystemTreeView::updateSubsetMarks()
{
    QModelIndexList list;
    if ( subsetCombo->currentIndex() != 0 ) // do not mark if "All" is selected
    {
        foreach( TreeItem * item, getActiveSubset() )
        {
            QModelIndex idx = modelInterface->find( item );
            list.append( idx );
        }
    }
    modelInterface->setFoundItems( list );
}

QWidget*
SystemTreeView::widget()
{
    return splitter;
}

void
SystemTreeView::initializeCombo()
{
    numberOfLeafs = 0;

    foreach( TreeItem * item, modelInterface->getTree()->getItems() )
    {
        if ( item->getChildren().size() == 0 )
        {
            numberOfLeafs++;
        }
    }
    fillSubsetCombo();
    resetSubsetCombo();
}


/**
 * @brief SystemTreeView::fillSubsetCombo
 * @param name, if not empty, the corresponding item will be selected
 */
void
SystemTreeView::fillSubsetCombo( const QString& name )
{
    QStringList list;

    QString all = "All (" + QString::number( numberOfLeafs ) + " elements)";

    list << all;
    list << "Visited";

    int idx = -1;

    QStringList keys = definedSubsets.keys();
    keys.sort();
    for ( int i = 0; i < keys.size(); ++i )
    {
        uint size = definedSubsets.value( keys.at( i ) ).size();
        list << keys.at( i ) + " (" + QString::number( size ) + " elements)";
        if ( !name.isEmpty() && keys.at( i ) == name )   // select new item
        {
            idx = list.size() - 1;
        }
    }

    subsetModel.setStringList( list );
    if ( idx >= 0 )
    {
        subsetCombo->setCurrentIndex( idx );
    }
}


/**
   updates the number of processes/threads for the visited subset, if visited is active
 */
void
SystemTreeView::updateSubsetModel()
{
    QString name = "Visited";
    int     idx  = subsetCombo->findText( name, Qt::MatchContains );
    if ( idx == subsetCombo->currentIndex() ) // "Visited" is visible
    {
        visitedSubset = ( static_cast<SystemTree*>( modelInterface->getTree() ) )->getVisitedItems();
        QString txt = name + " (" + QString::number( visitedSubset.size() ) + " elements)";
        subsetModel.setData( subsetModel.index( idx ), txt );
    }
}


/**
 * @brief TreeWidget::getActiveSubset
 * @return the TreeWidgetItems which belong the the currently selected subset
 */
const QList<TreeItem*>&
SystemTreeView::getActiveSubset()
{
    updateSubsetModel(); // update visited subset because TreeView may be not visible/active

    QString name = getActiveSubsetLabel();
    if ( name == "All" )
    {
        return modelInterface->getTree()->getItems();
    }
    else if ( name == "Visited" )
    {
        return visitedSubset;
    }
    else
    {
        return definedSubsets[ name ];
    }
}

void
SystemTreeView::setActiveSubset( int index )
{
    if ( index != subsetCombo->currentIndex() )
    {
        subsetCombo->setCurrentIndex( index );
    }
}

QStringList
SystemTreeView::getSubsetLabelList() const
{
    return subsetModel.stringList();
}

int
SystemTreeView::getActiveSubsetIndex() const
{
    return subsetCombo->currentIndex();
}

/**
 * @brief return current subset label, removes additional description
 */
QString
SystemTreeView::getActiveSubsetLabel() const
{
    QString name = subsetCombo->currentText();
    name.remove( QRegExp( "\\s*\\([0-9]* elements\\)$" ) );
    return name;
}

void
SystemTreeView::resetSubsetCombo()
{
    subsetCombo->setCurrentIndex( 0 );
}

QString
SystemTreeView::getContextDescription( TreeItem* item )
{
    QString description = "";

    if ( item->getType() == SYSTEMTREENODEITEM )
    {
        cube::SystemTreeNode* sys = static_cast<cube::SystemTreeNode* >( item->getCubeObject() );
        description = QString( sys->get_desc().c_str() );
    }

    return description;
}

void
SystemTreeView::createContextMenuActions()
{
    defineSubsetAct = new QAction( tr( "Define subset" ), this );
    defineSubsetAct->setStatusTip( tr( "Define a named subset with all selected items" ) );
    connect( defineSubsetAct, SIGNAL( triggered() ), this, SLOT( defineSubset() ) );
}

void
SystemTreeView::fillContextMenu()
{
    contextMenu->addAction( contextMenuHash.value( TreeItemInfo ) );
    contextMenu->addSeparator();
    contextMenu->addAction( contextMenuHash.value( ExpandMenu ) );
    contextMenu->addSeparator();
    contextMenu->addAction( contextMenuHash.value( FindItems ) );
    contextMenu->addAction( contextMenuHash.value( ClearFound ) );
    contextMenu->addAction( contextMenuHash.value( SortingMenu ) );
    contextMenu->addSeparator();
    contextMenu->addAction( defineSubsetAct );
    contextMenu->addSeparator();
    contextMenu->addAction( contextMenuHash.value( UserDefinedMinMax ) );
    contextMenu->addSeparator();
    contextMenu->addAction( contextMenuHash.value( CopyClipboard ) );
    contextMenu->addSeparator();
}

void
SystemTreeView::defineSubset()
{
    QModelIndexList  list = this->selectedIndexes();
    QList<TreeItem*> selectedList;
    foreach( QModelIndex listIndex, list )
    {
        selectedList.append( modelInterface->getTreeItem( listIndex ) );
    }

    if ( selectedList.size() < 3 )
    {
        Globals::setStatusMessage( "At least 3 selected items are necessary to define a subset", Warning );
        return;
    }

    // ok gets false if the user canceled the input
    bool    ok;
    QString name = QInputDialog::getText( this, tr( "Subset" ),
                                          tr( "Define named subset" ), QLineEdit::Normal,
                                          "", &ok );

    if ( name.length() > 0 )
    {
        definedSubsets.insert( name, selectedList );
        emit definedSubsetsChanged( name );
    }
}
