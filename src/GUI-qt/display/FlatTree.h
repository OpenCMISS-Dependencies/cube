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


#ifndef FLATTREE_H
#define FLATTREE_H

#include "Tree.h"

class FlatTree : public Tree
{
public:
    FlatTree();
    virtual QString
    getLabel();

protected:
    virtual TreeItem*
    createTree();

    virtual QString
    getItemName( cube::Vertex* vertex );

    virtual void
    computeBasicValues( const QList<Tree*>& leftTrees,
                        TreeItem*       top );

private:

    void
    computeValue( cube::list_of_metrics& mv,
                  TreeItem*          item );
    void
    computeValue( cube::list_of_metrics&      mv,
                  cube::list_of_sysresources& sv,
                  TreeItem*               item );

    bool
    hasCallee( cube::Vertex* region );
};

#endif // FLATTREE_H
