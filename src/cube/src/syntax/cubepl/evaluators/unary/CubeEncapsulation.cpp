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


#ifndef __ENCAPSULATION_EVALUATION_CPP
#define __ENCAPSULATION_EVALUATION_CPP 0

#include "CubeEncapsulation.h"
using namespace cube;
using namespace std;

EncapsulationEvaluation::EncapsulationEvaluation() : UnaryEvaluation()
{
};

EncapsulationEvaluation::EncapsulationEvaluation( GeneralEvaluation* _arg ) : UnaryEvaluation( _arg )
{
};


EncapsulationEvaluation::~EncapsulationEvaluation()
{
}

double
EncapsulationEvaluation::eval()
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        return arguments[ 0 ]->eval();
    }
    cerr << " Encapsulation with more than 1 parameters is not defined" << endl;
    return 0;
}



double
EncapsulationEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf, cube::Sysres* _sf, CalculationFlavour _tf   )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _value = arguments[ 0 ]->eval(     _cnode, _cf,
                                                  _sf, _tf
                                                  );
        return _value;
    }
    cerr << " Encapsulation with more than 1 parameters is not defined" << endl;
    return 0;
}



double
EncapsulationEvaluation::eval( cube::Cnode* _cnode, CalculationFlavour _cf )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _value = arguments[ 0 ]->eval(     _cnode, _cf
                                                  );
        return _value;
    }
    cerr << " Encapsulation with more than 1 parameters is not defined" << endl;
    return 0;
}

double
EncapsulationEvaluation::eval( double arg1, double arg2 )
{
    if ( getNumOfParameters() == 0 )
    {
        return 0;
    }
    if ( getNumOfParameters() == 1 )
    {
        double _value = arguments[ 0 ]->eval( arg1, arg2  );
        return _value;
    }
    cerr << " Encapsulation with more than 1 parameters is not defined" << endl;
    return 0;
}




double*
EncapsulationEvaluation::eval_row( Cnode*             _cnode,
                                   CalculationFlavour _cf )
{
    return arguments[ 0 ]->eval_row( _cnode, _cf );
}


#endif
