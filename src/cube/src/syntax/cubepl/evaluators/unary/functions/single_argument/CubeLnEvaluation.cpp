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


#ifndef __LN_EVALUATION_CPP
#define __LN_EVALUATION_CPP 0

#include <cmath>
#include <climits>
#include <float.h>

#include "CubeLnEvaluation.h"
using namespace cube;
using namespace std;


LnEvaluation::LnEvaluation() : UnaryEvaluation()
{
};


LnEvaluation::~LnEvaluation()
{
}

double
LnEvaluation::eval()
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double result = arguments[ 0 ]->eval();
        if ( result > 0 )
        {
            return log( result );
        }
        if ( result == 0. )
        {
            return -DBL_MAX;
        }
        if ( result < 0. )
        {
            cerr << " Cannot calculate ln(" << result << "). Return zero" << endl;
            return 0.;
        }
    }
    cerr << " Natural logarithm of more than 1 parameters is not defined" << endl;
    return 0.;
}



double
LnEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   )
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
        if ( _value > 0 )
        {
            return log( _value );
        }
        if ( _value == 0. )
        {
            return -DBL_MAX;
        }
        if ( _value < 0. )
        {
            cerr << " Cannot calculate ln(" << _value << "). Return zero" << endl;
            return 0.;
        }
    }
    cerr << "  Natural logarithm of more than 1 parameters is not defined" << endl;
    return 0;
}



double
LnEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _value = arguments[ 0 ]->eval(     _cnode, _cf
                                                  );
        if ( _value > 0 )
        {
            return log( _value );
        }
        if ( _value == 0. )
        {
            return -DBL_MAX;
        }
        if ( _value < 0. )
        {
            cerr << " Cannot calculate ln(" << _value << "). Return zero" << endl;
            return 0.;
        }
    }
    cerr << "  Natural logarithm of more than 1 parameters is not defined" << endl;
    return 0;
}

double
LnEvaluation::eval( double arg1, double arg2 )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _value = arguments[ 0 ]->eval( arg1, arg2  );
        if ( _value > 0 )
        {
            return log( _value );
        }
        if ( _value == 0. )
        {
            return -DBL_MAX;
        }
        if ( _value < 0. )
        {
            cerr << " Cannot calculate ln(" << _value << "). Return zero" << endl;
            return 0.;
        }
    }
    cerr << "  Natural logarithm of more than 1 parameters is not defined" << endl;
    return 0;
}



double*
LnEvaluation::eval_row( Cnode*             _cnode,
                        CalculationFlavour _cf )
{
    double* result =  arguments[ 0 ]->eval_row( _cnode, _cf );
    if ( result == NULL )
    {
        result = new double[ row_size ]();
    }
    for ( size_t i = 0; i < row_size; i++ )
    {
        if ( result[ i ] > 0 )
        {
            result[ i ] = log( result[ i ] );
        }
        else
        if ( result[ i ] == 0 )
        {
            result[ i ] = -DBL_MAX;
        }
        else
        {
            cerr << " Cannot calculate ln(" << result[ i ] << "). Return zero" << endl;
            result[ i ] = 0.;
        }
    }
    return result;
}




#endif
