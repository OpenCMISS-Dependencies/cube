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
 * \file CubeSysres.h
 * \brief Provides a general system resource with a tree like hierarchy.
 */
#ifndef CUBE_SYSRES_H
#define CUBE_SYSRES_H


#include "CubeVertex.h"
#include "CubeTypes.h"

/*
   *----------------------------------------------------------------------------
 *
 * class Sysres
 *
 ***********************----------------------------------------------------------------------------
 */

namespace cube
{
/**
 * System resources have a treelike structure -> therefore it is a kind of Vertex. And every Sysres has an id and name.
 */
class Sysres : public Vertex
{
public:

    Sysres( const std::string& name,
            uint32_t           id = 0,
            uint32_t           sysid = 0 )
        : Vertex( id ), name( name ), sys_id( sysid )
    {
        own_hash = construct_own_hash();
        kind     = CUBE_UNKNOWN;
    }
    Sysres( Sysres*            parent,
            const std::string& name,
            uint32_t           id = 0,
            uint32_t           sysid = 0 )
        : Vertex( parent, id ), name( name ), sys_id( sysid )
    {
        own_hash = construct_own_hash();
    }

    Sysres( const Sysres& copy )
        :  Vertex( copy.get_parent(), copy.get_id() ), name( copy.name ), sys_id( copy.sys_id )
    {
        for ( unsigned i = 0; i < copy.num_children(); ++i )
        {
            add_child( copy.get_child( i ) );
        }
    }



    inline
    const std::string&
    get_name() const
    {
        return name;
    }                                            // /< Name of the resource.

    inline
    void
    set_name( std::string newname )
    {
        name = newname;
    }                                                     // /< Set a name.

    inline
    uint32_t
    get_sys_id() const
    {
        return sys_id;
    }


    inline
    sysres_kind
    get_kind() const
    {
        return kind;
    };

    inline
    bool
    isMachine() const
    {
        return ( kind == CUBE_SYSTEM_TREE_NODE ) && ( get_parent() == NULL );
    };

    inline
    bool
    isNode() const
    {
        return ( kind == CUBE_SYSTEM_TREE_NODE ) && ( get_parent() != NULL );
    };

    inline
    bool
    isProcess() const
    {
        return kind == CUBE_LOCATION_GROUP;
    };

    inline
    bool
    isThread() const
    {
        return kind == CUBE_LOCATION;
    };

    inline
    bool
    isSystemTreeNode() const
    {
        return kind == CUBE_SYSTEM_TREE_NODE;
    };

    inline
    bool
    isLocationGroup() const
    {
        return kind == CUBE_LOCATION_GROUP;
    };

    inline
    bool
    isLocation() const
    {
        return kind == CUBE_LOCATION;
    };

    inline
    bool
    isUnknown() const
    {
        return kind == CUBE_UNKNOWN;
    };



/**
 * This sysres is a part of which sysres?
 */

    inline
    Sysres*
    get_parent() const
    {
        return static_cast<Sysres*>( Vertex::get_parent() );
    }
/**
 * This sysres contains i-th sysres.... which is ?
 */

    inline
    Sysres*
    get_child( unsigned int i ) const
    {
        return static_cast<Sysres*>( Vertex::get_child( i ) );
    }



protected:
    sysres_kind kind;
    std::string name;
    uint32_t    sys_id;
};
}

#endif
