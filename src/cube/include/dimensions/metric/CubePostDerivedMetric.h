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
 * \file CubePostDerivedMetric.h
 * \brief Defines a metric, which calculates its value using user defined expression in CubePL"
 */
#ifndef CUBE_POSTDERIVED_METRIC_H
#define CUBE_POSTDERIVED_METRIC_H

#include "CubeMetric.h"

#include "CubeValues.h"
#include "CubeGeneralEvaluation.h"

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

class PostDerivedMetric : public Metric
{
public:

    PostDerivedMetric( const std::string& disp_name,
                       const std::string& uniq_name,
                       const std::string& dtype,
                       const std::string& uom,
                       const std::string& val,
                       const std::string& url,
                       const std::string& descr,
                       FileFinder*        ffinder,
                       Metric*            parent,
                       uint32_t           id = 0,
                       const std::string  _expression = "",
                       const std::string& _expression_init = "",
                       const bool         threadwise = true,
                       VizTypeOfMetric    is_ghost = CUBE_METRIC_NORMAL
                       )
        : Metric( disp_name, uniq_name, dtype, uom, val, url, descr, ffinder, parent, id, _expression, _expression_init, "", "", threadwise,  is_ghost )
    {
        adv_sev_mat = NULL;
        initialized = false; // with creation initialisation is done
        cache_selection( ncid, ntid, cube::CUBE_METRIC_EXCLUSIVE );
    }
    virtual
    ~PostDerivedMetric();

    virtual row_of_objects_t*
    create_calltree_id_maps( IDdeliverer*      _id,
                             Cnode*            cnode,
                             row_of_objects_t* row )
    {
        return row;
    };

    virtual TypeOfMetric
    get_type_of_metric() const
    {
        return CUBE_METRIC_POSTDERIVED;
    };
    virtual std::string
    get_metric_kind() const
    {
        return "POSTDERIVED";
    };


    virtual
    void
    initialize( void );


    using cube::Metric::get_sev_adv;

    virtual Value*
    get_sev_adv( Cnode*             cnode,
                 CalculationFlavour cnf );

    virtual Value*
    get_sev_adv( Cnode*             cnode,
                 CalculationFlavour cfv,
                 Sysres*            sys,
                 CalculationFlavour sf );

    virtual double*
    get_sevs( Cnode*             cnode,
              CalculationFlavour cfv
              );
    virtual Value**
    get_sevs_adv( Cnode*             cnode,
                  CalculationFlavour cfv
                  );

    virtual char*
    get_sevs_raw( Cnode*             cnode,
                  CalculationFlavour cfv
                  );

    virtual double
    get_sev( Cnode*             cnode,
             CalculationFlavour cnf );

    virtual double
    get_sev( Cnode*             cnode,
             CalculationFlavour cfv,
             Sysres*            sys,
             CalculationFlavour sf );


    virtual IndexFormat
    get_index_format()
    {
        return CUBE_INDEX_FORMAT_SPARSE;
    };



    // /< Empty hook
};
}
#endif
