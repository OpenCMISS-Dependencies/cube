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
 * \file Cube.cpp
 * \brief Defines methods of the class cube and IO-interface.
 *
 */

// PGI compiler replaces all ofstream.open() calls by open64 and then cannot find a propper one. Result -> compilation error
#if !defined( __PGI ) && !defined( __CRAYXT )  && !defined( __CRAYXE )
#define _FILE_OFFSET_BITS 64
#endif


#include "CubeTypes.h"
#include "CubeSysres.h"
#include "CubeValue.h"
#include "CubeError.h"
#include "Cube.h"


using namespace std;
using namespace cube;






// --------------- calls with lists--------------------------------------------------------------------------------


Value*
Cube::get_sev_adv( list_of_metrics& metrics, Cnode* cnode, CalculationFlavour cf )
{
    if ( metrics.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, Cnode*, CalculationFlavour ): No metrics are spezified. Empty list." );
    }
    if ( cnode == NULL )
    {
        throw RuntimeError( "No cnode is specified for the calculation." );
    }

    Value* value = NULL;

    for ( unsigned i = 0; i < metrics.size(); i++ )
    {
        if ( value == NULL )
        {
            value = get_sev_adv( metrics[ i ].first, metrics[ i ].second, cnode, cf );
        }
        else
        {
            Value* _v =  get_sev_adv( metrics[ i ].first, metrics[ i ].second, cnode, cf );
            if ( _v != NULL )
            {
                ( *value ) += _v;
                delete _v;
            }
        }
    }
    return value;
}



double
Cube::get_sev( list_of_metrics& metrics, Cnode* cnode, CalculationFlavour cf )                                                                                              // sum over roots of calltree
{
    if ( metrics.size() == 0 )
    {
        return 0;
    }
    if ( cnode == NULL )
    {
        return 0;
    }
    Value* v = get_sev_adv( metrics, cnode,  cf );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(metrics, cnode,  cf) returned NULL " << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}






Value*
Cube::get_sev_adv( list_of_metrics& metrics, Region* region, CalculationFlavour rf )
{
    if ( metrics.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, Region*, CalculationFlavour ): No metrics are spezified. Empty list." );
    }
    if ( region == NULL )
    {
        throw RuntimeError( "No region is specified for the calculation." );
    }

    Value* value = NULL;
    for ( unsigned i = 0; i < metrics.size(); i++ )
    {
        if ( value == NULL )
        {
            value = get_sev_adv( metrics[ i ].first, metrics[ i ].second, region, rf );
        }
        else
        {
            Value* _v =  get_sev_adv( metrics[ i ].first, metrics[ i ].second, region, rf );
            if ( _v != NULL )
            {
                ( *value ) += _v;
                delete _v;
            }
        }
    }
    return value;
}


double
Cube::get_sev( list_of_metrics& metrics, Region* region, CalculationFlavour rf )                                                                                              // sum over roots of calltree
{
    if ( metrics.size() == 0 )
    {
        return 0;
    }
    if ( region == NULL )
    {
        return 0;
    }

    Value* v = get_sev_adv( metrics, region,  rf );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(metrics, region,  rf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}





Value*
Cube::get_sev_adv( list_of_metrics& metrics, Sysres* sysres, CalculationFlavour sf )
{
    if ( metrics.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, Sysres*, CalculationFlavour ): No metrics are spezified. Empty list." );
    }
    if ( sysres == NULL )
    {
        throw RuntimeError( "No system resource (machine, node, process or thread) is specified for the calculation." );
    }

    Value* value = NULL;
    for ( unsigned i = 0; i < metrics.size(); i++ )
    {
        if ( value == NULL )
        {
            value = get_sev_adv( metrics[ i ].first, metrics[ i ].second, sysres, sf );
        }
        else
        {
            Value* _v =  get_sev_adv( metrics[ i ].first, metrics[ i ].second, sysres, sf );
            if ( _v != NULL )
            {
                ( *value ) += _v;
                delete _v;
            }
        }
    }
    return value;
}



double
Cube::get_sev( list_of_metrics& metrics, Sysres* sysres, CalculationFlavour sf )                                                                                              // sum over roots of calltree
{
    if ( metrics.size() == 0 )
    {
        return 0.;
    }
    if ( sysres == NULL )
    {
        return 0.;
    }
    Value* v = get_sev_adv( metrics, sysres,  sf );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(metrics, sysres,  sf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}





Value*
Cube::get_sev_adv( list_of_cnodes& cnodes, Metric* metric, CalculationFlavour mf )
{
    if ( cnodes.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_cnodes, Metric*, CalculationFlavour ): No cnodes are spezified. Empty list." );
    }
    if ( metric == NULL )
    {
        throw RuntimeError( "No metric is specified for the calculation" );
    }
    Value* value = NULL;
    for ( unsigned i = 0; i < cnodes.size(); i++ )
    {
        if ( value == NULL )
        {
            value = get_sev_adv( metric, mf,   cnodes[ i ].first, cnodes[ i ].second );
        }
        else
        {
            Value* _v =  get_sev_adv( metric, mf,   cnodes[ i ].first, cnodes[ i ].second );
            if ( _v != NULL )
            {
                ( *value ) += _v;
                delete _v;
            }
        }
    }
    return value;
}




double
Cube::get_sev( list_of_cnodes& cnodes, Metric* metric, CalculationFlavour mf )                                                                                              // sum over roots of calltree
{
    if ( cnodes.size() == 0 )
    {
        return 0.;
    }
    if ( metric == NULL )
    {
        return 0;
    }
    Value* v = get_sev_adv( cnodes, metric,  mf );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(cnodes, metric,  mf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}








double
Cube::get_sev( list_of_cnodes& cnodes, Sysres* sysres, CalculationFlavour sf )                                                                                              // sum over roots of calltree
{
    return 0;                                                                                                                                                               // any combination of that call return 0, coz metric is not specified
}






Value*
Cube::get_sev_adv( list_of_regions& regions, Metric* metric, CalculationFlavour mf )
{
    if ( regions.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_regions, Metric*, CalculationFlavour ): No regions are spezified. Empty list." );
    }
    if ( metric == NULL )
    {
        throw RuntimeError( "No metric is specified for calculation" );
    }
    Value* value = NULL;
    for ( unsigned i = 0; i < regions.size(); i++ )
    {
        if ( value == NULL )
        {
            value = get_sev_adv( metric, mf,   regions[ i ].first, regions[ i ].second );
        }
        else
        {
            Value* _v =  get_sev_adv( metric, mf,   regions[ i ].first, regions[ i ].second );
            if ( _v != NULL )
            {
                ( *value ) += _v;
                delete _v;
            }
        }
    }
    return value;
}

double
Cube::get_sev( list_of_regions& regions, Metric* metric, CalculationFlavour mf )                                                                                              // sum over roots of calltree
{
    if ( regions.size() == 0 )
    {
        return 0.;
    }
    if ( metric == NULL )
    {
        return 0;
    }
    Value* v = get_sev_adv( regions, metric,  mf );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(regions, metric,  mf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}








double
Cube::get_sev( list_of_regions& regions, Sysres* sysres, CalculationFlavour sf )                                                                                              // sum over roots of calltree
{
    return 0;                                                                                                                                                                 // any combination of that call return 0, coz metric is not specified
}





Value*
Cube::get_sev_adv( list_of_sysresources& sysres, Metric* metric, CalculationFlavour mf )
{
    if ( sysres.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_sysresources, Metric*, CalculationFlavour ): No system resources (machines, nodes, processes or lists) are spezified. Empty list." );
    }
    if ( metric == NULL )
    {
        throw RuntimeError( "No metric is specified for the calculation" );
    }
    Value* value = NULL;
    for ( unsigned i = 0; i < sysres.size(); i++ )
    {
        if ( value == NULL )
        {
            value = get_sev_adv( metric, mf,   sysres[ i ].first, sysres[ i ].second );
        }
        else
        {
            Value* _v =  get_sev_adv( metric, mf,   sysres[ i ].first, sysres[ i ].second );
            if ( _v != NULL )
            {
                ( *value ) += _v;
                delete _v;
            }
        }
    }
    return value;
}

double
Cube::get_sev( list_of_sysresources& sysres, Metric* metric, CalculationFlavour mf )                                                                                              // sum over roots of calltree
{
    if ( sysres.size() == 0 )
    {
        return 0.;
    }
    if ( metric == NULL )
    {
        return 0;
    }
    Value* v = get_sev_adv( sysres, metric, mf );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(sysres, metric, mf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}



double
Cube::get_sev( list_of_sysresources& sysres, Cnode* cnode, CalculationFlavour cf )                                                                                              // sum over roots of calltree
{
    return 0.;                                                                                                                                                                  // any combination of that call return 0, coz metric is not specified
}


double
Cube::get_sev( list_of_sysresources& sysres, Region* region, CalculationFlavour rf )                                                                                              // sum over roots of calltree
{
    return 0.;                                                                                                                                                                    // any combination of that call return 0, coz metric is not specified
}


// ------------------------- right panel ---------------------------------

Value*
Cube::get_sev_adv( list_of_metrics& metrics, list_of_cnodes& cnodes, Sysres* sys, CalculationFlavour sf )
{
    if ( metrics.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, list_of_cnodes , Sysres *,  CalculationFlavour ): No metrics are spezified. Empty list." );
    }
    if ( cnodes.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, list_of_cnodes , Sysres *,  CalculationFlavour): No cnodes are spezified. Empty list." );
    }
    if ( sys == NULL )
    {
        throw RuntimeError( "No system resource (machine, node, process or thread) is specified for the calculation" );
    }

    Value* value = NULL;
    for ( unsigned i = 0; i < metrics.size(); i++ )
    {
        for ( unsigned j = 0; j < cnodes.size(); j++ )
        {
            if ( value == NULL )
            {
                value = get_sev_adv( metrics[ i ].first, metrics[ i ].second,  cnodes[ j ].first, cnodes[ j ].second,   sys, sf );
            }
            else
            {
                Value* _v =  get_sev_adv( metrics[ i ].first, metrics[ i ].second,  cnodes[ j ].first, cnodes[ j ].second,   sys, sf );
                if ( _v != NULL )
                {
                    ( *value ) += _v;
                    delete _v;
                }
            }
        }
    }
    return value;
}

double
Cube::get_sev( list_of_metrics& metrics, list_of_cnodes& cnodes, Sysres* sys, CalculationFlavour sf )                                                                                               // sum over roots of calltree
{
    if ( metrics.size() == 0 )
    {
        return 0;
    }
    if ( cnodes.size() == 0 )
    {
        return 0;
    }
    if ( sys == NULL )
    {
        return 0;
    }

    Value* v = get_sev_adv( metrics, cnodes, sys, sf );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(metrics, cnodes, sys, sf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}




Value*
Cube::get_sev_adv( list_of_metrics& metrics, list_of_regions& regions, Sysres* sys, CalculationFlavour sf )
{
    if ( metrics.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, list_of_regions , Sysres *,  CalculationFlavour ): No metrics are spezified. Empty list." );
    }
    if ( regions.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, list_of_regions , Sysres *,  CalculationFlavour ): No regions are spezified. Empty list." );
    }
    if ( sys == NULL )
    {
        throw RuntimeError( "No system resource (machine, node, process or thread) is specified for the calculation" );
    }

    Value* value = NULL;
    for ( unsigned i = 0; i < metrics.size(); i++ )
    {
        for ( unsigned j = 0; j < regions.size(); j++ )
        {
            if ( regions[ j ].first == NULL )
            {
                continue;
            }
            if ( value == NULL )
            {
                value = get_sev_adv( metrics[ i ].first, metrics[ i ].second,  regions[ j ].first, regions[ j ].second,   sys, sf );
            }
            else
            {
                Value* _v =  get_sev_adv( metrics[ i ].first, metrics[ i ].second,  regions[ j ].first, regions[ j ].second,   sys, sf );
                if ( _v != NULL )
                {
                    ( *value ) += _v;
                    delete _v;
                }
            }
        }
    }
    return value;
}

double
Cube::get_sev( list_of_metrics& metrics, list_of_regions& regions, Sysres* sys, CalculationFlavour sf )                                                                                               // sum over roots of calltree
{
    if ( metrics.size() == 0 )
    {
        return 0;
    }
    if ( regions.size() == 0 )
    {
        return 0;
    }
    if ( sys == NULL )
    {
        return 0;
    }

    Value* v = get_sev_adv( metrics, regions, sys, sf );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(metrics, regions, sys, sf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}




Value*
Cube::get_sev_adv( list_of_metrics& metrics, list_of_sysresources& sys_res, Cnode* cnode, CalculationFlavour cf )
{
    if ( metrics.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, list_of_sysresources , Cnode *,  CalculationFlavour ): No metrics are spezified. Empty list." );
    }
    if ( sys_res.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, list_of_sysresources , Cnode *,  CalculationFlavour ): No system resources (machines, nodes, processes  and threads) are spezified. Empty list." );
    }
    if ( cnode == NULL )
    {
        throw RuntimeError( "No cnode  is specified for the calculation" );
    }

    Value* value = NULL;
    for ( unsigned i = 0; i < metrics.size(); i++ )
    {
        for ( unsigned j = 0; j < sys_res.size(); j++ )
        {
            if ( value == NULL )
            {
                value = get_sev_adv( metrics[ i ].first, metrics[ i ].second, cnode, cf, sys_res[ j ].first, sys_res[ j ].second );
            }
            else
            {
                Value* _v =  get_sev_adv( metrics[ i ].first, metrics[ i ].second, cnode, cf, sys_res[ j ].first, sys_res[ j ].second );
                if ( _v != NULL )
                {
                    ( *value ) += _v;
                    delete _v;
                }
            }
        }
    }
    return value;
}

double
Cube::get_sev( list_of_metrics& metrics, list_of_sysresources& sys_res, Cnode* cnode, CalculationFlavour cf )                                                                                               // sum over roots of calltree
{
    if ( metrics.size() == 0 )
    {
        return 0;
    }
    if ( sys_res.size() == 0 )
    {
        return 0;
    }
    if ( cnode == NULL )
    {
        return 0;
    }

    Value* v = get_sev_adv( metrics, sys_res, cnode, cf );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(metrics, sys_res, cnode, cf) returnd NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}






Value*
Cube::get_sev_adv( list_of_metrics& metrics, list_of_sysresources& sys_res, Region* region, CalculationFlavour rf )
{
    if ( metrics.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, list_of_sysresources , Region *,  CalculationFlavour ): No metrics are spezified. Empty list." );
    }
    if ( sys_res.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, list_of_sysresources , Region *,  CalculationFlavour ): No system resources (machines, nodes, processes  and threads) are spezified. Empty list." );
    }
    if ( region == NULL )
    {
        throw RuntimeError( "No region is specified for the calculation" );
    }

    Value* value = NULL;
    for ( unsigned i = 0; i < metrics.size(); i++ )
    {
        for ( unsigned j = 0; j < sys_res.size(); j++ )
        {
            if ( value == 0 )
            {
                value = get_sev_adv( metrics[ i ].first, metrics[ i ].second, region, rf, sys_res[ j ].first, sys_res[ j ].second );
            }
            else
            {
                Value* _v =  get_sev_adv( metrics[ i ].first, metrics[ i ].second, region, rf, sys_res[ j ].first, sys_res[ j ].second );
                if ( _v != NULL )
                {
                    ( *value ) += _v;
                    delete _v;
                }
            }
        }
    }
    return value;
}

double
Cube::get_sev( list_of_metrics& metrics, list_of_sysresources& sys_res, Region* region, CalculationFlavour rf )                                                                                               // sum over roots of calltree
{
    if ( metrics.size() == 0 )
    {
        return 0;
    }
    if ( sys_res.size() == 0 )
    {
        return 0;
    }
    if ( region == NULL )
    {
        return 0;
    }

    Value* v = get_sev_adv( metrics, sys_res, region, rf );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(metrics, sys_res, region, rf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}






Value*
Cube::get_sev_adv( list_of_cnodes& cnodes, list_of_sysresources& sys_res, Metric* metric, CalculationFlavour mf )
{
    if ( cnodes.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_cnodes, list_of_sysresources , Metric *,  CalculationFlavour ): No cnodes are spezified. Empty list." );
    }
    if ( sys_res.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_cnodes, list_of_sysresources , Metric *,  CalculationFlavour ): No system resources (machines, nodes, processes  and threads) are spezified. Empty list." );
    }
    if ( metric == NULL )
    {
        throw RuntimeError( "No metric is specified for the calculation." );
    }

    Value* value = NULL;
    for ( unsigned i = 0; i < cnodes.size(); i++ )
    {
        for ( unsigned j = 0; j < sys_res.size(); j++ )
        {
            if ( value == NULL )
            {
                value = get_sev_adv( metric, mf, cnodes[ i ].first, cnodes[ i ].second, sys_res[ j ].first, sys_res[ j ].second );
            }
            else
            {
                Value* _v =  get_sev_adv( metric, mf, cnodes[ i ].first, cnodes[ i ].second, sys_res[ j ].first, sys_res[ j ].second );
                if ( _v != NULL )
                {
                    ( *value ) += _v;
                    delete _v;
                }
            }
        }
    }
    return value;
}

double
Cube::get_sev( list_of_cnodes& cnodes, list_of_sysresources& sys_res, Metric* metric, CalculationFlavour mf )                                                                                               // sum over roots of calltree
{
    if ( cnodes.size() == 0 )
    {
        return 0;
    }
    if ( sys_res.size() == 0 )
    {
        return 0;
    }
    if ( metric == NULL )
    {
        return 0;
    }

    Value* v = get_sev_adv( cnodes, sys_res, metric, mf );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(cnodes, sys_res, metric, mf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}









Value*
Cube::get_sev_adv( list_of_regions& regions, list_of_sysresources& sys_res, Metric* metric, CalculationFlavour mf )
{
    if ( regions.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_regions, list_of_sysresources , Metric *,  CalculationFlavour ): No regions are spezified. Empty list." );
    }
    if ( sys_res.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_regions, list_of_sysresources , Metric *,  CalculationFlavour ): No system resources (machines, nodes, processes  and threads) are spezified. Empty list." );
    }
    if ( metric == NULL )
    {
        throw RuntimeError( "No metric is specified for the calculation" );
    }

    Value* value = NULL;
    for ( unsigned i = 0; i < regions.size(); i++ )
    {
        for ( unsigned j = 0; j < sys_res.size(); j++ )
        {
            if ( value == NULL )
            {
                value = get_sev_adv( metric, mf, regions[ i ].first, regions[ i ].second, sys_res[ j ].first, sys_res[ j ].second );
            }
            else
            {
                Value* _v =  get_sev_adv( metric, mf, regions[ i ].first, regions[ i ].second, sys_res[ j ].first, sys_res[ j ].second );
                if ( _v != NULL )
                {
                    ( *value ) += _v;
                    delete _v;
                }
            }
        }
    }
    return value;
}

double
Cube::get_sev( list_of_regions& regions, list_of_sysresources& sys_res, Metric* metric, CalculationFlavour mf )                                                                                               // sum over roots of calltree
{
    if ( regions.size() == 0 )
    {
        return 0;
    }
    if ( sys_res.size() == 0 )
    {
        return 0;
    }
    if ( metric == NULL )
    {
        return 0;
    }

    Value* v = get_sev_adv( regions, sys_res, metric, mf );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(regions, sys_res, metric, mf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}



// --- additional call, which might replace others in future...

Value*
Cube::get_sev_adv( list_of_metrics&      metrics,
                   list_of_cnodes&       cnodes,
                   list_of_sysresources& sys_res )
{
    if ( metrics.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, list_of_cnodes ,list_of_sysresources): No metrics are spezified. Empty list." );
    }
    if ( cnodes.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, list_of_cnodes , list_of_sysresources): No cnodes are spezified. Empty list." );
    }
    if ( sys_res.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, list_of_cnodes , list_of_sysresources): No system resources are spezified. Empty list." );
    }

    Value* value = NULL;
    for ( unsigned i = 0; i < metrics.size(); i++ )
    {
        for ( unsigned j = 0; j < cnodes.size(); j++ )
        {
            for ( unsigned j = 0; j < sys_res.size(); j++ )
            {
                if ( value == NULL )
                {
                    value = get_sev_adv( metrics[ i ].first, metrics[ i ].second,  cnodes[ j ].first, cnodes[ j ].second,   sys_res[ j ].first, sys_res[ j ].second );
                }
                else
                {
                    Value* _v =  get_sev_adv( metrics[ i ].first, metrics[ i ].second,  cnodes[ j ].first, cnodes[ j ].second,   sys_res[ j ].first, sys_res[ j ].second  );
                    if ( _v != NULL )
                    {
                        ( *value ) += _v;
                        delete _v;
                    }
                }
            }
        }
    }
    return value;
}

double
Cube::get_sev( list_of_metrics&      metrics,
               list_of_cnodes&       cnodes,
               list_of_sysresources& sys_res )
{
    if ( metrics.size() == 0 )
    {
        return 0;
    }
    if ( cnodes.size() == 0 )
    {
        return 0;
    }
    if ( sys_res.size() == 0 )
    {
        return 0;
    }

    Value* v = get_sev_adv( metrics, cnodes, sys_res );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(regions, sys_res, metric, mf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}

Value*
Cube::get_sev_adv( list_of_metrics& metrics,
                   list_of_cnodes&  cnodes )
{
    if ( metrics.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, list_of_cnodes ): No metrics are spezified. Empty list." );
    }
    if ( cnodes.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, list_of_cnodes ): No cnodes are spezified. Empty list." );
    }
    Value* value = NULL;
    for ( unsigned j = 0; j < cnodes.size(); j++ )
    {
        if ( value == NULL )
        {
            value = get_sev_adv( metrics,  cnodes[ j ].first, cnodes[ j ].second );
        }
        else
        {
            Value* _v =  get_sev_adv( metrics,  cnodes[ j ].first, cnodes[ j ].second );
            if ( _v != NULL )
            {
                ( *value ) += _v;
                delete _v;
            }
        }
    }
    return value;
}
double
Cube::get_sev( list_of_metrics& metrics,
               list_of_cnodes&  cnodes )
{
    if ( metrics.size() == 0 )
    {
        return 0;
    }
    if ( cnodes.size() == 0 )
    {
        return 0;
    }

    Value* v = get_sev_adv( metrics, cnodes );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(regions, sys_res, metric, mf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}



Value*
Cube::get_sev_adv( list_of_metrics&      metrics,
                   list_of_sysresources& sys_res )
{
    if ( metrics.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, list_of_sysresources ): No metrics are spezified. Empty list." );
    }
    if ( sys_res.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics, list_of_sysresources ): No system resources are spezified. Empty list." );
    }
    Value* value = NULL;
    for ( unsigned j = 0; j < sys_res.size(); j++ )
    {
        if ( value == NULL )
        {
            value = get_sev_adv( metrics,  sys_res[ j ].first, sys_res[ j ].second );
        }
        else
        {
            Value* _v =  get_sev_adv( metrics,  sys_res[ j ].first, sys_res[ j ].second );
            if ( _v != NULL )
            {
                ( *value ) += _v;
                delete _v;
            }
        }
    }
    return value;
}
double
Cube::get_sev( list_of_metrics&      metrics,
               list_of_sysresources& sys_res )
{
    if ( metrics.size() == 0 )
    {
        return 0;
    }
    if ( sys_res.size() == 0 )
    {
        return 0;
    }

    Value* v = get_sev_adv( metrics, sys_res );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(regions, sys_res, metric, mf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}




Value*
Cube::get_sev_adv( list_of_cnodes&       cnodes,
                   list_of_sysresources& sys_res )
{
    if ( cnodes.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_cnodes, list_of_sysresources ): No cnodes are spezified. Empty list." );
    }
    if ( sys_res.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_cnodes, list_of_sysresources ): No system resources are spezified. Empty list." );
    }
    return NULL;
}
double
Cube::get_sev( list_of_cnodes&       cnodes,
               list_of_sysresources& sys_res )
{
    if ( cnodes.size() == 0 )
    {
        return 0;
    }
    if ( sys_res.size() == 0 )
    {
        return 0;
    }

    Value* v = get_sev_adv( cnodes, sys_res );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(regions, sys_res, metric, mf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}














Value*
Cube::get_sev_adv( list_of_metrics& metrics )
{
    if ( metrics.size() == 0 )
    {
        throw RuntimeError( "Error in calculation call  get_sev_adv(list_of_metrics ): No metrics are spezified. Empty list." );
    }
    Value* value = NULL;
    for ( unsigned i = 0; i < metrics.size(); i++ )
    {
        if ( value == NULL )
        {
            value = get_sev_adv( metrics[ i ].first, metrics[ i ].second );
        }
        else
        {
            Value* _v =  get_sev_adv( metrics[ i ].first, metrics[ i ].second );
            if ( _v != NULL )
            {
                ( *value ) += _v;
                delete _v;
            }
        }
    }
    return value;
}


double
Cube::get_sev( list_of_metrics& metrics )
{
    if ( metrics.size() == 0 )
    {
        return 0;
    }


    Value* v = get_sev_adv( metrics );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(regions, sys_res, metric, mf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}



Value*
Cube::get_sev_adv( list_of_cnodes& cnodes )
{
    // here one has t ocheck if all root metrics do have same  uom and one can sum them up, but with warning... result might be meaningless
    return NULL;
}



double
Cube::get_sev( list_of_cnodes& cnodes )
{
    if ( cnodes.size() == 0 )
    {
        return 0;
    }


    Value* v = get_sev_adv( cnodes );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(regions, sys_res, metric, mf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}




Value*
Cube::get_sev_adv( list_of_sysresources& sys_res )
{
    // here one has t ocheck if all root metrics do have same  uom and one can sum them up, but with warning... result might be meaningless
    return NULL;
}



double
Cube::get_sev( list_of_sysresources& sys_res )
{
    if ( sys_res.size() == 0 )
    {
        return 0;
    }


    Value* v = get_sev_adv( sys_res );
    if ( v == NULL )
    {
//         cerr << "get_sev_adv(regions, sys_res, metric, mf) returned NULL" << endl;
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}
