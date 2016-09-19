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


#ifndef __PLUS_EVALUATION_CPP
#define __PLUS_EVALUATION_CPP 0
#include "CubePlusEvaluation.h"

using namespace cube;
using namespace std;

PlusEvaluation::PlusEvaluation() : BinaryEvaluation()
{
};

PlusEvaluation::PlusEvaluation( GeneralEvaluation* _arg1,
                                GeneralEvaluation* _arg2 ) : BinaryEvaluation( _arg1, _arg2 )
{
};


PlusEvaluation::~PlusEvaluation()
{
}

double
PlusEvaluation::eval()
{
    double result = 0;
    for ( vector<GeneralEvaluation*>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter )
    {
        result += ( *iter )->eval();
    }
    return result;
}




double
PlusEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   )
{
    double result = 0;
    for ( vector<GeneralEvaluation*>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter )
    {
        result += ( *iter )->eval( _cnode, _cf,
                                   _sf, _tf );
    }
    return result;
}



double
PlusEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf )
{
    double result = 0;
    for ( vector<GeneralEvaluation*>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter )
    {
        result += ( *iter )->eval( _cnode, _cf );
    }
    return result;
}

double
PlusEvaluation::eval( double arg1, double arg2 )
{
    double result = 0;
    for ( vector<GeneralEvaluation*>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter )
    {
        result += ( *iter )->eval( arg1, arg2 );
    }
    return result;
}




double*
PlusEvaluation::eval_row( Cnode*             _cnode,
                          CalculationFlavour _cf )
{
    double* result  = arguments[ 0 ]->eval_row( _cnode, _cf );
    double* _result = arguments[ 1 ]->eval_row( _cnode, _cf );
    if ( result == NULL && _result == NULL )
    {
        return NULL;
    }
    else
    if ( _result == NULL )
    {
        return result;
    }
    else
    if ( result == NULL )
    {
        return _result;
    }
    else
    {
        for ( size_t i = 0; i < row_size; i++ )
        {
            result[ i ] += _result[ i ];
        }
        delete[] _result;
        return result;
    }
}


#endif
