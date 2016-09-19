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
 * \file CubeCnode.h
 * \brief Provides a class with an element of call tree.
 */
#ifndef CUBE_CNODE_H
#define CUBE_CNODE_H


#include <iosfwd>
#include <string>
#include <utility>
#include <vector>
#include "CubeVertex.h"
#include "CubeTypes.h"

/*
   *----------------------------------------------------------------------------
 *
 * class Cnode
 *
 **************************----------------------------------------------------------------------------
 */

namespace cube
{
/**
 * An element of the call tree. As child of general element of a tree (Vertex).
 */
class Cnode : public Vertex
{
public:

    Cnode( Region*            callee,
           const std::string& mod,
           int                line,
           Cnode*             parent,
           uint32_t           id = 0
           );

    ~Cnode();                                   // /< Desctructor... destroys remapping_cnode and children recursively.

    inline
    int
    get_line() const
    {
        return line;
    }                                     // /< Returns a start line of a region.

    inline

    std::string
    get_mod() const
    {
        return mod;
    }                                           // /< Returns a modus of a call node.

    inline
    Region*
    get_callee() const
    {
        return callee;
    }                                            // /< Returns a region, from where entered here.
    Region*
    get_caller() const;


    inline
    Cnode*
    get_parent() const
    {
        return static_cast<Cnode*>( Vertex::get_parent() ); // /< Get a call three from where enetered.
    }

    inline
    Cnode*
    get_child( unsigned int i ) const
    {
        return static_cast<Cnode*>( Vertex::get_child( i ) ); // /< Get a further calls of from this call.
    }

    void
    add_num_parameter( std::string key,
                       double      param );                                    // /< Adds an additional numeric parameter to the parameter list.
    void
    add_str_parameter( std::string key,
                       std::string param );                                         // /< Adds an additional string parameter to the parameter list.


    inline
    std::vector < std::pair < std::string, double> >
    get_num_parameters() const
    {
        return num_parameters;
    }
    // /< Returns a list of numerical parameters, with which a callee was called/marked.
    inline
    std::vector < std::pair  <std::string, std::string> >
    get_str_parameters() const
    {
        return str_parameters;
    }                                                     // /< Returns a list of string parameters, with which a callee was called/marked.



    inline
    bool
    isHidden() const
    {                                                    // /< Returns the status "hidden" or not of the node
        return is_hidden;
    }


    inline
    bool
    isVisible() const
    {                                                    // /< Returns the status "not hidden"
        return !isHidden();
    }

    Cnode*
    get_remapping_cnode( int64_t process_rank = -1 );                                // /< Returns either remapped cnode for given "process id"  or "common remapped cnode" or this.

    void
    set_remapping_cnode( Cnode* );                              // /< Sets common remapped cnode for this cnode and cluster normalization factor

    void
    set_remapping_cnode( int64_t,  Cnode *, int64_t );                      // /< Sets remapped cnode for this cnode depending on process id  and cluster normalization factor.

    int64_t
    get_cluster_normalization( int64_t );


    inline
    bool
    is_clustered()
    {
        return !no_remapping;
    }

    std::vector<cube::Cnode*>&
    get_whole_subtree()
    {
        return whole_subtree;
    }





    void
    hide();                                              // /< Hides cnode

    void
    set_as_leaf();                                      // /< Hides all leaves.

    void
    writeXML( std::ostream& out,
              bool          cube3export = false ) const;               // /< Writes a xml-representation of the call node.

    bool
    weakEqual( Cnode* _c );                                             // /< compared just a body of cnode, not the hierarhy...
    virtual std::string
    construct_own_hash();                                               // /< Constructs hash using id. It is never same, has <= defined operation.

    void
    whole_tree_is_collected()
    {
        collected_whole_tree = true;
    };

    bool
    is_whole_tree_collected()
    {
        return collected_whole_tree;
    }

    void
    fill_whole_tree_vector();

protected:



    std::vector<cube::Cnode*>&
    get_sub_tree_vector();

private:

    Region*                                           callee; // /< Region in a source code, where the call node was entered from.
    std::string                                       mod;    // /< Modus.
    int                                               line;   // /< Start line in a region.
    std::vector<std::pair<std::string, std::string> > str_parameters;
    std::vector<std::pair<std::string, double> >      num_parameters;
    Cnode*                                            common_remapping_cnode;
    bool                                              no_remapping;  // /< set to "true"ifnoclustering is engaged.
    std::map<int64_t, Cnode*>                         remapping_cnodes;
    bool                                              is_hidden;     // /< Signalizes, that this cnode is hidden and contributes to excl value of the parent.
    std::map<int64_t, int64_t>                        cluster_normalization;
    std::vector<Cnode*>                               whole_subtree; // used to calculate inclusive value by exclusive metric
    bool                                              collected_whole_tree;
};

bool
operator==( const Cnode& a,
            const Cnode& b );
}





#endif
