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
 * \file CubeHistogramValue.cpp
 * \brief   Defines the methods of the "HistogramValue".
 ************************************************/

#ifndef __HISTOGRAM_VALUE_CPP
#define __HISTOGRAM_VALUE_CPP

#include <sstream>
#include <cstring>
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "CubeValues.h"

using namespace std;
using namespace cube;

std::vector<cube::HistogramValueConfig> cube::HistogramValue::parameters;


HistogramValue::HistogramValue() : N( 0 )
{
    min           = 0;
    max           = 0;
    isSingleValue = false;
    values        = NULL;
    if ( histogram_preallocator.truecreation )
    {
        in     = new std::vector<wplet>();
        merger = new std::vector<plet>();
        bins   = new std::vector<double>();
    }
    else
    {
        in->clear();
        merger->clear();
        bins->clear();
    }
    N = 0;
}


HistogramValue::HistogramValue( uint64_t n ) : N( n )
{
    min           = 0;
    max           = 0;
    isSingleValue = false;
    values        = NULL;
    if ( histogram_preallocator.truecreation )
    {
        in     = new std::vector<wplet>();
        merger = new std::vector<plet>();
        bins   = new std::vector<double>();
    }
    else
    {
        in->clear();
        merger->clear();
        bins->clear();
    }
    if ( n == 0 )
    {
        return;
    }
    values = new double[ n ];
    for ( unsigned i = 0; i < n; ++i )
    {
        values[ i ] = 0.;
    }
    prepare();
}

HistogramValue::HistogramValue( double   _min,
                                double   _max,
                                uint64_t n ) : N( n )
{
    min           = _min;
    max           = _max;
    isSingleValue = false;
    values        = NULL;
    if ( histogram_preallocator.truecreation )
    {
        in     = new std::vector<wplet>();
        merger = new std::vector<plet>();
        bins   = new std::vector<double>();
    }
    else
    {
        in->clear();
        merger->clear();
        bins->clear();
    }
    if ( n == 0 )
    {
        return;
    }
    values = new double[ n ];
    for ( unsigned i = 0; i < n; ++i )
    {
        values[ i ] = 0.;
    }
    prepare();
}
HistogramValue::HistogramValue( double   _min,
                                double   _max,
                                uint64_t n,
                                double*  vals ) : N( n )
{
    min           = _min;
    max           = _max;
    isSingleValue = false;
    values        = NULL;
    if ( histogram_preallocator.truecreation )
    {
        in     = new std::vector<wplet>();
        merger = new std::vector<plet>();
        bins   = new std::vector<double>();
    }
    else
    {
        in->clear();
        merger->clear();
        bins->clear();
    }
    if ( n == 0 )
    {
        return;
    }

    values = new double[ n ];
    memcpy( values, vals, N * sizeof( double ) );


    prepare();
}

HistogramValue::HistogramValue( uint64_t n,
                                double*  vals ) : N( n )
{
    min           = vals[ 0 ];
    max           = vals[ 1 ];
    isSingleValue = false;
    values        = NULL;
    if ( histogram_preallocator.truecreation )
    {
        in     = new std::vector<wplet>();
        merger = new std::vector<plet>();
        bins   = new std::vector<double>();
    }
    else
    {
        in->clear();
        merger->clear();
        bins->clear();
    }
    if ( n == 0 )
    {
        return;
    }

    values = new double[ n ];
    memcpy( values, vals + 2, N * sizeof( double ) );

    prepare();
}



HistogramValue::~HistogramValue()
{
    delete[] values;
    values = NULL;
    if ( histogram_preallocator.truecreation )
    {
        delete in;
        in = NULL;
        delete merger;
        merger = NULL;
        delete bins;
        bins = NULL;
    }
    else
    {
        in->clear();
        merger->clear();
        bins->clear();
    }
};

/*************************************/


/***********************************
 * setNumTerms() and processArguments() are an example of how to pass parameters from metric
 * creation to the datatype. They are not used since the design has problems:
 * - only useful for parameters to all instances of the datatype
 * - consequently, parameter is not metric-specific
 */
void
HistogramValue::setNumTerms( size_t num )
{
    if ( num > 0 )
    {
        // TODO: debug output
//         cout << "Changed number of terms to: " << num << endl;
        delete[] values;
        N      = num;
        values = new double[ num ];
        for ( unsigned i = 0; i < num; i++ )
        {
            values[ i ] = 0.;
        }
    }
    else
    {
        throw RuntimeError( "HistorgramValue: num_terms needs to be positive" );
    }
}

/* Take generic argument list and parse it.
 * We only take the first argument, which is the actual number of terms used in the Metric
 */
void
HistogramValue::processArguments( std::vector<std::string> args )
{
    if ( args.size() == 1 )
    {
        stringstream ss;
        int          num = 0;
        ss << args[ 0 ];
        ss >> num;
        setNumTerms( num );
    }
    else
    {
        throw RuntimeError( "HistorgramValue: too many arguments for this datatype" );
    }
}




Value*
HistogramValue::clone()
{
    return new HistogramValue( min, max, N );
}

Value*
HistogramValue::copy()
{
    double* tmp_d = new double[ N + 2 /* min and max*/ ];
    toStream( ( char* )tmp_d );
    HistogramValue* _return = new HistogramValue( N, tmp_d /* min and max are not included*/ );
    delete[] tmp_d;
    return _return;
}


// overloaded new operato
void*
HistogramValue::operator new( size_t size )
{
    return ( void* )histogram_preallocator.Get();
}
// delete operator overloaded
void
HistogramValue::operator delete( void* p )
{
    histogram_preallocator.Put( ( HistogramValue* )p );
}



void
HistogramValue::init_new()
{
    HistogramValueConfig a;
    ( cube::HistogramValue::parameters ).push_back( a );
    index = ( cube::HistogramValue::parameters ).size() - 1;
};

void
HistogramValue::clone_new( Value* v )
{
    index = ( static_cast<HistogramValue*>( v ) )->index;
}



char*
HistogramValue::fromStream( char* cv )
{
    double* vals = ( double* )cv;
    min = vals[ 0 ];
    max = vals[ 1 ];
    char* stream = cv + 2 * sizeof( double );
    memcpy( values, stream, N * sizeof( double ) );
    prepare();
    return stream + N * sizeof( double );
}

double*
HistogramValue::fromStreamOfDoubles( double* cv )
{
    return ( double* )fromStream( ( char* )cv );
}

char*
HistogramValue::toStream( char* cv )
{
    double* vals = ( double* )cv;
    vals[ 0 ] = min;
    vals[ 1 ] = max;

    char* stream = cv + 2 * sizeof( double );
    memcpy( stream, values, N * sizeof( double ) );
    return stream + N * sizeof( double );
    ;
}

char*
HistogramValue::transformStream( char* cv, SingleValueTrafo* trafo )
{
    char*        stream = cv;
    DoubleValue* _v     = new  DoubleValue();


    stream = _v->transformStream( stream, trafo );
    if ( stream == cv )
    {
        delete _v;
        prepare();
        return cv;
    }
    stream = _v->transformStream( stream, trafo );

    for ( unsigned i = 0; i < N; ++i )
    {
        stream = _v->transformStream( stream, trafo );
    }
    delete _v;
    prepare();
    return stream;
}


void
HistogramValue::operator+=( Value* chval )
{
    if ( chval == NULL )
    {
        return;
    }

    std::vector<wplet> _in1 = get_points();
    std::vector<wplet> _in2 = ( ( HistogramValue* )chval )->get_points();

    min = std::min( min,   ( ( HistogramValue* )( chval ) )->min );
    max = std::max( max,   ( ( HistogramValue* )( chval ) )->max );




    prepare();


    std::vector<wplet> _in = get_points();



    merger->clear();

    for ( size_t i = 0; i < _in1.size(); i++ )
    {
        plet _plet1 = { 1, _in1[ i ].point, i };
        merger->push_back( _plet1 );
        plet _plet2 = { 2, _in2[ i ].point, i };
        merger->push_back( _plet2 );
        plet _plet3 = { 0, _in[ i ].point, i };
        merger->push_back( _plet3 );
    }

    sort( merger->begin(), merger->end(), pair_compare );
    calculate_distance( *merger, _in1, _in2, _in );







    std::vector<double> bins_copy = *bins;
    bins->assign( bins->size(), 0. );


    std::vector<double> rhs_bins = *( ( ( HistogramValue* )( chval ) )->bins );



    for ( size_t i = 0; i < _in1.size() - 1; i++ )
    {
        if ( _in1[ i ].curr_index == _in1[ i + 1 ].curr_index ) // full bin inside
        {
            _in1[ i ].curr_weight = 1.;
            _in1[ i ].next_weight = 0.;
        }
        else
        {
            _in1[ i ].curr_weight = ( _in[ _in1[ i ].next_index ].point - _in1[ i ].point ) / ( _in1[ i + 1 ].point - _in1[ i ].point );
            _in1[ i ].next_weight = 1. - _in1[ i ].curr_weight;
        }
        if ( _in2[ i ].curr_index == _in2[ i + 1 ].curr_index ) // full bin inside
        {
            _in2[ i ].curr_weight = 1.;
            _in2[ i ].next_weight = 0.;
        }
        else
        {
            _in2[ i ].curr_weight = ( _in[ _in2[ i ].next_index ].point - _in2[ i ].point ) / ( _in2[ i + 1 ].point - _in2[ i ].point );
            _in2[ i ].next_weight = 1. - _in2[ i ].curr_weight;
        }

        ( *bins )[ _in1[ i ].curr_index ] +=  ( bins_copy[ i ] * _in1[ i ].curr_weight );
        ( *bins )[ _in1[ i ].next_index ] +=  ( bins_copy[ i ] * _in1[ i ].next_weight );
        ( *bins )[ _in2[ i ].curr_index ] +=  ( rhs_bins[ i ] * _in2[ i ].curr_weight );
        ( *bins )[ _in2[ i ].next_index ] +=  ( rhs_bins[ i ] * _in2[ i ].next_weight );
    }




    for ( unsigned i = 0; i < N; ++i )
    {
        values[ i ] = ( *bins )[ i ];
    }
}

void
HistogramValue::operator-=( Value* chval )
{
    return;
}



uint16_t
HistogramValue::getUnsignedShort()
{
    return ( uint16_t )getDouble();
}
int16_t
HistogramValue::getSignedShort()
{
    return ( int16_t )getDouble();
}
uint32_t
HistogramValue::getUnsignedInt()
{
    return ( uint32_t )getDouble();
}
int32_t
HistogramValue::getSignedInt()
{
    return ( int32_t )getDouble();
}

uint64_t
HistogramValue::getUnsignedLong()
{
    return ( uint64_t )getDouble();
}
int64_t
HistogramValue::getSignedLong()
{
    return ( int64_t )getDouble();
}



char
HistogramValue::getChar()
{
    return ' ';
}





string
HistogramValue::getString()
{
    MinDoubleValue* _min = new MinDoubleValue( min );
    MaxDoubleValue* _max = new MaxDoubleValue( max );
    DoubleValue*    _d   = new DoubleValue();
    string          tmp  = _min->getString() + ":(";
    for ( unsigned i = 0; i < N; ++i )
    {
        *_d  = values[ i ];
        tmp += _d->getString();
        if ( i <  N - 1 )
        {
            tmp += ", ";
        }
    }
    tmp += "):" + _max->getString();

    delete _d;
    delete _min;
    delete _max;


    return tmp;
}

void
HistogramValue::calculate_distance( std::vector< plet >& _merger, std::vector< wplet >& _in1, std::vector< wplet >& _in2, std::vector< wplet >& _in )
{
    size_t pivot_index = 0;
    for ( size_t i = 1; i < _merger.size(); i++ )
    {
        if ( _merger[ i ].array == 0 )
        {
            pivot_index = _merger[ i ].index;
        }
        else if ( _merger[ i ].array == 1 )
        {
            _in1[ _merger[ i ].index ].curr_index = pivot_index;
            _in1[ _merger[ i ].index ].next_index = pivot_index + 1;
        }
        else if ( _merger[ i ].array == 2 )
        {
            _in2[ _merger[ i ].index ].curr_index = pivot_index;
            _in2[ _merger[ i ].index ].next_index = pivot_index + 1;
        }
    }
}



double
HistogramValue::sum()
{
    double s = 0.;
    for ( size_t i = 0; i < N; i++ )
    {
        s += values[ i ];
    }
    return s;
}




void
HistogramValue::prepare()
{
    if ( N == 0 )
    {
        return;
    }
    double   _max = max;
    double   _min = min;
    uint64_t _n   =  N;

    in->clear();
    merger->clear();

    double dx = ( _max - _min ) / _n;
    in->resize( _n + 1 );
    bins->clear();
    for ( size_t i = 0; i <= _n; i++ )
    {
        if ( i != _n )
        {
            bins->push_back( values[ i ] );
        }
        wplet _p = { 0., -1, -1, -1, -1 };
        _p.point = _min + i * dx;

        ( *in )[ i ] = ( _p );
    }
}


bool
cube::pair_compare( plet a, plet b )
{
    if ( a.point < b.point )
    {
        return true;
    }
    if ( a.point > b.point )
    {
        return false;
    }
    if ( a.point == b.point )
    {
        return a.array < b.array;
    }
    return false;
}



void
HistogramValue::operator*=( double dval )
{
    for ( unsigned i = 0; i < N; ++i )
    {
        ( values[ i ]  ) *= dval;
    }
}

void
HistogramValue::operator/=( double dval )
{
    if ( dval == 0. )
    {
        cerr << "ERROR: DEVISION BY ZERO!" << endl;
    }
    else
    {
        for ( unsigned i = 0; i < N; ++i )
        {
            ( values[ i ] ) /= dval;
        }
    }
}



void
HistogramValue::operator=( double d )
{
    throw RuntimeError( "Impossible to assign a  single double value to HistogramValue" );
}


void
HistogramValue::operator=( Value* val )
{
    throw RuntimeError( "Impossible to assign a  single general value to HistogramValue" );
}



void
HistogramValue::normalizeWithClusterCount( uint64_t _N )
{
    for ( uint64_t i = 0; i < N; i++ )
    {
        values[ i ] =  values[ i ] / ( double )_N;
    }
}

bool
HistogramValue::isZero()
{
    for ( unsigned i = 0; i < N; ++i )
    {
        if ( values[ i ] != 0. )
        {
            return false;
        }
    }
    return true;
}


#endif
