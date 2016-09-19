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


#ifndef __ARGUMENT_EVALUATION_CPP
#define __ARGUMENT_EVALUATION_CPP 0

#include "CubeServices.h"
#include "CubeArgumentEvaluation.h"



using namespace cube;

ArgumentEvaluation::ArgumentEvaluation( NumberOfArgument _argument ) : NullaryEvaluation()
{
    number_of_argument = _argument;
};


ArgumentEvaluation::~ArgumentEvaluation()
{
}

double
ArgumentEvaluation::eval()
{
    return 0.;
}


double
ArgumentEvaluation::eval( Cnode* _cnode, CalculationFlavour cf, Sysres* _sys, CalculationFlavour sf  )
{
    if (number_of_argument == cube::FIRST_ARGUMENT)
	return (double)(_cnode->get_id());
    return (double)(_sys->get_id());
};


double*
ArgumentEvaluation::eval_row( Cnode*             _cnode,
                              CalculationFlavour _cf )
{
    double* result = services::create_row_of_doubles( row_size );
    for ( size_t i = 0; i < row_size; i++ )
    {
        result[ i ] = (number_of_argument == cube::FIRST_ARGUMENT) ? (double)_cnode->get_id() : (double)i;
    }
    return result;
}



double
ArgumentEvaluation::eval(double arg1, double arg2)
{

    if (number_of_argument == cube::FIRST_ARGUMENT)
	return arg1;
    return arg2;
}

#endif
