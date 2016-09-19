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
 * \file cube4_cmp.cpp
 * \brief Compares two .cube files and prints out if they are equal or not.
 *
 */
/******************************************

   Performance Algebra Operation: COMPARE

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
 * - Read the first .cube input file and the second file.
 * - Calls cube_cmp(...) to compare them.
 * - Prints the result in human readable form.
 * - end.
 */
int
main( int argc, char* argv[] )
{
    int             ch;
    int             return_value = -1;
    vector <string> inputs;

    const string USAGE = "Usage: " + string( argv[ 0 ] ) + " [-h] <cube experiment 1> <cube experiment 2>\n"
                         "  -h     Help; Output a brief help message.\n"
    ;

    while ( ( ch = getopt( argc, argv, "o:ch?" ) ) != -1 )
    {
        switch ( ch )
        {
            case 'h':
            case '?':
                cerr << USAGE << endl;
                exit( 0 );
                break;
            default:
                cerr << USAGE << "\nError: Wrong arguments.\n";
                exit( 0 );
        }
    }

    if ( argc - optind == 2 )
    {
        string cur;
        for ( int i = optind; i < argc; i++ )
        {
            cur = argv[ i ];
            inputs.push_back( cur );
        }
    }
    else
    {
        cerr << USAGE << "Error: Two files required.\n\n";
        exit( 0 );
    }





    Cube* cube1 = new Cube();
    Cube* cube2 = new Cube();

//   cube2 = new Cube();
    try
    {
        cout << "Reading " << inputs[ 0 ] << " ... " << flush;
        if ( check_file( inputs[ 0 ].c_str() ) != 0 )
        {
            exit( -1 );
        }

        cube1->openCubeReport( inputs[ 0 ].c_str() );

        cout << "done." << endl;

        cout << "Reading " << inputs[ 1 ] << " ... " << flush;
        if ( check_file( inputs[ 1 ].c_str() ) != 0 )
        {
            exit( -1 );
        }
        cube2->openCubeReport( inputs[ 1 ].c_str() );
        cout << "done." << endl;

        // Write it
        cout << "++++++++++++ Compare operation begins ++++++++++++++++++++++++++" << endl;

        if ( cube::cube4_cmp( cube1, cube2 ) )
        {
            cout << "Experiments are equal." << endl;
            return_value = 0;
        }
        else
        {
            cout << "Experiments are not equal." << endl;
            return_value = 1;
        }

        cout << "+++++++++++++ Compare operation ends successfully ++++++++++++++++" << endl;
    }
    catch ( const RuntimeError& e )
    {
        cerr << "ERROR : " << e.get_msg() << endl;
    }
    delete cube1;
    delete cube2;
    return return_value;
}
