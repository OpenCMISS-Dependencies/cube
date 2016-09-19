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
 * \file cube4_derive.cpp
 * \brief Creates an metric using forumla and input from another metrics.
 *
 */


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

#include "cube4_derive_calls.h"
using namespace std;
using namespace cube;
using namespace services;



/**
 * Main program.
 * - Check the calling parameters.
 * - For every filename it does
 * - Read the  .cube file. and saves it on alternate way either in 0-th,
   or 1th place in cube[] array.
 * - Calls cube_merge(...) with previous cube (merged) and the current one.
 * - Saves a created merged cube in either "-o output" or "merge.cube|.gz" file.
 * - end.
 */
int
main( int argc, char* argv[] )
{
    int ch;
//     bool            subset   = true;
//     bool            collapse = false;
    vector <string> inputs;
    const char*     output = "derived";
    string          expression;
    bool            expression_defined = false;
    string          parent_metric;
    derivedMetric   new_metric_type = METRIC_INCLUSIVE;
    InclExclType    metric_type     = INCLUSIVE;
    PrePostType     metric_kind     = SIMPLE;

    const string LONG_USAGE = "  Creates a new metric <targetmetric>,  calculates its data  usung a CubePL expression \"cubepl expression\" and  stores it in the new cube <output experiment | derived.cubex>\n\n"
                              "  New metric can be a real metric, containing data - then you can omit option \"-t\", or derived metric, which doesn't containt any data, but gets calculated on-the-fly. \n"
                              "  In last case use option \"-t prederived|postderived\", to select, what kind of derivation is needed \n\n"
                              "  Prederived and normal metrics can be exclusive or inclusive related to the call path. By choosing option \"-r exclusive|inclisive\" you can specify this property of a metric. \n"
                              "  If the new metric of the type \"postderived\", then option \"-r\" doesn't have an action.\n\n"
    ;


    const string USAGE = "Usage: " + string( argv[ 0 ] ) + "[-h] [-t prederived|postderived ] -r exclusive|inclusive -e <cubepl expression> -p <parent_metric> <targetmetric> <cube experiment> [-o <output experiment>] \n"
                         "  -h     Help; Output a brief help message.\n"
    ;

    while ( ( ch = getopt( argc, argv, "t:p:e:c:o:r:h?" ) ) != -1 )
    {
        switch ( ch )
        {
            case 'o':
                if ( strcmp( optarg, "" ) == 0 )
                {
                    cerr << USAGE << endl;
                    exit( -1 );
                }
                output = optarg;
                break;
            case 't':
                if ( strcmp( optarg, "postderived" ) == 0 )
                {
                    metric_kind = POSTDERIVED;
                    break;
                }
                if ( strcmp( optarg, "prederived" ) == 0 )
                {
                    metric_kind = PREDERIVED;
                    break;
                }
                cerr << "Supported only two kind of derived metrics : postderived or prederived" << endl;
                exit( -2 );
                break;
            case 'r':
                if ( strcmp( optarg, "inclusive" ) == 0 )
                {
                    metric_type = INCLUSIVE;
                    break;
                }
                if ( strcmp( optarg, "exclusive" ) == 0 )
                {
                    metric_type = EXCLUSIVE;
                    break;
                }
                cerr << "Supported only two kind of metrics : exclusive or inclisive" << endl;
                exit( -2 );
                break;
            case 'e':
                expression         = string( optarg );
                expression_defined = true;
                break;
            case 'p':

                parent_metric = string( optarg );
                break;
            case 'h':
                cerr << USAGE << endl;
                cerr << LONG_USAGE << endl;
                exit( 0 );
                break;
            case '?':
                cerr << USAGE << endl;
                exit( 0 );
                break;
            default:
                cerr << USAGE << "\nError: Wrong arguments.\n";
                exit( 0 );
        }
    }


    if ( metric_kind == SIMPLE )
    {
        if ( metric_type == INCLUSIVE )
        {
            new_metric_type = METRIC_INCLUSIVE;
        }
        else
        {
            new_metric_type = METRIC_EXCLUSIVE;
        }
    }
    else
    if ( metric_kind == PREDERIVED )
    {
        if ( metric_type == INCLUSIVE )
        {
            new_metric_type = PREDERIVED_METRIC_INCLUSIVE;
        }
        else
        {
            new_metric_type = PREDERIVED_METRIC_EXCLUSIVE;
        }
    }
    else
    if ( metric_kind == POSTDERIVED )
    {
        new_metric_type = POSTDERIVED_METRIC;
    }


    if ( !expression_defined )
    {
        cerr << USAGE << "\nError: Expression for metric calculation is missing. Use the option -e <expression>.\n";
        exit( 0 );
    }

    if ( argc - optind != 2 )
    {
        cerr << USAGE << "\nError: Wrong arguments.\n";
        exit( 0 );
    }
    std::string target   = argv[ optind ];
    std::string cubename = argv[ optind + 1 ];

    Cube* inCube  = new Cube();
    Cube* outCube = NULL;
    try
    {
        inCube->openCubeReport( cubename );

        outCube = new Cube( *inCube, CUBE_DEEP_COPY );

        add_derived_metric( *outCube, *inCube, parent_metric, target, new_metric_type,    expression );

        outCube->writeCubeReport( output );
    }
    catch ( RuntimeError err )
    {
        cerr << err.get_msg() << endl;
    }

    delete inCube;
    delete outCube;
}
