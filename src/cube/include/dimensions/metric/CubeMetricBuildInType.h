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
 * \file
 * \brief Defines a metric class, which uses build in type as a data type
 *
 */
#ifndef CUBE_BUILDIN_METRIC_H
#define CUBE_BUILDIN_METRIC_H


#include <iosfwd>
#include <map>

// #include "CubeVertex.h"
#include "CubeTypes.h"
// #include "CubeCnode.h"
#include "CubeSystemTreeNode.h"
#include "CubeLocationGroup.h"
#include "CubeLocation.h"
#include "CubeSimpleCache.h"
#include "CubeMetric.h"
#include "CubeRowWiseMatrix.h"
/*
   *----------------------------------------------------------------------------
 *
 * class BuildInTypeMetric
 *
   =======
 */


namespace cube
{
template <class T>
class BuildInTypeMetric : public Metric
{
protected:
    SimpleCache<T>* t_cache;

public:

    BuildInTypeMetric<T>( const std::string & disp_name,
                          const std::string & uniq_name,
                          const std::string & dtype,
                          const std::string & uom,
                          const std::string & val,
                          const std::string & url,
                          const std::string & descr,
                          FileFinder *        ffinder,
                          Metric *            parent,
                          uint32_t id = 0,
                          VizTypeOfMetric is_ghost = CUBE_METRIC_NORMAL
                          )
        :
          Metric( disp_name,
                  uniq_name,
                  dtype,
                  uom,
                  val,
                  url,
                  descr,
                  ffinder,
                  parent,
                  id,
                  "",
                  "",
                  "",
                  "",
                  true,
                  is_ghost
                  )
    {
        t_cache = NULL;
    }
    virtual
    ~BuildInTypeMetric<T>()
    {
        delete t_cache;
    };

    virtual
    bool
    isBuildIn()
    {
        return true;
    };


    double
    get_sev( Cnode*  cnode,
             Thread* thrd );                                            // /< returns saved value, without taking into account, is it exclusive value or inclusive. (Backward compatibility... )

    Value*
    get_sev_adv( Cnode*  cnode,
                 Thread* thrd );                                             // /< returns saved value, without taking into account, is it exclusive value or inclusive.

    double
    get_adv_sev( Cnode*  cnode,
                 Thread* thrd );              // /< Returns the double representation of the value in the underlyoung AdvancedMatrix

    Value*
    get_adv_sev_adv( Cnode*  cnode,
                     Thread* thrd );                                                // /< returns the Value from the AdvancedMatrix. It is the main "get_" method.



    // /  Calculates inclusive value for given thread. Expands only calltree. Returns double representation of Value if possible.
    virtual double
    get_sev( Cnode*             cnode,
             CalculationFlavour cfv,
             Sysres*            sys,
             CalculationFlavour sf );


    // / Calculates inclusive value for given cnode and over all threads. Sums up over all threads. Assumed that all threads have continueos ids. Expands only calltree. Returns double representation of Value if possible.
    virtual double
    get_sev( Cnode*             cnode,
             CalculationFlavour cfv );

    // /  Calculates inclusive value for given thread. Expands only calltree.
    virtual Value*
    get_sev_adv( Cnode*             cnode,
                 CalculationFlavour cfv,
                 Sysres*            sys,
                 CalculationFlavour sf );


    // / Calculates inclusive value for given cnode and over all threads. Sums up over all threads. Assumed that all threads have continueos ids. Expands only calltree.
    virtual Value*
    get_sev_adv( Cnode*             cnode,
                 CalculationFlavour cfv );

    virtual
    void
    invalidateCachedValue( Cnode*             cnode,
                           CalculationFlavour cf,
                           Sysres*            sysres = 0,
                           CalculationFlavour sf = CUBE_CALCULATE_INCLUSIVE );

    virtual
    void
    invalidateCache( void );



    virtual
    void
    cache_selection( uint32_t     n_cid,
                     uint32_t     n_tid,
                     TypeOfMetric type );

    virtual
    void
    dropAllRows();
};



template <class T>
void
BuildInTypeMetric<T>::dropAllRows()
{
    Metric::dropAllRows();
    if ( t_cache )
    {
        t_cache->invalidate();
    }
}



template <class T>
Value*
BuildInTypeMetric<T>::get_adv_sev_adv( Cnode* cnode, Thread* thrd )
{
    Value* v = metric_value->clone();
    ( *v ) = get_adv_sev( cnode, thrd );
    return v;
}






template <class T>
double
BuildInTypeMetric<T>::get_adv_sev( Cnode* cnode, Thread* thrd )
{
    if ( !active )
    {
        return 0.;
    }

    if ( adv_sev_mat == NULL )
    {
        return 0.;
    }
    int64_t process_rank = thrd->get_parent()->get_rank();
    T       v            =  adv_sev_mat->getBuildInValue<T>( calltree_local_ids[ cnode->get_remapping_cnode( process_rank )->get_id() ], thrd->get_id() );
    int64_t _norm        = cnode->get_cluster_normalization( process_rank );
    if ( _norm > 0 )
    {
        v /= ( uint64_t )_norm;
    }
    return static_cast<double>( v );
}


// / ------------------------ Sum over all threads for given cnode and all subcnodes


template <class T>
double
BuildInTypeMetric<T>::get_sev( Cnode* cnode, CalculationFlavour cnf )
{
    if ( !active )
    {
        return 0.;
    }

    T v = static_cast<T>( 0 );
    for ( unsigned i = 0; i < ntid; i++ )
    {
        T       _v    =   adv_sev_mat->getBuildInValue<T>( calltree_local_ids[ cnode->get_remapping_cnode( sysv[ i ]->get_parent()->get_rank() )->get_id() ], i );
        int64_t _norm = cnode->get_cluster_normalization( sysv[ i ]->get_parent()->get_rank() );
        if ( _norm > 0 )
        {
            _v /= ( ( uint64_t )_norm );
        }
        v += _v;
    }
    return static_cast<double>( v );
}



// inclusive value and exclusive value in this metric are same
template <class T>
Value*
BuildInTypeMetric<T>::get_sev_adv( Cnode* cnode, CalculationFlavour cnf )
{
    Value* v = metric_value->clone();
    ( *v ) = get_sev( cnode, cnf );
    return v;
}



template <class T>
double
BuildInTypeMetric<T>::get_sev( Cnode* cnode, CalculationFlavour cnf, Sysres* sys, CalculationFlavour sf )
{
    if ( !active )
    {
        return 0.;
    }

    T v = adv_sev_mat->getBuildInValue<T>( calltree_local_ids[ cnode->get_id() ], sys->get_id() );
    return static_cast<double>( v );
}

// inclusive value and exclusive value in this metric are same
//
template <class T>
Value*
BuildInTypeMetric<T>::get_sev_adv( Cnode* cnode, CalculationFlavour cnf, Sysres* sys, CalculationFlavour sf )
{
    Value* v = metric_value->clone();
    ( *v ) = get_sev( cnode, cnf, sys, sf );
    return v;
}


template <class T>
double
BuildInTypeMetric<T>::get_sev( Cnode* cnode, Thread* thrd )
{
    if ( !active )
    {
        return 0.;
    }

    if ( adv_sev_mat != NULL )
    {
        int64_t process_rank = thrd->get_parent()->get_rank();
        T       _v           = adv_sev_mat->getBuildInValue<T>(
            calltree_local_ids[ cnode->get_remapping_cnode( process_rank )->get_id() ],
            thrd->get_id() );
        int64_t _norm = cnode->get_cluster_normalization( process_rank );
        if ( ( _norm > 0 ) )
        {
            _v /= ( ( uint64_t )_norm );
        }
        return static_cast<double>( _v );
    }
    return 0.;


    double ret = 0.;
    Value* v   = get_sev_adv( cnode, thrd );
    if ( v == NULL )
    {
        return 0.;
    }
    ret = v->getDouble();
    delete v;
    return ret;
}



template <class T>
Value*
BuildInTypeMetric<T>::get_sev_adv( Cnode* cnode, Thread* thrd )
{
    Value* v = metric_value->clone();
    ( *v ) = get_sev( cnode, thrd );
    return v;
}


template <class T>
void
BuildInTypeMetric<T>::cache_selection( uint32_t n_cid, uint32_t n_tid, TypeOfMetric type )
{
    t_cache =  new SimpleCache<T>( n_cid, n_tid, sizeof( T ), ( type == CUBE_METRIC_INCLUSIVE ) ? CUBE_CALCULATE_INCLUSIVE : CUBE_CALCULATE_EXCLUSIVE );
}


template <class T>
void
BuildInTypeMetric<T>::invalidateCachedValue( Cnode* cnode, CalculationFlavour cf, Sysres* sysres, CalculationFlavour sf )
{
    t_cache->invalidateCachedValue( cnode, cf, sysres, sf );
}

template <class T>
void
BuildInTypeMetric<T>::invalidateCache( void )
{
    t_cache->invalidate();
}
}


#endif
