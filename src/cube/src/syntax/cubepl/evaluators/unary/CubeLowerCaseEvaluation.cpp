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


#ifndef __LOWER_CASE_EVALUATION_CPP
#define __LOWER_CASE_EVALUATION_CPP 0

#include "CubeLowerCaseEvaluation.h"
#include "CubeServices.h"
using namespace cube;
using namespace std;

LowerCaseEvaluation::LowerCaseEvaluation() : StringEvaluation()
{
};

LowerCaseEvaluation::LowerCaseEvaluation( GeneralEvaluation* _val ) : StringEvaluation()
{
    arguments.push_back( _val );
};

LowerCaseEvaluation::~LowerCaseEvaluation()
{
};

string
LowerCaseEvaluation::strEval()
{
    StringEvaluation* _val = dynamic_cast<StringEvaluation*>( arguments[ 0 ] );

    return services::lowercase( _val->strEval() );
}


#endif
