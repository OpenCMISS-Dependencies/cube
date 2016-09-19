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
 * \file CubeIdentObject.h
 * \brief Provides an object with an internal ID.
 */

#ifndef CUBE_IDENTOBJECT_H
#define CUBE_IDENTOBJECT_H


#include <inttypes.h>


/*
   *----------------------------------------------------------------------------
 *
 * class IdentObject
 *
 ***********************----------------------------------------------------------------------------
 */

namespace cube
{
class IdentObject
{
public:
    IdentObject( uint32_t id = 0 ) : m_id( id ), m_filed_id( id )
    {
    }
    virtual
    ~IdentObject()
    {
    }

    inline
    void
    set_id( uint32_t id )
    {
        m_id = id;
    }                                       // /< Set an ID.

    inline
    uint32_t
    get_id() const
    {
        return m_id;
    }                                        // /< Gets the ID of the object.


    inline
    void
    set_filed_id( uint32_t id )
    {
        m_filed_id = id;
    }                                       // /< Set an ID.

    inline
    uint32_t
    get_filed_id() const
    {
        return m_filed_id;
    }                                        // /< Gets the ID of the object.





    inline
    std::string
    get_hash_id() const
    {
        return hash_id;
    };                                                                  // /<




    inline
    void
    calculate_hash()
    {
        hash_id = construct_hash_id();
    };


    static
    bool
    id_compare( IdentObject* a, IdentObject* b )
    {
        return ( a->get_id() ) < ( b->get_id() );
    }


protected:
    std::string hash_id;        // /< Hash ID of the object. Independent on ID and depends only on "properties" of object, but not on the place in the creation sequence (which is ID).
    virtual std::string
    construct_hash_id()
    {
        return "_ident_object_hash_";
    };

    uint32_t m_id;       // /< The ID of the object.
    uint32_t m_filed_id; // /< Not the "internal" ID , but assigned in some meta description,
};
}

#endif
