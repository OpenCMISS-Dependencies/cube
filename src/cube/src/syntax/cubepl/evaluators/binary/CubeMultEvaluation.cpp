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


#ifndef __MULT_EVALUATION_CPP
#define __MULT_EVALUATION_CPP 0

#include "CubeMultEvaluation.h"
using namespace cube;
using namespace std;



MultEvaluation::MultEvaluation() : BinaryEvaluation()
{
};

MultEvaluation::MultEvaluation( GeneralEvaluation* _arg1,
                                GeneralEvaluation* _arg2 ) : BinaryEvaluation( _arg1, _arg2 )
{
};


MultEvaluation::~MultEvaluation()
{
}

double
MultEvaluation::eval()
{
    if ( getNumOfParameters() == 2 )
    {
	double _first = ( arguments[ 1 ]->eval( ) );
        if ( _first == 0. )
        {
            return 0.;
        }
    double _second = ( arguments[ 0 ]->eval( ) );
	double _value =  _first * _second;
        if ( fabs( _value ) == std::numeric_limits<double>::infinity() )
        {
            if ( _value < 0 )
            {
                return -DBL_MAX;
            }
            else
            {
                return DBL_MAX;
            }
        }
        else
        {
            return _value;
        }
    }
    cerr << " Multiplication is not defined for else  than two paraments." << endl;
    return 0.;
}



double
MultEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   )
{
    if ( getNumOfParameters() == 2 )
    {
        double _first = ( arguments[ 1 ]->eval( _cnode, _cf,
                                                _sf, _tf ) );
        if ( _first == 0. )
        {
            return 0.;
        }
        double _second = ( arguments[ 0 ]->eval( _cnode, _cf, _sf, _tf ) );

        double _value =  _first * _second;
        if ( fabs( _value ) == std::numeric_limits<double>::infinity() )
        {
            if ( _value < 0 )
            {
                return -DBL_MAX;
            }
            else
            {
                return DBL_MAX;
            }
        }
        else
        {
            return _value;
        }
    }
    cerr << " Multiplication is not defined for else  than two paraments." << endl;
    return 0.;
}

double
MultEvaluation::eval( double arg1, double arg2  )
{
    if ( getNumOfParameters() == 2 )
    {
        double _first = ( arguments[ 1 ]->eval( arg1, arg2  ) );
        if ( _first == 0. )
        {
            return 0.;
        }
        double _second = ( arguments[ 0 ]->eval( arg1, arg2  ) );

        double _value =  _first * _second;
        if ( fabs( _value ) == std::numeric_limits<double>::infinity() )
        {
            if ( _value < 0 )
            {
                return -DBL_MAX;
            }
            else
            {
                return DBL_MAX;
            }
        }
        else
        {
            return _value;
        }
    }
    cerr << " Multiplication is not defined for else  than two paraments." << endl;
    return 0.;
}


double
MultEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf )
{
    if ( getNumOfParameters() == 2 )
    {
        double _first = ( arguments[ 1 ]->eval( _cnode, _cf ) );
        if ( _first == 0. )
        {
            return 0.;
        }
        double _second = ( arguments[ 0 ]->eval( _cnode, _cf ) );

        double _value =  _first * _second;
        if ( fabs( _value ) == std::numeric_limits<double>::infinity() )
        {
            if ( _value < 0 )
            {
                return -DBL_MAX;
            }
            else
            {
                return DBL_MAX;
            }
        }
        else
        {
            return _value;
        }
    }
    cerr << " Multiplication is not defined for else  than two paraments." << endl;
    return 0.;
}


double*
MultEvaluation::eval_row( Cnode*             _cnode,
                          CalculationFlavour _cf )
{
    // cube pl expression A*B is compiled into B->arg0, A->arg2. therefore we evaluate first arg2 (A)
    double* arg2 = arguments[ 1 ]->eval_row( _cnode, _cf );
    if ( arg2 == NULL  )
    {
        delete[] arg2;
        return NULL;
    }
    double* result = arg2;
    bool    zero   = true;
    for ( size_t i = 0; i < row_size; i++ )
    {
        zero &= ( arg2[ i ] == 0. );
        if ( !zero )
        {
            break;
        }
    }
    if ( zero )
    {
        delete[] arg2;
        return NULL;
    }
     // then arg1 (B)
    double* arg1 = arguments[ 0 ]->eval_row( _cnode, _cf );
    for ( size_t i = 0; i < row_size; i++ )
    {
        result[ i ] = ( arg1[ i ] ) * ( arg2[ i ] );
        if ( fabs( result[ i ]  ) == std::numeric_limits<double>::infinity() )
        {
            if ( result[ i ]  < 0 )
            {
                result[ i ] = -DBL_MAX;
            }
            else
            {
                result[ i ] = DBL_MAX;
            }
        }
    }
    delete arg1;
    return result;
}


#endif
