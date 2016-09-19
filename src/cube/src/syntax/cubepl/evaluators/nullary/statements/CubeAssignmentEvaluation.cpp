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


#ifndef __ASSIGNMENT_EVALUATION_CPP
#define __ASSIGNMENT_EVALUATION_CPP 0
#include "config.h"
#define CUBE_DEBUG_MODULE_NAME CUBEPL_VARIABLES
#include <UTILS_Debug.h>


#include "CubeAssignmentEvaluation.h"
#include "CubeStringEvaluation.h"

using namespace cube;
using namespace std;

#ifdef HAVE_CUBE_DEBUG
#include <sstream>
#endif

AssignmentEvaluation::~AssignmentEvaluation()
{
    delete value;
    delete index;
}

double
AssignmentEvaluation::eval()
{
    double _index = index->eval();

    if ( !value->isString() )
    {
        double _value = value->eval();
#ifdef HAVE_CUBE_DEBUG
        if ( verbose_execution )
        {
            UTILS_DEBUG_PRINTF
            (
                ( CUBE_DEBUG_CUBEPL_VARIABLES | CUBE_DEBUG_CUBEPL_EXECUTION ),
                "name=%s,adress=%d,index=%g,type=%s,new value=%g",
                variable_name.c_str(),
                variable,
                _index,
                ( ( kind == CUBEPL_GLOBAL_VARIABLE ) ?
                  "global" :
                  ( ( kind == CUBEPL_VARIABLE ) ?
                    "local" :
                    ( ( kind == CUBEPL_RESERVED_VARIABLE ) ? "reserved" : "unknown" )
                  )
                ),
                _value
            );
        }
#endif
        memory->put( variable, _index, _value, kind );
    }
    else
    {
        string _value =  ( dynamic_cast<StringEvaluation*>( value ) )->strEval();
#ifdef HAVE_CUBE_DEBUG
        if ( verbose_execution )
        {
            UTILS_DEBUG_PRINTF
            (
                ( CUBE_DEBUG_CUBEPL_VARIABLES | CUBE_DEBUG_CUBEPL_EXECUTION ),
                "name=%s,adress=%d,index=%g,type=%s,new value=%s",
                variable_name.c_str(),
                variable,
                _index,
                ( ( kind == CUBEPL_GLOBAL_VARIABLE ) ?
                  "global" :
                  ( ( kind == CUBEPL_VARIABLE ) ?
                    "local" :
                    ( ( kind == CUBEPL_RESERVED_VARIABLE ) ? "reserved" : "unknown" )
                  )
                ),
                _value.c_str()
            );
        }
#endif
        memory->put( variable, _index, _value, kind );
    }
    return 0.;
}



double
AssignmentEvaluation::eval( Cnode* cnode, CalculationFlavour cf, Sysres* sr, CalculationFlavour tf  )
{
    double _index = index->eval( cnode, cf, sr, tf  );

    if (  !value->isString() )
    {
        double _value = value->eval( cnode, cf, sr, tf );
        #ifdef HAVE_CUBE_DEBUG
        if ( verbose_execution )
        {
            UTILS_DEBUG_PRINTF
            (
                ( CUBE_DEBUG_CUBEPL_VARIABLES | CUBE_DEBUG_CUBEPL_EXECUTION ),
                "name=%s,adress=%d,index=%g,type=%s,new value=%g",
                variable_name.c_str(),
                variable,
                _index,
                ( ( kind == CUBEPL_GLOBAL_VARIABLE ) ?
                  "global" :
                  ( ( kind == CUBEPL_VARIABLE ) ?
                    "local" :
                    ( ( kind == CUBEPL_RESERVED_VARIABLE ) ? "reserved" : "unknown" )
                  )
                ),
                _value
            );
        }
#endif
        memory->put( variable, _index, _value, kind  );
    }
    else
    {
        string _value =  ( dynamic_cast<StringEvaluation*>( value ) )->strEval();
#ifdef HAVE_CUBE_DEBUG
        if ( verbose_execution )
        {
            UTILS_DEBUG_PRINTF
            (
                ( CUBE_DEBUG_CUBEPL_VARIABLES | CUBE_DEBUG_CUBEPL_EXECUTION ),
                "name=%s,adress=%d,index=%g,type=%s,new value=%s",
                variable_name.c_str(),
                variable,
                _index,
                ( ( kind == CUBEPL_GLOBAL_VARIABLE ) ?
                  "global" :
                  ( ( kind == CUBEPL_VARIABLE ) ?
                    "local" :
                    ( ( kind == CUBEPL_RESERVED_VARIABLE ) ? "reserved" : "unknown" )
                  )
                ),
                _value.c_str()
            );
        }
#endif
        memory->put( variable, _index, _value, kind  );
    }
    return 0.;
};



double
AssignmentEvaluation::eval( Cnode* cnode, CalculationFlavour cf )
{
    double _index = index->eval( cnode, cf  );

    if (  !value->isString() )
    {
        double _value = value->eval( cnode, cf );
        #ifdef HAVE_CUBE_DEBUG
        if ( verbose_execution )
        {
            UTILS_DEBUG_PRINTF
            (
                ( CUBE_DEBUG_CUBEPL_VARIABLES | CUBE_DEBUG_CUBEPL_EXECUTION ),
                "name=%s,adress=%d,index=%g,type=%s,new value=%g",
                variable_name.c_str(),
                variable,
                _index,
                ( ( kind == CUBEPL_GLOBAL_VARIABLE ) ?
                  "global" :
                  ( ( kind == CUBEPL_VARIABLE ) ?
                    "local" :
                    ( ( kind == CUBEPL_RESERVED_VARIABLE ) ? "reserved" : "unknown" )
                  )
                ),
                _value
            );
        }
#endif
        memory->put( variable, _index, _value, kind  );
    }
    else
    {
        string _value =  ( dynamic_cast<StringEvaluation*>( value ) )->strEval();
#ifdef HAVE_CUBE_DEBUG
        if ( verbose_execution )
        {
            UTILS_DEBUG_PRINTF
            (
                ( CUBE_DEBUG_CUBEPL_VARIABLES | CUBE_DEBUG_CUBEPL_EXECUTION ),
                "name=%s,adress=%d,index=%g,type=%s,new value=%s",
                variable_name.c_str(),
                variable,
                _index,
                ( ( kind == CUBEPL_GLOBAL_VARIABLE ) ?
                  "global" :
                  ( ( kind == CUBEPL_VARIABLE ) ?
                    "local" :
                    ( ( kind == CUBEPL_RESERVED_VARIABLE ) ? "reserved" : "unknown" )
                  )
                ),
                _value.c_str()
            );
        }
#endif
        memory->put( variable, _index, _value, kind  );
    }
    return 0.;
};


double
AssignmentEvaluation::eval( double arg1, double arg2 )
{
    double _index = index->eval( arg1, arg2  );
    if (  !value->isString() )
    {
        double _value = value->eval( arg1, arg2 );
#ifdef HAVE_CUBE_DEBUG
        if ( verbose_execution )
        {
            UTILS_DEBUG_PRINTF
            (
                ( CUBE_DEBUG_CUBEPL_VARIABLES | CUBE_DEBUG_CUBEPL_EXECUTION ),
                "name=%s,adress=%d,index=%g,type=%s,new value=%g",
                variable_name.c_str(),
                variable,
                _index,
                ( ( kind == CUBEPL_GLOBAL_VARIABLE ) ?
                  "global" :
                  ( ( kind == CUBEPL_VARIABLE ) ?
                    "local" :
                    ( ( kind == CUBEPL_RESERVED_VARIABLE ) ? "reserved" : "unknown" )
                  )
                ),
                _value
            );
        }
#endif
        memory->put( variable, _index, _value, kind  );
    }
    else
    {
        string _value =  ( dynamic_cast<StringEvaluation*>( value ) )->strEval();
#ifdef HAVE_CUBE_DEBUG
        if ( verbose_execution )
        {
            UTILS_DEBUG_PRINTF
            (
                ( CUBE_DEBUG_CUBEPL_VARIABLES | CUBE_DEBUG_CUBEPL_EXECUTION ),
                "name=%s,adress=%d,index=%g,type=%s,new value=%s",
                variable_name.c_str(),
                variable,
                _index,
                ( ( kind == CUBEPL_GLOBAL_VARIABLE ) ?
                  "global" :
                  ( ( kind == CUBEPL_VARIABLE ) ?
                    "local" :
                    ( ( kind == CUBEPL_RESERVED_VARIABLE ) ? "reserved" : "unknown" )
                  )
                ),
                _value.c_str()
            );
        }
#endif
        memory->put( variable, _index, _value, kind  );
    }
    return 0.;
};



double*
AssignmentEvaluation::eval_row( Cnode*             cnode,
                                CalculationFlavour cf )
{
    double _index = index->eval( cnode, cf );
    if (  !value->isString() )
    {
        double* _tmp = value->eval_row( cnode, cf );
#ifdef HAVE_CUBE_DEBUG
        if ( verbose_execution )
        {
            stringstream str;
            for ( size_t i = 0; i < row_size; ++i )
            {
                str << _tmp[ i ];
                if ( i != row_size - 1 )
                {
                    str << ",";
                }
            }
            std::string _str;
            str >> _str;
            UTILS_DEBUG_PRINTF
            (
                ( CUBE_DEBUG_CUBEPL_VARIABLES | CUBE_DEBUG_CUBEPL_EXECUTION ),
                "name=%s,adress=%d,index=%g,type=%s,value=%s",
                variable_name.c_str(),
                variable,
                _index,
                ( ( kind == CUBEPL_GLOBAL_VARIABLE ) ?
                  "global" :
                  ( ( kind == CUBEPL_VARIABLE ) ?
                    "local" :
                    ( ( kind == CUBEPL_RESERVED_VARIABLE ) ? "reserved" : "unknown" )
                  )
                ),
                _str.c_str()
            );
        }
#endif

        memory->put( variable, _index, _tmp, kind  );
    }
    return NULL;
}




#endif
