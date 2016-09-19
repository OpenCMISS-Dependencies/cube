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


#ifndef __CONSTANT_EVALUATION_CPP
#define __CONSTANT_EVALUATION_CPP 0

#include <string>
#include <sstream>

#include "CubeServices.h"
#include "CubeConstantEvaluation.h"



using namespace cube;
using namespace std;

ConstantEvaluation::ConstantEvaluation( double _const ) : NullaryEvaluation()
{
    constant = _const;
};

ConstantEvaluation::ConstantEvaluation( std::string _const ) : NullaryEvaluation()
{
    std::istringstream stream( _const );
    stream >> constant;
};

ConstantEvaluation::~ConstantEvaluation()
{
}

double
ConstantEvaluation::eval()
{
    return constant;
}


double
ConstantEvaluation::eval( double arg1, double arg2 )
{
    return constant;
}




double*
ConstantEvaluation::eval_row( Cnode*             _cnode,
                              CalculationFlavour _cf )
{
    double* result = services::create_row_of_doubles( row_size );
    for ( size_t i = 0; i < row_size; i++ )
    {
        result[ i ] = constant;
    }
    return result;
}




#endif
