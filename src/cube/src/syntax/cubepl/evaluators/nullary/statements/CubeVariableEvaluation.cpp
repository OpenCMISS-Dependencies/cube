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


#ifndef __VARIABLE_EVALUATION_CPP
#define __VARIABLE_EVALUATION_CPP 0

#include "config.h"
#define CUBE_DEBUG_MODULE_NAME CUBEPL_VARIABLES
#include <UTILS_Debug.h>



#include "CubeVariableEvaluation.h"
#include "CubeServices.h"

using namespace cube;
using namespace std;

#ifdef HAVE_CUBE_DEBUG
#include <sstream>
#endif

VariableEvaluation::~VariableEvaluation()
{
    delete index;
}


bool
VariableEvaluation::isString()
{
    return memory->type( variable, index->eval(), kind ) == CUBEPL_VALUE_STRING;
}

double
VariableEvaluation::eval()
{
    double _index = index->eval();
    double _value = memory->get( variable, _index, kind  );
#ifdef HAVE_CUBE_DEBUG
    if ( verbose_execution )
    {
        UTILS_DEBUG_PRINTF
        (
            ( CUBE_DEBUG_CUBEPL_VARIABLES | CUBE_DEBUG_CUBEPL_EXECUTION ),
            "name=%s,adress=%d,index=%g,type=%s,value=%g",
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
    return _value;
}



double
VariableEvaluation::eval( Cnode* cnode, CalculationFlavour cf, Sysres* sr, CalculationFlavour tf  )
{
    double _index = index->eval( cnode, cf, sr, tf );
    double _value = memory->get( variable, _index, kind  );
#ifdef HAVE_CUBE_DEBUG
    if ( verbose_execution )
    {
        UTILS_DEBUG_PRINTF
        (
            ( CUBE_DEBUG_CUBEPL_VARIABLES | CUBE_DEBUG_CUBEPL_EXECUTION ),
            "name=%s,adress=%d,index=%g,type=%s,value=%g",
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
    return _value;
};



double
VariableEvaluation::eval( Cnode* cnode, CalculationFlavour cf )
{
    double _index = index->eval( cnode, cf );
    double _value = memory->get( variable, _index, kind  );
#ifdef HAVE_CUBE_DEBUG
    if ( verbose_execution )
    {
        UTILS_DEBUG_PRINTF
        (
            ( CUBE_DEBUG_CUBEPL_VARIABLES | CUBE_DEBUG_CUBEPL_EXECUTION ),
            "name=%s,adress=%d,index=%g,type=%s,value=%g",
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
    return _value;
};


double
VariableEvaluation::eval( double arg1, double arg2 )
{
    double _index = index->eval( arg1, arg2 );
    double _value = memory->get( variable, _index, kind  );
#ifdef HAVE_CUBE_DEBUG
    if ( verbose_execution )
    {
        UTILS_DEBUG_PRINTF
        (
            ( CUBE_DEBUG_CUBEPL_VARIABLES | CUBE_DEBUG_CUBEPL_EXECUTION ),
            "name=%s,adress=%d,index=%g,type=%s,value=%g",
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
    return _value;
};



string
VariableEvaluation::strEval()
{
    double _index = index->eval();
    string _value = memory->get_as_string( variable, _index, kind  );
#ifdef HAVE_CUBE_DEBUG
    if ( verbose_execution )
    {
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
            _value.c_str()
        );
    }
#endif
    return _value;
}


double*
VariableEvaluation::eval_row( Cnode*             cnode,
                              CalculationFlavour cf )
{
    double  _index = index->eval( cnode, cf );
    double* _tmp   = memory->get_row( variable, _index, kind  );

    if ( _tmp == NULL )
    {
#ifdef HAVE_CUBE_DEBUG
        if ( verbose_execution )
        {
            UTILS_DEBUG_PRINTF
            (
                ( CUBE_DEBUG_CUBEPL_VARIABLES | CUBE_DEBUG_CUBEPL_EXECUTION ),
                "name=%s,adress=%d,index=%g,type=%s,value=NULL",
                variable_name.c_str(),
                variable,
                _index,
                ( ( kind == CUBEPL_GLOBAL_VARIABLE ) ?
                  "global" :
                  ( ( kind == CUBEPL_VARIABLE ) ?
                    "local" :
                    ( ( kind == CUBEPL_RESERVED_VARIABLE ) ? "reserved" : "unknown" )
                  )
                )
            );
        }
#endif
        return NULL;
    }
    double* to_return = services::create_row_of_doubles( row_size );
    memcpy( to_return, _tmp, row_size * sizeof( double ) );
#ifdef HAVE_CUBE_DEBUG
    if ( verbose_execution )
    {
        stringstream str;
        for ( size_t i = 0; i < row_size; ++i )
        {
            str << to_return[ i ];
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
    return to_return;
}



#endif
