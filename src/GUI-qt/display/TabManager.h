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


#ifndef TABWIDGETCONTAINER_H
#define TABWIDGETCONTAINER_H

#include <QSplitter>
#include "Constants.h"
#include "Cube.h"
#include "TabWidget.h"
#include "TreeItemMarker.h"

class Tree;
class TreeItem;

class MarkerTripletList : public QList<QList<TreeItem*> >
{
public:
    void
    addTriplet( TreeItem* m, TreeItem* c, TreeItem* s )
    {
        QList<TreeItem*> triplet;
        triplet.append( m );
        triplet.append( c );
        triplet.append( s );
        append( triplet );
    }
};

/** This class manages the 3 TabWidgets of type METRICTAB, CALLTAB and SYSTEMTAB.
 *  - it handles the tree selections, as this may have effect on other trees
 *
 */
class TabManager : public QSplitter
{
    Q_OBJECT
public:
    /**
     * creates an empty container for the three tabs with their trees.
     */
    TabManager( QWidget* parent = 0 );

    /** To be called after the settings have been processed. Ensures that at least one item is selected in
     *  all trees, connects signals and calculates all tree values */
    void
    initialize();

    /** recalculates all tree values */
    void
    reinit();

    /**
     * Initializes the container with the given cube data.
     * Creates the four trees and its views and initializes them.
     */
    void
    cubeOpened( cube::Cube* cube );
    void
    cubeClosed();

    QList<DisplayType>
    getOrder() const;
    void
    setOrder( const QList<DisplayType>& value );

    TabWidget*
    getTab( DisplayType tabType );

    Tree*
    getTree( TreeType type );

    TreeView*
    getView( TreeType type );

    // SettingsHandler interface
    void
    loadGlobalSettings( QSettings& )
    {
    };
    void
    saveGlobalSettings( QSettings& )
    {
    };
    void
    loadExperimentSettings( QSettings& );
    void
    saveExperimentSettings( QSettings& );
    QString
    settingName();

    // end SettingsHandler interface

    void
    setTreeFont( const QFont&  font,
                 const QColor& color );

    QList<TabWidget*>
    getTabWidgets();

    /** returns the tree in the active (visible) tab */
    Tree*
    getActiveTree( DisplayType tabType );

    DisplayType
    getDisplayType( TreeType treeType );

    /* repaints all tree views after colormap has been changed */
    void
    updateColors();

    void
    addStaticMarker( const TreeItemMarker* marker,
                     TreeItem*             metric,
                     TreeItem*             call,
                     TreeItem*             system );

    void
    removeStaticMarker( const TreeItemMarker* marker );

public slots:
    /** redraws all tree items with their changed properties, e.g. color, precision */
    void
    updateTreeItemProperties();

    /** redraws all tree items but doesn't recalculate their properties */
    void
    updateTreeItems();

private slots:
    void
    treeItemSelected( Tree* tree );
    void
    treeItemExpanded( TreeItem* item,
                      bool      expanded );
    void
    recalculateTree( Tree* tree );

private:
    QList<Tree*>                                     trees;       // the 4 trees: METRICTREE, CALLTREE, CALLFLAT, SYSTEMTREE
    QList<TreeView*>                                 views;       // one view for each tree
    QList<TabWidget*>                                tabWidgets;  // the three TabWidget: METRICTAB, CALLTAB, SYSTEMTAB
    QList<QAbstractItemModel*>                       proxyModels; // delete all models after cube is closed
    QMap<TreeType, DisplayType>                      typeHash;
    QList<DisplayType>                               order;
    bool                                             isInitialized; // true, if all values have been calculated
    QHash<const TreeItemMarker*, MarkerTripletList > markerHash;

    // --------------------------------------------------------------

    void
    activateStaticMarker();

    void
    createTreeAndView( TreeType    type,
                       cube::Cube* cube );
    void
    getNeighborTrees( QList<Tree*>& left,
                      QList<Tree*>& right,
                      Tree*         current );
};

#endif // TABWIDGETCONTAINER_H
