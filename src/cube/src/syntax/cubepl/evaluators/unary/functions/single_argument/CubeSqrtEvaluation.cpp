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


#ifndef __SQRT_EVALUATION_CPP
#define __SQRT_EVALUATION_CPP 0

#include <cmath>

#include "CubeSqrtEvaluation.h"
using namespace cube;
using namespace std;


SqrtEvaluation::SqrtEvaluation() : UnaryEvaluation()
{
};


SqrtEvaluation::~SqrtEvaluation()
{
}

double
SqrtEvaluation::eval()
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _arg = arguments[ 0 ]->eval();
        if ( _arg >= 0. )
        {
            return sqrt( _arg );
        }
        else
        {
            cerr << " Square root of " << _arg << "  is not supported. Return 0" << endl;
            return 0;
        }
    }
    cerr << " Square root of more than 1 parameters is not defined" << endl;
    return 0.;
}



double
SqrtEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _value = arguments[ 0 ]->eval(     _cnode, _cf,
                                                  _sf, _tf
                                                  );

        if ( _value >= 0. )
        {
            return sqrt( _value );
        }
        else
        {
            cerr << " Square root of " << _value << "  is not supported. Return 0" << endl;
            return 0;
        }
    }
    cerr << " Square root of more than 1 parameters is not defined" << endl;
    return 0;
}


double
SqrtEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _value = arguments[ 0 ]->eval(     _cnode, _cf
                                                  );
        if ( _value >= 0. )
        {
            return sqrt( _value );
        }
        else
        {
            cerr << " Square root of " << _value << "  is not supported. Return 0" << endl;
            return 0;
        }
    }
    cerr << " Square root of more than 1 parameters is not defined" << endl;
    return 0;
}


double
SqrtEvaluation::eval( double arg1, double arg2 )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _value = arguments[ 0 ]->eval( arg1, arg2  );
        if ( _value >= 0. )
        {
            return sqrt( _value );
        }
        else
        {
            cerr << " Square root of " << _value << "  is not supported. Return 0" << endl;
            return 0;
        }
    }
    cerr << " Square root of more than 1 parameters is not defined" << endl;
    return 0;
}

double*
SqrtEvaluation::eval_row( Cnode*             _cnode,
                          CalculationFlavour _cf )
{
    double* result =  arguments[ 0 ]->eval_row( _cnode, _cf );
    if ( result == NULL )
    {
        return NULL;
    }

    for ( size_t i = 0; i < row_size; i++ )
    {
        result[ i ] = sqrt( result[ i ] );
    }
    return result;
}


#endif
