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


#ifndef __ARGUMENT_EVALUATION_H
#define __ARGUMENT_EVALUATION_H 0

#include "CubeNullaryEvaluation.h"

namespace cube
{
// / Used to specify, if an inclusive or an exclusive value is being calculated
enum NumberOfArgument { FIRST_ARGUMENT = 0, SECOND_ARGUMENT = 1 };

class ArgumentEvaluation : public NullaryEvaluation
{
protected:
    NumberOfArgument number_of_argument;

public:
    ArgumentEvaluation( NumberOfArgument  );
    ~ArgumentEvaluation();

    virtual
    double
    eval();

    virtual
    double
    eval( double arg1,
          double arg2 );


    virtual
    double
    eval( Cnode *, CalculationFlavour, Sysres *, CalculationFlavour   );

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
        if ( number_of_argument == cube::FIRST_ARGUMENT )
        {
            std::cout << "arg1";
        }
        else
        {
            std::cout << "arg2";
        }
    };
};
};

#endif
