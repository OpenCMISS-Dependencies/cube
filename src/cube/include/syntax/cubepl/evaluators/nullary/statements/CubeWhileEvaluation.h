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


#ifndef __WHILE_EVALUATION_H
#define __WHILE_EVALUATION_H 0

#include "CubeNullaryEvaluation.h"

namespace cube
{
class WhileEvaluation : public NullaryEvaluation
{
protected:
    GeneralEvaluation* condition;
public:
    WhileEvaluation( GeneralEvaluation* _condition ) : NullaryEvaluation(), condition( _condition )
    {
    };
    ~WhileEvaluation();

    inline
    virtual
    void
    setRowSize( size_t size )
    {
        row_size = size;
        condition->setRowSize( size );
        for ( std::vector<GeneralEvaluation*>::iterator iter = arguments.begin(); iter != arguments.end(); ++iter )
        {
            GeneralEvaluation* arg = *iter;
            arg->setRowSize( size );
        }
    }

    virtual
    double
    eval();

    
    inline
    virtual
    void
    set_verbose_execution( bool _v )
    {
	GeneralEvaluation::set_verbose_execution( _v );
	condition->set_verbose_execution( _v );
    }
    
    
    virtual
    double
    eval( Cnode* cnode, CalculationFlavour cf, Sysres* sr, CalculationFlavour tf  )
    {
        unsigned counter = 0;
        while ( condition->eval( cnode, cf, sr, tf ) != 0. )
        {
            for ( unsigned i = 0; i < getNumOfParameters(); ++i )
            {
                arguments[ i ]->eval( cnode, cf, sr, tf );
            }
            counter++;
            if ( counter >= 1000000000 )
            {
                break;
            }
        }
        return 0.;
    };


    virtual
    double
    eval( Cnode* cnode, CalculationFlavour cf )
    {
        unsigned counter = 0;
        while ( condition->eval( cnode, cf ) )
        {
            for ( unsigned i = 0; i < getNumOfParameters(); ++i )
            {
                arguments[ i ]->eval( cnode, cf );
            }
            counter++;
            if ( counter >= 1000000000 )
            {
                break;
            }
        }
        return 0.;
    };


    virtual
    double*
    eval_row( Cnode*             cnode,
              CalculationFlavour cf )
    {
        unsigned counter = 0;
        while ( condition->eval( cnode, cf ) )
        {
            for ( unsigned i = 0; i < getNumOfParameters(); ++i )
            {
                delete[] arguments[ i ]->eval_row( cnode, cf );
            }
            counter++;
            if ( counter >= 1000000000 )
            {
                break;
            }
        }
        return NULL;
    }


    virtual
    double
    eval( double arg1, double arg2 )
    {
        unsigned counter = 0;
        while ( condition->eval( arg1, arg2 ) )
        {
            for ( unsigned i = 0; i < getNumOfParameters(); ++i )
            {
                arguments[ i ]->eval( arg1, arg2 );
            }
            counter++;
            if ( counter >= 1000000000 )
            {
                break;
            }
        }
        return 0.;
    };

    virtual
    void
    print()
    {
        std::cout << "while (";
        condition->print();
        std::cout << ") " << std::endl <<  "{" << std::endl;
        for ( unsigned i = 0; i < getNumOfParameters(); ++i )
        {
            arguments[ i ]->print();
        }
        std::cout << "};"  << std::endl;
    };
};
};

#endif
