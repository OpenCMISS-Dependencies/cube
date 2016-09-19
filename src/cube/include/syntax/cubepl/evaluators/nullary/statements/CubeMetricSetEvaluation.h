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


#ifndef __METRIC_SET_EVALUATION_H
#define __METRIC_SET_EVALUATION_H 0

#include "CubeNullaryEvaluation.h"

namespace cube
{
class MetricSetEvaluation : public NullaryEvaluation
{
protected:
    cube::Metric*      metric;
    GeneralEvaluation* property;
    GeneralEvaluation* value;



public:
    MetricSetEvaluation( cube::Metric*      _met,
                         GeneralEvaluation* _property,
                         GeneralEvaluation* _value ) : NullaryEvaluation(), metric( _met ), property( _property ), value( _value )
    {
    };
    ~MetricSetEvaluation();



    inline
    virtual
    void
    setRowSize( size_t size )
    {
        row_size = size;
        property->setRowSize( size );
        value->setRowSize( size );
    }

    inline
    virtual
    void
    set_verbose_execution( bool _v )
    {
	GeneralEvaluation::set_verbose_execution( _v );
	property->set_verbose_execution( _v );
	value->set_verbose_execution( _v );
    }
    
    
    virtual
    double
    eval();

    virtual
    double
    eval( Cnode* cnode, CalculationFlavour cf, Sysres* sr, CalculationFlavour tf  )
    {
        eval();
        return 0.;
    };


    virtual
    double
    eval( Cnode* cnode, CalculationFlavour cf )
    {
        eval();
        return 0.;
    };


    virtual
    double
    eval( double arg1, double arg2 )
    {
        eval();
        return 0.;
    };

    virtual
    double*
    eval_row( Cnode*             cnode,
              CalculationFlavour cf )
    {
        eval();
        return NULL;
    };


    virtual
    void
    print()
    {
        std::cout << "cube::metric::set::" << metric->get_uniq_name() << "(";
        property->print();
        std::cout << ",";
        value->print();
        std::cout << ");";
    };
};
};

#endif
