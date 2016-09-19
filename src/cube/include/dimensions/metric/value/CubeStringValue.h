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
 * \file CubeStringValue.h
 * \brief  Provides the string value with fixed length.
 ************************************************/
#ifndef __STRING_VALUE_H
#define __STRING_VALUE_H

#include <string>
#include <unistd.h>
#include "CubeValue.h"

#include <iostream>


namespace cube
{
class StringValue;
extern Preallocator<StringValue> string_preallocator;


typedef   struct
{
    int pivot;
}  __attribute__ ( ( __packed__ ) ) StringValueConfig;

/**
 * Value works with a string.
 */
class StringValue : public Value
{
private:
    static
    std::vector<StringValueConfig> parameters;
    size_t                         index;

protected:
    std::string value;                 // / Value itself.
    size_t      size;                  // / Lenght of the value.
public:
    StringValue();
    StringValue( uint16_t );
    StringValue( int16_t );
    StringValue( uint32_t );
    StringValue( int32_t );
    StringValue( uint64_t );
    StringValue( int64_t );
    StringValue( double );
    StringValue( char );
    StringValue( size_t,
                 char* );
    StringValue( std::string );

    virtual
    ~StringValue()
    {
    };
    virtual unsigned
    getSize();
    virtual double
    getDouble();
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

    virtual char*
    fromStream( char* );
    virtual double*
    fromStreamOfDoubles( double* stream );
    virtual char*
    toStream( char* );


    virtual Value*
    clone();
    virtual Value*
    copy();


    virtual StringValue
    operator+( const StringValue& );
    virtual StringValue
    operator-( const StringValue& );

    void*
    operator new( size_t size );
    void
    operator delete( void* p );



    virtual void
    Free()
    {
        delete ( StringValue* )this;
    }

    virtual void
    operator+=( Value* );
    virtual void
    operator-=( Value* );
    virtual void
    operator*=( double );                            // needed by algebra tools
    virtual void
    operator/=( double );                            // needed by algebra tools


    virtual void
    operator=( double );
    virtual void
    operator=( char );
    virtual void
    operator=( uint16_t );
    virtual void
    operator=( uint32_t );
    virtual void
    operator=( uint64_t );
    virtual void
    operator=( int16_t );
    virtual void
    operator=( int32_t );
    virtual void
    operator=( int64_t );
    virtual void
    operator=( Value* );
    virtual void
    operator=( std::string );
    virtual StringValue
    operator=( StringValue );                              // / Assignemnt operator.

    virtual bool
    isZero()
    {
        return size == 0;
    };

    virtual DataType
    myDataType()
    {
        return CUBE_DATA_TYPE_NONE;
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
