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
 * \file CubeCartesian.h
 * \brief Provides a cartesian topology of the application.
 *
 * \details Topology means "(x,y,z) -> threas" in 3D case.
 */
#ifndef CUBE_CARTESIAN_H
#define CUBE_CARTESIAN_H


#include <iosfwd>
#include <iostream>

/*
   *----------------------------------------------------------------------------
 *
 * class Cartesian
 *****************----------------------------------------------------------------------------
 */

#include <vector>
#include <map>
#include <string>
#include "CubeTypes.h"
#include "CubeThread.h"

namespace cube
{
class Sysres;
// class Thread;

// typedef for sun compiler
typedef std::multimap<const Sysres*, std::vector<long> > TopologyMap;


/**
 * Defines and provides cartesian ( x, y, z )-> thread topology
 */
class Cartesian
{
public:

    Cartesian ( long                     ndims,
                const std::vector<long>& dimv,
                const std::vector<bool>& periodv )
        : ndims( ndims ), dimv( dimv ), periodv( periodv )
    {
        name = "";
    }

    void
    def_coords( const Sysres*            sys,
                const std::vector<long>& coordv );

    void
    writeXML( std::ostream& out,
              bool          cube3export = false   ) const;

    long
    get_ndims()   const
    {
        return ndims;
    }                                                              ///< Returns a number of dimensions of the topology.
    const std::vector<long>&
    get_dimv()    const
    {
        return dimv;
    }                                                             ///< Returns a vector of number of indecies in every dimension.
    const std::vector<bool>&
    get_periodv() const
    {
        return periodv;
    }                                                                ///< Returns a vector of periodicities in every dimension.
    const std::vector<long>&
    get_coordv( Sysres* sys ) const;

    const std::vector<std::vector<long> >
    get_all_coordv( Sysres* sys ) const;


    const std::multimap<const Sysres*, std::vector<long> >&
    get_cart_sys() const
    {
        return sys2coordv;
    }                                                                                               ///< Gets a mapping "resource -> coordinate"

    Cartesian*
    clone( const std::vector<Thread*>& );               ///< creates a deep copy of himself. (using gibven Threads for cloned copy)

    void
    set_name( const std::string&  );       ///<  Sets a name of the topology

    std::string&
    get_name()
    {
        return name;
    };

    bool
    set_namedims( const std::vector<std::string>);     // Sets the name of the dimensions inside a topology

    const std::vector<std::string>&
    get_namedims() const
    {
        return namedims;
    };

    bool
    set_dim_name( const int  dim,
                  const std::string& );                   // Sets the name of the dimensions inside a topology
    const std::string
    get_dim_name( const int dim );


    bool
    operator==( const Cartesian& b );

    inline bool
    operator!=( const Cartesian& b )
    {
        return !( *this == b );
    };

private:

    std::string                                      name;       ///< Name of the topology.
    unsigned long                                    ndims;      ///< A number of dimensions in the topology.
    std::vector<std::string>                         namedims;   ///< Vector of dimension names.
    std::vector<long>                                dimv;       ///< Number of indicies in  everydimension
    std::vector<bool>                                periodv;    ///< Periodicity in  everydimension
    std::multimap<const Sysres*, std::vector<long> > sys2coordv; ///< Mapping "resource -> coordinate".
};
}

#endif
