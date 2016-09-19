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
 * \file CubeExclusiveMetric.cpp
 * \brief Defines methods to calculate incl/exclusve values if the metric contains only exclusive values

 ********************************************/


#include <iostream>

#include "CubeTypes.h"
#include "CubeSysres.h"
#include "CubeLocation.h"
#include "CubeLocationGroup.h"
#include "CubeSystemTreeNode.h"

#include "CubeExclusiveMetric.h"
#include "CubeServices.h"


using namespace std;
using namespace cube;


ExclusiveMetric::~ExclusiveMetric()
{
}


row_of_objects_t*
ExclusiveMetric::create_calltree_id_maps( IDdeliverer* ids, Cnode* root, row_of_objects_t* _row  )
{
    ids->reset();
    DeepSearchEnumerator enumerator;
    _row = enumerator.get_objects_to_enumerate( root, _row );
    for ( row_of_objects_t::iterator iter = _row->begin(); iter < _row->end(); ++iter )
    {
        if ( calltree_local_ids.size() <= ( *iter )->get_id() )
        {
            calltree_local_ids.resize( ( *iter )->get_id() + 1  );
        }
        calltree_local_ids[ ( *iter )->get_id() ] = ids->get_next_id();
    }

    return _row;
}








Value*
ExclusiveMetric::get_sev_adv( Cnode* cnode, CalculationFlavour cnf )
{
    if ( !active )   //  if value == VOID
    {
        return adv_sev_mat->getValue();
    }

    Value* v = NULL;
    v = cache->getCachedValue( cnode, cnf );
    if ( v != NULL )
    {
        return v;
    }

    v = adv_sev_mat->getValue();
    std::vector<Cnode*> _cnodes;  // collection of cnodes for the calculation
    _cnodes.push_back( cnode );
    for ( cnode_id_t cid = 0; cid < cnode->num_children(); cid++  )
    {
        Cnode* __c  = cnode->get_child( cid );
        bool   _add = ( __c->isHidden() ) || ( cnf == CUBE_CALCULATE_INCLUSIVE ); // either it is hidden, or we calculate inclusive value
        if ( _add )
        {
            _cnodes.push_back( __c );
            std::vector<Cnode*>& _vc = __c->get_whole_subtree();
            for ( std::vector<Cnode*>::const_iterator _iter = _vc.begin(); _iter != _vc.end(); ++_iter )
            {
                _cnodes.push_back( *_iter );
            }
        }
    }

// sum up the values
    for ( std::vector<Cnode*>::const_iterator _iter = _cnodes.begin(); _iter != _cnodes.end(); ++_iter )
    {
        Cnode* __c = *_iter;
        if ( !__c->is_clustered() )
        {
            Value* tmp = adv_sev_mat->sumRow( calltree_local_ids[ __c->get_remapping_cnode()->get_id() ] );
            ( *v ) += tmp;
            delete tmp;
        }
        else
        {
            size_t sysv_size = sysv.size();
            for ( size_t i = 0; i < sysv_size; ++i )
            {
                Thread*  _thrd        = sysv[ i ];
                Process* _proc        = _thrd->get_parent();
                int64_t  process_rank = _proc->get_rank();
                Value*   tmp          = adv_sev_mat->getValue( calltree_local_ids[ __c->get_remapping_cnode( process_rank  )->get_id() ], _thrd->get_id() );
                int64_t  _norm        = __c->get_cluster_normalization( process_rank );
                if ( _norm > 0 )
                {
                    tmp->normalizeWithClusterCount( ( uint64_t )_norm );
                }
                ( *v ) += tmp;
                delete tmp;
            }
        }
    }
    cache->setCachedValue( v,  cnode, cnf );
    return v;
}






// inclusive value and exclusive value in this metric are same
Value*
ExclusiveMetric::get_sev_adv( Cnode* cnode, CalculationFlavour cnf, Sysres* sys, CalculationFlavour sf )
{
    Value* v = adv_sev_mat->getValue();

    if ( !active )   //  if value == VOID
    {
        return v;
    }

    if ( ( sys->isSystemTreeNode() || sys->isLocationGroup() ) && sf == cube::CUBE_CALCULATE_EXCLUSIVE )
    {
        return v;
    }

    Value* cached_value = cache->getCachedValue( cnode, cnf, sys, sf );
    if ( cached_value != NULL )
    {
        delete v;
        return cached_value;
    }

    if ( sys->isSystemTreeNode() )
    {
        SystemTreeNode* _sys = ( SystemTreeNode* )sys;
        // first add values of all sub system nodes
        for ( unsigned i = 0; i < _sys->num_children(); ++i )
        {
            Value* _v = get_sev_adv( cnode, cnf, _sys->get_child( i ), cube::CUBE_CALCULATE_INCLUSIVE );
            ( *v ) += _v;
            delete _v;
        }
        // then add all values of all sub local groups
        for ( unsigned i = 0; i < _sys->num_groups(); ++i )
        {
            Value* _v = get_sev_adv( cnode, cnf, _sys->get_location_group( i ), cube::CUBE_CALCULATE_INCLUSIVE );
            ( *v ) += _v;
            delete _v;
        }
    }
    if ( sys->isLocationGroup() )
    {
        LocationGroup* _lg = ( LocationGroup* )sys;

        for ( unsigned i = 0; i < _lg->num_children(); ++i )
        {
            Value* _v = get_sev_adv( cnode, cnf, _lg->get_child( i ), cube::CUBE_CALCULATE_INCLUSIVE );
            ( *v ) += _v;
            delete _v;
        }
    }
    if ( sys->isLocation() )
    {
        std::vector<Cnode*> _cnodes;  // collection of cnodes for the calculation
        _cnodes.push_back( cnode );
        for ( cnode_id_t cid = 0; cid < cnode->num_children(); cid++  )
        {
            Cnode* __c  = cnode->get_child( cid );
            bool   _add = ( __c->isHidden() ) || ( cnf == CUBE_CALCULATE_INCLUSIVE ); // either it is hidden, or we calculate inclusive value
            if ( _add )
            {
                _cnodes.push_back( __c );
                std::vector<Cnode*>& _vc = __c->get_whole_subtree();
                for ( std::vector<Cnode*>::const_iterator _iter = _vc.begin(); _iter != _vc.end(); ++_iter )
                {
                    _cnodes.push_back( *_iter );
                }
            }
        }

        for ( std::vector<Cnode*>::const_iterator _iter = _cnodes.begin(); _iter != _cnodes.end(); ++_iter )
        {
            Cnode* __c = *_iter;


            Location*      _loc    = ( Location* )sys;
            LocationGroup* _lg     = _loc->get_parent();
            int64_t        lg_rank = _lg->get_rank();
            Value*         _v      = adv_sev_mat->getValue( calltree_local_ids[ __c->get_remapping_cnode( lg_rank  )->get_id() ], _loc->get_id() );
            int64_t        _norm   = __c->get_cluster_normalization( lg_rank );
            if ( _norm > 0 )
            {
                _v->normalizeWithClusterCount( ( uint64_t )_norm );
            }
            ( *v ) += _v;
            delete _v;
        }
    }
    cache->setCachedValue( v, cnode, cnf, sys, sf );
    return v;
}



Value**
ExclusiveMetric::get_sevs_adv( Cnode* cnode, CalculationFlavour cnf )
{
    if ( !active )   //  if value == VOID
    {
        return NULL;
    }
    Value** to_return = services::create_row_of_values( ntid );


    std::vector<Cnode*> _cnodes;  // collection of cnodes for the calculation
    _cnodes.push_back( cnode );
    for ( cnode_id_t cid = 0; cid < cnode->num_children(); cid++  )
    {
        Cnode* __c  = cnode->get_child( cid );
        bool   _add = ( __c->isHidden() ) || ( cnf == CUBE_CALCULATE_INCLUSIVE ); // either it is hidden, or we calculate inclusive value
        if ( _add )
        {
            _cnodes.push_back( __c );
            std::vector<Cnode*>& _vc = __c->get_whole_subtree();
            for ( std::vector<Cnode*>::const_iterator _iter = _vc.begin(); _iter != _vc.end(); ++_iter )
            {
                _cnodes.push_back( *_iter );
            }
        }
    }
    for ( std::vector<Cnode*>::const_iterator _iter = _cnodes.begin(); _iter != _cnodes.end(); ++_iter )
    {
        Cnode* __c = *_iter;

        if ( !__c->is_clustered() )
        {
            char* _v =   adv_sev_mat->getRow( calltree_local_ids[ __c->get_remapping_cnode()->get_id() ] );
            if ( _v == NULL )
            {
                services::delete_row_of_values( to_return, ntid );
                return NULL;
            }
            char* start = _v;


            for ( unsigned i = 0; i < ntid; ++i )
            {
                Value* _val = metric_value->clone();
                start = _val->fromStream( start );
                if ( to_return[ i ] == NULL )
                {
                    to_return[ i ] = _val;
                }
                else
                {
                    *( to_return[ i ] ) += _val;
                    delete _val;
                }
            }
        }
        else
        {
            size_t sysv_size = sysv.size();
            for ( size_t i = 0; i < sysv_size; ++i )
            {
                Thread*  _thrd        = sysv[ i ];
                Process* _proc        = _thrd->get_parent();
                int64_t  process_rank = _proc->get_rank();
                Value*   tmp          = adv_sev_mat->getValue( calltree_local_ids[ __c->get_remapping_cnode( process_rank  )->get_id() ], _thrd->get_id() );
                int64_t  _norm        = __c->get_cluster_normalization( process_rank );
                if ( _norm > 0 )
                {
                    tmp->normalizeWithClusterCount( ( uint64_t )_norm );
                }
		if ( to_return[ i ] == NULL )
                {
                    to_return[ i ] = tmp;
                }
                else
                {
                    *( to_return[ i ] ) += tmp;
                    delete tmp;
                }
            }
        }
    }

    return to_return;
}
