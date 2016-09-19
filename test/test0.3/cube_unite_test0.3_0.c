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
 * \file cube_unite_test0.3_0.c
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

#ifdef WITH_CUBE4
#include "cubew_cube.h"
#endif

#ifdef WITH_CUBE3
#include "cube.h"
#endif

#define NDIMS 2

// / Start point of this example.
int
main( int argc, char* argv[] )
{
//    FILE* fp = fopen(cubefile, "w");
    cube_t* cube = cube_create( "values_support", CUBE_MASTER, CUBE_FALSE );
    if ( !cube )
    {
        fprintf( stderr, "cube_create failed!\n" );
        exit( 1 );
    }

    /* generate header */
    cube_def_mirror( cube, "http://icl.cs.utk.edu/software/kojak/" );
    cube_def_mirror( cube, "http://www.fz-juelich.de/jsc/kojak/" );
    cube_def_attr( cube, "description", "a simple example" );
    cube_def_attr( cube, "experiment time", "November 1st, 2004" );

//    if (fp == NULL) {
//        fprintf(stderr,"Error opening file %s: %s\n", cubefile, strerror(errno));
//        exit(2);
//    } else
    printf( "Test file \"values_support.cubex\" is being generated ...\n" );

    /* define metric tree */
    cube_metric* met_uint8, * met_int8, * met_uint16, * met_int16, * met_uint32, * met_int32, * met_uint64, * met_int64, * met_double, * met_tau_atomic, * met_min_double, * met_max_double, * met_rate,
    * met_scale_func;

    met_uint8 = cube_def_met( cube, "Counter1", "Uniq_name1", "UINT8", "sec", "",
                              "@mirror@patterns-2.1.html#execution",
                              "root node", NULL, CUBE_METRIC_EXCLUSIVE );

    met_int8 = cube_def_met( cube, "Counter2", "Uniq_name2", "INT8", "sec", "",
                             "@mirror@patterns-2.1.html#execution",
                             "root node", NULL, CUBE_METRIC_EXCLUSIVE );


    met_uint16 = cube_def_met( cube, "Counter3", "Uniq_name3", "UINT16", "sec", "",
                               "@mirror@patterns-2.1.html#execution",
                               "root node", NULL, CUBE_METRIC_EXCLUSIVE );


    met_int16 = cube_def_met( cube, "Counter4", "Uniq_name4", "INT16", "sec", "",
                              "@mirror@patterns-2.1.html#execution",
                              "root node", NULL, CUBE_METRIC_EXCLUSIVE );



    met_uint32 = cube_def_met( cube, "Counter5", "Uniq_name5", "UINT32", "sec", "",
                               "@mirror@patterns-2.1.html#execution",
                               "root node", NULL, CUBE_METRIC_EXCLUSIVE );


    met_int32 = cube_def_met( cube, "Counter6", "Uniq_name6", "INT32", "sec", "",
                              "@mirror@patterns-2.1.html#execution",
                              "root node", NULL, CUBE_METRIC_EXCLUSIVE );

    met_uint64 = cube_def_met( cube, "Counter7", "Uniq_name7", "UINT64", "sec", "",
                               "@mirror@patterns-2.1.html#execution",
                               "root node", NULL, CUBE_METRIC_EXCLUSIVE );


    met_int64 = cube_def_met( cube, "Counter8", "Uniq_name8", "INT64", "sec", "",
                              "@mirror@patterns-2.1.html#execution",
                              "root node", NULL, CUBE_METRIC_EXCLUSIVE );

    met_double = cube_def_met( cube, "Counter9", "Uniq_name9", "FLOAT", "sec", "",
                               "@mirror@patterns-2.1.html#execution",
                               "root node", NULL, CUBE_METRIC_EXCLUSIVE );


    met_tau_atomic = cube_def_met( cube, "Counter10", "Uniq_name10", "TAU_ATOMIC", "sec", "",
                                   "@mirror@patterns-2.1.html#execution",
                                   "root node", NULL, CUBE_METRIC_EXCLUSIVE );

    met_min_double = cube_def_met( cube, "Counter11", "Uniq_name11", "MINDOUBLE", "sec", "",
                                   "@mirror@patterns-2.1.html#execution",
                                   "root node", NULL, CUBE_METRIC_EXCLUSIVE );

    met_max_double = cube_def_met( cube, "Counter12", "Uniq_name12", "MAXDOUBLE", "sec", "",
                                   "@mirror@patterns-2.1.html#execution",
                                   "root node", NULL, CUBE_METRIC_EXCLUSIVE );

    met_rate = cube_def_met( cube, "Counter13", "Uniq_name13", "RATE", "op/sec", "",
                             "@mirror@patterns-2.1.html#execution",
                             "root node", NULL, CUBE_METRIC_EXCLUSIVE );

    met_scale_func = cube_def_met( cube, "Counter14", "Uniq_name14", "SCALE_FUNC", "sec", "",
                                   "@mirror@patterns-2.1.html#execution",
                                   "root node", NULL, CUBE_METRIC_EXCLUSIVE );
    /* define call tree */
    char*        mod = "/ICL/CUBE/example.c";
    cube_region* regn0, * regn1, * regn2, * regn3;
    regn0 = cube_def_region( cube, "main", "main", "mpi", "barrier", 21, 100, "", "1st level", mod );
    regn1 = cube_def_region( cube, "<<init>>foo", "<<init>>foo", "mpi", "p2p", 1, 10, "", "2nd level", mod );
    regn2 = cube_def_region( cube, "<<loop>>bar", "<<loop>>bar", "mpi", "collective", 11, 20, "", "2nd level", mod );
    regn3 = cube_def_region( cube, "<<loop>>tau_cnode", "<<loop>>tau_cnode", "pthread", "collective", 142, 10, "aaa", "4nd level", mod );

    cube_cnode* cnode0, * cnode1, * cnode2, * cnode3, * cnode4, * cnode5, * cnode6, * cnode7, * cnode8, * cnode9, * cnode10, * cnode11, * cnode12, * cnode13;
    cnode0  = cube_def_cnode_cs( cube, regn0, mod, 21, NULL );
    cnode1  = cube_def_cnode_cs( cube, regn1, mod, 60, cnode0 );
    cnode2  = cube_def_cnode_cs( cube, regn2, mod, 80, cnode0 );
    cnode3  = cube_def_cnode_cs( cube, regn1, mod, 360, cnode1 );
    cnode4  = cube_def_cnode_cs( cube, regn2, mod, 80, cnode2 );
    cnode5  = cube_def_cnode_cs( cube, regn1, mod, 123, NULL );
    cnode6  = cube_def_cnode_cs( cube, regn2, mod, 180, cnode1 );
    cnode7  = cube_def_cnode_cs( cube, regn1, mod, 260, cnode5 );
    cnode8  = cube_def_cnode_cs( cube, regn2, mod, 8, cnode5 );
    cnode9  = cube_def_cnode_cs( cube, regn3, mod, 2, cnode7 );
    cnode10 = cube_def_cnode_cs( cube, regn2, mod, 32, cnode8 );
    cnode11 = cube_def_cnode_cs( cube, regn1, mod, 52, cnode8 );
    cnode12 = cube_def_cnode_cs( cube, regn2, mod, 2352, cnode7 );
    cnode13 = cube_def_cnode_cs( cube, regn1, mod, 352, cnode7 );



    cube_cnode_add_numeric_parameter( cnode0, "Phase", 1 );
    cube_cnode_add_numeric_parameter( cnode0, "Phase", 2 );
    cube_cnode_add_string_parameter( cnode0, "Iteration", "Initialization" );
    cube_cnode_add_string_parameter( cnode2, "Etappe", "Finish" );


    /* define location tree */
    cube_machine* mach  = cube_def_mach( cube, "MSC<<juelich>>", "" );
    cube_node*    node  = cube_def_node( cube, "Athena<<juropa>>", mach );
    cube_process* proc0 = cube_def_proc( cube, "Process 0<<master>>", 0, node );
    cube_process* proc1 = cube_def_proc( cube, "Process 1<<worker>>", 1, node );

    cube_thread* thrd0 = cube_def_thrd( cube, "Thread 0<<iterator>>", 0, proc0 );
    cube_thread* thrd1 = cube_def_thrd( cube, "Thread 1<<solver>>", 1, proc0 );
    cube_thread* thrd2 = cube_def_thrd( cube, "Thread 2<<iterator>>", 2, proc0 );
    cube_thread* thrd3 = cube_def_thrd( cube, "Thread 3<<solver>>", 3, proc0 );
    cube_thread* thrd4 = cube_def_thrd( cube, "Thread 4<<iterator>>", 4, proc1 );
    cube_thread* thrd5 = cube_def_thrd( cube, "Thread 5<<solver>>", 5, proc1 );
    cube_thread* thrd6 = cube_def_thrd( cube, "Thread 6<<iterator>>", 6, proc1 );
    cube_thread* thrd7 = cube_def_thrd( cube, "Thread 7<<solver>>", 7, proc1 );

    /* define first topology */
    long            dimv0[ NDIMS ]    = { 5, 5 };
    int             periodv0[ NDIMS ] = { 1, 0 };
    cube_cartesian* cart0             = cube_def_cart( cube, NDIMS, dimv0, periodv0 );
    cube_cart_set_name( cart0, "My Topology 1" );

    long coordv[ NDIMS ] = { 0, 0 };
    cube_def_coords( cube, cart0, thrd1, coordv );

    /* define second topology */
    long            dimv1[ NDIMS ]    = { 3, 3 };
    int             periodv1[ NDIMS ] = { 1, 0 };
    cube_cartesian* cart1             = cube_def_cart( cube, NDIMS, dimv1, periodv1 );
    cube_cart_set_name( cart1, "2323 Topoly<>&SDFGs\"\"ogy 3" );

    long coordv0[ NDIMS ] = { 0, 1 };
    long coordv1[ NDIMS ] = { 1, 0 };
    cube_def_coords( cube, cart1, thrd0, coordv0 );
    cube_def_coords( cube, cart1, thrd1, coordv1 );





    /* define second topology*/
    long            dimv2[ 4 ]    = { 3, 3, 3, 3 };
    int             periodv2[ 4 ] = { 1, 0, 0, 0 };
    cube_cartesian* cart2         = cube_def_cart( cube, 4, dimv2, periodv2 );

    long coordv20[ 4 ] = { 0, 1, 0, 0 };
    long coordv21[ 4 ] = { 1, 0, 0, 0 };
    cube_def_coords( cube, cart2, thrd0, coordv20 );
    cube_def_coords( cube, cart2, thrd1, coordv21 );
    cube_cart_set_name( cart2, "Second" );


    /* define third topology*/
    long            dimv3[ 14 ]    = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
    int             periodv3[ 14 ] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    cube_cartesian* cart3          = cube_def_cart( cube, 14, dimv3, periodv3 );
    long            coordv32[ 14 ] = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 };
    long            coordv33[ 14 ] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    cube_def_coords( cube, cart3, thrd0, coordv32 );
    cube_def_coords( cube, cart3, thrd1, coordv33 );
    cube_cart_set_name( cart3, "Third" );





    cube_set_statistic_name( cube, "mystat" );

//    cube_set_metrics_title(cube, "salghafjh$%&$%&/SDFX");
//    cube_set_calltree_title(cube, "caltreesalghafjh$%&$%&/SDFX");
//    cube_set_systemtree_title(cube, "systeMsalghafjh$%&$%&/SDFX");

//    cube_enable_flat_tree(cube, CUBE_FALSE);


    /* generate definitions */
//    cube_write_def(cube);

    /* set severities */
    /* unset severities default to zero */


    cube_type_uint8      sev_uint8[ 8 ];
    cube_type_int8       sev_int8[ 8 ];
    cube_type_uint16     sev_uint16[ 8 ];
    cube_type_int16      sev_int16[ 8 ];
    cube_type_uint32     sev_uint32[ 8 ];
    cube_type_int32      sev_int32[ 8 ];
    cube_type_uint64     sev_uint64[ 8 ];
    cube_type_int64      sev_int64[ 8 ];
    cube_type_double     sev_double[ 8 ];
    cube_type_double     sev_min_double[ 8 ];
    cube_type_double     sev_max_double[ 8 ];
    cube_type_tau_atomic sev_tau_atomic[ 8 ];
    cube_type_scale_func sev_scale_func[ 8 ];
    cube_type_rate       sev_rate[ 8 ];



    sev_uint8[ 0 ].value = 123;
    sev_uint8[ 1 ].value = 23;
    sev_uint8[ 2 ].value = 3;
    sev_uint8[ 3 ].value = 13;
    sev_uint8[ 4 ].value = 203;
    sev_uint8[ 5 ].value = 33;
    sev_uint8[ 6 ].value = 253;
    sev_uint8[ 7 ].value = 3;


    sev_int8[ 0 ].value = -123;
    sev_int8[ 1 ].value = 13;
    sev_int8[ 2 ].value = -3;
    sev_int8[ 3 ].value = -13;
    sev_int8[ 4 ].value = 133;
    sev_int8[ 5 ].value = -33;
    sev_int8[ 6 ].value = -53;
    sev_int8[ 7 ].value = 73;

    sev_uint16[ 0 ].value = 3123;
    sev_uint16[ 1 ].value = 413;
    sev_uint16[ 2 ].value = 5433;
    sev_uint16[ 3 ].value = 3;
    sev_uint16[ 4 ].value = 0;
    sev_uint16[ 5 ].value = 12453;
    sev_uint16[ 6 ].value = 53;
    sev_uint16[ 7 ].value = 33;

    sev_int16[ 0 ].value = -2123;
    sev_int16[ 1 ].value = 413;
    sev_int16[ 2 ].value = -6323;
    sev_int16[ 3 ].value = -613;
    sev_int16[ 4 ].value = 3133;
    sev_int16[ 5 ].value = -1633;
    sev_int16[ 6 ].value = -253;
    sev_int16[ 7 ].value = 773;


    sev_uint32[ 0 ].value = 31123;
    sev_uint32[ 1 ].value = 4113;
    sev_uint32[ 2 ].value = 51433;
    sev_uint32[ 3 ].value = 31;
    sev_uint32[ 4 ].value = 01;
    sev_uint32[ 5 ].value = 22453;
    sev_uint32[ 6 ].value = 153;
    sev_uint32[ 7 ].value = 3113;

    sev_int32[ 0 ].value = -21123;
    sev_int32[ 1 ].value = 413;
    sev_int32[ 2 ].value = -16323;
    sev_int32[ 3 ].value = -13;
    sev_int32[ 4 ].value = 31133;
    sev_int32[ 5 ].value = -11633;
    sev_int32[ 6 ].value = -11253;
    sev_int32[ 7 ].value = 11773;

    sev_uint64[ 0 ].value = 331123;
    sev_uint64[ 1 ].value = 46113;
    sev_uint64[ 2 ].value = 513433;
    sev_uint64[ 3 ].value = 371;
    sev_uint64[ 4 ].value = 013;
    sev_uint64[ 5 ].value = 224753;
    sev_uint64[ 6 ].value = 1533;
    sev_uint64[ 7 ].value = 31173;

    sev_int64[ 0 ].value = -231123;
    sev_int64[ 1 ].value = 4173;
    sev_int64[ 2 ].value = -163323;
    sev_int64[ 3 ].value = -173;
    sev_int64[ 4 ].value = 3114333;
    sev_int64[ 5 ].value = -116733;
    sev_int64[ 6 ].value = -112353;
    sev_int64[ 7 ].value = 117773;


    sev_double[ 0 ].value = -2.31123;
    sev_double[ 1 ].value = 417.3;
    sev_double[ 2 ].value = -16.3323;
    sev_double[ 3 ].value = -17.3;
    sev_double[ 4 ].value = 3114.333;
    sev_double[ 5 ].value = -11.6733;
    sev_double[ 6 ].value = -1.12353;
    sev_double[ 7 ].value = 0.117773;

    sev_min_double[ 0 ].value = -52.31123;
    sev_min_double[ 1 ].value = 4217.3;
    sev_min_double[ 2 ].value = 516.3323;
    sev_min_double[ 3 ].value = -137.3;
    sev_min_double[ 4 ].value = -14.333;
    sev_min_double[ 5 ].value = -3.6733;
    sev_min_double[ 6 ].value = -.353;
    sev_min_double[ 7 ].value = 0.117773;

    sev_max_double[ 0 ].value = .1123;
    sev_max_double[ 1 ].value = -417.3;
    sev_max_double[ 2 ].value = -14.23;
    sev_max_double[ 3 ].value = 17.3;
    sev_max_double[ 4 ].value = -214.333;
    sev_max_double[ 5 ].value = -211.6733;
    sev_max_double[ 6 ].value = -13.12353;
    sev_max_double[ 7 ].value = -65.117773;







    sev_tau_atomic[ 0 ].N    = 1;
    sev_tau_atomic[ 0 ].Min  = 2;
    sev_tau_atomic[ 0 ].Max  = 2;
    sev_tau_atomic[ 0 ].Sum  = 2;
    sev_tau_atomic[ 0 ].Sum2 = 4;

    sev_tau_atomic[ 1 ].N    = 2;
    sev_tau_atomic[ 1 ].Min  = 2;
    sev_tau_atomic[ 1 ].Max  = 5;
    sev_tau_atomic[ 1 ].Sum  = 7;
    sev_tau_atomic[ 1 ].Sum2 = 29;

    sev_tau_atomic[ 2 ].N    = 3;
    sev_tau_atomic[ 2 ].Min  = 1;
    sev_tau_atomic[ 2 ].Max  = 5;
    sev_tau_atomic[ 2 ].Sum  = 9;
    sev_tau_atomic[ 2 ].Sum2 = 35;


/* Empty value... min and max value should have be "zero" elements in monoid (R, min/max)*/
    sev_tau_atomic[ 3 ].N    = 0;
    sev_tau_atomic[ 3 ].Min  = 99e+99;
    sev_tau_atomic[ 3 ].Max  = -99e+99;
    sev_tau_atomic[ 3 ].Sum  = 0;
    sev_tau_atomic[ 3 ].Sum2 = 0;

    sev_tau_atomic[ 4 ].N    = 1;
    sev_tau_atomic[ 4 ].Min  = -12;
    sev_tau_atomic[ 4 ].Max  = -12;
    sev_tau_atomic[ 4 ].Sum  = -12;
    sev_tau_atomic[ 4 ].Sum2 = 144;

    sev_tau_atomic[ 5 ].N    = 2;
    sev_tau_atomic[ 5 ].Min  = -10;
    sev_tau_atomic[ 5 ].Max  = 10;
    sev_tau_atomic[ 5 ].Sum  = 0;
    sev_tau_atomic[ 5 ].Sum2 = 200;

    sev_tau_atomic[ 6 ].N    = 3;
    sev_tau_atomic[ 6 ].Min  = -5;
    sev_tau_atomic[ 6 ].Max  = 5;
    sev_tau_atomic[ 6 ].Sum  = 1;
    sev_tau_atomic[ 6 ].Sum2 = 51;

    sev_tau_atomic[ 7 ].N    = 1;
    sev_tau_atomic[ 7 ].Min  = 100;
    sev_tau_atomic[ 7 ].Max  = 100;
    sev_tau_atomic[ 7 ].Sum  = 100;
    sev_tau_atomic[ 7 ].Sum2 = 10000;

    unsigned i = 0;
    for ( i = 0; i < 8; i++ )
    {
        unsigned j = 0;
        for ( j = 0; j < 27; j++ )
        {
            sev_scale_func[ i ].value[ j ] = i + j;
        }
    }





    sev_rate[ 0 ].numerator   = 1;
    sev_rate[ 0 ].denominator = 2;

    sev_rate[ 1 ].numerator   = 2;
    sev_rate[ 1 ].denominator = 2;

    sev_rate[ 2 ].numerator   = 3;
    sev_rate[ 2 ].denominator = 1;


/* Empty value... min and max value should have be "zero" elements in monoid (R, min/max)*/
    sev_rate[ 3 ].numerator   = 0;
    sev_rate[ 3 ].denominator = 9.456;

    sev_rate[ 4 ].numerator   = 1;
    sev_rate[ 4 ].denominator = -12;

    sev_rate[ 5 ].numerator   = 2;
    sev_rate[ 5 ].denominator = -10;

    sev_rate[ 6 ].numerator   = 3;
    sev_rate[ 6 ].denominator = -5;

    sev_rate[ 7 ].numerator   = 1;
    sev_rate[ 7 ].denominator = 100;




    printf( "TO UINT8\n" );
    cube_write_sev_row_of_cube_type_uint8( cube, met_uint8, cnode0, sev_uint8 );
    cube_write_sev_row_of_cube_type_int8( cube, met_uint8, cnode1, sev_int8 );
    cube_write_sev_row_of_cube_type_uint16( cube, met_uint8, cnode2, sev_uint16 );
    cube_write_sev_row_of_cube_type_int16( cube, met_uint8, cnode3, sev_int16 );
    cube_write_sev_row_of_cube_type_uint32( cube, met_uint8, cnode4, sev_uint32 );
    cube_write_sev_row_of_cube_type_int32( cube, met_uint8, cnode5, sev_int32 );
    cube_write_sev_row_of_cube_type_uint64( cube, met_uint8, cnode6, sev_uint64 );
    cube_write_sev_row_of_cube_type_int64( cube, met_uint8, cnode7, sev_int64 );
    cube_write_sev_row_of_cube_type_double( cube, met_uint8, cnode8, sev_double );
    cube_write_sev_row_of_cube_type_tau_atomic( cube, met_uint8, cnode9, sev_tau_atomic );
    cube_write_sev_row_of_cube_type_double( cube, met_uint8, cnode9, sev_min_double );
    cube_write_sev_row_of_cube_type_double( cube, met_uint8, cnode10, sev_max_double );
    cube_write_sev_row_of_cube_type_rate( cube, met_uint8, cnode12, sev_rate );
    cube_write_sev_row_of_cube_type_scale_func( cube, met_uint8, cnode13, sev_scale_func );

    printf( "TO INT8\n" );

    cube_write_sev_row_of_cube_type_uint8( cube, met_int8, cnode0, sev_uint8 );
    cube_write_sev_row_of_cube_type_int8( cube, met_int8, cnode1, sev_int8 );
    cube_write_sev_row_of_cube_type_uint16( cube, met_int8, cnode2, sev_uint16 );
    cube_write_sev_row_of_cube_type_int16( cube, met_int8, cnode3, sev_int16 );
    cube_write_sev_row_of_cube_type_uint32( cube, met_int8, cnode4, sev_uint32 );
    cube_write_sev_row_of_cube_type_int32( cube, met_int8, cnode5, sev_int32 );
    cube_write_sev_row_of_cube_type_uint64( cube, met_int8, cnode6, sev_uint64 );
    cube_write_sev_row_of_cube_type_int64( cube, met_int8, cnode7, sev_int64 );
    cube_write_sev_row_of_cube_type_double( cube, met_int8, cnode8, sev_double );
    cube_write_sev_row_of_cube_type_tau_atomic( cube, met_int8, cnode9, sev_tau_atomic );
    cube_write_sev_row_of_cube_type_double( cube, met_int8, cnode10, sev_min_double );
    cube_write_sev_row_of_cube_type_double( cube, met_int8, cnode11, sev_max_double );
    cube_write_sev_row_of_cube_type_rate( cube, met_int8, cnode12, sev_rate );
    cube_write_sev_row_of_cube_type_scale_func( cube, met_int8, cnode13, sev_scale_func );
    printf( "TO UINT16\n" );

    cube_write_sev_row_of_cube_type_uint8( cube, met_uint16, cnode0, sev_uint8 );
    cube_write_sev_row_of_cube_type_int8( cube, met_uint16, cnode1, sev_int8 );
    cube_write_sev_row_of_cube_type_uint16( cube, met_uint16, cnode2, sev_uint16 );
    cube_write_sev_row_of_cube_type_int16( cube, met_uint16, cnode3, sev_int16 );
    cube_write_sev_row_of_cube_type_uint32( cube, met_uint16, cnode4, sev_uint32 );
    cube_write_sev_row_of_cube_type_int32( cube, met_uint16, cnode5, sev_int32 );
    cube_write_sev_row_of_cube_type_uint64( cube, met_uint16, cnode6, sev_uint64 );
    cube_write_sev_row_of_cube_type_int64( cube, met_uint16, cnode7, sev_int64 );
    cube_write_sev_row_of_cube_type_double( cube, met_uint16, cnode8, sev_double );
    cube_write_sev_row_of_cube_type_tau_atomic( cube, met_uint16, cnode9, sev_tau_atomic );
    cube_write_sev_row_of_cube_type_double( cube, met_uint16, cnode10, sev_min_double );
    cube_write_sev_row_of_cube_type_double( cube, met_uint16, cnode11, sev_max_double );
    cube_write_sev_row_of_cube_type_rate( cube, met_uint16, cnode12, sev_rate );
    cube_write_sev_row_of_cube_type_scale_func( cube, met_uint16, cnode13, sev_scale_func );
    printf( "TO INT16\n" );

    cube_write_sev_row_of_cube_type_uint8( cube, met_int16, cnode0, sev_uint8 );
    cube_write_sev_row_of_cube_type_int8( cube, met_int16, cnode1, sev_int8 );
    cube_write_sev_row_of_cube_type_uint16( cube, met_int16, cnode2, sev_uint16 );
    cube_write_sev_row_of_cube_type_int16( cube, met_int16, cnode3, sev_int16 );
    cube_write_sev_row_of_cube_type_uint32( cube, met_int16, cnode4, sev_uint32 );
    cube_write_sev_row_of_cube_type_int32( cube, met_int16, cnode5, sev_int32 );
    cube_write_sev_row_of_cube_type_uint64( cube, met_int16, cnode6, sev_uint64 );
    cube_write_sev_row_of_cube_type_int64( cube, met_int16, cnode7, sev_int64 );
    cube_write_sev_row_of_cube_type_double( cube, met_int16, cnode8, sev_double );
    cube_write_sev_row_of_cube_type_tau_atomic( cube, met_int16, cnode9, sev_tau_atomic );
    cube_write_sev_row_of_cube_type_double( cube, met_int16, cnode10, sev_min_double );
    cube_write_sev_row_of_cube_type_double( cube, met_int16, cnode11, sev_max_double );
    cube_write_sev_row_of_cube_type_rate( cube, met_int16, cnode12, sev_rate );
    cube_write_sev_row_of_cube_type_scale_func( cube, met_int16, cnode13, sev_scale_func );

    printf( "TO UINT32\n" );

    cube_write_sev_row_of_cube_type_uint8( cube, met_uint32, cnode0, sev_uint8 );
    cube_write_sev_row_of_cube_type_int8( cube, met_uint32, cnode1, sev_int8 );
    cube_write_sev_row_of_cube_type_uint16( cube, met_uint32, cnode2, sev_uint16 );
    cube_write_sev_row_of_cube_type_int16( cube, met_uint32, cnode3, sev_int16 );
    cube_write_sev_row_of_cube_type_uint32( cube, met_uint32, cnode4, sev_uint32 );
    cube_write_sev_row_of_cube_type_int32( cube, met_uint32, cnode5, sev_int32 );
    cube_write_sev_row_of_cube_type_uint64( cube, met_uint32, cnode6, sev_uint64 );
    cube_write_sev_row_of_cube_type_int64( cube, met_uint32, cnode7, sev_int64 );
    cube_write_sev_row_of_cube_type_double( cube, met_uint32, cnode8, sev_double );
    cube_write_sev_row_of_cube_type_tau_atomic( cube, met_uint32, cnode9, sev_tau_atomic );
    cube_write_sev_row_of_cube_type_double( cube, met_uint32, cnode10, sev_min_double );
    cube_write_sev_row_of_cube_type_double( cube, met_uint32, cnode11, sev_max_double );
    cube_write_sev_row_of_cube_type_rate( cube, met_uint32, cnode12, sev_rate );
    cube_write_sev_row_of_cube_type_scale_func( cube, met_uint32, cnode13, sev_scale_func );

    printf( "TO INT32\n" );

    cube_write_sev_row_of_cube_type_uint8( cube, met_int32, cnode0, sev_uint8 );
    cube_write_sev_row_of_cube_type_int8( cube, met_int32, cnode1, sev_int8 );
    cube_write_sev_row_of_cube_type_uint16( cube, met_int32, cnode2, sev_uint16 );
    cube_write_sev_row_of_cube_type_int16( cube, met_int32, cnode3, sev_int16 );
    cube_write_sev_row_of_cube_type_uint32( cube, met_int32, cnode4, sev_uint32 );
    cube_write_sev_row_of_cube_type_int32( cube, met_int32, cnode5, sev_int32 );
    cube_write_sev_row_of_cube_type_uint64( cube, met_int32, cnode6, sev_uint64 );
    cube_write_sev_row_of_cube_type_int64( cube, met_int32, cnode7, sev_int64 );
    cube_write_sev_row_of_cube_type_double( cube, met_int32, cnode8, sev_double );
    cube_write_sev_row_of_cube_type_tau_atomic( cube, met_int32, cnode9, sev_tau_atomic );
    cube_write_sev_row_of_cube_type_double( cube, met_int32, cnode10, sev_min_double );
    cube_write_sev_row_of_cube_type_double( cube, met_int32, cnode11, sev_max_double );
    cube_write_sev_row_of_cube_type_rate( cube, met_int32, cnode12, sev_rate );
    cube_write_sev_row_of_cube_type_scale_func( cube, met_int32, cnode13, sev_scale_func );


    printf( "TO UINT64\n" );

    cube_write_sev_row_of_cube_type_uint8( cube, met_uint64, cnode0, sev_uint8 );
    cube_write_sev_row_of_cube_type_int8( cube, met_uint64, cnode1, sev_int8 );
    cube_write_sev_row_of_cube_type_uint16( cube, met_uint64, cnode2, sev_uint16 );
    cube_write_sev_row_of_cube_type_int16( cube, met_uint64, cnode3, sev_int16 );
    cube_write_sev_row_of_cube_type_uint32( cube, met_uint64, cnode4, sev_uint32 );
    cube_write_sev_row_of_cube_type_int32( cube, met_uint64, cnode5, sev_int32 );
    cube_write_sev_row_of_cube_type_uint64( cube, met_uint64, cnode6, sev_uint64 );
    cube_write_sev_row_of_cube_type_int64( cube, met_uint64, cnode7, sev_int64 );
    cube_write_sev_row_of_cube_type_double( cube, met_uint64, cnode8, sev_double );
    cube_write_sev_row_of_cube_type_tau_atomic( cube, met_uint64, cnode9, sev_tau_atomic );
    cube_write_sev_row_of_cube_type_double( cube, met_uint64, cnode10, sev_min_double );
    cube_write_sev_row_of_cube_type_double( cube, met_uint64, cnode11, sev_max_double );
    cube_write_sev_row_of_cube_type_rate( cube, met_uint64, cnode12, sev_rate );
    cube_write_sev_row_of_cube_type_scale_func( cube, met_uint64, cnode13, sev_scale_func );

    printf( "TO INT64\n" );

    cube_write_sev_row_of_cube_type_uint8( cube, met_int64, cnode0, sev_uint8 );
    cube_write_sev_row_of_cube_type_int8( cube, met_int64, cnode1, sev_int8 );
    cube_write_sev_row_of_cube_type_uint16( cube, met_int64, cnode2, sev_uint16 );
    cube_write_sev_row_of_cube_type_int16( cube, met_int64, cnode3, sev_int16 );
    cube_write_sev_row_of_cube_type_uint32( cube, met_int64, cnode4, sev_uint32 );
    cube_write_sev_row_of_cube_type_int32( cube, met_int64, cnode5, sev_int32 );
    cube_write_sev_row_of_cube_type_uint64( cube, met_int64, cnode6, sev_uint64 );
    cube_write_sev_row_of_cube_type_int64( cube, met_int64, cnode7, sev_int64 );
    cube_write_sev_row_of_cube_type_double( cube, met_int64, cnode8, sev_double );
    cube_write_sev_row_of_cube_type_tau_atomic( cube, met_int64, cnode9, sev_tau_atomic );
    cube_write_sev_row_of_cube_type_double( cube, met_int64, cnode10, sev_min_double );
    cube_write_sev_row_of_cube_type_double( cube, met_int64, cnode11, sev_max_double );
    cube_write_sev_row_of_cube_type_rate( cube, met_int64, cnode12, sev_rate );
    cube_write_sev_row_of_cube_type_scale_func( cube, met_int64, cnode13, sev_scale_func );

    printf( "TO DOUBLE\n" );

    cube_write_sev_row_of_cube_type_uint8( cube, met_double, cnode0, sev_uint8 );
    cube_write_sev_row_of_cube_type_int8( cube, met_double, cnode1, sev_int8 );
    cube_write_sev_row_of_cube_type_uint16( cube, met_double, cnode2, sev_uint16 );
    cube_write_sev_row_of_cube_type_int16( cube, met_double, cnode3, sev_int16 );
    cube_write_sev_row_of_cube_type_uint32( cube, met_double, cnode4, sev_uint32 );
    cube_write_sev_row_of_cube_type_int32( cube, met_double, cnode5, sev_int32 );
    cube_write_sev_row_of_cube_type_uint64( cube, met_double, cnode6, sev_uint64 );
    cube_write_sev_row_of_cube_type_int64( cube, met_double, cnode7, sev_int64 );
    cube_write_sev_row_of_cube_type_double( cube, met_double, cnode8, sev_double );
    cube_write_sev_row_of_cube_type_tau_atomic( cube, met_double, cnode9, sev_tau_atomic );
    cube_write_sev_row_of_cube_type_double( cube, met_double, cnode9, sev_min_double );
    cube_write_sev_row_of_cube_type_double( cube, met_double, cnode10, sev_max_double );
    cube_write_sev_row_of_cube_type_rate( cube, met_double, cnode11, sev_rate );
    cube_write_sev_row_of_cube_type_scale_func( cube, met_double, cnode13, sev_scale_func );

    printf( "TO MIN_DOUBLE\n" );

    cube_write_sev_row_of_cube_type_uint8( cube, met_min_double, cnode0, sev_uint8 );
    cube_write_sev_row_of_cube_type_int8( cube, met_min_double, cnode1, sev_int8 );
    cube_write_sev_row_of_cube_type_uint16( cube, met_min_double, cnode2, sev_uint16 );
    cube_write_sev_row_of_cube_type_int16( cube, met_min_double, cnode3, sev_int16 );
    cube_write_sev_row_of_cube_type_uint32( cube, met_min_double, cnode4, sev_uint32 );
    cube_write_sev_row_of_cube_type_int32( cube, met_min_double, cnode5, sev_int32 );
    cube_write_sev_row_of_cube_type_uint64( cube, met_min_double, cnode6, sev_uint64 );
    cube_write_sev_row_of_cube_type_int64( cube, met_min_double, cnode7, sev_int64 );
    cube_write_sev_row_of_cube_type_double( cube, met_min_double, cnode8, sev_double );
    cube_write_sev_row_of_cube_type_tau_atomic( cube, met_min_double, cnode9, sev_tau_atomic );
    cube_write_sev_row_of_cube_type_double( cube, met_min_double, cnode10, sev_min_double );
    cube_write_sev_row_of_cube_type_double( cube, met_min_double, cnode11, sev_max_double );
    cube_write_sev_row_of_cube_type_rate( cube, met_min_double, cnode12, sev_rate );
    cube_write_sev_row_of_cube_type_scale_func( cube, met_min_double, cnode13, sev_scale_func );

    printf( "TO MAX_DOUBLE\n" );

    cube_write_sev_row_of_cube_type_uint8( cube, met_max_double, cnode0, sev_uint8 );
    cube_write_sev_row_of_cube_type_int8( cube, met_max_double, cnode1, sev_int8 );
    cube_write_sev_row_of_cube_type_uint16( cube, met_max_double, cnode2, sev_uint16 );
    cube_write_sev_row_of_cube_type_int16( cube, met_max_double, cnode3, sev_int16 );
    cube_write_sev_row_of_cube_type_uint32( cube, met_max_double, cnode4, sev_uint32 );
    cube_write_sev_row_of_cube_type_int32( cube, met_max_double, cnode5, sev_int32 );
    cube_write_sev_row_of_cube_type_uint64( cube, met_max_double, cnode6, sev_uint64 );
    cube_write_sev_row_of_cube_type_int64( cube, met_max_double, cnode7, sev_int64 );
    cube_write_sev_row_of_cube_type_double( cube, met_max_double, cnode8, sev_double );
    cube_write_sev_row_of_cube_type_tau_atomic( cube, met_max_double, cnode9, sev_tau_atomic );
    cube_write_sev_row_of_cube_type_double( cube, met_max_double, cnode10, sev_min_double );
    cube_write_sev_row_of_cube_type_double( cube, met_max_double, cnode11, sev_max_double );
    cube_write_sev_row_of_cube_type_rate( cube, met_max_double, cnode12, sev_rate );
    cube_write_sev_row_of_cube_type_scale_func( cube, met_max_double, cnode13, sev_scale_func );


    printf( "TO TAU ATOMIC\n" );

    cube_write_sev_row_of_cube_type_uint8( cube, met_tau_atomic, cnode0, sev_uint8 );
    cube_write_sev_row_of_cube_type_int8( cube, met_tau_atomic, cnode1, sev_int8 );
    cube_write_sev_row_of_cube_type_uint16( cube, met_tau_atomic, cnode2, sev_uint16 );
    cube_write_sev_row_of_cube_type_int16( cube, met_tau_atomic, cnode3, sev_int16 );
    cube_write_sev_row_of_cube_type_uint32( cube, met_tau_atomic, cnode4, sev_uint32 );
    cube_write_sev_row_of_cube_type_int32( cube, met_tau_atomic, cnode5, sev_int32 );
    cube_write_sev_row_of_cube_type_uint64( cube, met_tau_atomic, cnode6, sev_uint64 );
    cube_write_sev_row_of_cube_type_int64( cube, met_tau_atomic, cnode7, sev_int64 );
    cube_write_sev_row_of_cube_type_double( cube, met_tau_atomic, cnode8, sev_double );
    cube_write_sev_row_of_cube_type_tau_atomic( cube, met_tau_atomic, cnode9, sev_tau_atomic );
    cube_write_sev_row_of_cube_type_double( cube, met_tau_atomic, cnode10, sev_min_double );
    cube_write_sev_row_of_cube_type_double( cube, met_tau_atomic, cnode11, sev_max_double );
    cube_write_sev_row_of_cube_type_rate( cube, met_tau_atomic, cnode12, sev_rate );
    cube_write_sev_row_of_cube_type_scale_func( cube, met_tau_atomic, cnode13, sev_scale_func );

    printf( "TO RATE\n" );

//     cube_write_sev_row_of_cube_type_uint8(cube, met_rate, cnode0, sev_uint8);
//     cube_write_sev_row_of_cube_type_int8(cube, met_rate, cnode1, sev_int8);
//     cube_write_sev_row_of_cube_type_uint16(cube, met_rate, cnode2, sev_uint16);
//     cube_write_sev_row_of_cube_type_int16(cube, met_rate, cnode3, sev_int16);
//     cube_write_sev_row_of_cube_type_uint32(cube, met_rate, cnode4, sev_uint32);
//     cube_write_sev_row_of_cube_type_int32(cube, met_rate, cnode5, sev_int32);
//     cube_write_sev_row_of_cube_type_uint64(cube, met_rate, cnode6, sev_uint64);
//     cube_write_sev_row_of_cube_type_int64(cube, met_rate, cnode7, sev_int64);
//     cube_write_sev_row_of_cube_type_double(cube, met_rate, cnode8, sev_double);
//     cube_write_sev_row_of_cube_type_tau_atomic(cube, met_rate, cnode9, sev_tau_atomic);
//     cube_write_sev_row_of_cube_type_double(cube, met_rate, cnode10, sev_min_double);
//     cube_write_sev_row_of_cube_type_double(cube, met_rate, cnode11, sev_max_double);
    cube_write_sev_row_of_cube_type_rate( cube, met_rate, cnode12, sev_rate );
    printf( "TO SCALE_FUNC\n" );

//     cube_write_sev_row_of_cube_type_uint8(cube, met_rate, cnode0, sev_uint8);
//     cube_write_sev_row_of_cube_type_int8(cube, met_rate, cnode1, sev_int8);
//     cube_write_sev_row_of_cube_type_uint16(cube, met_rate, cnode2, sev_uint16);
//     cube_write_sev_row_of_cube_type_int16(cube, met_rate, cnode3, sev_int16);
//     cube_write_sev_row_of_cube_type_uint32(cube, met_rate, cnode4, sev_uint32);
//     cube_write_sev_row_of_cube_type_int32(cube, met_rate, cnode5, sev_int32);
//     cube_write_sev_row_of_cube_type_uint64(cube, met_rate, cnode6, sev_uint64);
//     cube_write_sev_row_of_cube_type_int64(cube, met_rate, cnode7, sev_int64);
//     cube_write_sev_row_of_cube_type_double(cube, met_rate, cnode8, sev_double);
//     cube_write_sev_row_of_cube_type_tau_atomic(cube, met_rate, cnode9, sev_tau_atomic);
//     cube_write_sev_row_of_cube_type_double(cube, met_rate, cnode10, sev_min_double);
//     cube_write_sev_row_of_cube_type_double(cube, met_rate, cnode11, sev_max_double);
//     cube_write_sev_row_of_cube_type_rate( cube, met_rate, cnode12, sev_rate );
    cube_write_sev_row_of_cube_type_scale_func( cube, met_scale_func,  cnode13, sev_scale_func );
    printf( "DONE\n" );
    /* generate severities */
//    cube_write_sev_matrix(cube, fp);

//    if (fclose(fp)) {
//        fprintf(stderr,"Error closing file %s: %s\n", cubefile, strerror(errno));
//    } else
//        printf("Test file %s complete.\n", cubefile);

//    cube_finalize(cube);
    cube_free( cube );
    return 0;
}
