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
 * \file CubeStringValue.cpp
 * \brief  Defines the methods of the "StringValue".
 ************************************************/
#ifndef __STRING_VALUE_CPP
#define __STRING_VALUE_CPP

#include <sstream>
#include <cstring>
#include <string>
#include <iostream>
#include "CubeValues.h"
#include "CubeServices.h"


using namespace std;
using namespace cube;

std::vector<cube::StringValueConfig> cube::StringValue::parameters;

StringValue::StringValue()
{
    isSingleValue = false;

    size = 0;
    value.empty();
}

StringValue::StringValue( uint16_t s )
{
    isSingleValue = false;

    size = ( size_t )s;
    value.resize( size, ' ' );
}


StringValue::StringValue( int16_t s )
{
    if ( s < 0 )
    {
        throw RuntimeError( "[StringValue(int16_t s)] Size of string cannot be negative." );
    }
    isSingleValue = false;

    size = s;
    value.resize( size, ' ' );
}


StringValue::StringValue( uint32_t s )
{
    isSingleValue = false;

    size = ( size_t )s;
    value.resize( size, ' ' );
}


StringValue::StringValue( int32_t s )
{
    if ( s < 0 )
    {
        throw RuntimeError( "[StringValue(int32_t s)] Size of string cannot be negative." );
    }
    isSingleValue = false;

    size = s;
    value.resize( size, ' ' );
}
StringValue::StringValue( uint64_t s )
{
    isSingleValue = false;

    size = ( size_t )s;
    value.resize( size, ' ' );
}


StringValue::StringValue( int64_t s )
{
    if ( s < 0 )
    {
        throw RuntimeError( "[StringValue(int64_t s)] Size of string cannot be negative." );
    }
    isSingleValue = false;

    size = s;
    value.resize( size, ' ' );
}



StringValue::StringValue( double dv )
{
    isSingleValue = false;

    stringstream sstr;
    string       str;
    sstr << dv;
    sstr >> value;
    size = value.size();
}

StringValue::StringValue( char cv )
{
    isSingleValue = false;

//     cout << "char CONSTRUCTOR" << endl;
    stringstream sstr;
    string       str;
    sstr << cv;
    sstr >> value;
    size = value.size();
}
StringValue::StringValue( size_t n,
                          char*  cv )
{
    isSingleValue = false;
    char* tmp = new char[ n + 1 ];
    memset( tmp, 0, n + 1 );
    memcpy( tmp, cv, n );
    value = *tmp;
    delete[] tmp;
    size = value.size();
}

StringValue::StringValue( string sv )
{
    isSingleValue = false;

    value = sv;
    size  = value.size();
}

unsigned
StringValue::getSize()
{
    return size;
}


double
StringValue::getDouble()
{
    return 0.;
}

uint16_t
StringValue::getUnsignedShort()
{
    return ( uint16_t )getDouble();
}

int16_t
StringValue::getSignedShort()
{
    return ( int16_t )getDouble();
}

uint32_t
StringValue::getUnsignedInt()
{
    return ( uint32_t )getDouble();
}



int32_t
StringValue::getSignedInt()
{
    return ( int32_t )getDouble();
}

uint64_t
StringValue::getUnsignedLong()
{
    return ( uint64_t )getDouble();
}

int64_t
StringValue::getSignedLong()
{
    return ( int64_t )getDouble();
}
char
StringValue::getChar()
{
    return ' ';
}

string
StringValue::getString()
{
    return value;
}

/**
 * Creates the copy and sets the value to ' '.
 */
Value*
StringValue::clone()
{
    string v( ' ', value.size() );
    return new StringValue( v );
}

/**
 * Creates the copy.
 */
Value*
StringValue::copy()
{
    return new StringValue( value );
}


/**
 * Sets the value from stream and returns the position in stream right after the value.
 */
char*
StringValue::fromStream( char* cv )
{
    char* tmp = new char[ size + 1 ];
    memset( tmp, 0, size + 1 );
    memcpy( tmp, cv, size );
    value = *tmp;
    delete[] tmp;
    return cv + size;
}

double*
StringValue::fromStreamOfDoubles( double* cv )
{
    value = services::numeric2string( *cv );
    return cv + sizeof( double );
}




/**
 * Saves the value in the stream and returns the position in stream right after the value.
 */
char*
StringValue::toStream( char* cv )
{
    memcpy( cv, value.c_str(),  size );
    return cv + size;
}



// overloaded new operator
void*
StringValue::operator new( size_t size )
{
    return ( void* )string_preallocator.Get();
}
// delete operator overloaded
void
StringValue::operator delete( void* p )
{
    string_preallocator.Put( ( StringValue* )p );
}



StringValue
StringValue::operator+( const StringValue& ch )
{
    StringValue tmp = *this;
    return tmp;
}

StringValue
StringValue::operator-( const StringValue& ch )
{
    StringValue tmp = *this;
    return tmp;
}

void
StringValue::operator+=( Value* chval )
{
// #warning " Operation + is for string undefined and has no action"
    return;
}

void
StringValue::operator-=( Value* chval )
{
// #warning " Operation + is for string undefined  and has no action"
    return;
}


void
StringValue::operator*=( double dval )
{
// #warning " Operation + is for string undefined and has no action"
    return;
}

void
StringValue::operator/=( double dval )
{
// #warning " Operation + is for string undefined and has no action"
    return;
}



void
StringValue::operator=( double d )
{
    stringstream sstr;
    string       str;
    sstr << d;
    sstr >> value;
    value.reserve( size );
}

void
StringValue::operator=( char c )
{
    stringstream sstr;
    string       str;
    sstr << c;
    sstr >> value;
    value.reserve( size );
}

void
StringValue::operator=( uint16_t ui )
{
    stringstream sstr;
    string       str;
    sstr << ui;
    sstr >> value;
    value.reserve( size );
}


void
StringValue::operator=( uint32_t i )
{
    stringstream sstr;
    string       str;
    sstr << i;
    sstr >> value;
    value.reserve( size );
}

void
StringValue::operator=( uint64_t ul )
{
    stringstream sstr;
    string       str;
    sstr << ul;
    sstr >> value;
    value.reserve( size );
}

void
StringValue::operator=( int16_t si )
{
    stringstream sstr;
    string       str;
    sstr << si;
    sstr >> value;
    value.reserve( size );
}

void
StringValue::operator=( int32_t i )
{
    stringstream sstr;
    string       str;
    sstr << i;
    sstr >> value;
    value.reserve( size );
}

void
StringValue::operator=( int64_t sl )
{
    stringstream sstr;
    string       str;
    sstr << sl;
    sstr >> value;
    value.reserve( size );
}

void
StringValue::operator=( string v )
{
    value = v.substr( 0, ( v.size() < size ) ? v.size() : size );
    value.reserve( size );
}


void
StringValue::operator=( Value* v )
{
    value = v->getString();
    value.reserve( size );
}


StringValue
StringValue::operator=( StringValue v )
{
    if ( &v == this )
    {
        return *this;
    }
    value = v.getString();
    value.reserve( size );
    return *this;
}

void
StringValue::normalizeWithClusterCount( uint64_t N )
{
}

void
StringValue::init_new()
{
    StringValueConfig a;
    ( cube::StringValue::parameters ).push_back( a );
    index = ( cube::StringValue::parameters ).size() - 1;
};

void
StringValue::clone_new( Value* v )
{
    index = ( static_cast<StringValue*>( v ) )->index;
}


#endif
