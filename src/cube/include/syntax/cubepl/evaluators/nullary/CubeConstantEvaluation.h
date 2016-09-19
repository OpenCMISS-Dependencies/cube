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


#ifndef __CONSTANT_EVALUATION_H
#define __CONSTANT_EVALUATION_H 0

#include "CubeNullaryEvaluation.h"

namespace cube
{
class ConstantEvaluation : public NullaryEvaluation
{
protected:
    double constant;

public:
    ConstantEvaluation( double _const );
    ConstantEvaluation( std::string _const );
    ~ConstantEvaluation();

    virtual
    double
    eval();

    virtual
    double
    eval( Cnode*, CalculationFlavour, Sysres*, CalculationFlavour   )
    {
        return eval();
    };


    virtual
    double
    eval( Cnode*, CalculationFlavour )
    {
        return eval();
    };

    virtual
    double*
    eval_row( Cnode*             _cnode,
              CalculationFlavour _cf );


    virtual
    void
    print()
    {
        std::cout << constant;
    };

    virtual
    double
    eval( double arg1,
          double arg2 );
};
};

#endif
