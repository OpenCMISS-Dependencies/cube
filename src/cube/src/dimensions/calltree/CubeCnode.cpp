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
 * \file CubeCnode.cpp
 * \brief Defines the methods to deal with the elements of a call tree.
 */

#include <iostream>
#include <utility>
#include <vector>
#include <sstream>

#include "CubeCnode.h"
#include "CubeRegion.h"
#include "CubeServices.h"

using namespace std;
using namespace cube;
using namespace services;
/**

 * Created an element of a call tree.
 *
 * As a friend class of "Region" is might call "add_excl_cnode".
 */

Cnode::Cnode( Region*            _callee,
              const std::string& _mod,
              int                _line,
              Cnode*             _parent,
              uint32_t           id ) :
    Vertex( _parent, id ), callee( _callee ), mod( _mod ), line( _line ), is_hidden( false )
{
    // Vertex::set_parent(parent);
    common_remapping_cnode = NULL;
    no_remapping           = true;
    own_hash               = construct_own_hash();
    callee->add_excl_cnode( this );
    collected_whole_tree = false;

    Cnode* cur_parent   = _parent;
    bool   is_recursive = false;
    while ( cur_parent != NULL )
    {
        if ( cur_parent->get_callee() == _callee )
        {
            is_recursive = true;
            break;
        }

        cur_parent = cur_parent->get_parent();
    }

    if ( is_recursive == false )
    {
        _callee->add_incl_cnode( this );
    }
}


Cnode::~Cnode()
{
    // delete remapping_cnode;
    // children -> not yet.
}


Region*
Cnode::get_caller() const
{
    if ( get_parent() == NULL )
    {
        return NULL;
    }
    return get_parent()->get_callee();
}



Cnode*
Cnode::get_remapping_cnode( int64_t process_rank )
{
    if ( no_remapping && common_remapping_cnode == NULL )
    {
        return this;
    }
    if ( common_remapping_cnode != NULL )
    {
        return common_remapping_cnode;
    }
    std::map<int64_t, Cnode*>::iterator iter = remapping_cnodes.find( process_rank );
    if ( iter == remapping_cnodes.end() )
    {
        return this;
    }
    return ( *iter ).second;
}

int64_t
Cnode::get_cluster_normalization( int64_t process_rank )
{
    std::map<int64_t, int64_t>::iterator iter = cluster_normalization.find( process_rank );
    if ( iter == cluster_normalization.end() )
    {
        return 0;
    }
    return ( *iter ).second;
}

void
Cnode::set_remapping_cnode( Cnode* r_cnode )
{
    common_remapping_cnode = r_cnode;
}


void
Cnode::set_remapping_cnode( int64_t process_rank, Cnode* r_cnode,
                            int64_t _cluster_normalization )
{
    no_remapping                          = false;
    remapping_cnodes[ process_rank ]      = r_cnode;
    cluster_normalization[ process_rank ] = _cluster_normalization;
}



void
Cnode::add_num_parameter( string key, double param )
{
    pair<string, double> _parameter;
    _parameter.first  = key;
    _parameter.second = param;
    num_parameters.push_back( _parameter );
}



void
Cnode::add_str_parameter( string key, string param )
{
    pair<string, string> _parameter;
    _parameter.first  = key;
    _parameter.second = param;
    str_parameters.push_back( _parameter );
}




void
Cnode::set_as_leaf()
{
    for ( unsigned i = 0; i < num_children(); ++i )
    {
        Cnode* child = get_child( i );
        child->hide();
    }
}



void
Cnode::hide()
{
    is_hidden = true;

    // hide also all children
    for ( unsigned i = 0; i < num_children(); ++i )
    {
        Cnode* child = get_child( i );
        child->hide();
    }
}




/**
 * \details Writes with deep search algorithm.
 */
void
Cnode::writeXML( ostream& out, bool cube3export ) const
{
    out << indent() << "    <cnode id=\"" << get_id() << "\" ";
    if ( get_line() != -1 )
    {
        out << "line=\"" << get_line() << "\" ";
    }
    if ( get_mod() != "" )
    {
        out << "mod=\"" << escapeToXML( get_mod() ) << "\" ";
    }
    out << "calleeId=\"" << get_callee()->get_id() << "\">" <<  '\n';

    for ( unsigned i = 0; i < num_parameters.size(); ++i )
    {
        out << indent() << "        <parameter partype=\"numeric\" parkey=\"" << escapeToXML( num_parameters[ i ].first ) << "\" parvalue=\"" << num_parameters[ i ].second  << "\"/>" <<  '\n';
    }
    for ( unsigned i = 0; i < str_parameters.size(); ++i )
    {
        out << indent() << "        <parameter partype=\"string\" parkey=\"" << escapeToXML( str_parameters[ i ].first.c_str() )  << "\" parvalue=\""  << escapeToXML( str_parameters[ i ].second.c_str() )   << "\"/>" <<  '\n';
    }


    writeAttributes( out,  indent() + "        ", cube3export );

    for ( unsigned int i = 0; i < num_children(); ++i )
    {
        Cnode* child = get_child( i );
        if ( cube3export )
        {
            if ( child->isVisible() ) // only visible children get saved.  (coz data of hidden children is stored in excl value of parent)
            {
                child->writeXML( out );
            }
        }
        else
        {
            child->writeXML( out ); // in the case of cube4 all children get stored.
        }
    }

    out << indent() << "    </cnode>\n";
}


bool
Cnode::weakEqual( Cnode* _c )
{
    if ( get_mod() != _c->get_mod() ||
         !( *( get_callee() ) == *( _c->get_callee() ) ) ||
         get_line() != _c->get_line()
         )
    {
        return false;
    }
    return true;
}


std::string
Cnode::construct_own_hash()
{
//     return "";
    std::stringstream sstr;
    std::string       str;
    std::string       n_params;
    std::string       s_params;
    sstr << line;
    sstr >> str;

    for ( std::vector<std::pair<std::string, std::string> >::iterator it = str_parameters.begin(); it != str_parameters.end(); ++it )
    {
        s_params += ( *it ).first + "=" + ( *it ).second;
    }
    for ( std::vector<std::pair<std::string, double> >::iterator it = num_parameters.begin(); it != num_parameters.end(); ++it )
    {
        std::string _str;
        sstr << ( *it ).second;
        sstr >> _str;
        n_params = n_params + ( ( *it ).first ) + "=" + _str;
    }
    return mod + "." + ( get_callee()->get_hash_id() ) + str + "." + s_params + "." + n_params;
}


void
Cnode::fill_whole_tree_vector()
{
    whole_subtree = get_sub_tree_vector();
}

std::vector<Cnode*>&
Cnode::get_sub_tree_vector()
{
//     whole_subtree.push_back(this);
    for ( unsigned i = 0; i < num_children(); i++ )
    {
        whole_subtree.push_back( get_child( i ) );
        std::vector<Cnode*>& _vec = get_child( i )->get_sub_tree_vector();
        for ( std::vector<Cnode*>::const_iterator _iter = _vec.begin(); _iter != _vec.end(); ++_iter )
        {
            whole_subtree.push_back( *_iter );
        }
    }
    collected_whole_tree = true;
    return whole_subtree;
}





namespace cube
{
bool
operator==( const Cnode& a, const Cnode& b )
{
    if ( a.get_level() != b.get_level()
         ||
         a.get_mod() != b.get_mod()
         ||
         !( *a.get_callee() == *b.get_callee() )
         ||
         a.get_line() != b.get_line()
         )
    {
        return false;
    }

    Cnode* pa = a.get_parent();
    Cnode* pb = b.get_parent();
    if ( pa != NULL && pb != NULL )
    {
        return *pa == *pb;
    }
    else
    {
        return pa == pb;
    }
}
}
