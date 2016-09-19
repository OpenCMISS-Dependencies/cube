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
 * \file CubeVertex.h
 * \brief Provides an element of a tree.
 */
#ifndef CUBE_VERTEX_H
#define CUBE_VERTEX_H

#include <string>
#include <vector>
#include <map>
#include <ostream>

#include "CubeIdentObject.h"


/*
   *----------------------------------------------------------------------------
 *
 * class Vertex
 *
 *******************************----------------------------------------------------------------------------
 */

namespace cube
{
/**
 * Every element of a tree has an ID. Therefore is Vertex a kind of IdenObject.
 */
class Vertex : public IdentObject
{
public:

    Vertex( Vertex*  parent,
            uint32_t id = 0 );
    Vertex( uint32_t id = 0 ) : IdentObject( id )
    {
        parent = NULL;
    }

    inline
    std::string
    indent() const
    {
        return std::string( 2 * get_level(), ' ' );
    }                                                                       ///< By printing is a proper level of indent

    inline
    unsigned int
    num_children() const
    {
        return childv.size();
    }                                                           ///< How many undertrees ?

    inline
    Vertex*
    get_parent() const
    {
        return parent;
    }                                            ///< Root element?

    inline
    std::vector<Vertex*>&
    get_children()
    {
        return childv;
    }

    /// set attributes
    void
    def_attr( const std::string& key,
              const std::string& value );

    /// get attribute
    std::string
    get_attr( const std::string& key ) const;

    const std::map<std::string, std::string>&
    get_attrs() const;

    Vertex*
    get_child( unsigned int i ) const;

    const std::vector<Vertex*>&
    get_children() const;                       ///< Returns a reference to the vector of its children

    int
    get_level() const;     /// Recursiv calculation of a level in the tree hierarchy . Moved from private are for development of student M.Meyer

    uint64_t
    total_num_children();     /// Recursiv calculation of a number of all children in the tree hierarchy .



    bool
    weakEqual( Vertex* );


    void
    set_parent( Vertex* vertex );

protected:


    void
    writeAttributes( std::ostream&      out,
                     const std::string& indent,
                     bool               cube3export = false
                     ) const;

    void
    add_child( Vertex* vertex );
    virtual std::string
    construct_hash_id();

    virtual std::string
    construct_own_hash()
    {
        return "_vertex_hash_";
    };
    virtual std::string
    get_own_hash()
    {
        return own_hash;
    };


    std::string                        own_hash;
    std::map<std::string, std::string> attrs;       ///< attributes <key, value>
//     std::map<std::string, unsigned> children_counter;

    std::vector<Vertex*> childv;
    Vertex*              parent;
    uint64_t             total_number_of_children; // including all sublings...
};
}

#endif
