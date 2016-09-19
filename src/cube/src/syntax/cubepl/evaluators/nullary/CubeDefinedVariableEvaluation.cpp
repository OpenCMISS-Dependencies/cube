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


#ifndef __DEFINED_VARIABLE_EVALUATION_CPP
#define __DEFINED_VARIABLE_EVALUATION_CPP 0

#include "CubeDefinedVariableEvaluation.h"

using namespace cube;
using namespace std;

DefinedVariableEvaluation::~DefinedVariableEvaluation()
{
}


double
DefinedVariableEvaluation::eval()
{
    return ( memory->defined( variable_name ) ) ? 1. : 0.;
}



string
DefinedVariableEvaluation::strEval()
{
    return ( memory->defined( variable_name ) ) ? "1" : "0";
}


#endif
