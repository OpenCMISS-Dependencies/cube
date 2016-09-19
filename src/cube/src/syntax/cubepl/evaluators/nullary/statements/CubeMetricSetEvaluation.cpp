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


#ifndef __METRIC_SET_EVALUATION_CPP
#define __METRIC_SET_EVALUATION_CPP 0

#include "CubeMetricSetEvaluation.h"
#include "CubeStringConstantEvaluation.h"

using namespace cube;
using namespace std;


MetricSetEvaluation::~MetricSetEvaluation()
{
    delete property;
    delete value;
}

double
MetricSetEvaluation::eval()
{
    if (metric == NULL)
    {
	return 0.;
    }
    StringConstantEvaluation* prop = dynamic_cast<StringConstantEvaluation*>( property );
    StringConstantEvaluation* val  = dynamic_cast<StringConstantEvaluation*>( value );

    string _prop  = ( prop  != NULL ) ? prop->strEval() : "";
    string _value = ( val != NULL ) ? val->strEval() : "";

    if ( _prop == "value" )
    {
        metric->set_val( _value );
        return 0.;
    }
    cerr << "Supported only \"value\". Property \"" << _prop << "\" is ignored" << endl;
    return 0.;
}


#endif
