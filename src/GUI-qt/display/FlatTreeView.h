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


#ifndef FLATTREEVIEW_H
#define FLATTREEVIEW_H

#include "TreeView.h"

class FlatTreeView : public TreeView
{
public:
    FlatTreeView( TreeModelInterface* modelInterface,
                  const QString&      tabLabel );

    virtual void
    setActive( bool );

protected:
    virtual void
    fillContextMenu();

protected:
    virtual QString
    getContextDescription( TreeItem* item );
};

#endif // FLATTREEVIEW_H
