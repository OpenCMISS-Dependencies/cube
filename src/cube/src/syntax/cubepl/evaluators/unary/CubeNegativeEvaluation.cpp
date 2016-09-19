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


#ifndef __NEGATIVE_EVALUATION_CPP
#define __NEGATIVE_EVALUATION_CPP 0

#include "CubeNegativeEvaluation.h"
using namespace cube;
using namespace std;


NegativeEvaluation::NegativeEvaluation() : UnaryEvaluation()
{
}

NegativeEvaluation::NegativeEvaluation( GeneralEvaluation* _arg ) : UnaryEvaluation( _arg )
{
}


NegativeEvaluation::~NegativeEvaluation()
{
}

double
NegativeEvaluation::eval()
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        return -( arguments[ 0 ]->eval() );
    }
    cerr << " Negation of more than 1 parameters is not defined" << endl;
    return 0;
}



double
NegativeEvaluation::eval( Cnode* _cnode, CalculationFlavour _cf, Sysres* _sys, CalculationFlavour _sf  )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        return -( arguments[ 0 ]->eval( _cnode, _cf, _sys, _sf ) );
    }
    cerr << " Negation of more than 1 parameters is not defined" << endl;
    return 0;
}



double
NegativeEvaluation::eval( Cnode* _cnode, CalculationFlavour _cf )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        return -( arguments[ 0 ]->eval( _cnode, _cf ) );
    }
    cerr << " Negation of more than 1 parameters is not defined" << endl;
    return 0;
}

double
NegativeEvaluation::eval( double arg1, double arg2 )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        return -( arguments[ 0 ]->eval( arg1, arg2 ) );
    }
    cerr << " Negation of more than 1 parameters is not defined" << endl;
    return 0;
}



double*
NegativeEvaluation::eval_row( Cnode*             _cnode,
                              CalculationFlavour _cf )
{
    double* result =  arguments[ 0 ]->eval_row( _cnode, _cf );
    if ( result == NULL )
    {
        return NULL;
    }
    for ( size_t i = 0; i < row_size; i++ )
    {
        result[ i ] = -( result[ i ] );
    }
    return result;
}


#endif
