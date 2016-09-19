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


#ifndef __CUBE_CALLTREE_CALLS_H
#define __CUBE_CALLTREE_CALLS_H


#include "CubeCnode.h"


/**
 * Prints the format of "usage" of this tool.
 */
void
usage( const char* argv )
{
    std::cout << "usage: " << argv << " -a -m metricname -f filename -h\n";
    exit( 0 );
}



/**
 * Recursiv counting of the level in a tree hierarchy of given cnode.
 */
int
get_level( const cube::Cnode& c )
{
    cube::Cnode* ptr = c.get_parent();
    int          level( 0 );
    while ( true )
    {
        if ( ptr == 0 )
        {
            break;
        }
        else
        {
            level++;
            ptr = ptr->get_parent();
        }
    }
    return level;
}


#endif
