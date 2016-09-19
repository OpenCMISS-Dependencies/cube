/****************************************************************************
**  CUBE        http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 1998-2016                                                **
**  Forschungszentrum Juelich GmbH, Juelich Supercomputing Centre          **
**                                                                         **
**  Copyright (c) 2009-2015                                                **
**  German Research School for Simulation Sciences GmbH,                   **
**  Laboratory for Parallel Programming                                    **
**                                                                         **
**  This software may be modified and distributed under the terms of       **
**  a BSD-style license.  See the COPYING file in the package base         **
**  directory for details.                                                 **
****************************************************************************/



/**
 * \file CubeInclusiveMetric.h
 * \brief Defines a metric, whick collects inclusive values.By calling "get_incl_sev..." it returns the saved value, by calling "get_excl_sev..." it calculates it via "inc_sev  - Sum (inc_sev of direct children)"
 */
#ifndef CUBE_INCLUSIVE_METRIC_H
#define CUBE_INCLUSIVE_METRIC_H


#include <iosfwd>
#include <map>

// #include "CubeMatrix.h"
#include "CubeVertex.h"
#include "CubeCnode.h"
#include "CubeThread.h"
#include "CubeMetric.h"
#include "CubeAdvMatrix.h"
// #include "CubeAdvancedMatrix.h"
#include "CubeRowWiseMatrix.h"
#include "CubeIDdeliverer.h"
#include "CubeWideSearchEnumerator.h"
/*
   *----------------------------------------------------------------------------
 *
 * class Metric
 *
 *******************************----------------------------------------------------------------------------
 */

namespace cube
{
/*  class Cnode;
   class Thread;*/

/**
 * Metric has a treelike structure and is a kind of Vertex.
 */

class InclusiveMetric : public Metric
{
public:

    InclusiveMetric( const std::string& disp_name,
                     const std::string& uniq_name,
                     const std::string& dtype,
                     const std::string& uom,
                     const std::string& val,
                     const std::string& url,
                     const std::string& descr,
                     FileFinder*        ffinder,
                     Metric*            parent,
                     uint32_t           id = 0,
                     VizTypeOfMetric    is_ghost = CUBE_METRIC_NORMAL )
        : Metric( disp_name, uniq_name, dtype, uom, val, url, descr, ffinder, parent, id, "", "", "", "", true,  is_ghost )
    {
    }
    virtual
    ~InclusiveMetric();

    virtual TypeOfMetric
    get_type_of_metric() const
    {
        return CUBE_METRIC_INCLUSIVE;
    };
    virtual std::string
    get_metric_kind() const
    {
        return "INCLUSIVE";
    };
    virtual row_of_objects_t*
    create_calltree_id_maps( IDdeliverer*,
                             Cnode*,
                             row_of_objects_t*  );                                      // /< Creates enumeration of Callpath-Tree according the "wide search" prescription. It is optimal for the calculation of exclusive values in inclusive metric.



    using cube::Metric::get_sev_adv;


    virtual Value*
    get_sev_adv( Cnode*             cnode,
                 CalculationFlavour cnf );

    virtual Value*
    get_sev_adv( Cnode*             cnode,
                 CalculationFlavour cfv,
                 Sysres*            sys,
                 CalculationFlavour sf );



    virtual Value**
    get_sevs_adv( Cnode*             cnode,
                  CalculationFlavour cfv
                  );







    virtual IndexFormat
    get_index_format()
    {
        return CUBE_INDEX_FORMAT_DENSE;
    };
};
}
#endif