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


#ifndef __CUBEPL2_MEMORY_MANAGER_CPP
#define __CUBEPL2_MEMORY_MANAGER_CPP 0

#include <vector>
#include <iostream>
#include <sstream>
#include <float.h>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <string>
#include <algorithm>

#include "CubePL2MemoryManager.h"
#include "CubePL2MemoryInitializer.h"
#include "CubeError.h"


using namespace std;
using namespace cube;


CubePL2MemoryManager::CubePL2MemoryManager() : CubePLMemoryManager()
{
    cubepl_memory_initializer = new CubePL2MemoryInitializer( this );

    max_reserved_memory_size = 100;
    init();
    reserved_variables[ "cube::#mirrors" ]         = cubepl2::CUBE_NUM_MIRRORS;
    reserved_variables[ "cube::#metrics" ]         = cubepl2::CUBE_NUM_METRICS;
    reserved_variables[ "cube::#root::metrics" ]   = cubepl2::CUBE_NUM_ROOT_METRICS;
    reserved_variables[ "cube::#regions" ]         = cubepl2::CUBE_NUM_REGIONS;
    reserved_variables[ "cube::#callpaths" ]       = cubepl2::CUBE_NUM_CALLPATHS;
    reserved_variables[ "cube::#root::callpaths" ] = cubepl2::CUBE_NUM_ROOT_CALLPATHS;
    reserved_variables[ "cube::#locations" ]       = cubepl2::CUBE_NUM_LOCATIONS;
    reserved_variables[ "cube::#locationgroups" ]  = cubepl2::CUBE_NUM_LOCATION_GROUPS;
    reserved_variables[ "cube::#stns" ]            = cubepl2::CUBE_NUM_STNS;
    reserved_variables[ "cube::#rootstns" ]        = cubepl2::CUBE_NUM_ROOT_STNS;
    reserved_variables[ "cube::filename" ]         = cubepl2::CUBE_FILENAME;

    reserved_variables[ "cube::metric::uniq::name" ]     = cubepl2::CUBE_METRIC_UNIQ_NAME;
    reserved_variables[ "cube::metric::disp::name" ]     = cubepl2::CUBE_METRIC_DISP_NAME;
    reserved_variables[ "cube::metric::url" ]            = cubepl2::CUBE_METRIC_URL;
    reserved_variables[ "cube::metric::description" ]    = cubepl2::CUBE_METRIC_DESCRIPTION;
    reserved_variables[ "cube::metric::dtype" ]          = cubepl2::CUBE_METRIC_DTYPE;
    reserved_variables[ "cube::metric::uom" ]            = cubepl2::CUBE_METRIC_UOM;
    reserved_variables[ "cube::metric::expression" ]     = cubepl2::CUBE_METRIC_EXPRESSION;
    reserved_variables[ "cube::metric::initexpression" ] = cubepl2::CUBE_METRIC_INIT_EXPRESSION;
    reserved_variables[ "cube::metric::#children" ]      = cubepl2::CUBE_METRIC_NUM_CHILDREN;
    reserved_variables[ "cube::metric::parent::id" ]     = cubepl2::CUBE_METRIC_PARENT_ID;
    reserved_variables[ "cube::metric::children" ]       = cubepl2::CUBE_METRIC_CHILDREN;
    reserved_variables[ "cube::metric::enumeration" ]    = cubepl2::CUBE_METRIC_ENUMERATION;

    reserved_variables[ "cube::callpath::mod" ]         = cubepl2::CUBE_CALLPATH_MOD;
    reserved_variables[ "cube::callpath::line" ]        = cubepl2::CUBE_CALLPATH_LINE;
    reserved_variables[ "cube::callpath::#children" ]   = cubepl2::CUBE_CALLPATH_NUM_CHILDREN;
    reserved_variables[ "cube::callpath::children" ]    = cubepl2::CUBE_CALLPATH_CHILDREN;
    reserved_variables[ "cube::callpath::calleeid" ]    = cubepl2::CUBE_CALLPATH_CALLEE_ID;
    reserved_variables[ "cube::callpath::parent::id" ]  = cubepl2::CUBE_CALLPATH_PARENT_ID;
    reserved_variables[ "cube::callpath::enumeration" ] = cubepl2::CUBE_CALLPATH_ENUMERATION;


    reserved_variables[ "cube::region::name" ]          = cubepl2::CUBE_REGION_NAME;
    reserved_variables[ "cube::region::mangled::name" ] = cubepl2::CUBE_REGION_MANGLED_NAME;
    reserved_variables[ "cube::region::paradigm" ]      = cubepl2::CUBE_REGION_PARADIGM;
    reserved_variables[ "cube::region::role" ]          = cubepl2::CUBE_REGION_ROLE;
    reserved_variables[ "cube::region::url" ]           = cubepl2::CUBE_REGION_URL;
    reserved_variables[ "cube::region::description" ]   = cubepl2::CUBE_REGION_DESCRIPTION;
    reserved_variables[ "cube::region::mod" ]           = cubepl2::CUBE_REGION_MOD;
    reserved_variables[ "cube::region::begin::line" ]   = cubepl2::CUBE_REGION_BEGIN_LINE;
    reserved_variables[ "cube::region::end::line" ]     = cubepl2::CUBE_REGION_END_LINE;

    reserved_variables[ "cube::stn::name" ]            = cubepl2::CUBE_STN_NAME;
    reserved_variables[ "cube::stn::description" ]     = cubepl2::CUBE_STN_DESCRIPTION;
    reserved_variables[ "cube::stn::class" ]           = cubepl2::CUBE_STN_CLASS;
    reserved_variables[ "cube::stn::#children" ]       = cubepl2::CUBE_STN_NUM_CHILDREN;
    reserved_variables[ "cube::stn::children" ]        = cubepl2::CUBE_STN_ENUMERATION;
    reserved_variables[ "cube::stn::#locationgroups" ] = cubepl2::CUBE_STN_NUM_LOCATION_GROUPS;
    reserved_variables[ "cube::stn::locationgroups" ]  = cubepl2::CUBE_STN_LOCATION_GROUPS;
    reserved_variables[ "cube::stn::parent::id" ]      = cubepl2::CUBE_STN_PARENT_ID;
    reserved_variables[ "cube::stn::parent::sysid" ]   = cubepl2::CUBE_STN_PARENT_SYS_ID;

    reserved_variables[ "cube::locationgroup::name" ]          = cubepl2::CUBE_LOCATION_GROUP_NAME;
    reserved_variables[ "cube::locationgroup::parent::id" ]    = cubepl2::CUBE_LOCATION_GROUP_PARENT_ID;
    reserved_variables[ "cube::locationgroup::parent::sysid" ] = cubepl2::CUBE_LOCATION_GROUP_PARENT_SYS_ID;
    reserved_variables[ "cube::locationgroup::rank" ]          = cubepl2::CUBE_LOCATION_GROUP_RANK;
    reserved_variables[ "cube::locationgroup::type" ]          = cubepl2::CUBE_LOCATION_GROUP_TYPE;
    reserved_variables[ "cube::locationgroup::void" ]          = cubepl2::CUBE_LOCATION_GROUP_VOID;
    reserved_variables[ "cube::locationgroup::#locations" ]    = cubepl2::CUBE_LOCATION_GROUP_NUM_LOCATIONS;
    reserved_variables[ "cube::locationgroup::locations" ]     = cubepl2::CUBE_LOCATION_GROUP_LOCATIONS;

    reserved_variables[ "cube::location::name" ]          = cubepl2::CUBE_LOCATION_NAME;
    reserved_variables[ "cube::location::type" ]          = cubepl2::CUBE_LOCATION_TYPE;
    reserved_variables[ "cube::location::parent::id" ]    = cubepl2::CUBE_LOCATION_PARENT_ID;
    reserved_variables[ "cube::location::parent::sysid" ] = cubepl2::CUBE_LOCATION_PARENT_SYS_ID;
    reserved_variables[ "cube::location::rank" ]          = cubepl2::CUBE_LOCATION_RANK;
    reserved_variables[ "cube::location::void" ]          = cubepl2::CUBE_LOCATION_VOID;


    reserved_variables[ "cube::#locations::void" ]         = cubepl2::CUBE_NUM_VOID_LOCS;
    reserved_variables[ "cube::#locations::nonvoid" ]      = cubepl2::CUBE_NUM_NONVOID_LOCS;
    reserved_variables[ "cube::#locationgroups::void" ]    = cubepl2::CUBE_NUM_VOID_LGS;
    reserved_variables[ "cube::#locationgroups::nonvoid" ] = cubepl2::CUBE_NUM_NONVOID_LGS;
};

CubePL2MemoryManager::~CubePL2MemoryManager()
{
//     cout << " Statistic on CubePL:" << endl;
//     cout << " Max size of used memory : " << memory.size() << endl;
//     cout << " Max size of used global memory : " << global_memory.size() << endl;
    clear_memory();
};


MemoryAdress
CubePL2MemoryManager::register_variable( std::string    name,
                                         KindOfVariable var )
{
    size_t                                        _addr = 0;
    std::map<std::string, MemoryAdress>::iterator it;
    it = reserved_variables.find( name );
    if ( it  != reserved_variables.end() )
    {
        return ( *it ).second;
    }

    it = registered_variables.find( name );
    if ( it  != registered_variables.end() )
    {
        return ( *it ).second;
    }

    it = registered_global_variables.find( name );
    if ( it  != registered_global_variables.end() )
    {
        return ( *it ).second;
    }

    switch ( var )
    {
        case  CUBEPL_RESERVED_VARIABLE:
        {
            _addr = reserved_memory.size();
            reserved_memory.resize( _addr + 1 );
            reserved_variables[ name ] = _addr;
            break;
        }
        case  CUBEPL_VARIABLE:
        {
            _addr                        = page_size;
            registered_variables[ name ] = _addr;
            page_size++;
            memory.resize( memory_stack.top() + page_size );
            break;
        }
        case  CUBEPL_GLOBAL_VARIABLE:
        {
            _addr = global_memory.size();
            global_memory.resize( _addr + 1 );
            registered_global_variables[ name ] = _addr;
            break;
        }
        default:
            throw CubePLError( "Unknown type of CubePL variable." );
    }
    return _addr;
}


KindOfVariable
CubePL2MemoryManager::kind_of_variable( std::string name )
{
    std::map<std::string, MemoryAdress>::iterator it;
    it = reserved_variables.find( name );
    if ( it  != reserved_variables.end() )
    {
        return CUBEPL_RESERVED_VARIABLE;
    }
    it = registered_global_variables.find( name );
    if ( it  != registered_global_variables.end() )
    {
        return CUBEPL_GLOBAL_VARIABLE;
    }
    it = registered_variables.find( name );
    if ( it  != registered_variables.end() )
    {
        return CUBEPL_VARIABLE;
    }
    throw CubePLError( "Variable " + name + " is not registered yet" );
}


void
CubePL2MemoryManager::clear_memory()
{
    for ( std::vector< std::vector<CubePL2MemoryDuplet> >::iterator it = memory.begin(); it != memory.end(); ++it )
    {
        std::vector<CubePL2MemoryDuplet> _vec = *it;
        for ( std::vector<CubePL2MemoryDuplet>::iterator it1 = _vec.begin(); it1 != _vec.end(); ++it1 )
        {
            delete[] ( *it1 ).row_value;
            ( *it1 ).row_value = NULL;
        }
    }
    for ( std::vector< std::vector<CubePL2MemoryDuplet> >::iterator it = reserved_memory.begin(); it != reserved_memory.end(); ++it )
    {
        std::vector<CubePL2MemoryDuplet> _vec = *it;
        for ( std::vector<CubePL2MemoryDuplet>::iterator it1 = _vec.begin(); it1 != _vec.end(); ++it1 )
        {
            delete[] ( *it1 ).row_value;
            ( *it1 ).row_value = NULL;
        }
    }
    for ( std::vector< std::vector<CubePL2MemoryDuplet> >::iterator it = global_memory.begin(); it != global_memory.end(); ++it )
    {
        std::vector<CubePL2MemoryDuplet> _vec = *it;
        for ( std::vector<CubePL2MemoryDuplet>::iterator it1 = _vec.begin(); it1 != _vec.end(); ++it1 )
        {
            delete[] ( *it1 ).row_value;
            ( *it1 ).row_value = NULL;
        }
    }
    global_memory.clear();
    memory.clear();
    reserved_memory.resize( max_reserved_memory_size );
}


void
CubePL2MemoryManager::init()
{
    clear_memory();
    while ( memory_stack.size() > 0 )
    {
        memory_stack.pop();
    }
    memory_stack.push( 0 );
    registered_variables.clear();
    registered_variables[ "calculation::metric::id" ]   = cubepl2::CALCULATION_METRIC_ID;
    registered_variables[ "calculation::callpath::id" ] = cubepl2::CALCULATION_CALLPATH_ID;
    registered_variables[ "calculation::region::id" ]   = cubepl2::CALCULATION_REGION_ID;
    registered_variables[ "calculation::sysres::id" ]   = cubepl2::CALCULATION_SYSRES_ID;
    registered_variables[ "calculation::sysres::kind" ] = cubepl2::CALCULATION_SYSRES_KIND;
    page_size                                           = 5;
};

void
CubePL2MemoryManager::new_page()
{
    if ( page_size == 0 )
    {
        return;
    }
    if ( memory_stack.top() > memory.size() )
    {
        throw CubePLMemoryLayoutError( "Memory stack point out of memory range" );
    }
    if ( memory.size() - memory_stack.top() < 2 * page_size )
    {
        memory.resize( 10 * page_size + memory_stack.top() ); // Extend the memory for double page. Complete current and allocate new one
    }
    memory_stack.push( memory_stack.top() + page_size );      // move page pointer to another page;
};

void
CubePL2MemoryManager::throw_page()
{
    if ( memory_stack.size() > 1 )
    {
        memory_stack.pop();
    }
}



bool
CubePL2MemoryManager::defined( std::string name )
{
    std::map<std::string, MemoryAdress>::iterator it;
    it = reserved_variables.find( name );
    if ( it  != reserved_variables.end() )
    {
        return true;
    }
    it = registered_global_variables.find( name );
    if ( it  != registered_global_variables.end() )
    {
        return true;
    }
    it = registered_variables.find( name );
    if ( it  != registered_variables.end() )
    {
        return true;
    }
    return false;
}



void
CubePL2MemoryManager::clear_variable( MemoryAdress   adress,
                                      KindOfVariable var  )
{
    if ( var == CUBEPL_VARIABLE )
    {
        for ( std::vector<CubePL2MemoryDuplet>::iterator it1 = memory[ memory_stack.top() +  adress ].begin(); it1 != memory[ memory_stack.top() +  adress ].end(); ++it1 )
        {
            delete[] ( *it1 ).row_value;
            ( *it1 ).row_value = NULL;
        }
        memory[ memory_stack.top() +  adress ].clear();
    }
    if ( var == CUBEPL_RESERVED_VARIABLE )
    {
        for ( std::vector<CubePL2MemoryDuplet>::iterator it1 = reserved_memory[ adress ].begin(); it1 != reserved_memory[ adress ].end(); ++it1 )
        {
            delete[] ( *it1 ).row_value;
            ( *it1 ).row_value = NULL;
        }
        reserved_memory[ adress ].clear();
    }
    if ( var == CUBEPL_GLOBAL_VARIABLE )
    {
        global_memory[ adress ].clear();
        for ( std::vector<CubePL2MemoryDuplet>::iterator it1 = global_memory[ adress ].begin(); it1 != global_memory[ adress ].end(); ++it1 )
        {
            delete[] ( *it1 ).row_value;
            ( *it1 ).row_value = NULL;
        }
    }
}

void
CubePL2MemoryManager::put( MemoryAdress   adress,
                           double         index,
                           double         value,
                           KindOfVariable var   )
{
    size_t _index = ( size_t )index;
    switch ( var )
    {
        case CUBEPL_VARIABLE:
        {
            if ( memory_stack.top() +  adress <= memory.size() )
            {
                if (  memory[ memory_stack.top() +  adress ].size() <= _index )
                {
                    memory[ memory_stack.top() +  adress ].resize( _index + 20 );
                }
            }
            memory[ memory_stack.top() +  adress ][ _index ].double_value = value;
            memory[ memory_stack.top() +  adress ][ _index ].state        = CUBEPL_VALUE_DOUBLE;
            break;
        }
        case CUBEPL_RESERVED_VARIABLE:
        {
            if (  reserved_memory[ adress ].size() <= _index )
            {
                reserved_memory[  adress ].resize( _index + 20 );
            }
            reserved_memory[ adress ][ _index ].double_value = value;
            reserved_memory[ adress ][ _index ].state        = CUBEPL_VALUE_DOUBLE;
            break;
        }
        case  CUBEPL_GLOBAL_VARIABLE:
        {
            if (  global_memory[ adress ].size() <= _index )
            {
                global_memory[  adress ].resize( _index + 20 );
            }
            global_memory[ adress ][ _index ].double_value = value;
            global_memory[ adress ][ _index ].state        = CUBEPL_VALUE_DOUBLE;
            break;
        }
        default:
            throw CubePLError( "Unknown type of CubePL variable." );
    }
    ;
};


void
CubePL2MemoryManager::put( MemoryAdress   adress,
                           double         index,
                           string         value,
                           KindOfVariable var  )
{
    size_t _index = ( size_t )index;
    switch ( var )
    {
        case CUBEPL_VARIABLE:
        {
            if (  memory[ memory_stack.top() +  adress ].size() <= _index )
            {
                memory[ memory_stack.top() +  adress ].resize( _index + 20 );
            }
            memory[ memory_stack.top() +  adress ][ _index ].state        = CUBEPL_VALUE_STRING;
            memory[ memory_stack.top() +  adress ][ _index ].string_value = value;
            break;
        }
        case CUBEPL_RESERVED_VARIABLE:
        {
            if (  reserved_memory[  adress ].size() <= _index )
            {
                reserved_memory[  adress ].resize( _index + 20 );
            }
            reserved_memory[  adress ][ _index ].state        = CUBEPL_VALUE_STRING;
            reserved_memory[  adress ][ _index ].string_value = value;
            break;
        }
        case  CUBEPL_GLOBAL_VARIABLE:
        {
            if (  global_memory[  adress ].size() <= _index )
            {
                global_memory[  adress ].resize( _index + 20 );
            }
            global_memory[  adress ][ _index ].state        = CUBEPL_VALUE_STRING;
            global_memory[  adress ][ _index ].string_value = value;
            break;
        }
        default:
            throw CubePLError( "Unknown type of CubePL variable." );
    }
    ;
};

void
CubePL2MemoryManager::put( MemoryAdress   adress,
                           double         index,
                           double*        value,
                           KindOfVariable var
                           )
{
    size_t _index = ( size_t )index;
    switch ( var )
    {
        case CUBEPL_VARIABLE:
        {
            MemoryAdress _top = memory_stack.top();

            if (  memory[ _top +  adress ].size() <= _index )
            {
                memory[ _top +  adress ].resize( _index + 20 );
            }
            memory[ _top +  adress ][ _index ].state = CUBEPL_VALUE_ROW;
            delete[] memory[ _top +  adress ][ _index ].row_value;
            memory[ _top +  adress ][ _index ].row_value = value;
            break;
        }
        case CUBEPL_RESERVED_VARIABLE:
        {
            if (  reserved_memory[  adress ].size() <= _index )
            {
                reserved_memory[  adress ].resize( _index + 20 );
            }
            reserved_memory[  adress ][ _index ].state = CUBEPL_VALUE_ROW;
            delete[] reserved_memory[  adress ][ _index ].row_value;
            reserved_memory[  adress ][ _index ].row_value = value;
            break;
        }
        case  CUBEPL_GLOBAL_VARIABLE:
        {
            if (  global_memory[  adress ].size() <= _index )
            {
                global_memory[  adress ].resize( _index + 20 );
            }
            global_memory[  adress ][ _index ].state = CUBEPL_VALUE_ROW;
            delete[] global_memory[  adress ][ _index ].row_value;
            global_memory[  adress ][ _index ].row_value = value;
            break;
        }
        default:
            throw CubePLError( "Unknown type of CubePL variable." );
    }
    ;
}




void
CubePL2MemoryManager::push_back( MemoryAdress   adress,
                                 double         value,
                                 KindOfVariable var )
{
    switch ( var )
    {
        case CUBEPL_VARIABLE:
        {
            CubePL2MemoryDuplet _t;
            _t.double_value = value;
            _t.state        = CUBEPL_VALUE_DOUBLE;

            memory[ memory_stack.top() +  adress ].push_back( _t );
            break;
        }
        case CUBEPL_RESERVED_VARIABLE:
        {
            CubePL2MemoryDuplet _t;
            _t.double_value = value;
            _t.state        = CUBEPL_VALUE_DOUBLE;

            reserved_memory[ adress ].push_back( _t );
            break;
        }
        case  CUBEPL_GLOBAL_VARIABLE:
        {
            CubePL2MemoryDuplet _t;
            _t.double_value = value;
            _t.state        = CUBEPL_VALUE_DOUBLE;

            global_memory[ adress ].push_back( _t );
            break;
        }
        default:
            throw CubePLError( "Unknown type of CubePL variable." );
    }
    ;
}

void
CubePL2MemoryManager::push_back( MemoryAdress   adress,
                                 string         value,
                                 KindOfVariable var )
{
    switch ( var )
    {
        case CUBEPL_VARIABLE:
        {
            CubePL2MemoryDuplet _t;
            _t.string_value = value;
            _t.state        = CUBEPL_VALUE_STRING;
            memory[ memory_stack.top() +  adress ].push_back( _t );
            break;
        }
        case CUBEPL_RESERVED_VARIABLE:
        {
            CubePL2MemoryDuplet _t;
            _t.string_value = value;
            _t.state        = CUBEPL_VALUE_STRING;
            reserved_memory[ adress ].push_back( _t );
            break;
        }
        case  CUBEPL_GLOBAL_VARIABLE:
        {
            CubePL2MemoryDuplet _t;
            _t.string_value = value;
            _t.state        = CUBEPL_VALUE_STRING;
            global_memory[ adress ].push_back( _t );
            break;
        }
        default:
            throw CubePLError( "Unknown type of CubePL variable." );
    }
}

double
CubePL2MemoryManager::get( MemoryAdress   adress,
                           double         index,
                           KindOfVariable var   )
{
    size_t _index = ( size_t )index;
    switch ( var )
    {
        case CUBEPL_VARIABLE:
        {
            if ( memory[ memory_stack.top() +  adress ].size() <= _index )
            {
                return 0.;
            }
            else
            {
                CubePL2MemoryDuplet& _t =  memory[ memory_stack.top() +  adress ][ _index ];
                if ( _t.state == CUBEPL_VALUE_STRING ) // convert to double if possible
                {
                    std::istringstream stream( _t.string_value );
                    double             t;
                    stream >> t;
                    _t.double_value = t;
                    _t.state        = CUBEPL_VALUE_EQUAL;
                }
                return _t.double_value;
            }
            break;
        }
        case CUBEPL_RESERVED_VARIABLE:
        {
            if ( reserved_memory[ adress ].size() <= _index )
            {
                return 0.;
            }
            else
            {
                CubePL2MemoryDuplet& _t =  reserved_memory[ adress ][ _index ];
                if ( _t.state == CUBEPL_VALUE_STRING ) // convert to double if possible
                {
                    std::istringstream stream( _t.string_value );
                    double             t;
                    stream >> t;
                    _t.double_value = t;
                    _t.state        = CUBEPL_VALUE_EQUAL;
                }
                return _t.double_value;
            }
            break;
        }
        case  CUBEPL_GLOBAL_VARIABLE:
        {
            if ( global_memory[ adress ].size() <= _index )
            {
                return 0.;
            }
            else
            {
                CubePL2MemoryDuplet& _t =  global_memory[ adress ][ _index ];
                if ( _t.state == CUBEPL_VALUE_STRING ) // convert to double if possible
                {
                    std::istringstream stream( _t.string_value );
                    double             t;
                    stream >> t;
                    _t.double_value = t;
                    _t.state        = CUBEPL_VALUE_EQUAL;
                }
                return _t.double_value;
            }
            break;
        }
        default:
            throw CubePLError( "Unknown type of CubePL variable." );
    }
    return 0.;
}

string
CubePL2MemoryManager::get_as_string( MemoryAdress   adress,
                                     double         index,
                                     KindOfVariable var  )
{
    size_t _index = ( size_t )index;

    switch ( var )
    {
        case CUBEPL_VARIABLE:
        {
            if ( memory[ memory_stack.top() +  adress ].size() <= _index )
            {
                return "";
            }
            else
            {
                CubePL2MemoryDuplet& _t =  memory[ memory_stack.top() +  adress ][ _index ];
                if ( _t.state == CUBEPL_VALUE_DOUBLE ) // convert to string if possible
                {
                    stringstream sstr;
                    string       str;
                    sstr <<  setprecision( 14 ) <<  _t.double_value;
                    sstr >> _t.string_value;
                    _t.state = CUBEPL_VALUE_EQUAL;
                }
                return _t.string_value;
            }
            break;
        }
        case CUBEPL_RESERVED_VARIABLE:
        {
            if ( reserved_memory[ adress ].size() <= _index )
            {
                return "";
            }
            else
            {
                CubePL2MemoryDuplet& _t =  reserved_memory[ adress ][ _index ];
                if ( _t.state == CUBEPL_VALUE_DOUBLE )   // convert to string if possible
                {
                    stringstream sstr;
                    string       str;
                    sstr <<  setprecision( 14 ) <<  _t.double_value;
                    sstr >> _t.string_value;
                    _t.state = CUBEPL_VALUE_EQUAL;
                }
                return _t.string_value;
            }
            break;
        }
        case  CUBEPL_GLOBAL_VARIABLE:
        {
            if ( global_memory[ adress ].size() <= _index )
            {
                return "";
            }
            else
            {
                CubePL2MemoryDuplet& _t =  global_memory[ adress ][ _index ];
                if ( _t.state == CUBEPL_VALUE_DOUBLE )   // convert to string if possible
                {
                    stringstream sstr;
                    string       str;
                    sstr <<  setprecision( 14 ) <<  _t.double_value;
                    sstr >> _t.string_value;
                    _t.state = CUBEPL_VALUE_EQUAL;
                }
                return _t.string_value;
            }
            break;
        }
        default:
            throw CubePLError( "Unknown type of CubePL variable." );
    }
    return "";
}

double*
CubePL2MemoryManager::get_row( MemoryAdress   adress,
                               double         index,
                               KindOfVariable var )
{
    size_t _index = ( size_t )index;
    switch ( var )
    {
        case CUBEPL_VARIABLE:
        {
            if ( memory[ memory_stack.top() +  adress ].size() <= _index )
            {
                return NULL;
            }
            else
            {
                CubePL2MemoryDuplet& _t =  memory[ memory_stack.top() +  adress ][ _index ];
                if ( _t.state != CUBEPL_VALUE_ROW && _t.row_value == NULL ) // convert to double if possible
                {
                    double  value = get( adress, index, var );
                    double* _row  = new double[ row_size ];
                    std::fill_n( _row, row_size, value );
                    _t.row_value = _row;
                    _t.state     = CUBEPL_VALUE_ROW;
                }
                return _t.row_value;
            }
            break;
        }
        case CUBEPL_RESERVED_VARIABLE:
        {
            if ( reserved_memory[ adress ].size() <= _index )
            {
                return NULL;
            }
            else
            {
                CubePL2MemoryDuplet& _t =  reserved_memory[ adress ][ _index ];
                if ( _t.state != CUBEPL_VALUE_ROW && _t.row_value == NULL ) // convert to double if possible
                {
                    double  value = get( adress, index, var );
                    double* _row  = new double[ row_size ];
                    for ( size_t i = 0; i < row_size; i++ )
                    {
                        _row[ i ] = value;
                    }
                    _t.row_value = _row;
                    _t.state     = CUBEPL_VALUE_ROW;
                }
                return _t.row_value;
            }
            break;
        }
        case  CUBEPL_GLOBAL_VARIABLE:
        {
            if ( global_memory[ adress ].size() <= _index )
            {
                return NULL;
            }
            else
            {
                CubePL2MemoryDuplet& _t =  global_memory[ adress ][ _index ];
                if ( _t.state != CUBEPL_VALUE_ROW && _t.row_value == NULL ) // convert to double if possible
                {
                    double  value = get( adress, index, var );
                    double* _row  = new double[ row_size ];
                    for ( size_t i = 0; i < row_size; i++ )
                    {
                        _row[ i ] = value;
                    }
                    _t.row_value = _row;
                    _t.state     = CUBEPL_VALUE_ROW;
                }
                return _t.row_value;
            }
            break;
        }
        default:
            throw CubePLError( "Unknown type of CubePL variable." );
    }
    return NULL;
}





size_t
CubePL2MemoryManager::size_of( MemoryAdress   adress,
                               KindOfVariable var )
{
    if ( var == CUBEPL_VARIABLE )
    {
        return memory[ memory_stack.top() +  adress ].size();
    }
    if ( var == CUBEPL_RESERVED_VARIABLE )
    {
        return reserved_memory[ adress ].size();
    }
    if ( var == CUBEPL_GLOBAL_VARIABLE )
    {
        return global_memory[ adress ].size();
    }
    throw CubePLError( "Unknown type of CubePL variable." );
}

CubePL2StateOfCubePLVariable
CubePL2MemoryManager::type( MemoryAdress   adress,
                            double         index,
                            KindOfVariable var )
{
    size_t _index = ( size_t )index;

    switch ( var )
    {
        case CUBEPL_VARIABLE:
        {
            if ( memory[ memory_stack.top() +  adress ].size() <= _index )
            {
                return CUBEPL_VALUE_DOUBLE;
            }
            else
            {
                CubePL2MemoryDuplet& _t =  memory[ memory_stack.top() +  adress ][ _index ];
                return _t.state;
            }
            break;
        }
        case CUBEPL_RESERVED_VARIABLE:
        {
            if ( reserved_memory[ adress ].size() <= _index )
            {
                return CUBEPL_VALUE_DOUBLE;
            }
            else
            {
                CubePL2MemoryDuplet& _t =  reserved_memory[ adress ][ _index ];
                return _t.state;
            }
            break;
        }
        case  CUBEPL_GLOBAL_VARIABLE:
        {
            if ( global_memory[ adress ].size() <= _index )
            {
                return CUBEPL_VALUE_DOUBLE;
            }
            else
            {
                CubePL2MemoryDuplet& _t =  global_memory[ adress ][ _index ];
                return _t.state;
            }
            break;
        }
        default:
            throw CubePLError( "Unknown type of CubePL variable." );
    }
    return CUBEPL_VALUE_DOUBLE;
}


#endif
