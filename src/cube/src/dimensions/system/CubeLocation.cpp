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
 * \file CubeLocation.cpp
 * \brief Defines a xml-output of a location description in a .cube file.
 *
 ********************************************/

#include <iostream>
#include <sstream>

#include "CubeLocation.h"
#include "CubeServices.h"
#include "CubeError.h"

using namespace std;
using namespace cube;
using namespace services;


Location::Location( const std::string& name,
                    int                rank,
                    LocationGroup*     proc,
                    LocationType       type,
                    uint32_t           id,
                    uint32_t           sysid )
    : Sysres( ( Sysres* )proc, name, id, sysid ), rank( rank ), type( type )
{
    own_hash = construct_own_hash();
    kind     = CUBE_LOCATION;
}








std::string
Location::get_type_as_string() const
{
    switch ( get_type() )
    {
        case cube::CUBE_LOCATION_TYPE_CPU_THREAD:
            return "thread";
        case cube::CUBE_LOCATION_TYPE_GPU:
            return "gpu";
        case cube::CUBE_LOCATION_TYPE_METRIC:
            return "metric";
        default:
            return "not supported";
    }
}


LocationType
Location::getLocationType( std::string type )
{
    if ( type.compare( "thread" ) == 0 )
    {
        return cube::CUBE_LOCATION_TYPE_CPU_THREAD;
    }
    if ( type.compare( "gpu" ) == 0 )
    {
        return cube::CUBE_LOCATION_TYPE_GPU;
    }
    if ( type.compare( "metric" ) == 0 )
    {
        return cube::CUBE_LOCATION_TYPE_METRIC;
    }
    throw AnchorSyntaxError( "Location type " + type + " is not supported!" );
}


void
Location::writeXML( ostream& out,
                    bool     cube3export  ) const
{
    if ( cube3export )
    {
        out << indent() << "    <thread Id=\"" << get_id() << "\">" <<  '\n';
    }
    else
    {
        out << indent() << "    <location Id=\"" << get_id() << "\">" <<  '\n';
    }
    out << indent() << "      <name>" << escapeToXML( get_name() ) << "</name>\n";
    out << indent() << "      <rank>" << get_rank() << "</rank>\n";
    if ( !cube3export )
    {
        out << indent() << "    <type>" << get_type_as_string() << "</type>" <<  '\n';
    }
    writeAttributes( out,  indent() + "    ", cube3export  );
    if ( cube3export )
    {
        out << indent() << "    </thread>\n";
    }
    else
    {
        out << indent() << "    </location>\n";
    }
}


std::string
Location::construct_own_hash()
{
    std::stringstream sstr;
    std::string       str;
    sstr << get_rank();
    sstr >> str;
    std::string result;
    result = string( "thread." ) + name + string( ":" ) + str;
    return result;
}
