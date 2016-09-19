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
 * \file CubeTauAtomicValue.cpp
 * \brief Defines the methods of the "TauAtomicValue".
 ************************************************/
#ifndef __TAU_ATOMIC_VALUE_CPP
#define __TAU_ATOMIC_VALUE_CPP

#include <sstream>
#include <cstring>
#include <string>
#include <cmath>
#include <iostream>
#include "CubeValues.h"
#include "CubeError.h"

using namespace std;
using namespace cube;

std::vector<cube::TauAtomicValueConfig> cube::TauAtomicValue::parameters;

TauAtomicValue::TauAtomicValue() : N( 0 ), MinValue(), MaxValue(), Sum( 0. ), Sum2( 0. )
{
    isSingleValue = false;
}

TauAtomicValue::TauAtomicValue( unsigned n,
                                double   min,
                                double   max,
                                double   sum,
                                double   sum2 ) : N( n ), MinValue( min ), MaxValue( max ), Sum( sum ), Sum2( sum2 )
{
    isSingleValue = false;
}


uint16_t
TauAtomicValue::getUnsignedShort()
{
    calcAvgVar();
    return ( uint16_t )Average;
}

int16_t
TauAtomicValue::getSignedShort()
{
    calcAvgVar();
    return ( int16_t )Average;
}

uint32_t
TauAtomicValue::getUnsignedInt()
{
    calcAvgVar();
    return ( uint32_t )Average;
}

int32_t
TauAtomicValue::getSignedInt()
{
    calcAvgVar();
    return ( int32_t )Average;
}


uint64_t
TauAtomicValue::getUnsignedLong()
{
    calcAvgVar();
    return ( uint64_t )Average;
}

int64_t
TauAtomicValue::getSignedLong()
{
    calcAvgVar();
    return ( int64_t )Average;
}


/**
 * As char will be returned just first char of the char representation
 */
char
TauAtomicValue::getChar()
{
    return ' ';
}



/**
 * Creates the string representation of the value.
 */
string
TauAtomicValue::getString()
{
// #warning " STRING representation of TAV Value should be developed"
    calcAvgVar();
    stringstream sstr;
    string       str;
    sstr << "(" <<  N.getString() << "," << MinValue.getString() << "," << MaxValue.getString() << "):";
    if ( N.getUnsignedInt() != 0. )
    {
        sstr << Average << "," << Variance << endl;
    }
    else
    {
        sstr << "-" << "," << "-" << endl;
    }
    sstr >> str;
    return str;
}


// overloaded new operator
void*
TauAtomicValue::operator new( size_t size )
{
    return ( void* )tau_preallocator.Get();
}
// delete operator overloaded
void
TauAtomicValue::operator delete( void* p )
{
    tau_preallocator.Put( ( TauAtomicValue* )p );
}


/**
 * Sets the value from stream and returns the position in stream right after the value.
 */
char*
TauAtomicValue::fromStream( char* cv )
{
    return Sum2.fromStream( Sum.fromStream( MaxValue.fromStream( MinValue.fromStream( N.fromStream( cv ) ) ) ) );
}

double*
TauAtomicValue::fromStreamOfDoubles( double* cv )
{
    return Sum2.fromStreamOfDoubles( Sum.fromStreamOfDoubles( MaxValue.fromStreamOfDoubles( MinValue.fromStreamOfDoubles( N.fromStreamOfDoubles( cv ) ) ) ) );
}



/**
 * Saves the value in the stream and returns the position in stream right after the value.
 */
char*
TauAtomicValue::toStream( char* cv )
{
    return Sum2.toStream( Sum.toStream( MaxValue.toStream( MinValue.toStream( N.toStream( cv ) ) ) ) );
}

/**
 * Transforms the endianness in the stream according its layout
 */
char*
TauAtomicValue::transformStream( char* cv, SingleValueTrafo* trafo )
{
    return Sum2.transformStream( Sum.transformStream( MaxValue.transformStream( MinValue.transformStream( N.transformStream( cv, trafo ), trafo ), trafo ), trafo ), trafo );
}




void
TauAtomicValue::operator=( double d )
{
    throw  RuntimeError( "Impossible to assign a  single double to TauAtomicValue" );
}


void
TauAtomicValue::operator=( Value* v )
{
    throw  RuntimeError( "Impossible to assign a singlegeneral Value to TauAtomicValue" );
}


// -------------------- PRIVATE METHODS- ------------------------

void
TauAtomicValue::calcAvgVar()
{
    uint32_t n  = N.getUnsignedInt();
    double   dn = N.getDouble();
    double   s  = Sum.getDouble();
    double   s2 = Sum2.getDouble();

    Average  =  ( n == 0 ) ? s / ( dn + 1e-256 )  : s / dn;
    Variance = ( n == 0 ) ? 0.             : sqrt( 1. / ( dn ) * ( s2 - 1. / dn * s * s ) );
}


void
TauAtomicValue::normalizeWithClusterCount( uint64_t norm )
{
    N.normalizeWithClusterCount( norm );
    Sum.normalizeWithClusterCount( norm );
    Sum2.normalizeWithClusterCount( norm );
}



void
TauAtomicValue::init_new()
{
    TauAtomicValueConfig a;
    ( cube::TauAtomicValue::parameters ).push_back( a );
    index = ( cube::TauAtomicValue::parameters ).size() - 1;
};

void
TauAtomicValue::clone_new( Value* v )
{
    index = ( static_cast<TauAtomicValue*>( v ) )->index;
}


#endif
