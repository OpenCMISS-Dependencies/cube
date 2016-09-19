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
 *	\file CubeVertex.cpp
 *	\brief Defines a general element of a tree. Many elements (metrics, machine, calltree) in CUBE have a treelike structure.
 *
 ********************************************/

#include "CubeError.h"
#include "CubeVertex.h"
#include "CubeServices.h"
#include <sstream>
#include <iomanip>

#define VERTEX_DEPTH_FOR_ID_DIGITS 6

using namespace std;
using namespace cube;




Vertex::Vertex( Vertex*  _parent,
                uint32_t id )
    : IdentObject( id ), parent( _parent )
{
//   children_counter.clear();
    own_hash = construct_own_hash();
    if ( parent != NULL )
    {
        parent->add_child( this );
    }
    total_number_of_children = ( uint64_t )-1;
}

int
Vertex::get_level() const
{
    if ( parent == NULL )
    {
        return 0;
    }
    else
    {
        return parent->get_level() + 1;
    }
}

Vertex*
Vertex::get_child( unsigned int i ) const
{
    if ( i >= childv.size() )
    {
        throw RuntimeError( "Vertex::get_child(i): out of range" );
    }
    return childv[ i ];
}

const std::vector<Vertex*>&
Vertex::get_children() const
{
    return childv;
}





uint64_t
Vertex::total_num_children()
{
    if ( total_number_of_children != ( uint64_t )-1 )
    {
        return total_number_of_children + 1;
    }

    total_number_of_children = 0;
    for ( uint64_t i = 0; i < num_children(); ++i )
    {
        total_number_of_children += get_child( i )->total_num_children();
    }
    return total_number_of_children + 1;
}






void
Vertex::add_child( Vertex* v )
{
/*    std::string _hash= v->get_own_hash();
    cout << " _:_::: " << own_hash << " " << _hash << "_:_" << endl;
    unsigned n =0;
    std::map<std::string, unsigned int>::iterator i = children_counter.find(_hash);
    if(i != children_counter.end())
    {
        n = i->second;
        cout << " --- Key found " << (_hash)  << " : " << n << endl;

    }
    n++;
    children_counter[_hash] = n;*/
    childv.push_back( v );
}

void
Vertex::set_parent( Vertex* vertex )
{
    parent = vertex;
    if ( vertex == NULL )
    {
        return;
    }
    parent->add_child( this );
}

void
Vertex::def_attr( const string& key, const string& value )
{
    attrs[ key ] = value;
}

string
Vertex::get_attr( const std::string& key ) const
{
    map<std::string, std::string>::const_iterator it = attrs.find( key );
    if ( it != attrs.end() )
    {
        return it->second;
    }

    return string();
}

const
std::map<std::string, std::string>&
Vertex::get_attrs() const
{
    return attrs;
}


void
Vertex::writeAttributes( ostream& out, const string& _indent, bool cube3export ) const
{
    if ( cube3export ) // we skip attributes if we export into cube3 format
    {
        return;
    }
    // metadata info
    map<string, string>::const_iterator ai;
    for ( ai = attrs.begin(); ai != attrs.end(); ++ai )
    {
        out << _indent << "<attr " << "key=" << "\"" << services::escapeToXML( ai->first ) << "\" "
            << "value=" << "\"" << services::escapeToXML( ai->second ) << "\"/>" << '\n';
    }
}


std::string
Vertex::construct_hash_id()
{
//     return "";
    int      level      = get_level();
    unsigned appearance = 0;
    if ( parent != NULL )
    {
        appearance = parent->num_children();
    }

    std::stringstream sstr;
    std::string       str1;
    std::string       str2;
    sstr <<  right << setw( VERTEX_DEPTH_FOR_ID_DIGITS ) << level;
    sstr >> str1;
    str1 = str1 + own_hash;
    sstr << setw( -1 ) << appearance;
    sstr >> str2;
    return str1 + str2;
}



bool
Vertex::weakEqual( Vertex* v )
{
    if ( get_hash_id().compare( v->get_hash_id() ) != 0 )
    {
        return false;
    }
    return true;
}
