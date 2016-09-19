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


/**
 *
 * \file CubeNDoublesValue.h
 * \brief  Provides the experimental complex value, consisting of N "DoubleValue"'s. Needed for future to support atomic events.
 ************************************************/
#ifndef __NDOUBLES_VALUE_H
#define __NDOUBLES_VALUE_H

#include <istream>
#include <stdlib.h>
#include "CubeError.h"
#include "CubeValues.h"

#include <iostream>

namespace cube
{
class NDoublesValue;
extern Preallocator<NDoublesValue> ndoubles_preallocator;

typedef   struct
{
    int pivot;
}  __attribute__ ( ( __packed__ ) ) NDoublesValueConfig;



class NDoublesValue : public Value
{
private:
    static
    std::vector<NDoublesValueConfig> parameters;
    size_t                           index;

public:
    uint64_t N;                       // / Number of values.
    double*  values;                  // / "heart" of the value.


public:
    NDoublesValue();
    NDoublesValue( uint64_t );
//                                 NDoublesValue(double);
    NDoublesValue( uint64_t,
                   double* );

    virtual
    ~NDoublesValue()
    {
        delete[] values;
        values = NULL;
        N      = 0;
    }


    void
    processArguments( std::vector<std::string> args ); // / parse arguments from metric
    void
    setNumTerms( size_t num );                         // / set number of terms stored according to metric



    inline
    virtual unsigned
    getSize()
    {
        return ( N == 0 ) ? 0 : N * ( sizeof( double ) );
    }

    inline
    virtual double
    getDouble()
    {
        double sum = 0;
        for ( unsigned i = 0; i < N; i++ )
        {
            sum += values[ i ];
        }
        return sum;
    }
    virtual uint16_t
    getUnsignedShort();
    virtual int16_t
    getSignedShort();
    virtual uint32_t
    getUnsignedInt();
    virtual int32_t
    getSignedInt();
    virtual uint64_t
    getUnsignedLong();
    virtual int64_t
    getSignedLong();
    virtual char
    getChar();
    virtual std::string
    getString();
    virtual double
    getValue( unsigned idx );

    virtual char*
    fromStream( char* );
    virtual double*
    fromStreamOfDoubles( double* stream );

    virtual char*
    toStream( char* );
    virtual char*
    transformStream( char*,
                     SingleValueTrafo* );

    inline
    virtual Value*
    clone()
    {
        return new NDoublesValue( N );
    }
    virtual Value*
    copy();

    const double*
    get_values( void )
    {
        return values;
    }

    uint64_t
    get_number_of_values( void )
    {
        return N;
    }

    virtual void
    operator+=( Value* );
    virtual void
    operator-=( Value* );
    virtual void
    operator*=( double );                            // needed by algebra tools
    virtual void
    operator/=( double );                            // needed by algebra tools

    void*
    operator new( size_t size );
    void
    operator delete( void* p );


    virtual void
    Free()
    {
        delete ( NDoublesValue* )this;
    }

    virtual void
    operator=( double );

    virtual void
    operator=( Value* );


    virtual bool
    isZero();                        // faked check. Should be fixed if NDoubles get to use in cube.

    virtual DataType
    myDataType()
    {
        return CUBE_DATA_TYPE_NDOUBLES;
    };                                             // not supported yet
    virtual void
    normalizeWithClusterCount( uint64_t );

    virtual
    void
    init_new();

    virtual
    void
    clone_new( Value* );
};
}
#endif
