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


#ifndef __DIVIDE_EVALUATION_CPP
#define __DIVIDE_EVALUATION_CPP 0

#include "CubeDivideEvaluation.h"

using namespace cube;
using namespace std;



DivideEvaluation::DivideEvaluation() : BinaryEvaluation()
{
};

DivideEvaluation::DivideEvaluation( GeneralEvaluation* _arg1,
                                    GeneralEvaluation* _arg2 ) : BinaryEvaluation( _arg1, _arg2 )
{
};


DivideEvaluation::~DivideEvaluation()
{
}

double
DivideEvaluation::eval()
{
    if ( getNumOfParameters() == 2 )
    {
        double denom = arguments[ 0 ]->eval();
        if ( denom == 0. )
        {
            return 0.;
        }

        double divid = arguments[ 1 ]->eval();



        if ( divid == 0 )
        {
            if ( denom > 0 )
            {
                return DBL_MAX;
            }
            else
            {
                return -DBL_MAX;
            }
        }
        else
        {
            return ( denom ) / ( divid );
        }
    }
    cerr << " Division is not defined for else  than two paraments." << endl;
    return 0.;
}



double
DivideEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   )
{
    if ( getNumOfParameters() == 2 )
    {
        double denom = arguments[ 0 ]->eval( _cnode, _cf,
                                             _sf, _tf );
        if ( denom == 0. )
        {
            return 0.;
        }
        double divid = arguments[ 1 ]->eval( _cnode, _cf,
                                             _sf, _tf );



        if ( divid == 0. )
        {
            if ( denom > 0. )
            {
                return DBL_MAX;
            }
            else
            {
                return -DBL_MAX;
            }
        }
        else
        {
            return ( denom ) / ( divid );
        }
    }
    cerr << " Division is not defined for else  than two paraments." << endl;
    return 0.;
}


double
DivideEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf )
{
    if ( getNumOfParameters() == 2 )
    {
        double denom = arguments[ 0 ]->eval( _cnode, _cf );
        if ( denom == 0. )
        {
            return 0.;
        }
        double divid = arguments[ 1 ]->eval( _cnode, _cf );




        if ( divid == 0 )
        {
            if ( denom > 0 )
            {
                return DBL_MAX;
            }
            else
            {
                return -DBL_MAX;
            }
        }
        else
        {
            return ( denom ) / ( divid );
        }
    }
    cerr << " Division is not defined for else  than two paraments." << endl;
    return 0.;
}

double
DivideEvaluation::eval( double arg1, double arg2 )
{
    if ( getNumOfParameters() == 2 )
    {
        double denom = arguments[ 0 ]->eval( arg1, arg2 );
        if ( denom == 0. )
        {
            return 0.;
        }

        double divid = arguments[ 1 ]->eval( arg1, arg2 );

        if ( divid == 0 )
        {
            if ( denom > 0 )
            {
                return DBL_MAX;
            }
            else
            {
                return -DBL_MAX;
            }
        }
        else
        {
            return ( denom ) / ( divid );
        }
    }
    cerr << " Division is not defined for else  than two paraments." << endl;
    return 0.;
}



double*
DivideEvaluation::eval_row( Cnode*             _cnode,
                            CalculationFlavour _cf )
{
    double* denom = arguments[ 0 ]->eval_row( _cnode, _cf );
    double* divid = arguments[ 1 ]->eval_row( _cnode, _cf );
    if ( denom == NULL || divid == NULL )
    {
        delete[] denom;
        delete[] divid;
        return NULL;
    }

    double* result = denom;
    for ( size_t i = 0; i < row_size; i++ )
    {
        if ( denom[ i ] == 0. )
        {
            result[ i ] = 0.;
            continue;
        }

        if ( divid[ i ] == 0 )
        {
            if ( denom[ i ] > 0 )
            {
                result[ i ] = DBL_MAX;
            }
            else
            {
                result[ i ] = -DBL_MAX;
            }
        }
        else
        {
            result[ i ] = ( denom[ i ] ) / ( divid[ i ] );
        }
    }
    delete[] divid;
    return result;
}


#endif
