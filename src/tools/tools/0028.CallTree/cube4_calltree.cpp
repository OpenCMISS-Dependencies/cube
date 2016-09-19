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
 * \file cube_calltree.cpp
 * \brief Prints out in a tree format a call path with values for given
   (-m metricname) metric.
 *
 * It implements a non recursive algorithm to print out the tree structure.
   Cnodes are saved in a vector ccnt.  Internaly they save a parent-child relation
   between eachother. It is a reason for "non recursive" algorithm.
 */

// #ifndef CUBE_COMPRESSED
#include <fstream>
// #else
#include "CubeZfstream.h"
// #endif

#include <iomanip>
#include <string>
#include <algorithm>
#include <numeric>
#include <list>
#include <map>
#include <iostream>

#include "Cube.h"
#include "CubeCnode.h"
#include "CubeThread.h"
#include "CubeMetric.h"
#include "CubeRegion.h"
#include "helper.h"
#include "CubeServices.h"

#include "CnodeInfo.h"

#include <cstdio>
#include <unistd.h>

#include "calltree_calls.h"

using namespace std;
using namespace cube;
using namespace services;


/**
 * Main program.
 * - Check calling arguments
 * - Read the .cube file
 * - If necessary -> collect data from metrics.
 * - Get a cnodes and print them out (with or wthout metrics value) with
   indentation according to the level of cnode in tree hierarchy.
 * - end.
 */
int
main( int argc, char* argv[] )
{
//   size_t fcnt(0); // unused
    bool annotate( false );
    int  c;
    opterr = 0;
    string metric_name( "undef" );
    string cubefile( "undef" );
    bool   do_metric( false );

    while ( ( c = getopt( argc, argv, "ham:f:" ) ) != -1 )
    {
        switch ( c )
        {
            case 'a':
                annotate = true;
                break;
            case 'm':
                metric_name = string( optarg );
                do_metric   = true;
                break;
            case 'f':
                cubefile = string( optarg );
                break;
            case 'h':
                usage( argv[ 0 ] );
                break;
        }
    }

    if ( cubefile == "undef" )
    {
        usage( argv[ 0 ] );
    }

    Cube* input = NULL;

    try
    {
        // Read input files
        cerr << "Reading " << cubefile << "...";
        input = new Cube();
        if ( check_file( cubefile.c_str() ) != 0 )
        {
            exit( -1 );
        }
/*
   #ifndef CUBE_COMPRESSED
    ifstream in(cubefile.c_str());
   #else
    gzifstream in(cubefile.c_str(), ios_base::in|ios_base::binary);
   #endif
    if (!in) {
        exit(1);
    }
    in >> *input;*/

        input->openCubeReport( cubefile.c_str() );

        cerr << " done." << endl;


        const vector<Cnode*>&  cnodes  = input->get_cnodev();
        const vector<Thread*>& threads = input->get_thrdv();
        size_t                 ccnt    = cnodes.size();
        size_t                 tcnt    = threads.size();

        vector<string> callpaths( ccnt, "undefined" );
        vector<string> cnode_names( ccnt, "undefined" );

        CCnodeInfo cninfo( input );

        vector<double>* pvec = NULL;        // initialized only if do_metric="true" and used also in this case.

        if ( do_metric )
        {
            Metric* metric = input->get_met( metric_name );
            pvec = new vector<double>( ccnt, 0 );
            for ( size_t ci = 0; ci < ccnt; ++ci )
            {
                Cnode* c = cnodes[ ci ];
                //        const Region* r = c->get_callee();  // unused
                for ( size_t ti = 0; ti < tcnt; ++ti )
                {
                    Thread* t   = threads[ ti ];
                    double  val = input->get_sev( metric, c, t );
                    ( *pvec )[ ci ] += val;
                }
            }
        }
        /*
            build string-formatted --calltree--

            example:

         + task_init
         |   + mpi_init
         |   |   + tracing
         |   + bcast_int
         |   |   + mpi_bcast
         |   + barrier_sync
         |   |   + mpi_barrier
         + read_input
         |   + bcast_int
         |   |   + mpi_bcast

            results in cnode_names[] and callpaths[]
         */

        cnode_names[ 0 ] = cnodes[ 0 ]->get_callee()->get_name();
        callpaths[ 0 ]   = "/" + cnode_names[ 0 ];
        size_t nl( ( size_t )-1 ); /* height of the current stack  */
        for ( size_t ci = 1; ci < ccnt; ++ci )
        {
            Cnode*        c      = cnodes[ ci ];
            Cnode*        parent = c->get_parent();
            const Region* r      = c->get_callee();
            string        name   = lowercase( r->get_name() );
            nl++;
            if ( cnodes[ ci - 1 ] != parent )
            {
                nl = get_level( *c ) - 1;
            }
            cnode_names[ ci ] = "  + " + name;
            for ( size_t sf = 0; sf < nl; sf++ )
            {
                cnode_names[ ci ] = "  | " + cnode_names[ ci ];
            }
            callpaths[ ci ] = get_callpath_for_cnode( *c );
        }
        int maxstrlen( 0 );
        for ( size_t i = 0; i < ccnt; i++ )
        {
            int l = cnode_names[ i ].length();
            if ( l > maxstrlen )
            {
                maxstrlen = l;
            }
        }
        for ( size_t i = 0; i < ccnt; i++ )
        {
            cout << left;
            if ( do_metric )
            {
                cout << setw( 16 )
                     << ( *pvec )[ i ];
            }
            cout << setw( maxstrlen + 4 )
                 << cnode_names[ i ];
            if ( annotate )
            {
                cout << Callpathtype2String( cninfo[ i ] )
                     << ":";
            }
            cout << callpaths[ i ] << endl;
        }
    }
    catch ( const RuntimeError& err )
    {
        cerr << err.get_msg() << endl;
    }

    if ( input != NULL )
    {
        delete input;
    }
}
