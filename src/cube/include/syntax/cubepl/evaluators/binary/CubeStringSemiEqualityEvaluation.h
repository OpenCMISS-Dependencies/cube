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


#ifndef __STRING_SEMI_EQUALITY_EVALUATION_H
#define __STRING_SEMI_EQUALITY_EVALUATION_H 0

#include "CubeBinaryEvaluation.h"

namespace cube
{
class StringSemiEqualityEvaluation : public BinaryEvaluation
{
public:
    StringSemiEqualityEvaluation();
    StringSemiEqualityEvaluation( GeneralEvaluation*,
                                  GeneralEvaluation* );
    virtual
    ~StringSemiEqualityEvaluation();

    virtual
    double
    eval();

    virtual
    double eval( Cnode *, CalculationFlavour, Sysres *, CalculationFlavour   );

    virtual
    double eval( Cnode *, CalculationFlavour );


    virtual
    void
    print()
    {
        arguments[ 0 ]->print();
        std::cout << " seq ";
        arguments[ 1 ]->print();
    };

    virtual
    double
    eval( double arg1,
          double arg2 );
};
};

#endif
