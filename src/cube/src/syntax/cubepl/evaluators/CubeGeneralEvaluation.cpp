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



#include <vector>


#include "CubeGeneralEvaluation.h"
#include "CubePL1MemoryManager.h"

using namespace cube;
using namespace std;

GeneralEvaluation::GeneralEvaluation()
{
    row_size          = 0;
    verbose_execution = false;
    is_string         = false;
};

GeneralEvaluation::~GeneralEvaluation()
{
    for ( vector<GeneralEvaluation*>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter )
    {
        delete ( *iter );
    }
}


size_t
GeneralEvaluation::getNumOfParameters()
{
    return arguments.size();
}


size_t
GeneralEvaluation::getNumOfArguments()
{
    return 0;
}


void
GeneralEvaluation::addArgument( GeneralEvaluation* _arg )
{
    arguments.push_back( _arg );
}


double
GeneralEvaluation::eval()
{
    return 0;
}

