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


#ifndef __WHILE_EVALUATION_CPP
#define __WHILE_EVALUATION_CPP 0

#include "CubeWhileEvaluation.h"

using namespace cube;


WhileEvaluation::~WhileEvaluation()
{
    delete condition;
}

double
WhileEvaluation::eval()
{
    unsigned counter = 0;
    while ( condition->eval() != 0 )
    {
        for ( unsigned i = 0; i < getNumOfParameters(); ++i )
        {
            arguments[ i ]->eval();
        }
        counter++;
        if ( counter >= 1000000000 )
        {
            break;
        }
    }
    return 0.;
}


#endif
