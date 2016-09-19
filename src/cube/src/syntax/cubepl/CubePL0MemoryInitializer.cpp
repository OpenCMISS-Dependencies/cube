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


#ifndef __CUBEPL0_MEMORY_INITIALIZER_CPP
#define __CUBEPL0_MEMORY_INITIALIZER_CPP 0

#include <vector>
#include <stack>
#include <iostream>
#include <float.h>
#include <cmath>
#include <map>

using namespace std;

#include "CubePL0MemoryInitializer.h"
#include "Cube.h"
#include "CubeMetric.h"
#include "CubeCnode.h"
#include "CubeRegion.h"
#include "CubeSysres.h"


using namespace cube;



void
CubePL0MemoryInitializer:: memory_init()
{
    cubepl0_memory_manager->init();
}

void
CubePL0MemoryInitializer:: memory_new()
{
    cubepl0_memory_manager->new_page();
}

void
CubePL0MemoryInitializer::memory_cleanup()
{
    cubepl0_memory_manager->throw_page();
}


void
CubePL0MemoryInitializer:: memory_setup( cube::Cube* cube )
{
    const std::vector<std::string>&           mirror_urlv     = cube->get_mirrors();
    const std::vector<cube::Metric*>&         metv            = cube->get_metv();
    const std::vector<cube::Metric*>&         root_metv       = cube->get_root_metv();
    const std::vector<cube::Region*>&         regv            = cube->get_regv();
    const std::vector<cube::Cnode*>&          cnodev          = cube->get_cnodev();
    const std::vector<cube::Cnode*>&          root_cnodev     = cube->get_root_cnodev();
    const std::vector<cube::Location*>&       locationv       = cube->get_locationv();
    const std::vector<cube::LocationGroup*>&  location_groupv = cube->get_location_groupv();

    const std::string& cubename = cube->get_cubename();


    cubepl0_memory_manager->new_page();
    cubepl0_memory_manager->clear_variable( cubepl0::CALCULATION_CALLPATH_MOD );
    cubepl0_memory_manager->clear_variable( cubepl0::CALCULATION_CALLPATH_LINE );
    cubepl0_memory_manager->clear_variable( cubepl0::CALCULATION_CALLPATH_ID );
    cubepl0_memory_manager->clear_variable( cubepl0::CALCULATION_CALLPATH_NUM_CHILDREN );
    cubepl0_memory_manager->clear_variable( cubepl0::CALCULATION_REGION_NAME );
    cubepl0_memory_manager->clear_variable( cubepl0::CALCULATION_REGION_URL );
    cubepl0_memory_manager->clear_variable( cubepl0::CALCULATION_REGION_DESCRIPTION );
    cubepl0_memory_manager->clear_variable( cubepl0::CALCULATION_REGION_MOD );
    cubepl0_memory_manager->clear_variable( cubepl0::CALCULATION_REGION_BEGIN_LINE );
    cubepl0_memory_manager->clear_variable( cubepl0::CALCULATION_REGION_END_LINE );
    cubepl0_memory_manager->clear_variable( cubepl0::CALCULATION_REGION_ID );



    cubepl0_memory_manager->init();
    cubepl0_memory_manager->put( cubepl0::CUBE_NUM_MIRRORS, 0, mirror_urlv.size() );
    cubepl0_memory_manager->put( cubepl0::CUBE_NUM_METRICS, 0, metv.size() );
    cubepl0_memory_manager->put( cubepl0::CUBE_NUM_ROOT_METRICS, 0, root_metv.size() );
    cubepl0_memory_manager->put( cubepl0::CUBE_NUM_REGIONS, 0, regv.size() );
    cubepl0_memory_manager->put( cubepl0::CUBE_NUM_CALLPATHS, 0, cnodev.size() );
    cubepl0_memory_manager->put( cubepl0::CUBE_NUM_ROOT_CALLPATHS, 0, root_cnodev.size() );
    cubepl0_memory_manager->put( cubepl0::CUBE_NUM_THREADS, 0, locationv.size() );

    cubepl0_memory_manager->put( cubepl0::CUBE_FILENAME, 0, cubename );

    uint32_t num_void_procs    = cube->get_number_void_processes();
    uint32_t num_void_thrds    = cube->get_number_void_threads();
    uint32_t num_nonvoid_procs = location_groupv.size() - num_void_procs;
    uint32_t num_nonvoid_thrds = locationv.size() - num_void_thrds;

    cubepl0_memory_manager->put( cubepl0::CUBE_NUM_VOID_PROCS, 0, num_void_procs );
    cubepl0_memory_manager->put( cubepl0::CUBE_NUM_VOID_THRDS, 0, num_void_thrds );
    cubepl0_memory_manager->put( cubepl0::CUBE_NUM_NONVOID_PROCS, 0, num_nonvoid_procs );
    cubepl0_memory_manager->put( cubepl0::CUBE_NUM_NONVOID_THRDS, 0, num_nonvoid_thrds );
}

void
CubePL0MemoryInitializer::memory_setup( cube::Metric* metric )
{
    if (metric == NULL)
	return;
    cubepl0_memory_manager->put( cubepl0::CALCULATION_METRIC_UNIQ_NAME, 0,  metric->get_uniq_name() );
    cubepl0_memory_manager->put( cubepl0::CALCULATION_METRIC_DISP_NAME, 0, metric->get_disp_name() );
    cubepl0_memory_manager->put( cubepl0::CALCULATION_METRIC_URL, 0, metric->get_url() );
    cubepl0_memory_manager->put( cubepl0::CALCULATION_METRIC_DESCRIPTION, 0, metric->get_descr() );
    cubepl0_memory_manager->put( cubepl0::CALCULATION_METRIC_DTYPE, 0, metric->get_dtype() );
    cubepl0_memory_manager->put( cubepl0::CALCULATION_METRIC_UOM, 0, metric->get_uom() );
    cubepl0_memory_manager->put( cubepl0::CALCULATION_METRIC_EXPRESSION, 0, metric->get_expression() );
    cubepl0_memory_manager->put( cubepl0::CALCULATION_METRIC_ID, 0, metric->get_id() );
    cubepl0_memory_manager->put( cubepl0::CALCULATION_METRIC_NUM_CHILDREN, 0, metric->num_children() );
}


void
CubePL0MemoryInitializer:: memory_setup( cube::Cnode* cnode )
{
    cubepl0_memory_manager->push_back( cubepl0::CALCULATION_CALLPATH_MOD,  cnode->get_mod() );
    cubepl0_memory_manager->push_back( cubepl0::CALCULATION_CALLPATH_LINE,  cnode->get_line() );
    cubepl0_memory_manager->push_back( cubepl0::CALCULATION_CALLPATH_ID,  cnode->get_id() );
    cubepl0_memory_manager->push_back( cubepl0::CALCULATION_CALLPATH_NUM_CHILDREN,  cnode->num_children() );
    this->memory_setup( cnode->get_callee() );

    if ( cnode->get_parent() != NULL )
    {
        this->memory_setup( cnode->get_parent() );
    }
}


void
CubePL0MemoryInitializer:: memory_setup( cube::Region* region )
{
    cubepl0_memory_manager->push_back( cubepl0::CALCULATION_REGION_NAME,  region->get_name() );
    cubepl0_memory_manager->push_back( cubepl0::CALCULATION_REGION_URL, region->get_url() );
    cubepl0_memory_manager->push_back( cubepl0::CALCULATION_REGION_DESCRIPTION,  region->get_descr() );
    cubepl0_memory_manager->push_back( cubepl0::CALCULATION_REGION_MOD,  region->get_mod() );
    cubepl0_memory_manager->push_back( cubepl0::CALCULATION_REGION_BEGIN_LINE,  region->get_begn_ln() );
    cubepl0_memory_manager->push_back( cubepl0::CALCULATION_REGION_END_LINE,  region->get_end_ln() );
    cubepl0_memory_manager->push_back( cubepl0::CALCULATION_REGION_ID,  region->get_id() );
}

void
CubePL0MemoryInitializer::memory_setup(  cube::Sysres* sysres )
{
    cubepl0_memory_manager->put( cubepl0::CALCULATION_SYSRES_NAME, 0, sysres->get_name() );
    cubepl0_memory_manager->put( cubepl0::CALCULATION_SYSRES_ID,   0, sysres->get_id() );
    // type specification
    bool     is_void = false;
    Process* _p      = NULL;
    Thread*  _th     = NULL;
    switch ( sysres->get_kind() )
    {
        case CUBE_MACHINE:
            cubepl0_memory_manager->put( cubepl0::CALCULATION_SYSRES_KIND,   0, "MACHINE" );
            break;
        case CUBE_NODE:
            cubepl0_memory_manager->put( cubepl0::CALCULATION_SYSRES_KIND,   0, "NODE" );
            break;
        case CUBE_PROCESS:
            _p =  ( ( Process* )( sysres ) );
            cubepl0_memory_manager->put( cubepl0::CALCULATION_SYSRES_KIND,   0, "PROCESS" );
            cubepl0_memory_manager->put( cubepl0::CALCULATION_SYSRES_RANK,   0, _p->get_rank() );
            is_void = ( _p->get_name().find( "VOID" ) != string::npos );
            cubepl0_memory_manager->put( cubepl0::CALCULATION_PROCESS_VOID, 0, ( is_void ) ? 1 : 0 );
            break;
        case CUBE_THREAD:
            _th     = ( ( Thread* )( sysres ) );
            is_void = ( _th->get_parent()->get_name().find( "VOID" ) != string::npos );
            cubepl0_memory_manager->put( cubepl0::CALCULATION_PROCESS_VOID, 0, ( is_void ) ? 1 : 0 );
            is_void = ( _th->get_name().find( "VOID" ) != string::npos );
            cubepl0_memory_manager->put( cubepl0::CALCULATION_THREAD_VOID, 0, ( is_void ) ? 1 : 0 );
            cubepl0_memory_manager->put( cubepl0::CALCULATION_SYSRES_KIND,   0, "THREAD" );
            cubepl0_memory_manager->put( cubepl0::CALCULATION_SYSRES_RANK,   0, _th->get_rank() );
            break;
        case CUBE_UNKNOWN:
        default:
            cubepl0_memory_manager->put( cubepl0::CALCULATION_SYSRES_KIND,   0, "UNKNOWN" );
            break;
    }
}

#endif
