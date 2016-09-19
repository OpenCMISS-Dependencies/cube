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


#ifndef __MINUS_EVALUATION_CPP
#define __MINUS_EVALUATION_CPP 0

#include "CubeMinusEvaluation.h"
using namespace cube;
using namespace std;



MinusEvaluation::MinusEvaluation() : BinaryEvaluation()
{
};

MinusEvaluation::MinusEvaluation( GeneralEvaluation* _arg1,
                                  GeneralEvaluation* _arg2 ) : BinaryEvaluation( _arg1, _arg2 )
{
};


MinusEvaluation::~MinusEvaluation()
{
}

double
MinusEvaluation::eval()
{
    if ( getNumOfParameters() == 2 )
    {
        return ( arguments[ 0 ]->eval() ) - ( arguments[ 1 ]->eval() );
    }
    cerr << " Substraction is not defined for else  than two paraments." << endl;
    return 0.;
}




double
MinusEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   )
{
    if ( getNumOfParameters() == 2 )
    {
        return ( arguments[ 0 ]->eval( _cnode, _cf,
                                       _sf, _tf ) ) - ( arguments[ 1 ]->eval( _cnode, _cf,
                                                                              _sf, _tf ) );
    }
    cerr << " Substraction is not defined for else  than two paraments." << endl;
    return 0.;
}


double
MinusEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf )
{
    if ( getNumOfParameters() == 2 )
    {
        return ( arguments[ 0 ]->eval( _cnode, _cf ) ) - ( arguments[ 1 ]->eval( _cnode, _cf ) );
    }
    cerr << " Substraction is not defined for else  than two paraments." << endl;
    return 0.;
}

double
MinusEvaluation::eval( double arg1, double arg2 )
{
    if ( getNumOfParameters() == 2 )
    {
        return ( arguments[ 0 ]->eval( arg1, arg2 ) ) - ( arguments[ 1 ]->eval( arg1, arg2 ) );
    }
    cerr << " Substraction is not defined for else  than two paraments." << endl;
    return 0.;
}




double*
MinusEvaluation::eval_row( Cnode*             _cnode,
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
        for ( size_t i = 0; i < row_size; i++ )
        {
            _result[ i ] = -_result[ i ];
        }
        return _result;
    }
    else
    {
        for ( size_t i = 0; i < row_size; i++ )
        {
            result[ i ] -= _result[ i ];
        }
        delete[] _result;
        return result;
    }
}


#endif
