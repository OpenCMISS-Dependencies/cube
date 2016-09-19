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
 * \file CubeLocationGroup.cpp
 * \brief Defines a method to save a xml-representation of a location group in a .cubex file.
 ********************************************/

#include <iostream>
#include <sstream>

#include "CubeSystemTreeNode.h"
#include "CubeLocationGroup.h"
#include "CubeLocation.h"
#include "CubeServices.h"
#include "CubeError.h"

using namespace std;
using namespace cube;
using namespace services;




LocationGroup::LocationGroup( const std::string& name,
                              SystemTreeNode*    stn,
                              int                rank,
                              LocationGroupType  type,
                              uint32_t           id,
                              uint32_t           sysid )
    : Sysres( name, id, sysid ), rank( rank ), type( type )
{
    own_hash = construct_own_hash();
    kind     = CUBE_LOCATION_GROUP;
    parent   = stn;
    if ( stn != NULL )
    {
        stn->add_location_group( this );
    }
    else
    {
        throw RuntimeError( "Location Group cannot have NULL as a parent value in the system tree node." );
    }
}
// /< Thread does have a rank.


std::string
LocationGroup::get_type_as_string() const
{
    switch ( get_type() )
    {
        case cube::CUBE_LOCATION_GROUP_TYPE_PROCESS:
            return "process";
        case cube::CUBE_LOCATION_GROUP_TYPE_METRICS:
            return "metrics";
        default:
            return "not supported";
    }
}


LocationGroupType
LocationGroup::getLocationGroupType( std::string type )
{
    if ( type.compare( "process" ) == 0 )
    {
        return cube::CUBE_LOCATION_GROUP_TYPE_PROCESS;
    }
    if ( type.compare( "metrics" ) == 0 )
    {
        return cube::CUBE_LOCATION_GROUP_TYPE_METRICS;
    }
    throw AnchorSyntaxError( "Location group type " + type + " is not supported!" );
}


void
LocationGroup::writeXML( ostream& out,
                         bool     cube3export ) const
{
    if ( cube3export )
    {
        out << indent() << "    <process Id=\"" << get_id() << "\">" <<  '\n';
    }
    else
    {
        out << indent() << "    <locationgroup Id=\"" << get_id() << "\">" <<  '\n';
    }
    out << indent() << "      <name>" << escapeToXML( get_name() ) << "</name>" <<  '\n';
    out << indent() << "      <rank>" << get_rank() << "</rank>" <<  '\n';
    if ( !cube3export )
    {
        out << indent() << "    <type>" << get_type_as_string() << "</type>" <<  '\n';
    }
    writeAttributes( out,  indent() + "    ", cube3export  );
    for ( unsigned int i = 0; i < num_children(); ++i )
    {
        const Location* thrd = get_child( i );
        thrd->writeXML( out, cube3export  );
    }
    if ( cube3export )
    {
        out << indent() << "    </process>" <<  '\n';
    }
    else
    {
        out << indent() << "    </locationgroup>" <<  '\n';
    }
}

std::string
LocationGroup::construct_own_hash()
{
    std::stringstream sstr;
    std::string       str;
    sstr << rank;
    sstr >> str;
    std::string result;
    result = string( "thread." ) + name + string( ":" ) + str;
    return result;
}
