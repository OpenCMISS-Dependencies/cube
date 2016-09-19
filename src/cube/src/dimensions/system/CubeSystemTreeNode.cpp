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
 * \file CubeSystemTreeNode.cpp
 * \brief Defines a function for xml-representation of the general system tree node.
 ********************************************/


#include <iostream>


#include "CubeSystemTreeNode.h"
#include "CubeLocationGroup.h"
#include "CubeServices.h"

using namespace std;
using namespace cube;
using namespace services;

SystemTreeNode::~SystemTreeNode()
{
}



void
SystemTreeNode::writeXML( std::ostream& out,
                          bool          cube3export ) const
{
    if ( cube3export )
    {
        if ( get_parent() == NULL )
        {
            out << indent() << "    <machine Id=\"" << get_id() << "\">" <<  '\n';
        }
        else
        {
            out << indent() << "    <node Id=\"" << get_id() << "\">" <<  '\n';
        }
    }
    else
    {
        out << indent() << "    <systemtreenode Id=\"" << get_id() << "\">" <<  '\n';
    }

    out << indent() << "      <name>" << escapeToXML( get_name() ) << "</name>" <<  '\n';
    if ( !cube3export )
    {
        out << indent() << "    <class>" << get_class() << "</class>" <<  '\n';
        if ( !get_desc().empty() || get_desc() != "" )
        {
            out << indent() << "      <descr>" << escapeToXML( get_desc() ) << "</descr>" <<  '\n';
        }
    }
    else  // export to cube3
    if ( get_parent() == NULL )   // only for machine
    {
        if ( !get_desc().empty() || get_desc() != "" )
        {
            out << indent() << "      <descr>" << escapeToXML( get_desc() ) << "</descr>" <<  '\n';
        }
    }


    writeAttributes( out,  indent() + "      ", cube3export  );

    for ( unsigned int i = 0; i < groups.size(); ++i )
    {
        const LocationGroup* lg = groups[ i ];
        lg->writeXML( out, cube3export  );
    }
    for ( unsigned int i = 0; i < num_children(); ++i )
    {
        const SystemTreeNode* node = get_child( i );
        node->writeXML( out, cube3export );
    }
    if ( cube3export )
    {
        if ( get_parent() == NULL )
        {
            out << indent() << "    </machine>" <<  '\n';
        }
        else
        {
            out << indent() << "    </node>" <<  '\n';
        }
    }
    else
    {
        out << indent() << "    </systemtreenode>" <<  '\n';
    }
}
