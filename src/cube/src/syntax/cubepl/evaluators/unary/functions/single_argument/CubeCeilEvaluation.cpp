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


#ifndef __CEIL_EVALUATION_CPP
#define __CEIL_EVALUATION_CPP 0

#include <cmath>

#include "CubeCeilEvaluation.h"
using namespace cube;
using namespace std;


CeilEvaluation::CeilEvaluation() : UnaryEvaluation()
{
};


CeilEvaluation::~CeilEvaluation()
{
}

double
CeilEvaluation::eval()
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        return ceil( arguments[ 0 ]->eval() );
    }
    cerr << " Ceil  of more than 1 parameters is not defined" << endl;
    return 0.;
}




double
CeilEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _value = ceil( arguments[ 0 ]->eval(        _cnode, _cf,
                                                           _sf, _tf
                                                           ) );
        return _value;
    }
    cerr << " Ceil  of more than 1 parameters is not defined" << endl;
    return 0;
}


double
CeilEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _value = ceil( arguments[ 0 ]->eval(        _cnode, _cf
                                                           ) );
        return _value;
    }
    cerr << " Ceil  of more than 1 parameters is not defined" << endl;
    return 0;
}


double
CeilEvaluation::eval( double arg1, double arg2 )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _value = ceil( arguments[ 0 ]->eval( arg1, arg2 ) );
        return _value;
    }
    cerr << " Ceil  of more than 1 parameters is not defined" << endl;
    return 0;
}
double*
CeilEvaluation::eval_row( Cnode*             _cnode,
                          CalculationFlavour _cf )
{
    double* result =  arguments[ 0 ]->eval_row( _cnode, _cf );
    if ( result == NULL )
    {
        return NULL;
    }
    for ( size_t i = 0; i < row_size; i++ )
    {
        result[ i ] = ceil( result[ i ] );
    }
    return result;
}



#endif
