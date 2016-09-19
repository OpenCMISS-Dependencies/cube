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


#ifndef __AND_EVALUATION_CPP
#define __AND_EVALUATION_CPP 0

#include "CubeAndEvaluation.h"
using namespace cube;
using namespace std;

AndEvaluation::AndEvaluation() : BinaryEvaluation()
{
};

AndEvaluation::AndEvaluation( GeneralEvaluation* _arg1,
                              GeneralEvaluation* _arg2 ) : BinaryEvaluation( _arg1, _arg2 )
{
};


AndEvaluation::~AndEvaluation()
{
}

double
AndEvaluation::eval()
{
    double result = 0;
    for ( vector<GeneralEvaluation*>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter )
    {
        result = ( *iter )->eval();
        if ( result == 0. )
        {
            return 0.;
        }
    }
    return 1;
}


double
AndEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   )
{
    double result = 0;
    for ( vector<GeneralEvaluation*>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter )
    {
        result = ( *iter )->eval( _cnode, _cf,
                                  _sf, _tf );
        if ( result == 0. )
        {
            return 0.;
        }
    }
    return 1;
}





double
AndEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf )
{
    double result = 0;
    for ( vector<GeneralEvaluation*>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter )
    {
        result = ( *iter )->eval( _cnode, _cf );
        if ( result == 0. )
        {
            return 0.;
        }
    }
    return 1.;
}

double
AndEvaluation::eval( double arg1, double arg2  )
{
    double result = 0;
    for ( vector<GeneralEvaluation*>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter )
    {
        result = ( *iter )->eval( arg1, arg2 );
        if ( result == 0. )
        {
            return 0.;
        }
    }
    return result;
}



double*
AndEvaluation::eval_row( Cnode*             _cnode,
                         CalculationFlavour _cf )
{
    double* result  = arguments[ 0 ]->eval_row( _cnode, _cf );
    double* _result = arguments[ 1 ]->eval_row( _cnode, _cf );
    if ( result == NULL || _result == NULL )
    {
        return NULL;
    }
    for ( size_t i = 0; i < row_size; i++ )
    {
        result[ i ] = ( result[ i ] != 0. &&  _result[ i ] != 0. ) ? 1. : 0.;
    }
    delete[] _result;
    return result;
}


#endif
