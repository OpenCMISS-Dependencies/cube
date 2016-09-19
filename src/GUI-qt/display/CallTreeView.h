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


#ifndef CALLTREEVIEW_H
#define CALLTREEVIEW_H

#include "TreeView.h"

class CallTreeView : public TreeView
{
    Q_OBJECT
public:
    CallTreeView( TreeModelInterface* modelInterface,
                  const QString&      tabLabel );

    virtual void
    setActive( bool );

protected:
    virtual void
    fillContextMenu();

    virtual QString
    getContextDescription( TreeItem* item );

private:
    enum CallTreeContextMenuAction { ShowIterations, HideIterations, SetAsLoop, Reroot, Prune, SetAsLeaf };
    QHash <CallTreeContextMenuAction, QAction*> callContextMenuHash; // contains all call tree specific context menu actions
    TreeView*                                   flatTreeView;
    bool                                        flatTreeSelectionChanged;
    void
    createContextMenuActions();
    void
    location( TreeItem* item,
              bool      takeCallee );

private slots:
    void
    setAsLoop();
    void
    showIterations();
    void
    hideIterations();
    void
    onRerootItem();
    void
    onPruneItem();
    void
    onSetAsLeaf();
    void
    onLocation();
    void
    onLocationCallee();
    void
    callFlatItemSelected();
};

#endif // CALLTREEVIEW_H
