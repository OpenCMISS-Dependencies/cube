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


#ifndef __STRING_SEMI_EQUALITY_EVALUATION_CPP
#define __STRING_SEMI_EQUALITY_EVALUATION_CPP 0


#include "CubeServices.h"
#include "CubeStringSemiEqualityEvaluation.h"
#include "CubeStringEvaluation.h"

#define SEMI_COMPARE_STRING( A, B ) services::lowercase( A->strEval() ).compare(  services::lowercase( B->strEval() ) ) == 0



using namespace cube;
using namespace std;



StringSemiEqualityEvaluation::StringSemiEqualityEvaluation() : BinaryEvaluation()
{
};

StringSemiEqualityEvaluation::StringSemiEqualityEvaluation( GeneralEvaluation* _arg1,
                                                            GeneralEvaluation* _arg2 ) : BinaryEvaluation( _arg1, _arg2 )
{
};


StringSemiEqualityEvaluation::~StringSemiEqualityEvaluation()
{
}

double
StringSemiEqualityEvaluation::eval()
{
    StringEvaluation* _val1 = dynamic_cast<StringEvaluation*>( arguments[ 1 ] );
    StringEvaluation* _val2 = dynamic_cast<StringEvaluation*>( arguments[ 0 ] );

    if ( _val1 == NULL )
    {
        return 0.;
    }
    if ( _val2 == NULL )
    {
        return 0.;
    }


    return ( SEMI_COMPARE_STRING( _val1, _val2 ) ) ? 1. : 0.;
}




double
StringSemiEqualityEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   )
{
    return eval();
}

double
StringSemiEqualityEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf )
{
    return eval();
}

double
StringSemiEqualityEvaluation::eval( double arg1, double arg2 )
{
    return eval();
}


#endif
