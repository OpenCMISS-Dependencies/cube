/****************************************************************************
**  CUBE        http://www.score-p.org/                                    **
**  SCALASCA    http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 1998-2013                                                **
**  Forschungszentrum Juelich GmbH, Juelich Supercomputing Centre          **
**                                                                         **
**  Copyright (c) 2009-2013                                                **
**  German Research School for Simulation Sciences GmbH,                   **
**  Laboratory for Parallel Programming                                    **
**                                                                         **
**  This software may be modified and distributed under the terms of       **
**  a BSD-style license.  See the COPYING file in the package base         **
**  directory for details.                                                 **
****************************************************************************/
/**
 * \file cube_test.c
   \brief A small example of using "libcubew3.a".
   One creates a cube file "example.cube", defined structure of metrics, call tree, machine, cartesian topology
   and fills teh severity matrix.
   At the end the file will be written on the disk.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>       /* time */

#ifdef WITH_CUBE4
#include "cubew_cube.h"
#endif

#ifdef WITH_CUBE3
#include "cube.h"
#endif

#define NTHREADS 5000000
#define NCNODES 100

#define NDIMS 2

// / Start point of this example.
int
main( int argc, char* argv[] )
{
//    FILE* fp = fopen(cubefile, "w");
    cube_t* cube = cube_create( "example5", CUBE_MASTER, CUBE_FALSE );
    if ( !cube )
    {
        fprintf( stderr, "cube_create failed!\n" );
        exit( 1 );
    }
    srand( time( NULL ) );
    /* generate header */
    cube_def_mirror( cube, "http://icl.cs.utk.edu/software/kojak/" );
    cube_def_mirror( cube, "http://www.fz-juelich.de/jsc/kojak/" );
    cube_def_attr( cube, "description", "a simple example" );
    cube_def_attr( cube, "experiment time", "November 1st, 2004" );

//    if (fp == NULL) {
//        fprintf(stderr,"Error opening file %s: %s\n", cubefile, strerror(errno));
//        exit(2);
//    } else
    printf( "Test file \"example5.cubex\" is being generated ...\n" );
    fflush( stdout );
    /* define metric tree */
    cube_metric* mets[ 3 ];
    printf( "Create 3 metrics ...\n" );
    fflush( stdout );
    mets[ 0 ] = cube_def_met( cube, "Time", "Uniq_name1", "FLOAT", "sec", "",
                              "@mirror@patterns-2.1.html#execution",
                              "root node", NULL, CUBE_METRIC_INCLUSIVE );
    mets[ 1 ] = cube_def_met( cube, "User time", "Uniq_name2", "FLOAT", "sec", "",
                              "http://www.cs.utk.edu/usr.html",
                              "2nd level", mets[ 0 ], CUBE_METRIC_INCLUSIVE );
    mets[ 2 ] = cube_def_met( cube, "System time", "Uniq_name3", "FLOAT", "sec", "",
                              "http://www.cs.utk.edu/sys.html",
                              "2nd level", mets[ 0 ], CUBE_METRIC_INCLUSIVE );
    printf( "Created 3 metrics ...\n" );
    fflush( stdout );
    /* define call tree */
    char*        mod = "/ICL/CUBE/example.c";
    cube_region* regn0, * regn1;
    regn0 = cube_def_region( cube, "main", "main", "mpi", "barrier", 21, 100, "", "1st level", mod );
    regn1 = cube_def_region( cube, "<<init>>foo", "<<init>>foo", "pthread", "p2p", 1, 10, "", "2nd level", mod );

    cube_cnode* cnode0;
    cube_cnode* cnode1[ NCNODES ];
    cnode0 = cube_def_cnode_cs( cube, regn0, mod, 21, NULL );
    unsigned c_i = 0;
    for ( c_i = 0; c_i < NCNODES; c_i++ )
    {
        int v1 = ( c_i <= 5 ) ? 0 : rand() % ( c_i - 1 );

        cnode1[ c_i ] = cube_def_cnode_cs( cube, regn1, mod, 60, cnode1[ v1 ]  );
    }

    printf( "Creates calltree (%d callpaths) ...\n", NCNODES );
    fflush( stdout );

    /* define location tree */
    cube_machine* mach  = cube_def_mach( cube, "MSC<<juelich>>", "" );
    cube_node*    node  = cube_def_node( cube, "Athena<<juropa>>", mach );
    cube_process* proc0 = cube_def_proc( cube, "Process 0<<master>>", 0, node );
    cube_thread*  thrd[ NTHREADS ];

    unsigned t_i = 0;
    for ( t_i = 0; t_i < NTHREADS; t_i++ )
    {
        thrd[ t_i ] = cube_def_thrd( cube, "Thread 0<<iterator>>", t_i, proc0 );
    }
    printf( "Created %d threads ...\n", NTHREADS );
    fflush( stdout );
    /* define first topology */
    long            dimv0[ NDIMS ]    = { 5, 5 };
    int             periodv0[ NDIMS ] = { 1, 0 };
    cube_cartesian* cart0             = cube_def_cart( cube, NDIMS, dimv0, periodv0 );
    cube_cart_set_name( cart0, "My Topology 1" );

    long coordv[ NDIMS ] = { 0, 0 };
    cube_def_coords( cube, cart0, thrd[ 1 ], coordv );

    /* define second topology */
    long            dimv1[ NDIMS ]    = { 3, 3 };
    int             periodv1[ NDIMS ] = { 1, 0 };
    cube_cartesian* cart1             = cube_def_cart( cube, NDIMS, dimv1, periodv1 );
    cube_cart_set_name( cart1, "2323 Topoly<>&SDFGs\"\"ogy 3" );

    long coordv0[ NDIMS ] = { 0, 1 };
    long coordv1[ NDIMS ] = { 1, 0 };
    cube_def_coords( cube, cart1, thrd[ 0 ], coordv0 );
    cube_def_coords( cube, cart1, thrd[ 1 ], coordv1 );





    /* define second topology*/
    long            dimv2[ 4 ]    = { 3, 3, 3, 3 };
    int             periodv2[ 4 ] = { 1, 0, 0, 0 };
    cube_cartesian* cart2         = cube_def_cart( cube, 4, dimv2, periodv2 );

    long coordv20[ 4 ] = { 0, 1, 0, 0 };
    long coordv21[ 4 ] = { 1, 0, 0, 0 };
    cube_def_coords( cube, cart2, thrd[ 0 ], coordv20 );
    cube_def_coords( cube, cart2, thrd[ 1 ], coordv21 );
    cube_cart_set_name( cart2, "Second" );


    /* define third topology*/
    long            dimv3[ 14 ]    = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
    int             periodv3[ 14 ] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    cube_cartesian* cart3          = cube_def_cart( cube, 14, dimv3, periodv3 );
    long            coordv32[ 14 ] = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 };
    long            coordv33[ 14 ] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    cube_def_coords( cube, cart3, thrd[ 0 ], coordv32 );
    cube_def_coords( cube, cart3, thrd[ 1 ], coordv33 );
    cube_cart_set_name( cart3, "Third" );






    double* sev = ( double* )malloc( NTHREADS * sizeof( double ) );
    for ( t_i = 0; t_i < NTHREADS; t_i++ )
    {
        sev[ t_i ] = t_i * 0.23 + t_i;
    }

    unsigned mi = 0;
    for ( mi = 0; mi < 3; mi++ ) // / run over the metrics
    {
        printf( "Write metric %d \n", mi );
        fflush( stdout );
        carray* sequence = cube_get_cnodes_for_metric( cube, mets[ mi ] );

        unsigned ci = 0;
        for ( ci = 0; ci < sequence->size; ci++ )
        {
            if ( ci % 20 == 0 )
            {
                printf( "Write %d callpaths \n", ci );
                fflush( stdout );
            }
            cube_write_sev_row_of_doubles( cube, mets[ mi ], ( cube_cnode* )( sequence->data )[ ci ], sev );
        }
    }
    printf( "Done. \n" );
    fflush( stdout );
    free( sev );
    cube_free( cube );
    return 0;
}
