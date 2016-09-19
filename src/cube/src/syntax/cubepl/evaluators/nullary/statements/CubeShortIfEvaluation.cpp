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


#ifndef __SHORT_IF_EVALUATION_CPP
#define __SHORT_IF_EVALUATION_CPP 0

#include "CubeShortIfEvaluation.h"

using namespace cube;


ShortIfEvaluation::~ShortIfEvaluation()
{
    delete condition;
}

double
ShortIfEvaluation::eval()
{
    double _tmp = condition->eval();
    if ( _tmp != 0 )
    {
        for ( unsigned i = 0; i < getNumOfParameters(); ++i )
        {
            arguments[ i ]->eval();
        }
    }
    return 0.;
}


#endif
