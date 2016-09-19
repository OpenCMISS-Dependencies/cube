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
 * \file CubeMetric.cpp
 * \brief Defines a methods of a Metric

 ********************************************/


#include <iostream>

#include "CubeTypes.h"
#include "CubeSysres.h"
#include "CubeLocation.h"
#include "CubeLocationGroup.h"
#include "CubeSystemTreeNode.h"

#include "CubeMetric.h"
#include "CubeServices.h"
#include "CubeError.h"
#include "CubeObjectsEnumerator.h"
#include "CubeCnode.h"
#include "CubeRegion.h"
#include "CubeStrategies.h"
#include "CubeRowWiseMatrix.h"
#include "CubeIDdeliverer.h"
#include "CubeFileFinder.h"
#include "CubeCaches.h"
#include "CubePLMemoryManager.h"
#include "CubeGeneralEvaluation.h"

using namespace std;
using namespace cube;
using namespace services;




Metric::Metric( const std::string& disp_name,
                const std::string& uniq_name,
                const std::string& dtype,
                const std::string& uom,
                const std::string& val,
                const std::string& url,
                const std::string& descr,
                FileFinder*        ffinder,
                Metric*            parent,
                uint32_t           id,
                const std::string& _expression,
                const std::string& _expression_init,
                const std::string& _expression_aggr_plus,
                const std::string& _expression_aggr_minus,
                bool               rowwise,
                VizTypeOfMetric    _is_ghost
                )
    : Vertex( parent, id ),
    disp_name( disp_name ),
    uniq_name( uniq_name ),
    dtype( dtype ),
    uom( uom ),
    val( val ),
    url( url ),
    descr( descr ),
    filefinder( ffinder ),
    expression( _expression ),
    expression_init( _expression_init ),
    expression_aggr_plus( _expression_aggr_plus ),
    expression_aggr_minus( _expression_aggr_minus ),
    is_rowwise( rowwise ),
    is_ghost( _is_ghost )
{
    own_hash              = construct_own_hash();
    own_data_type         = get_data_type();
    adv_sev_mat           = NULL;
    cache                 = NULL;
    initialized           = false;
    evaluation            = NULL;
    init_evaluation       = NULL;
    aggr_plus_evaluation  = NULL;
    aggr_minus_evaluation = NULL;
    cubepl_memory_manager = NULL;
    active                = true;
    used_by_others        = false;
    my_strategy           = BasicStrategy::selectInitialStrategy(); // cube::CUBE_ALL_IN_MEMORY_STRATEGY;
    metric_value          = selectValueOnDataType( own_data_type );
    metric_value->processArguments( dtype_arg_vec );
    metric_value->init_new();
}



Metric::~Metric()
{
    delete cache;
    delete adv_sev_mat;
    delete evaluation;
    delete init_evaluation;
    delete aggr_plus_evaluation;
    delete aggr_minus_evaluation;
    delete metric_value;
}


void
Metric::writeData()
{
    if ( adv_sev_mat != NULL )
    {
        adv_sev_mat->writeData();
    }
}


std::string
Metric::get_val() const
{
    Metric* _parent = get_parent();
    if ( val != "" || ( _parent == NULL ) )
    {
        return val;
    }
    else
    {
        return _parent->get_val();
    }
}


// ------------------ adv part -----------------------


void
Metric::printLocalCallTreeIDs()
{
    cout << " ------- LOCAL IDS --------- " << endl;
    size_t idx = 0;
    for ( vector<signed int>::iterator iter = calltree_local_ids.begin(); iter != calltree_local_ids.end(); iter++, idx++ )
    {
        cout << idx << " -> " << ( *iter ) << endl;
    }
    cout << " --------------------------- " << endl;
}



void
Metric::setStrategy( CubeStrategy strategy )
{
    my_strategy = strategy; // store my own strategy for postponed initiailzation: in create_data_container
    if ( adv_sev_mat != NULL )
    {
        adv_sev_mat->setStrategy( strategy );
    }
}


void
Metric::dropRow( Cnode* cnode )
{
    if ( !isInitialized() )
    {
        return;
    }
    if ( cnode->is_clustered() )
    {
        for ( size_t i = 0; i < sysv.size(); ++i )
        {
            adv_sev_mat->dropRow( calltree_local_ids[ cnode->get_remapping_cnode( sysv[ i ]->get_parent()->get_rank() )->get_id() ] );
        }
    }
    else
    {
        adv_sev_mat->dropRow( calltree_local_ids[ cnode->get_remapping_cnode()->get_id() ] );
    }
}

void
Metric::dropAllRows()
{
    if ( used_by_others )
    {
        return;
    }
    if ( isInitialized() && adv_sev_mat != NULL )
    {
        adv_sev_mat->dropAllRows();
    }
    if ( cache )
    {
        cache->invalidate();
    }
}

void
Metric::create_data_container( uint32_t n_cid, uint32_t n_tid/*, bool existing */)
{
    if (initialized)
        return;
    ncid = n_cid;
    ntid = n_tid;
    if ( get_type_of_metric() == CUBE_METRIC_POSTDERIVED ||
         get_type_of_metric() == CUBE_METRIC_PREDERIVED_INCLUSIVE ||
         get_type_of_metric() == CUBE_METRIC_PREDERIVED_EXCLUSIVE
         )
    {
        if ( evaluation != NULL )
        {
            evaluation->setRowSize( ntid );
        }
        if ( init_evaluation != NULL )
        {
            init_evaluation->setRowSize( ntid );
        }
        return;
    }
    if ( adv_sev_mat != NULL )
    {
        delete adv_sev_mat;
        adv_sev_mat = NULL;
    }
    if ( adv_sev_mat == NULL )
    {
        cache_selection( ncid, ntid, get_type_of_metric() );
        try
        {
            adv_sev_mat = 
                new RowWiseMatrix( filefinder->getMetricData( this ), filefinder->getMetricIndex( this ), ncid, n_tid, metric_value );
        }
        catch ( const NoFileError& err )      // Assuming, that exception only in the case of missing file thrown. Then create
        {
            delete adv_sev_mat;
            adv_sev_mat = new RowWiseMatrix( ncid, ntid, metric_value );
        }
        setStrategy( my_strategy );
    }
    initialized = true;
}



row_of_objects_t*
Metric::create_calltree_id_maps( IDdeliverer* ids, Cnode* root, row_of_objects_t* _row )
{
    ObjectsEnumerator enumerator;
    ids->reset();
    row_of_objects_t* row = enumerator.get_objects_to_enumerate( root, _row );

    int i = 0;
    for ( row_of_objects_t::iterator iter = row->begin(); iter < row->end(); ++iter, ++i )
    {
        calltree_local_ids[ ( *iter )->get_id() ] = ids->get_next_id();
    }
    return row;
}



void
Metric::set_dimensions( std::vector<Cnode*>& _cnodev, std::vector<Cnode*>& _roots_cnodev, std::vector<SystemTreeNode*>& _stnv, std::vector<Location*>& _locv )
{
    cnodev       = _cnodev;
    roots_cnodev = _roots_cnodev;
    roots_sysv   = _stnv;
    sysv         = _locv;
//     initialized  = true;
}


std::vector<std::string>
Metric::split_dtype_args()
{
    vector<string> v;
    string         arg_str;
    size_t         pos = 0;
    // find opening bracket, if not present return empty vector
    for (; ( pos < dtype.size() ) && ( dtype[ pos ] != '(' ); ++pos )
    {
        ;                                                       // { }
    }
    if ( pos == dtype.size() )
    {
        return v;
    }
    // opening brace found, split string into base and argument part
    dtype_arg_str = dtype.substr( pos, string::npos ); // argument string, save for reuse on writing the cube file
    dtype.erase( pos, string::npos );                  // base data type
    arg_str = dtype_arg_str.substr( 1 );
    if ( arg_str[ arg_str.size() - 1 ] != ')' )
    {
        return v;                                     // malformed argument string, don't parse
    }
    // tokenize arguments into vector. not very robust, also does not
    // trim whitespace.
    pos = 0;
    string tmp;
    for (; pos < arg_str.size(); ++pos )
    {
        if ( ( arg_str[ pos ] == ',' ) || ( arg_str[ pos ] == ')' ) )
        {
            tmp = arg_str.substr( 0, pos );
            arg_str.erase( 0, pos + 1 );
            v.push_back( tmp );
            pos = 0;
        }
    }
    return v;
}


DataType
Metric::get_data_type()
{
    if ( dtype == "INT8" )
    {
        return CUBE_DATA_TYPE_INT8;
    }
    if ( ( dtype == "UINT8" )
         ||
         ( dtype == "CHAR" )
         )
    {
        return CUBE_DATA_TYPE_UINT8;
    }
    if (
        ( dtype == "INT16" )
        ||
        ( dtype == "SIGNED SHORT INT" )
        ||
        ( dtype == "SHORT INT" )
        )
    {
        return CUBE_DATA_TYPE_INT16;
    }
    if (
        ( dtype == "UINT16" )
        ||
        ( dtype == "UNSIGNED SHORT INT" )
        )
    {
        return CUBE_DATA_TYPE_UINT16;
    }
    if (
        ( dtype == "INT32" )
        ||
        ( dtype == "SIGNED INT" )
        ||
        ( dtype == "INT" )
        )
    {
        return CUBE_DATA_TYPE_INT32;
    }

    if (
        ( dtype == "UINT32" )
        ||
        ( dtype == "UNSIGNED INT" )
        )
    {
        return CUBE_DATA_TYPE_UINT32;
    }
    if (
        ( dtype == "INT64" )
        ||
        ( dtype == "SIGNED INTEGER" )
        ||
        ( dtype == "INTEGER" )
        )
    {
        return CUBE_DATA_TYPE_INT64;
    }
    if (
        ( dtype == "UINT64" )
        ||
        ( dtype == "UNSIGNED INTEGER" )
        )
    {
        return CUBE_DATA_TYPE_UINT64;
    }

    if (
        ( dtype == "DOUBLE" )
        ||
        ( dtype == "FLOAT" )
        )
    {
        return CUBE_DATA_TYPE_DOUBLE;
    }

    if ( dtype == "COMPLEX" )
    {
        return CUBE_DATA_TYPE_COMPLEX;
    }
    if ( dtype == "TAU_ATOMIC" )
    {
        return CUBE_DATA_TYPE_TAU_ATOMIC;
    }
    if ( dtype == "MINDOUBLE" )
    {
        return CUBE_DATA_TYPE_MIN_DOUBLE;
    }
    if ( dtype == "MAXDOUBLE" )
    {
        return CUBE_DATA_TYPE_MAX_DOUBLE;
    }
    if ( dtype == "RATE" )
    {
        return CUBE_DATA_TYPE_RATE;
    }
    if ( dtype == "SCALE_FUNC" )
    {
        return CUBE_DATA_TYPE_SCALE_FUNC;
    }
    // from  here
    dtype_arg_vec = split_dtype_args();

    if ( dtype == "HISTOGRAM" )
    {
        return CUBE_DATA_TYPE_HISTOGRAM;
    }
    if ( dtype == "NDOUBLES" )
    {
        return CUBE_DATA_TYPE_NDOUBLES;
    }
    return CUBE_DATA_TYPE_DOUBLE;
}


IndexFormat
Metric::get_index_format()
{
    return CUBE_INDEX_FORMAT_DENSE;
}


void
Metric::setMemoryManager( CubePLMemoryManager* _manager )
{
    cubepl_memory_manager = _manager;
}

// /< Cube parses expression and creates an hierarchy of evaluators, which is saved here

void
Metric::setEvaluation( GeneralEvaluation* _ev )
{
    delete evaluation;
    invalidateCache();
    evaluation = _ev;
}

void
Metric::setInitEvaluation( GeneralEvaluation* _ev )
{
    delete init_evaluation;
    invalidateCache();
    init_evaluation = _ev;
}

void
Metric::setAggrPlusEvaluation( GeneralEvaluation* _ev )
{
    delete aggr_plus_evaluation;
    invalidateCache();
    aggr_plus_evaluation = _ev;
}

void
Metric::setAggrMinusEvaluation( GeneralEvaluation* _ev )
{
    delete aggr_minus_evaluation;
    invalidateCache();
    aggr_minus_evaluation = _ev;
}

vector<Cnode*>
Metric::get_optimal_sequence( const vector<Cnode*>& calltree )
{
    vector<Cnode*> v( calltree.size(), NULL );
    for ( vector<Cnode* >::const_iterator iter = calltree.begin(); iter < calltree.end(); ++iter )
    {
        v[ calltree_local_ids[ ( *iter )->get_id() ] ] = *iter;
    }
    return v;
}

void
Metric::set_adv_sev_adv( Cnode* cnode, Thread* thrd, Value* value )
{
    if ( cnode != NULL && thrd != NULL && adv_sev_mat != NULL )
    {
        int64_t process_rank = thrd->get_parent()->get_rank();
        adv_sev_mat->setValue( value, calltree_local_ids[ cnode->get_remapping_cnode( process_rank )->get_id() ], thrd->get_id() );
    }
}

Value*
Metric::get_adv_sev_adv( Cnode* cnode, Thread* thrd )
{
    if ( adv_sev_mat == NULL )
    {
        return NULL;
    }
    int64_t process_rank = thrd->get_parent()->get_rank();
    Value*  v            =  adv_sev_mat->getValue( calltree_local_ids[ cnode->get_remapping_cnode( process_rank )->get_id() ], thrd->get_id() );
    int64_t _norm        = cnode->get_cluster_normalization( process_rank );
    if ( _norm > 0 )
    {
        v->normalizeWithClusterCount( ( uint64_t )_norm );
    }
    return v;
}




void
Metric::set_adv_sev( Cnode* cnode, Thread* thrd, double value )
{
    if ( cnode != NULL && thrd != NULL && adv_sev_mat != NULL )
    {
        int64_t process_rank = thrd->get_parent()->get_rank();
        adv_sev_mat->setValue( value, calltree_local_ids[ cnode->get_remapping_cnode( process_rank )->get_id() ], thrd->get_id() );
    }
    else
    {
        cerr << " Something is wrong in Metric::set_adv_set(" << cnode << ", " << thrd << ", " << value << ")" << endl;
    }
}

double
Metric::get_adv_sev( Cnode* cnode, Thread* thrd )
{
    Value* v = get_adv_sev_adv( cnode, thrd );
    if ( v == NULL )
    {
        return 0.;
    }
    double d = v->getDouble();
    delete v;
    return d;
}


// / ------------------------ Sum over all threads for given cnode and all subcnodes


double
Metric::get_sev( Cnode* cnode, CalculationFlavour cnf )
{
    double ret = 0.;
    Value* v   = get_sev_adv( cnode, cnf );
    if ( v == NULL )
    {
        return 0.;
    }
    ret = v->getDouble();
    delete v;
    return ret;
}



double
Metric::get_sev( Cnode* cnode, CalculationFlavour cnf, Sysres* sys, CalculationFlavour sf )
{
    double ret = 0.;
    Value* v   = get_sev_adv( cnode, cnf, sys, sf );
    if ( v == NULL )
    {
        return 0.;
    }
    ret = v->getDouble();
    delete v;
    return ret;
}

// inclusive value and exclusive value in this metric are same
Value*
Metric::get_sev_adv( Cnode* cnode, CalculationFlavour cnf )
{
    Value* v =  metric_value->clone();
    for ( unsigned i = 0; i < ntid; ++i )
    {
        Value*  _v    =   adv_sev_mat->getValue( calltree_local_ids[ cnode->get_remapping_cnode( sysv[ i ]->get_parent()->get_rank() )->get_id() ], i );
        int64_t _norm = cnode->get_cluster_normalization( sysv[ i ]->get_parent()->get_rank() );
        if ( _norm > 0 )
        {
            _v->normalizeWithClusterCount( ( uint64_t )_norm );
        }
        ( *v ) += _v;
        delete _v;
    }
    return v;
}






// inclusive value and exclusive value in this metric are same
//
Value*
Metric::get_sev_adv( Cnode* cnode, CalculationFlavour cnf, Sysres* sys, CalculationFlavour sf )
{
    Value* v = NULL;
    v = adv_sev_mat->getValue( calltree_local_ids[ cnode->get_id() ], sys->get_id() );
    return v;
}

// // // ---------------------- rowwise calls ---------------------

// --------// inclusive value and exclusive value in this metric are same
Value**
Metric::get_sevs_adv( Cnode* cnode, CalculationFlavour cnf )
{
    if ( adv_sev_mat == NULL )
    {
        return NULL;
    }
    char* _v =   adv_sev_mat->getRow( calltree_local_ids[ cnode->get_id() ] );
    if ( _v == NULL )
    {
        return NULL;
    }
    Value** to_return = services::create_row_of_values( ntid );
    char*   start     = _v;
    for ( unsigned i = 0; i < ntid; ++i )
    {
        Value* _val = metric_value->clone();
        start = _val->fromStream( start );

        int64_t _norm = cnode->get_cluster_normalization( sysv[ i ]->get_parent()->get_rank() );
        if ( _norm > 0 )
        {
            _val->normalizeWithClusterCount( ( uint64_t )_norm );
        }
        to_return[ i ] = _val;
    }
    return to_return;
}


char*
Metric::get_sevs_raw( Cnode* cnode, CalculationFlavour cnf )
{
    if ( adv_sev_mat == NULL )
    {
        return NULL;
    }

    return adv_sev_mat->getRow( calltree_local_ids[ cnode->get_id() ] );
}




double*
Metric::get_sevs( Cnode* cnode, CalculationFlavour cnf )
{
    Value** _v        =   get_sevs_adv( cnode, cnf );
    double* to_return = services::create_row_of_doubles( ntid );
    if ( _v == NULL )
    {
        return to_return;
    }
    for ( unsigned i = 0; i < ntid; ++i )
    {
        to_return[ i ] = _v[ i ]->getDouble();
    }
    services::delete_row_of_values( _v, ntid );
    return to_return;
}



Value**
Metric::get_saved_sevs_adv( Cnode* cnode )
{
    if ( adv_sev_mat == NULL )
    {
        return NULL;
    }

    char* _v =   adv_sev_mat->getRow( calltree_local_ids[ cnode->get_id() ] );
    if ( _v == NULL )
    {
        return NULL;
    }
    Value** to_return = services::create_row_of_values( ntid );
    char*   start     = _v;
    for ( unsigned i = 0; i < ntid; ++i )
    {
        Value* _val = metric_value->clone();
        start = _val->fromStream( start );

        int64_t _norm = cnode->get_cluster_normalization( sysv[ i ]->get_parent()->get_rank() );
        if ( _norm > 0 )
        {
            _val->normalizeWithClusterCount( ( uint64_t )_norm );
        }
        to_return[ i ] = _val;
    }
    return to_return;
}


char*
Metric::get_saved_sevs_raw( Cnode* cnode )
{
    if ( adv_sev_mat == NULL )
    {
        return NULL;
    }

    return adv_sev_mat->getRow( calltree_local_ids[ cnode->get_id() ] );
}




double*
Metric::get_saved_sevs( Cnode* cnode )
{
    Value** _v        =   get_saved_sevs_adv( cnode );
    double* to_return = services::create_row_of_doubles( ntid );
    if ( _v == NULL )
    {
        return to_return;
    }
    for ( unsigned i = 0; i < ntid; ++i )
    {
        to_return[ i ] = _v[ i ]->getDouble();
    }
    services::delete_row_of_values( _v, ntid );
    return to_return;
}



void
Metric::set_sevs_raw( Cnode* cnode,
                      char*  _val
                      )
{
    if ( adv_sev_mat == NULL )
    {
        return;
    }

    adv_sev_mat->setRow( _val, calltree_local_ids[ cnode->get_id() ] );
}

// ------------------ adv part end -------------------

// ------------------OLD PART ---- -
void
Metric::set_sev( Cnode* cnode, Thread* thrd, double value )
{
// #warning "Backward compatibility" .
    set_adv_sev( cnode, thrd, value );
}

double
Metric::get_sev( Cnode* cnode, Thread* thrd )
{
    double ret = 0.;
    Value* v   = get_sev_adv( cnode, thrd );
    if ( v == NULL )
    {
        return 0.;
    }
    ret = v->getDouble();
    delete v;
    return ret;
}


Value*
Metric::get_sev_adv( Cnode* cnode, Thread* thrd )
{
    if ( adv_sev_mat != NULL )
    {
        int64_t process_rank = thrd->get_parent()->get_rank();
        Value*  _v           = adv_sev_mat->getValue(
            calltree_local_ids[ cnode->get_remapping_cnode( process_rank )->get_id() ],
            thrd->get_id() );
        int64_t _norm = cnode->get_cluster_normalization( process_rank );
        if ( ( _norm > 0 ) &&  ( _v != NULL ) )
        {
            _v->normalizeWithClusterCount( ( uint64_t )_norm );
        }
        return _v;
    }
    Value* v =  metric_value->clone();
    return v;
}



double
Metric::sum()
{
// #warning "SUM is not implemented yet. The meaning of this method is malicious for inclusive of simple metrics. In the future will disapear."
    cerr << " Operation SUM over whole metric is not implemented yet. It returns always \"12345\"." << endl;
    return 12345.;
}

void
Metric::clear()
{
// #warning "clear() is not implemented yet"
    cerr << " clear() of metric is not implemented yet. Neither SCALASCA, nor GUI calls it. " << endl;
}






void
Metric::writeXML_data( ostream& out,  std::vector<Cnode*>& _cnodev,  std::vector<Thread*>& _thrdv )
{
    if ( get_val() == "VOID" )
    {
        return;
    }

    std::vector<Thread*> __thrd_sorted = _thrdv;
    sort( __thrd_sorted.begin(), __thrd_sorted.end(), &IdentObject::id_compare );



    out << "<matrix metricId=\"" << get_id() << "\">" <<  '\n';

    for ( std::vector<Cnode*>::iterator citer = _cnodev.begin(); citer < _cnodev.end(); ++citer )
    {
        if ( ( *citer )->isHidden() )  // do not write hidden cnodes. Their value is included in the exclusive value of parent.
        {
            continue;
        }
        out << "<row cnodeId=\"" << ( *citer )->get_id() << "\">" <<  '\n';
        std::vector<Thread*>::iterator siter = __thrd_sorted.begin();
        for (; siter < __thrd_sorted.end(); ++siter )
        {
            Value* v = get_sev_adv( *citer, CUBE_CALCULATE_EXCLUSIVE, *siter, CUBE_CALCULATE_EXCLUSIVE );  // / CUBE 3 saves exclusive values alond Calltree
            if ( v != NULL )
            {
                out << v->getString() <<  '\n';
                delete v;
            }
            else
            {
                out << "0" <<  '\n';
            }
        }
        out << "</row>" << flush <<  '\n';
    }
    out << "</matrix>" << flush <<  '\n';
}

void
Metric::writeXML( ostream& out, bool cube3_export ) const
{
    out << indent() << "    <metric id=\"" << get_id();
    if ( !cube3_export )
    {
        out << "\" type=\"" << get_metric_kind(); // / CUBE 3 conform. no spezification about type of metric
        if ( isGhost() )
        {
            out << "\" viztype=\"GHOST";
        }
    }
    out << "\">" <<  '\n';

    out << indent() << "      <disp_name>" << escapeToXML( get_disp_name() ) << "</disp_name>" <<  '\n';
    out << indent() << "      <uniq_name>" << escapeToXML( get_uniq_name() ) << "</uniq_name>" <<  '\n';
    out << indent() << "      <dtype>" << escapeToXML( get_dtype_with_args() ) << "</dtype>" <<  '\n';
    out << indent() << "      <uom>"  << escapeToXML( get_uom() ) << "</uom>" <<  '\n';
    if ( !get_val().empty() )
    {
        out << indent() << "      <val>" << escapeToXML( get_val() ) << "</val>\n";
    }
    out << indent() << "      <url>" << escapeToXML( get_url() ) << "</url>" <<  '\n';
    out << indent() << "      <descr>" << escapeToXML( get_descr() ) << "</descr>" <<  '\n';
    if ( !cube3_export  && ( get_type_of_metric() == CUBE_METRIC_POSTDERIVED || get_type_of_metric() == CUBE_METRIC_PREDERIVED_INCLUSIVE ||  get_type_of_metric() == CUBE_METRIC_PREDERIVED_EXCLUSIVE ) )
    {
        out << indent() << "      <cubepl";
        if ( is_rowwise == false )
        {
            out << " rowwise=false";
        }
        out << ">" << escapeToXML( get_expression() ) << "</cubepl>" <<  '\n';
        if ( get_init_expression() != "" )
        {
            out << indent() << "      <cubeplinit>" << escapeToXML( get_init_expression() ) << "</cubeplinit>" <<  '\n';
        }
        if ( get_aggr_plus_expression() != "" )
        {
            out << indent() << "      <cubeplaggr cubeplaggrtype=plus>" << escapeToXML( get_aggr_plus_expression() ) << "</cubeplaggr>" <<  '\n';
        }
        if ( get_aggr_minus_expression() != "" )
        {
            out << indent() << "      <cubeplaggr cubeplaggrtype=plus>" << escapeToXML( get_aggr_minus_expression() ) << "</cubeplaggr>" <<  '\n';
        }
    }

    writeAttributes( out,  indent() + "      ", cube3_export  );


    for ( unsigned int i = 0; i < num_children(); ++i )
    {
        const Metric* p = get_child( i );
        p->writeXML( out, cube3_export );
    }
    out << indent() << "    </metric>" <<  '\n';
}


// --------- preparation calls for derived metrics---- defined here, used from derived metrics

void
Metric::post_calculation_cleanup()
{
    cubepl_memory_manager->get_memory_initializer()->memory_cleanup();
}
void
Metric::pre_calculation_preparation( Cnode* cnode )
{
    setup_cubepl_memory();
    setup_cubepl_memory_for_itself();
    setup_cubepl_memory_for_cnode( cnode );
}


void
Metric::pre_calculation_preparation( Cnode* cnode, Sysres* sys )
{
    setup_cubepl_memory();
    setup_cubepl_memory_for_itself();
    setup_cubepl_memory_for_cnode( cnode );

    setup_cubepl_memory_for_sysres( sys );
}

void
Metric::setup_cubepl_memory()
{
    cubepl_memory_manager->get_memory_initializer()->memory_new();
}

void
Metric::setup_cubepl_memory_for_itself()
{
    cubepl_memory_manager->get_memory_initializer()->memory_setup( this );
}

void
Metric::setup_cubepl_memory_for_cnode( Cnode* cnode )
{
    cubepl_memory_manager->get_memory_initializer()->memory_setup(  cnode );
    setup_cubepl_memory_for_region( cnode->get_callee() );
}

void
Metric::setup_cubepl_memory_for_region( Region* region )
{
    cubepl_memory_manager->get_memory_initializer()->memory_setup( region );
}

void
Metric::setup_cubepl_memory_for_sysres( Sysres* sysres )
{
    cubepl_memory_manager->get_memory_initializer()->memory_setup(  sysres );
}





// -------------------- GET TYPE OF METRIC
Value*
Metric::get_metric_value()
{
    return metric_value->clone();
}

Value*
Metric::its_value()
{
    return metric_value;
}



bool
Metric::asInclusiveMetric()
{
    return metric_value->asInclusiveMetric();
}

bool
Metric::asExclusiveMetric()
{
    return metric_value->asExclusiveMetric();
}


TypeOfMetric
Metric::get_type_of_metric( std::string s_kind )
{
    if ( s_kind == "" )
    {
        return CUBE_METRIC_EXCLUSIVE;
    }
    if ( s_kind == "POSTDERIVED"  || s_kind == "DERIVED" )
    {
        return CUBE_METRIC_POSTDERIVED;
    }
    if ( s_kind == "PREDERIVED_INCLUSIVE" )
    {
        return CUBE_METRIC_PREDERIVED_INCLUSIVE;
    }
    if ( s_kind == "PREDERIVED_EXCLUSIVE" )
    {
        return CUBE_METRIC_PREDERIVED_EXCLUSIVE;
    }
    if ( s_kind == "EXCLUSIVE" )
    {
        return CUBE_METRIC_EXCLUSIVE;
    }
    if ( s_kind == "INCLUSIVE" )
    {
        return CUBE_METRIC_INCLUSIVE;
    }
    if ( s_kind == "SIMPLE" )
    {
        return CUBE_METRIC_SIMPLE;
    }
    return CUBE_METRIC_EXCLUSIVE;
};


VizTypeOfMetric
Metric::get_viz_type_of_metric( std::string s_kind )
{
    if ( s_kind == "" || s_kind == "NORMAL" )
    {
        return CUBE_METRIC_NORMAL;
    }
    if ( s_kind == "GHOST"  )
    {
        return CUBE_METRIC_GHOST;
    }
    return CUBE_METRIC_NORMAL;
};

bool
Metric::isIntegerType()
{
    switch ( get_data_type() )
    {
        case CUBE_DATA_TYPE_UINT64:
        case CUBE_DATA_TYPE_INT64:
        case CUBE_DATA_TYPE_UINT32:
        case CUBE_DATA_TYPE_INT32:
        case CUBE_DATA_TYPE_UINT16:
        case CUBE_DATA_TYPE_INT16:
        case CUBE_DATA_TYPE_UINT8:
        case CUBE_DATA_TYPE_INT8:
            return true;
        default:
            return false;
    }
}


bool
Metric::weakEqual( Metric* _m )
{
    if ( get_uniq_name() != _m->get_uniq_name() )
    {
        return false;
    }
    return true;
}

bool
Metric::weakEqual( Vertex* _v )
{
    cerr << " Hook for weakEqual" << endl;
    return false;
}

void
cube::Metric::invalidateCachedValue( Cnode* cnode, CalculationFlavour cf, Sysres* sysres, CalculationFlavour sf )
{
    cache->invalidateCachedValue( cnode, cf, sysres, sf );
}


void
cube::Metric::invalidateCache( void )
{
    cache->invalidate();
}


void
cube::Metric::cache_selection( uint32_t n_cid, uint32_t n_tid, TypeOfMetric type )
{
    delete cache;
    cache =  new SimpleCache<double>( n_cid, n_tid, metric_value->getSize(), ( type == CUBE_METRIC_INCLUSIVE ) ? CUBE_CALCULATE_INCLUSIVE : CUBE_CALCULATE_EXCLUSIVE );
}



void
cube::Metric::isUsedByOthers( bool value )
{
    used_by_others = value;
}
