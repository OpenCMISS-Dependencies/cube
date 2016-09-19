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


#ifndef __GENERAL_EVALUATION_H
#define __GENERAL_EVALUATION_H 0

#include <string>
#include <vector>
#include <iostream>
#include <float.h>
#include <cmath>
#include "CubeTypes.h"
#include "CubeSysres.h"
#include "CubeMetric.h"
#include "CubeThread.h"
#include "CubeSysres.h"
#include "CubeProcess.h"
#include "CubeNode.h"
#include "CubeMachine.h"
#include "CubeTypes.h"
#include "CubePL1MemoryManager.h"


namespace cube
{
class GeneralEvaluation
{
protected:
    size_t                           row_size;
    std::vector<GeneralEvaluation* > arguments;

    bool is_string;
    bool verbose_execution;
protected:


    size_t
    getNumOfParameters();

    virtual size_t
    getNumOfArguments();

public:
    GeneralEvaluation();

    virtual
    ~GeneralEvaluation();


    inline
    virtual
    bool
    isString()
    {
        return is_string;
    };

    inline
    virtual
    void
    setRowSize( size_t size )
    {
        row_size = size;
        for ( std::vector<GeneralEvaluation*>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter )
        {
            GeneralEvaluation* arg = *iter;
            arg->setRowSize( size );
        }
    };


    void
    addArgument( GeneralEvaluation* _arg );

    inline
    virtual
    double
    eval( Cnode* _cnode, CalculationFlavour _cf, Sysres* _sys, CalculationFlavour _sf  )
    {
        _cnode++;
        _sys++;
        _sf = _cf;
        _cf = _sf;
        return eval();
    };


    inline
    virtual
    double
    eval( Cnode* _cnode, CalculationFlavour _cf )
    {
        _cnode++;
        _cf = ( _cf ) ? CUBE_CALCULATE_INCLUSIVE : CUBE_CALCULATE_EXCLUSIVE;
        return eval();
    };


    inline
    virtual
    double*
    eval_row( Cnode* _cnode, CalculationFlavour _cf )
    {
        _cnode++;
        _cf = ( _cf ) ? CUBE_CALCULATE_INCLUSIVE : CUBE_CALCULATE_EXCLUSIVE;
        return NULL;
    };

    virtual
    double
    eval( double arg1, double arg2 )
    {
        return 0. * ( arg1 + arg2 );
    };

    inline
    virtual
    void
    set_verbose_execution( bool _v )
    {
        verbose_execution = _v;
        for ( std::vector<GeneralEvaluation*>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter )
        {
            GeneralEvaluation* arg = *iter;
            arg->set_verbose_execution( _v );
        }
    }


    virtual
    double
    eval();

    virtual
    void
    print()
    {
    };
};
}

#endif
