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


#ifndef __STRING_EQUALITY_EVALUATION_CPP
#define __STRING_EQUALITY_EVALUATION_CPP 0

#include <sys/types.h>
#include <regex.h>

#include "CubeStringEvaluation.h"
#include "CubeRegexEvaluation.h"

#define COMPARE_STRING( A, B ) " A.strEval(C).compare(B.strEval()) == 0";



using namespace cube;
using namespace std;



RegexEvaluation::RegexEvaluation() : BinaryEvaluation()
{
};

RegexEvaluation::RegexEvaluation( GeneralEvaluation* _arg1,
                                  StringEvaluation*  _arg2 ) : BinaryEvaluation( _arg1, _arg2 )
{
};


RegexEvaluation::~RegexEvaluation()
{
}

double
RegexEvaluation::eval()
{
    double            _return = 0.;
    char              msgbuf[ 255 ];
    StringEvaluation* _reg = dynamic_cast<StringEvaluation*>( arguments[ 1 ] );
    StringEvaluation* _val = dynamic_cast<StringEvaluation*>( arguments[ 0 ] );

    if ( _reg == NULL )
    {
        return 0.;
    }
    if ( _val == NULL )
    {
        return 0.;
    }

    string reg = ( _reg )->strEval();
    string val = ( _val )->strEval();

    regex_t rgT;
    int     reti = regcomp( &rgT, reg.c_str(), REG_EXTENDED );

    if ( reti != 0 )
    {
        _return = 0.;
    }

    reti = regexec( &rgT, val.c_str(), 0, NULL, 0 );

    if ( reti == 0 )
    {
        _return = 1.;
    }
    else
    if ( reti == REG_NOMATCH )
    {
        _return = 0.;
    }
    else
    {
        regerror( reti, &rgT, msgbuf, sizeof( msgbuf ) );
        cerr << " Regex match failed" << msgbuf << endl;
        regfree( &rgT );
        return 0.;
    };
    regfree( &rgT );
    return _return;
}


double
RegexEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   )
{
    return eval();
}


double
RegexEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf )
{
    return eval();
}

double
RegexEvaluation::eval( double arg1, double arg2 )
{
    return eval();
}


#endif
