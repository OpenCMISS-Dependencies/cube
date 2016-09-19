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
 * \file cube4_cut.cpp
 * \brief Cuts, reroots or prunes the calltree of a cube.
 *
 */
/******************************************

   Performance Algebra Operation: CUT (Calltree)

 *******************************************/

#include <cstdlib>
#include <iostream>
#include <string>

#include <sstream>
#include <unistd.h>

#include "Cube.h"
#include "CubeCnode.h"
#include "CubeMachine.h"
#include "CubeMetric.h"
#include "CubeRegion.h"
#include "CubeServices.h"
#include "algebra4.h"
using namespace std;
using namespace cube;
using namespace services;
/**
 * Main program.
 * - Check calling arguments
 * - Read the  .cube input file.
 * - Calls  cube_cut(...) to cut the calltree.
 * - Saves the result in "-o outputfile" or "cut.cube|.gz" file.
 * - end.
 */
int
main( int argc, char* argv[] )
{
    int             ch;
    bool            subset   = false;
    bool            collapse = false;
    vector <string> inputs;
    vector <string> prunes;
    string          root;
    const char*     output = "cut";

    const string USAGE = "Usage: " + string( argv[ 0 ] ) +
                         " [-h] [-c|-C] [-r nodename] [-p nodename] [-o output] <cube experiment>\n"
                         "  -r     Re-root calltree at named node\n"
                         "  -p     Prune calltree from named node (== \"inline\")\n"
                         "  -o     Name of the output file (default: " + output + ")\n"
                         "  -c     Reduce system dimension, if experiments are incompatible. \n"
                         "  -C     Collapse system dimension! Overrides option -c.\n"
                         "  -h     Help; Show this brief help message and exit.\n"

    ;

    while ( ( ch = getopt( argc, argv, "cCr:p:o:h" ) ) != -1 )
    {
        switch ( ch )
        {
            case 'o':
                output = optarg;
                break;
            case 'p':
                prunes.push_back( optarg );
                break;
            case 'r':
                root = optarg;
                break;
            case 'c':
                subset = true;
                break;
            case 'C':
                collapse = true;
                break;
            case 'h':
            case '?':
                cerr << USAGE << endl;
                exit( 0 );
                break;
            default:
                cerr << USAGE << "\nError: Wrong arguments.\n";
                exit( 1 );
        }
    }

    if ( argc - optind != 1 )
    {
        cerr << USAGE << "\nError: Wrong arguments.\n";
        exit( 1 );
    }

    for ( int i = optind; i < argc; i++ )
    {
        inputs.push_back( argv[ i ] );
    }


//   if (!cube_stat(inputs[0])) exit(2);

    if ( ( root == "" ) && ( prunes.size() == 0 ) )
    {
        cerr << "Must specify new root node and/or node(s) to prune" << endl;
        exit( 2 );
    }

    for ( size_t p = 0; p < prunes.size(); p++ )
    {
        if ( prunes[ p ] == root )
        {
            cerr << "Can't both reroot and prune node '" << root << "'" << endl;
            exit( 2 );
        }
    }




    Cube* inCube = new Cube();
    Cube* tmp    = new Cube();

    cout << "Reading " << inputs[ 0 ] << " ... " << flush;
    if ( check_file( inputs[ 0 ].c_str() ) != 0 )
    {
        exit( -1 );
    }
    inCube->openCubeReport( inputs[ 0 ].c_str() );
    cout << "done." << endl;

    cout << "++++++++++++ Cut operation begins ++++++++++++++++++++++++++" << endl;
    Cube* cut = new Cube();

    cube4_cut( cut, tmp, inCube, subset, collapse, root, prunes );
    delete tmp;
    delete inCube;

//   if (outCube == NULL) {
//     cerr << "Error: Failed to locate new root node '" << root << "'" << endl;
//     exit(2);
//   }

    cout << "++++++++++++ Cut operation ends successfully ++++++++++++++++" << endl;

    cout << "Writing " << output << " ... " << flush;
    cut->writeCubeReport( output );
    delete cut;
    cout << "done." << endl;


    return 0;
}
