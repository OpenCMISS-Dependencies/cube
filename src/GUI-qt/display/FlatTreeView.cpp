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


#include "FlatTreeView.h"
#include "FlatTree.h"
#include "Globals.h"
#include "TabManager.h"

FlatTreeView::FlatTreeView( TreeModelInterface* modelInterface,
                            const QString&      tabLabel ) : TreeView( modelInterface, tabLabel )
{
}

void
FlatTreeView::setActive( bool active )
{
    /**
       select all entries in the flat tree, that have the same label as the selected entries
       in the call tree
     */
    if ( active )
    {
        Tree* callTree = Globals::getTabManager()->getTree( CALLTREE );

        bool                   deselect = true;
        const QList<TreeItem*> selected = callTree->getSelectionList();
        foreach( TreeItem * item, selected )
        {
            this->selectAll( item->getName(), deselect );
            deselect = false;
        }
        emit recalculateRequest( getTree() );
    }
}

/**
 * creates context menu items for call trees
 */
void
FlatTreeView::fillContextMenu()
{
    TreeView::fillContextMenu();
}

QString
FlatTreeView::getContextDescription( TreeItem* item )
{
    if ( item->getCubeObject() && item->getType() == REGIONITEM )
    {
        cube::Region* region = static_cast<cube::Region* >( item->getCubeObject() );
        return QString( region->get_descr().c_str() );
    }
    return "";
}
