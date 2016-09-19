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


#ifndef __HALF_SMALLER_EVALUATION_CPP
#define __HALF_SMALLER_EVALUATION_CPP 0

#include "CubeHalfSmallerEvaluation.h"
using namespace cube;
using namespace std;



HalfSmallerEvaluation::HalfSmallerEvaluation() : BinaryEvaluation()
{
};

HalfSmallerEvaluation::HalfSmallerEvaluation( GeneralEvaluation* _arg1,
                                              GeneralEvaluation* _arg2 ) : BinaryEvaluation( _arg1, _arg2 )
{
};


HalfSmallerEvaluation::~HalfSmallerEvaluation()
{
}

double
HalfSmallerEvaluation::eval()
{
    return ( ( arguments[ 0 ]->eval() ) <= ( arguments[ 1 ]->eval() ) ) ? 1. : 0.;
}




double
HalfSmallerEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   )
{
    return ( ( arguments[ 0 ]->eval( _cnode, _cf, _sf, _tf ) ) <= ( arguments[ 1 ]->eval( _cnode, _cf, _sf, _tf ) ) ) ? 1. : 0.;
}



double
HalfSmallerEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf )
{
    return ( ( arguments[ 0 ]->eval( _cnode, _cf ) ) <= ( arguments[ 1 ]->eval( _cnode, _cf ) ) ) ? 1. : 0.;
}


double
HalfSmallerEvaluation::eval( double arg1, double arg2 )
{
    return ( ( arguments[ 0 ]->eval( arg1, arg2 ) ) <= ( arguments[ 1 ]->eval( arg1, arg2 ) ) ) ? 1. : 0.;
}


double*
HalfSmallerEvaluation::eval_row( Cnode*             _cnode,
                                 CalculationFlavour _cf )
{
    double* result  = arguments[ 0 ]->eval_row( _cnode, _cf );
    double* _result = arguments[ 1 ]->eval_row( _cnode, _cf );
    if ( result == NULL && _result == NULL )
    {
        return NULL;
    }
    if ( result == NULL )
    {
        result = _result;
        for ( size_t i = 0; i < row_size; i++ )
        {
            result[ i ] = ( result[ i ] >= 0. ) ? 1. : 0.;
        }
    }
    else
    if ( _result == NULL )
    {
        for ( size_t i = 0; i < row_size; i++ )
        {
            result[ i ] = ( result[ i ] <= 0. ) ? 1. : 0.;
        }
    }
    else
    {
        for ( size_t i = 0; i < row_size; i++ )
        {
            result[ i ] = ( result[ i ] <= _result[ i ] ) ? 1. : 0.;
        }
        delete _result;
    }
    return result;
}


#endif
