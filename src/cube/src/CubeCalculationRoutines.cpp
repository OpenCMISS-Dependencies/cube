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
#include "CubeMetric.h"
#include "CubeRegion.h"
#include "Cube.h"


using namespace std;
using namespace cube;





// ------------------- ANALYSIS PART --------------------------


Value*
Cube::get_sev_adv( Metric* metric, CalculationFlavour mf, Cnode* cnode, CalculationFlavour cnf, Sysres* sys, CalculationFlavour sf )
{
    if ( metric->isBuildIn() )
    {
        double _ret = get_sev( metric, mf, cnode, cnf,  sys, sf );
        Value* _v   = metric->get_metric_value();
        ( *_v ) = _ret;
        return _v;
    }
    else
    {
        Value* v = metric->get_sev_adv( cnode, cnf, sys, sf );
        if ( v == NULL )
        {
            return v;
        }
        if ( mf == cube::CUBE_CALCULATE_EXCLUSIVE )
        {
            for ( unsigned i = 0; i < metric->num_children(); i++ )                                                                                            // recursiv call for  children of the metric
            {
                Metric* _met = metric->get_child( i );
                Value*  _v   = get_sev_adv( _met, cube::CUBE_CALCULATE_INCLUSIVE, cnode, cnf, sys, sf );
                if ( _v == NULL )
                {
                    continue;
                }
                ( *v ) -= _v;
                delete _v;
            }
        }
        return v;
    }
}



double
Cube::get_sev( Metric* metric, CalculationFlavour mf, Cnode* cnode, CalculationFlavour cnf, Sysres* sysres, CalculationFlavour sf )
{
    if ( !metric->isBuildIn() )
    {
        Value* v = get_sev_adv( metric, mf, cnode, cnf, sysres, sf );
        if ( mf == cube::CUBE_CALCULATE_EXCLUSIVE )
        {
            for ( unsigned i = 0; i < metric->num_children(); i++ )                                                                                            // recursiv call for  children of the metric
            {
                Metric* _met = metric->get_child( i );
                Value*  _v   = get_sev_adv( _met, cube::CUBE_CALCULATE_INCLUSIVE, cnode, cnf, sysres, sf );
                if ( _v == NULL )
                {
                    continue;
                }
                ( *v ) -= _v;
                delete _v;
            }
        }
        double d = v->getDouble();
        delete v;
        return d;
    }
    else
    {
        double v = metric->get_sev( cnode, cnf, sysres, sf );
        if ( mf == cube::CUBE_CALCULATE_EXCLUSIVE )
        {
            for ( unsigned i = 0; i < metric->num_children(); i++ )                                                                                            // recursiv call for  children of the metric
            {
                Metric* _met = metric->get_child( i );
                double  _v   = get_sev( _met, cube::CUBE_CALCULATE_INCLUSIVE, cnode, cnf, sysres, sf );
                v -= _v;
            }
        }
        return v;
    }
}













Value*
Cube::get_sev_adv( Metric* metric, CalculationFlavour mf, Region* region, CalculationFlavour rf, Sysres* sys, CalculationFlavour sf )
{
    if ( metric->isBuildIn() )
    {
        double _ret = get_sev( metric, mf, region, rf,  sys, sf );
        Value* _v   = metric->get_metric_value();
        ( *_v ) = _ret;
        return _v;
    }
    else
    {
        CalculationFlavour _rf = rf;

        Value* value = NULL;
        // collectiong cnodes, which have to be calculated for region
        std::vector<Cnode*> v;
        std::vector<Cnode*> cnodes = get_cnodev();
        if ( region->is_subroutines() )                                                                                        // subrouties have to be calculated
        {
            for ( std::vector<Cnode*>::iterator citer = cnodes.begin(); citer != cnodes.end(); ++citer )
            {
                // if the call item's callee is the selected region
                // then take all its subroutines
                if ( ( *citer )->get_callee() == region )
                {
                    for ( unsigned j = 0; j < ( *citer )->num_children(); j++ )
                    {
                        if (  ( ( *citer )->get_child( j ) )->get_callee() != region )
                        {
                            v.push_back( ( *citer )->get_child( j ) );
                        }
                    }
                }
            }
            for ( unsigned i = 0; i < v.size(); i++ )
            {
                for ( unsigned j = 0; j < v[ i ]->num_children(); j++ )
                {
                    if ( ( v[ i ]->get_child( j ) )->get_callee() != region )
                    {
                        v.push_back( v[ i ]->get_child( j ) );
                    }
                }
            }
            _rf = cube::CUBE_CALCULATE_EXCLUSIVE;                                                          // force the exclusve calculation it the case of subroutines calculation.
        }
        else                                                                                               // region
        {
            for ( std::vector<Cnode*>::iterator citer = cnodes.begin(); citer != cnodes.end(); ++citer )
            {
                if ( ( *citer )->get_callee() == region )
                {
                    v.push_back( *citer );
                }
            }
        }

        for ( std::vector<Cnode*>::iterator viter = v.begin(); viter != v.end(); ++viter )
        {
            Value* _v = get_sev_adv( metric, cube::CUBE_CALCULATE_INCLUSIVE, ( *viter ), _rf, sys, sf );
            if ( value == NULL )
            {
                value = _v;
            }
            else
            {
                if ( _v != NULL )
                {
                    ( *value ) += _v;
                    delete _v;
                }
            }
        }


        if ( mf == cube::CUBE_CALCULATE_EXCLUSIVE )
        {
            for ( unsigned i = 0; i < metric->num_children(); i++ )                                                                                        // recursiv call for  children of the metric
            {
                Metric* _met = metric->get_child( i );
                Value*  _v   = get_sev_adv( _met, cube::CUBE_CALCULATE_INCLUSIVE, region, rf, sys, sf );
                if ( _v == NULL )
                {
//                 cerr << " get_sev_adv(_met, cube::CUBE_CALCULATE_INCLUSIVE, region, rf, sys, sf) returned NULL" << endl;
                    continue;
                }
                if ( value == NULL )
                {
                    value = _v->clone();
                }
                ( *value ) -= _v;
                delete _v;
            }
        }
        return value;
    }
}







double
Cube::get_sev( Metric* metric, CalculationFlavour mf, Region* region, CalculationFlavour rnf, Sysres* sysres, CalculationFlavour sf )
{
    if ( !metric->isBuildIn() )
    {
        Value* v = get_sev_adv( metric, mf, region, rnf, sysres, sf );
        if ( v == NULL )
        {
            return 0.;
        }
        double d = v->getDouble();
        delete v;
        return d;
    }
    else
    {
        CalculationFlavour _rf = rnf;

        double value = 0.;
        // collectiong cnodes, which have to be calculated for region
        std::vector<Cnode*> v;
        std::vector<Cnode*> cnodes = get_cnodev();
        if ( region->is_subroutines() )                                                                                        // subrouties have to be calculated
        {
            for ( std::vector<Cnode*>::iterator citer = cnodes.begin(); citer != cnodes.end(); ++citer )
            {
                // if the call item's callee is the selected region
                // then take all its subroutines
                if ( ( *citer )->get_callee() == region )
                {
                    for ( unsigned j = 0; j < ( *citer )->num_children(); j++ )
                    {
                        if (  ( ( *citer )->get_child( j ) )->get_callee() != region )
                        {
                            v.push_back( ( *citer )->get_child( j ) );
                        }
                    }
                }
            }
            for ( unsigned i = 0; i < v.size(); i++ )
            {
                for ( unsigned j = 0; j < v[ i ]->num_children(); j++ )
                {
                    if ( ( v[ i ]->get_child( j ) )->get_callee() != region )
                    {
                        v.push_back( v[ i ]->get_child( j ) );
                    }
                }
            }
            _rf = cube::CUBE_CALCULATE_EXCLUSIVE;                                                          // force the exclusve calculation it the case of subroutines calculation.
        }
        else                                                                                               // region
        {
            for ( std::vector<Cnode*>::iterator citer = cnodes.begin(); citer != cnodes.end(); ++citer )
            {
                if ( ( *citer )->get_callee() == region )
                {
                    v.push_back( *citer );
                }
            }
        }

        for ( std::vector<Cnode*>::iterator viter = v.begin(); viter != v.end(); ++viter )
        {
            double _v = get_sev( metric, cube::CUBE_CALCULATE_INCLUSIVE, ( *viter ), _rf, sysres, sf );
            value += _v;
        }


        if ( mf == cube::CUBE_CALCULATE_EXCLUSIVE )
        {
            for ( unsigned i = 0; i < metric->num_children(); i++ )                                                                                        // recursiv call for  children of the metric
            {
                Metric* _met = metric->get_child( i );
                double  _v   = get_sev( _met, cube::CUBE_CALCULATE_INCLUSIVE, region, rnf, sysres, sf );
                value -= _v;
            }
        }
        return value;
    }
}










Value*
Cube::get_sev_adv( Metric* metric, CalculationFlavour mf, Cnode* cnode, CalculationFlavour cnf )
{
    if ( metric->isBuildIn() )
    {
        double _ret = get_sev( metric, mf, cnode, cnf );
        Value* _v   = metric->get_metric_value();
        ( *_v ) = _ret;
        return _v;
    }
    else
    {
        Value* v = metric->get_sev_adv( cnode, cnf );

        if ( v == NULL )
        {
            return v;
        }

        if ( mf == cube::CUBE_CALCULATE_EXCLUSIVE )
        {
            for ( unsigned i = 0; i < metric->num_children(); i++ )                                                                                            // recursiv call for  children of the metric
            {
                Metric* _met = metric->get_child( i );
                Value*  _v   = get_sev_adv( _met, cube::CUBE_CALCULATE_INCLUSIVE, cnode, cnf );
                if ( _v == NULL )
                {
                    continue;
                }
                ( *v ) -= _v;
                delete _v;
            }
        }
        return v;
    }
}




double
Cube::get_sev( Metric* metric, CalculationFlavour mf, Cnode* cnode, CalculationFlavour cnf )
{
    if ( !metric->isBuildIn() )
    {
        Value* v = get_sev_adv( metric, mf, cnode, cnf );
        if ( v == NULL )
        {
//         cerr << "get_sev_adv(metric, mf, cnode, cnf) returned NULL" << endl;
            return 0.;
        }
        double d = v->getDouble();
        delete v;
        return d;
    }
    else
    {
        double v = metric->get_sev( cnode, cnf );
        if ( mf == cube::CUBE_CALCULATE_EXCLUSIVE )
        {
            for ( unsigned i = 0; i < metric->num_children(); i++ )                                                                                            // recursiv call for  children of the metric
            {
                Metric* _met = metric->get_child( i );
                double  _v   = get_sev( _met, cube::CUBE_CALCULATE_INCLUSIVE, cnode, cnf );
                v -= _v;
            }
        }
        return v;
    }
}









Value*
Cube::get_sev_adv( Metric* metric, CalculationFlavour mf, Sysres* sys, CalculationFlavour sf )
{
    if ( metric->isBuildIn() )
    {
        double _ret = get_sev( metric, mf, sys, sf );
        Value* _v   = metric->get_metric_value();
        ( *_v ) = _ret;
        return _v;
    }
    else
    {
        // over cnode roots
        std::vector<Cnode*> roots = get_root_cnodev();
        Value*              v     = NULL;
        for ( std::vector<Cnode*>::iterator citer = roots.begin(); citer != roots.end(); ++citer )
        {
            Cnode* cnode_root = ( *citer );

            Value* inc_metric_v = metric->get_sev_adv( cnode_root, cube::CUBE_CALCULATE_INCLUSIVE, sys, sf );
            if ( inc_metric_v == NULL )
            {
//             cerr << "get_sev_adv(cnode_root, cube::CUBE_CALCULATE_INCLUSIVE, sys, sf)  returned NULL " << endl;
                return inc_metric_v;
            }
            if ( v == NULL )
            {
                v = inc_metric_v;
            }
            else
            {
                ( *v ) += inc_metric_v;
                delete inc_metric_v;
            }
        }
        if ( mf == cube::CUBE_CALCULATE_EXCLUSIVE )
        {
            for ( unsigned i = 0; i < metric->num_children(); i++ )                                                                                        // recursiv call for  children of the metric
            {
                Metric* _met = metric->get_child( i );
                Value*  _v   = get_sev_adv( _met, cube::CUBE_CALCULATE_INCLUSIVE, sys, sf );
                if ( _v == NULL )
                {
//                     cerr << "get_sev_adv(_met, cube::CUBE_CALCULATE_INCLUSIVE, sys, sf) returned NULL " << endl;
                    continue;
                }
                ( *v ) -= _v;
                delete _v;
            }
        }
        return v;
    }
}






double
Cube::get_sev( Metric* metric, CalculationFlavour mf, Sysres* sys, CalculationFlavour sf )
{
    if ( !metric->isBuildIn() )
    {
        Value* v = get_sev_adv( metric, mf, sys, sf );
        if ( v == NULL )
        {
            return 0.;
        }
        double d = v->getDouble();
        delete v;
        return d;
    }
    else
    {
        // over cnode roots
        std::vector<Cnode*> roots = get_root_cnodev();
        double              v     = 0.;
        for ( std::vector<Cnode*>::iterator citer = roots.begin(); citer != roots.end(); ++citer )
        {
            Cnode* cnode_root = ( *citer );

            double inc_metric_v = metric->get_sev( cnode_root, cube::CUBE_CALCULATE_INCLUSIVE, sys, sf );
            v += inc_metric_v;
        }
        if ( mf == cube::CUBE_CALCULATE_EXCLUSIVE )
        {
            for ( unsigned i = 0; i < metric->num_children(); i++ )                                                                                        // recursiv call for  children of the metric
            {
                Metric* _met = metric->get_child( i );
                double  _v   = get_sev( _met, cube::CUBE_CALCULATE_INCLUSIVE, sys, sf );
                v -= _v;
            }
        }
        return v;
    }
}














Value*
Cube::get_sev_adv( Metric* metric, CalculationFlavour mf, Region* region, CalculationFlavour rf )
{
    if ( metric->isBuildIn() )
    {
        double _ret = get_sev( metric, mf, region, rf );
        Value* _v   = metric->get_metric_value();
        ( *_v ) = _ret;
        return _v;
    }
    else
    {
        CalculationFlavour _rf = rf;

        Value* value = NULL;
        // collectiong cnodes, which have to be calculated for region
        std::vector<Cnode*> v;
        std::vector<Cnode*> cnodes = get_cnodev();

        if ( region->is_subroutines() )                                                                                        // subrouties have to be calculated
        {
            for ( std::vector<Cnode*>::iterator citer = cnodes.begin(); citer != cnodes.end(); ++citer )
            {
                // if the call item's callee is the selected region
                // then take all its subroutines
                if ( ( *citer )->get_callee() == region )
                {
                    for ( unsigned j = 0; j < ( *citer )->num_children(); j++ )
                    {
                        if (  ( ( *citer )->get_child( j ) )->get_callee() != region )
                        {
                            v.push_back( ( *citer )->get_child( j ) );
                        }
                    }
                }
            }
            for ( unsigned i = 0; i < v.size(); i++ )
            {
                for ( unsigned j = 0; j < v[ i ]->num_children(); j++ )
                {
                    if ( ( v[ i ]->get_child( j ) )->get_callee() != region )
                    {
                        v.push_back( v[ i ]->get_child( j ) );
                    }
                }
            }
            _rf = cube::CUBE_CALCULATE_EXCLUSIVE;                                                          // force the exclusve calculation it the case of subroutines calculation.
        }
        else                                                                                               // region->is_subroutines()
        {
            for ( std::vector<Cnode*>::iterator citer = cnodes.begin(); citer != cnodes.end(); ++citer )
            {
                if ( ( *citer )->get_callee() == region )
                {
                    v.push_back( *citer );
                }
            }
        }

        for ( std::vector<Cnode*>::iterator viter = v.begin(); viter != v.end(); ++viter )
        {
            Value* _v = get_sev_adv( metric, cube::CUBE_CALCULATE_INCLUSIVE, ( *viter ), _rf );
            if ( value == NULL )
            {
                value = _v;
            }
            else
            {
                if ( _v != NULL )
                {
                    ( *value ) += _v;
                    delete _v;
                }
            }
        }


        if ( mf == cube::CUBE_CALCULATE_EXCLUSIVE )
        {
            for ( unsigned i = 0; i < metric->num_children(); i++ )                                                                                        // recursiv call for  children of the metric
            {
                Metric* _met = metric->get_child( i );
                Value*  _v   = get_sev_adv( _met, cube::CUBE_CALCULATE_INCLUSIVE, region, rf );
                if ( _v == NULL )
                {
                    continue;
                }
                ( *value ) -= _v;
                delete _v;
            }
        }
        return value;
    }
}







double
Cube::get_sev( Metric* metric, CalculationFlavour mf, Region* region, CalculationFlavour rf )
{
    if ( !metric->isBuildIn() )
    {
        Value* v = get_sev_adv( metric, mf, region, rf );
        if ( v == NULL )
        {
            return 0.;
        }
        double d = v->getDouble();
        delete v;
        return d;
    }
    else
    {
        CalculationFlavour _rf = rf;

        double value = 0.;
        // collectiong cnodes, which have to be calculated for region
        std::vector<Cnode*> v;
        std::vector<Cnode*> cnodes = get_cnodev();

        if ( region->is_subroutines() )                                                                                        // subrouties have to be calculated
        {
            for ( std::vector<Cnode*>::iterator citer = cnodes.begin(); citer != cnodes.end(); ++citer )
            {
                // if the call item's callee is the selected region
                // then take all its subroutines
                if ( ( *citer )->get_callee() == region )
                {
                    for ( unsigned j = 0; j < ( *citer )->num_children(); j++ )
                    {
                        if (  ( ( *citer )->get_child( j ) )->get_callee() != region )
                        {
                            v.push_back( ( *citer )->get_child( j ) );
                        }
                    }
                }
            }
            for ( unsigned i = 0; i < v.size(); i++ )
            {
                for ( unsigned j = 0; j < v[ i ]->num_children(); j++ )
                {
                    if ( ( v[ i ]->get_child( j ) )->get_callee() != region )
                    {
                        v.push_back( v[ i ]->get_child( j ) );
                    }
                }
            }
            _rf = cube::CUBE_CALCULATE_EXCLUSIVE;                                                          // force the exclusve calculation it the case of subroutines calculation.
        }
        else                                                                                               // region->is_subroutines()
        {
            for ( std::vector<Cnode*>::iterator citer = cnodes.begin(); citer != cnodes.end(); ++citer )
            {
                if ( ( *citer )->get_callee() == region )
                {
                    v.push_back( *citer );
                }
            }
        }

        for ( std::vector<Cnode*>::iterator viter = v.begin(); viter != v.end(); ++viter )
        {
            double _v = get_sev( metric, cube::CUBE_CALCULATE_INCLUSIVE, ( *viter ), _rf );
            value += _v;
        }


        if ( mf == cube::CUBE_CALCULATE_EXCLUSIVE )
        {
            for ( unsigned i = 0; i < metric->num_children(); i++ )                                                                                        // recursiv call for  children of the metric
            {
                Metric* _met = metric->get_child( i );
                double  _v   = get_sev( _met, cube::CUBE_CALCULATE_INCLUSIVE, region, rf );
                value -= _v;
            }
        }
        return value;
    }
}





















Value*
Cube::get_sev_adv( Metric* metric, CalculationFlavour mf )                                                                                             // sum over roots of calltree
{

    if ( metric->isBuildIn() )
    {
        double _ret = get_sev( metric, mf );
        Value* _v   = metric->get_metric_value();
        ( *_v ) = _ret;
        return _v;
    }
    else
    {
        vector<Cnode*> croots = get_root_cnodev();
        Value*         v      = NULL;

        for ( unsigned i = 0; i < croots.size(); i++ )
        {
            Value* _v = get_sev_adv( metric, cube::CUBE_CALCULATE_INCLUSIVE, croots[ i ], cube::CUBE_CALCULATE_INCLUSIVE );
            if ( _v == NULL )
            {
//             cerr << "get_sev_adv(metric, mf, croots[i], cube::CUBE_CALCULATE_INCLUSIVE) returned NULL " << endl;
                continue;
            }
            if ( v == NULL )
            {
                v = _v;
            }
            else
            {
                ( *v ) += _v;
                delete _v;
            }
        }
        if ( mf == cube::CUBE_CALCULATE_INCLUSIVE )
        {
            return v;
        }

        if ( mf == cube::CUBE_CALCULATE_EXCLUSIVE )
        {
            Value* _v = NULL;
            for ( unsigned j = 0; j < metric->num_children(); j++ )                                                                                        // recursiv call for  children of the metric
            {
                Metric* _met = metric->get_child( j );
                Value*  __v  = get_sev_adv( _met, cube::CUBE_CALCULATE_INCLUSIVE );

                if ( __v == NULL )
                {
//                 cerr << "Second get_sev_adv(croots[i], cube::CUBE_CALCULATE_INCLUSIVE) returned NULL " << endl;
                    continue;
                }
                if ( _v == NULL )
                {
                    _v = __v;
                }
                else
                {
                    ( *_v ) += __v;
                    delete __v;
                }
            }

            if ( _v != NULL )
            {
                if ( v == NULL )
                {
//                 cerr << " Inclusive value of metric is NULL" << endl;
                    v = _v;
                }
                else
                {
                    ( *v ) -= _v;
                    delete _v;
                }
            }
            return v;
        }
        return v;
    }
}





double
Cube::get_sev( Metric* metric, CalculationFlavour mf )                                                                                             // sum over roots of calltree
{
    if ( !metric->isBuildIn() )
    {
        Value* v = get_sev_adv( metric, mf );
        double d = v->getDouble();
        delete v;
        return d;
    }
    else
    {
        vector<Cnode*> croots = get_root_cnodev();
        double         ret    = 0.;

        for ( unsigned i = 0; i < croots.size(); i++ )
        {
            double _v = get_sev( metric, cube::CUBE_CALCULATE_INCLUSIVE, croots[ i ], cube::CUBE_CALCULATE_INCLUSIVE );
            ret += _v;
        }
        if ( mf == cube::CUBE_CALCULATE_EXCLUSIVE )
        {
            double _v = 0.;
            for ( unsigned j = 0; j < metric->num_children(); j++ )                                                                                        // recursiv call for  children of the metric
            {
                Metric* _met = metric->get_child( j );
                double  __v  = get_sev( _met, cube::CUBE_CALCULATE_INCLUSIVE );
                _v += __v;
            }

            ret -= _v;
        }
        return ret;
    }
}







































double
Cube::get_sev( Cnode* cnode, CalculationFlavour cf )
{
    return 0.;                                                                                            // coz this call is done by call tree, being in almost left panel
}

double
Cube::get_sev( Region* region, CalculationFlavour rf )
{
    return 0.;                                                                                            // coz this call is done by flattree, being in almost left panel
}

double
Cube::get_sev( Sysres* sys, CalculationFlavour sf )
{
    return 0.;                                                                                            // coz this call is done by system tree, being in almost left panel
}
