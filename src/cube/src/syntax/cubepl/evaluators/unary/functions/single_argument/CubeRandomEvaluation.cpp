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


#ifndef __RANDOM_EVALUATION_CPP
#define __RANDOM_EVALUATION_CPP 0

#include <cmath>
#include <cstdlib>
#include "CubeRandomEvaluation.h"
using namespace cube;
using namespace std;


RandomEvaluation::RandomEvaluation() : UnaryEvaluation()
{
};


RandomEvaluation::~RandomEvaluation()
{
}

double
RandomEvaluation::eval()
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double scale   = arguments[ 0 ]->eval();
        double _result = ( double )rand() / RAND_MAX;
        return _result * scale;
    }
    cerr << " Random generator with more than 1 parameter is not defined" << endl;
    return 0.;
}



double
RandomEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _scale = arguments[ 0 ]->eval(     _cnode, _cf,
                                                  _sf, _tf
                                                  );
        double _result = ( double )rand() / RAND_MAX;
        return _result * _scale;
    }
    cerr << " Random generator with more than 1 parameters is not defined" << endl;
    return 0;
}



double
RandomEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _scale = arguments[ 0 ]->eval(     _cnode, _cf
                                                  );
        double _result = ( double )rand() / RAND_MAX;
        return _result * _scale;
    }
    cerr << " Random generator with more than 1 parameters is not defined" << endl;
    return 0;
}

double
RandomEvaluation::eval( double arg1, double arg2 )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _scale  = arguments[ 0 ]->eval( arg1, arg2 );
        double _result = ( double )rand() / RAND_MAX;
        return _result * _scale;
    }
    cerr << " Random generator with more than 1 parameters is not defined" << endl;
    return 0;
}
double*
RandomEvaluation::eval_row( Cnode*             _cnode,
                            CalculationFlavour _cf )
{
    double* result =  arguments[ 0 ]->eval_row( _cnode, _cf );
    if ( result == NULL )
    {
        return NULL;
    }
    for ( size_t i = 0; i < row_size; i++ )
    {
        double _scale = result[ i ];
        result[ i ] = ( double )rand() / RAND_MAX * _scale;
    }
    return result;
}



#endif
