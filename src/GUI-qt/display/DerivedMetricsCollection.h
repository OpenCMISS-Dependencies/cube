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



#ifndef _DERIVED_METRICS_COLLECTION_H
#define _DERIVED_METRICS_COLLECTION_H

#include <QString>
#include <QStringList>

#define NUM_DERIVED_METRICS 11
enum DERIVED_METRIC_KEY { DN_NO_METRIC        = 0,
                          DM_KENOBI           = 1,
                          DM_REGION_NAME      = 2,
                          DM_LEAVES           = 3,
                          DM_ROOTS            = 4,
                          DM_LEVEL            = 5,
                          DM_RECURSIONS_LEVEL = 6,
                          DM_NUM_RECURSIONS   = 7,
                          DM_MAX_GROUP_TIME   = 8,
                          DM_PERFECT_PARALL   = 9,
                          DM_PARALLELIZATION  = 10
    
};

/*-------------------------------------------------------------------------*/
/**
 * @file  DerivedMetricsCollection.h
 * @brief Declaration of the class DerivedMetricsCollection
 *
 * This header file provides the declaration of the class DerivedMetricsCollection.
 */
/*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/
/**
 * @class DerivedMetricsCollection
 * @brief Provides a collection of Help textes.
 *
 * This class provides a simple collection of derived metrics, which can  be used
 * in analysis.
 */
/*-------------------------------------------------------------------------*/

class DerivedMetricsCollection
{
public:

    /// returns text of derived metrics settign for clipboard
    static QString&
    getDerivedMetricText( int );

    static QStringList
    getDerivedMetricsNames();

private:

    static QString derived_metrics[ NUM_DERIVED_METRICS ];
    static QString derived_metrics_names[ NUM_DERIVED_METRICS ];
};


#endif
