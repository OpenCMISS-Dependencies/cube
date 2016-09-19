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


#ifndef __POWER_EVALUATION_CPP
#define __POWER_EVALUATION_CPP 0

#include "CubePowerEvaluation.h"
using namespace cube;
using namespace std;



PowerEvaluation::PowerEvaluation() : BinaryEvaluation()
{
};

PowerEvaluation::PowerEvaluation( GeneralEvaluation* _arg1,
                                  GeneralEvaluation* _arg2 ) : BinaryEvaluation( _arg1, _arg2 )
{
};


PowerEvaluation::~PowerEvaluation()
{
}

double
PowerEvaluation::eval()
{
    if ( getNumOfParameters() == 2 )
    {
        double base  = arguments[ 0 ]->eval();
        double power = arguments[ 1 ]->eval();
        return pow( base, power );
    }
    cerr << " Power is not defined for else  than two paraments." << endl;
    return 0.;
}


double
PowerEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   )
{
    if ( getNumOfParameters() == 2 )
    {
        double base = arguments[ 0 ]->eval(       _cnode, _cf,
                                                  _sf, _tf );
        double power = arguments[ 1 ]->eval(      _cnode, _cf,
                                                  _sf, _tf );
        return pow( base, power );
    }
    cerr << " Power is not defined for else  than two paraments." << endl;
    return 0.;
}


double
PowerEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf )
{
    if ( getNumOfParameters() == 2 )
    {
        double base  = arguments[ 0 ]->eval(       _cnode, _cf );
        double power = arguments[ 1 ]->eval(      _cnode, _cf );
        return pow( base, power );
    }
    cerr << " Power is not defined for else  than two paraments." << endl;
    return 0.;
}

double
PowerEvaluation::eval( double arg1, double arg2 )
{
    if ( getNumOfParameters() == 2 )
    {
        double base  = arguments[ 0 ]->eval( arg1, arg2 );
        double power = arguments[ 1 ]->eval( arg1, arg2 );
        return pow( base, power );
    }
    cerr << " Power is not defined for else  than two paraments." << endl;
    return 0.;
}



double*
PowerEvaluation::eval_row( Cnode*             _cnode,
                           CalculationFlavour _cf )
{
    double* result = arguments[ 0 ]->eval_row( _cnode, _cf );
    if ( result == NULL )
    {
        return NULL;
    }
    double* _result = arguments[ 1 ]->eval_row( _cnode, _cf );
    if ( _result == NULL )
    {
        for ( size_t i = 0; i < row_size; i++ )
        {
            result[ i ] =  ( result[ i ] != 0. ) ? 1. : 0.;
        }
    }
    else
    {
        for ( size_t i = 0; i < row_size; i++ )
        {
            result[ i ] =  pow( result[ i ], _result[ i ] );
        }
        delete _result;
    }
    return result;
}


#endif
