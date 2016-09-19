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


#ifndef __ATAN_EVALUATION_CPP
#define __ATAN_EVALUATION_CPP 0

#include <cmath>

#include "CubeATanEvaluation.h"
using namespace cube;
using namespace std;



ATanEvaluation::ATanEvaluation() : UnaryEvaluation()
{
};


ATanEvaluation::~ATanEvaluation()
{
}

double
ATanEvaluation::eval()
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        return atan( arguments[ 0 ]->eval() );
    }
    cerr << " Arctangent of more than 1 parameters is not defined" << endl;
    return 0.;
}



double
ATanEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _value = atan( arguments[ 0 ]->eval(        _cnode, _cf,
                                                           _sf, _tf
                                                           ) );
        return _value;
    }
    cerr << " Arctangent of  more than 1 parameters is not defined" << endl;
    return 0;
}


double
ATanEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _value = atan( arguments[ 0 ]->eval(        _cnode, _cf
                                                           ) );
        return _value;
    }
    cerr << " Arctangent of  more than 1 parameters is not defined" << endl;
    return 0;
}


double
ATanEvaluation::eval( double arg1, double arg2 )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _value = atan( arguments[ 0 ]->eval(  arg1, arg2 ) );
        return _value;
    }
    cerr << " Arctangent of  more than 1 parameters is not defined" << endl;
    return 0;
}

double*
ATanEvaluation::eval_row( Cnode*             _cnode,
                          CalculationFlavour _cf )
{
    double* result =  arguments[ 0 ]->eval_row( _cnode, _cf );
    if ( result == NULL )
    {
        return NULL;
    }
    for ( size_t i = 0; i < row_size; i++ )
    {
        result[ i ] = atan( result[ i ] );
    }
    return result;
}

#endif
