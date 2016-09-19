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
 * \file CubeLocationGroup.h
 * \brief Provides a relevant description of a locations group.
 */

#ifndef CUBE_LOCATION_GROUP_H
#define CUBE_LOCATION_GROUP_H


#include "CubeSysres.h"
#include "CubeTypes.h"



namespace cube
{
inline bool
operator==( const LocationGroup& a,
            const LocationGroup& b );

/**
 * Thread is a basics kind of system resources. It doesn't return any "children"
 */
class LocationGroup : public Sysres
{
public:

    LocationGroup( const std::string& name,
                   SystemTreeNode*    stn,
                   int                rank,
                   LocationGroupType  type = cube::CUBE_LOCATION_GROUP_TYPE_PROCESS,
                   uint32_t           id = 0,
                   uint32_t           sysid = 0 );



    std::string
    get_type_as_string() const;


    inline
    int
    get_rank() const
    {
        return rank;
    }



    inline
    LocationGroupType
    get_type() const
    {
        return type;
    }


    inline
    SystemTreeNode*
    get_parent() const
    {
        return ( SystemTreeNode* )Vertex::get_parent();
    }

    inline
    Location*
    get_child( unsigned int i ) const
    {
        return ( Location* )Vertex::get_child( i );
    }


    // / returns treu, if ids of all children are gapless monoton increasing. Used on the calculation optimisation

    inline
    bool
    isContinous()
    {
        return false;
    }





    void
    writeXML( std::ostream& out,
              bool          cube3export = false  ) const;    // /< Writes a xml-representation of a thread in a .cube file.


    virtual std::string
    construct_own_hash();


    static
    LocationGroupType
    getLocationGroupType( std::string );

private:
    int               rank;
    LocationGroupType type;
};


inline bool
operator==( const LocationGroup& a, const LocationGroup& b )
{
    int         _a  = a.get_rank();
    int         _b  = b.get_rank();
    std::string __a = a.get_name();
    std::string __b = b.get_name();

    return ( _a == _b ) && ( __a.compare( __b ) == 0 );
}




inline bool
operator<( const LocationGroup& a, const LocationGroup& b )
{
    int _a = a.get_rank();
    int _b = b.get_rank();
    return _a < _b;
}
}



#endif
