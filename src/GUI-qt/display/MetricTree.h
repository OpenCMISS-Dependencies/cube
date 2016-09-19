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


#ifndef METRICTREE_H
#define METRICTREE_H

#include "Tree.h"
#include <QHash>

class MetricTree : public Tree
{
public:
    MetricTree();

    virtual QString
    getLabel();

    TreeItem*
    getTreeItem( std::string metricId );

    /** returns the maximum value of the tree which contains the given item */
    virtual double
    getMaxValue( TreeItem* item = 0 ) const;

    void
    computeExternalReferenceValues( cube::Cube* cubeExternal );

    double
    getExternalReferenceValue( const QString& name );

protected:
    virtual TreeItem*
    createTree();

    virtual QString
    getItemName( cube::Vertex* vertex );

    virtual bool
    itemIsValid( cube::Vertex* vertex );

    virtual void
    computeBasicValues( const QList<Tree*>& leftTrees,
                        TreeItem*           top );

    virtual void
    computeMaxValues();

private:
    // ---- calculation functions
    void
    computeValue( TreeItem* item );

    void
    computeValue( cube::list_of_cnodes& cv,
                  TreeItem*             item );
    void
    computeValue( cube::list_of_regions& rv,
                  TreeItem*              item );
    void
    computeValue( cube::list_of_sysresources& sv,
                  TreeItem*                   item );

    void
    computeValue( cube::list_of_regions&      rv,
                  cube::list_of_sysresources& sv,
                  TreeItem*                   item );
    void
    computeValue( cube::list_of_cnodes&       cv,
                  cube::list_of_sysresources& sv,
                  TreeItem*                   item );

    // value modus related attribute
    QHash<TreeItem*, int> maxValueIndex;
    QList<double>         maxValues;

    // external metric items and their names for the external percentage value modus
    QList<TreeItem*>       externalMetricItems;
    QHash<QString, double> externalMetricValue;
    QList<std::string>     externalMetricNames;
};

#endif // METRICTREE_H
