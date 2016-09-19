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
 * \file CubeMetric.h
 * \brief Defines a metric class as a element of the metric dimension. It doesn't make any assumptotion about relations between part-child entries. Inclusive and Exclusive Value returns jast saved element.
 *
 */
#ifndef CUBE_METRIC_H
#define CUBE_METRIC_H


#include <iosfwd>
#include <vector>

#include "CubeVertex.h"
#include "CubeTypes.h"
#include "CubeSysres.h"
#include "CubeSystemTreeNode.h"
#include "CubeLocationGroup.h"
#include "CubeLocation.h"


#include "CubeCnode.h"

#include "CubeCaches.h"


/*
 *----------------------------------------------------------------------------
 *
 * class Metric
 *
   =======
 */


namespace cube
{
/**
 * Metric has a treelike structure and is a kind of Vertex.
 *
 * Metric has a display name, unique name, data type, unit of measure (sec, ms and so on), url to the documentation, description of the metric, ID.
 */
class Metric : public Vertex
{
public:

    Metric( const std::string& disp_name,
            const std::string& uniq_name,
            const std::string& dtype,
            const std::string& uom,
            const std::string& val,
            const std::string& url,
            const std::string& descr,
            FileFinder*        ffinder,
            Metric*            parent,
            uint32_t           id = 0,
            const std::string& _expression = "",
            const std::string& _expression_init = "",
            const std::string& _expression_aggr_plus = "",
            const std::string& _expression_aggr_minus = "",
            bool               rowwise = true,
            VizTypeOfMetric    _is_ghost = CUBE_METRIC_NORMAL
            );

    virtual
    ~Metric();

    inline
    Metric*
    get_child( unsigned int i ) const
    {
        return static_cast<Metric*>( Vertex::get_child( i ) );
    }

    inline
    Metric*
    get_parent()     const
    {
        return static_cast<Metric*>( Vertex::get_parent() );
    }

/**
 * Return metric value characteristics (such as "VOID") if optionally set,
   or value characteristics inherited from ancestors.
 */
    std::string
    get_val() const;


    static TypeOfMetric
    get_type_of_metric( std::string s_kind );

    static VizTypeOfMetric
    get_viz_type_of_metric( std::string s_kind );


    virtual TypeOfMetric
    get_type_of_metric() const
    {
        return CUBE_METRIC_SIMPLE;
    };

    virtual std::string
    get_metric_kind() const
    {
        return "";
    }

    inline
    std::string
    get_disp_name() const
    {
        return disp_name;
    }

    inline
    std::string
    get_uniq_name() const
    {
        return uniq_name;
    }

    inline
    std::string
    get_dtype()     const
    {
        return dtype;
    }

    inline
    std::string
    get_dtype_with_args() const
    {
        return get_dtype() + dtype_arg_str;
    }

    inline
    std::string
    get_uom()       const
    {
        return uom;
    }                                                                   // /< Unit of measurement

    inline
    std::string
    get_descr()     const
    {
        return descr;
    }                                                                   // /< Description of metric

    inline
    std::string
    get_url()       const
    {
        return url;
    }                                                                   // /< URL for metric details

    inline
    std::string
    get_expression()       const
    {
        return expression;
    }                                                                   // /< URL for metric details

    inline
    void
    set_expression( std::string _expression )                            // /< Sets the new expression into the metric. No compilation or check is perfomed.
    {
        expression = _expression;
    }

    inline
    std::string
    get_init_expression()       const
    {
        return expression_init;
    }                                                                   // /< URL for metric details

    inline
    void
    set_init_expression( std::string _expression )                            // /< Sets the new expression into the metric. No compilation or check is perfomed.
    {
        expression_init = _expression;
    }


    inline
    std::string
    get_aggr_plus_expression()       const
    {
        return expression_aggr_plus;
    }                                                                   // /< URL for metric details


    inline
    void
    set_aggr_plus_expression( std::string _expression )                            // /< Sets the new expression into the metric. No compilation or check is perfomed.
    {
        expression_aggr_plus = _expression;
    }



    inline
    std::string
    get_aggr_minus_expression()       const
    {
        return expression_aggr_minus;
    }                                                                   // /< URL for metric details


    inline
    void
    set_aggr_minus_expression( std::string _expression )                            // /< Sets the new expression into the metric. No compilation or check is perfomed.
    {
        expression_aggr_minus = _expression;
    }



    void
    setMemoryManager( CubePLMemoryManager* _manager );                                                     // /< Cube parses expression and creates an hierarchy of evaluators, which is saved here



    inline
    GeneralEvaluation*
    getEvaluation()                                                     // /< Returns an object to evaluate the value of the metric
    {
        return evaluation;
    }


    void
    setEvaluation( GeneralEvaluation* _ev );                                                     // /< Cube parses expression and creates an hierarchy of evaluators, which is saved here





    inline
    GeneralEvaluation*
    getInitEvaluation()                                                     // /< Returns an object to evaluate the value of the metric
    {
        return init_evaluation;
    }
    void
    setInitEvaluation( GeneralEvaluation* _ev );


    inline
    GeneralEvaluation*
    getAggrPlusEvaluation()                                                     // /< Returns an object to evaluate the value of the metric
    {
        return aggr_plus_evaluation;
    }
    void
    setAggrPlusEvaluation( GeneralEvaluation* _ev );



    inline
    GeneralEvaluation*
    getAggrMinusEvaluation()                                                     // /< Returns an object to evaluate the value of the metric
    {
        return aggr_plus_evaluation;
    }
    void
    setAggrMinusEvaluation( GeneralEvaluation* _ev );




    inline
    const std::string&
    get_val()
    {
        return val;
    }                                                                   // /< set value type


    inline
    void
    set_val( const std::string& value )
    {
        val    = value;
        active = !( val.compare( "VOID" ) == 0 );
    }                                                                   // /< set value type



    virtual
    void
    initialize( void )
    {
    };





    Value*
    get_metric_value();


    Value*
    its_value();

    double
    get_sev( Cnode*  cnode,
             Thread* thrd );                                            // /< returns saved value, without taking into account, is it exclusive value or inclusive. (Backward compatibility... )
    Value*
    get_sev_adv( Cnode*  cnode,
                 Thread* thrd );                                             // /< returns saved value, without taking into account, is it exclusive value or inclusive.

    void
    set_sev( Cnode*  cnode,
             Thread* thrd,
             double  value );                                           // /< saves value in Metric.                                                                (Backward compatibility ...)

    double
    sum();                                                              // /< Returns sum of all data in the metric. [ Taken fror cube3... not adapted to Cube4 yet]
    void
    clear();                                                            // /< Clears all the data in the metric.     [ Taken fror cube3... not adapted to Cube4 yet]


    virtual bool
    weakEqual( Metric* );

    virtual bool
    weakEqual( Vertex* );


    virtual std::string
    construct_own_hash()
    {
        return uniq_name;
    };                                                                  // /< Uniq name per definitio is a hash.

    // / writes the metric definition into fstream (writes child metrics recursively)
    void
    writeXML( std::ostream& out,
              bool          cube3_export = true ) const;

    // / writes the data into fstream. Produces cube3 layout.
    void
    writeXML_data( std::ostream&         out,
                   std::vector<Cnode*>&  cnodev,
                   std::vector<Thread*>& thrdv );



    // ---------- adv Data start ------------------------
    void
    writeData();                                                              // /< Performs actual final of the data to disk (Of cached data container - then last line gets saved)

    virtual
    void create_data_container( uint32_t, uint32_t);                         // /< Creates data container, in the case, the cube will lbe created.
//     virtual
//     void create_existing_data_container( uint32_t, uint32_t );                // /< Creates data container, in the case, the cube is existing, (Readonly modus)
    virtual
    row_of_objects_t*
    create_calltree_id_maps( IDdeliverer*,
                             Cnode*,
                             row_of_objects_t*  );                                              // /< Creates apropriate enumeration of the calltree. Exclusive metric need deep search, Inclusive -> wide search enumeration.


    void
    set_dimensions( std::vector<Cnode*>&,
                    std::vector<Cnode*>&,
                    std::vector<Machine*>&,
                    std::vector<Thread*>& );                                                                     // /< saves inside of the object Metric information about another dimensions, their roots and flat list of elements.



    inline
    bool
    isInitialized()
    {
        return initialized;
    };                                                                            // /< Metric internal flag, which invokes initialization by first data acces .


    inline
    bool
    isInactive()
    {
        return !active;
    };                                                                          // /< Metrics internal flag, whiche signalizes,, that metric is not being active and should be not stored.


    virtual
    bool
    isBuildIn()
    {
        return false;
    };



    inline
    void
    setActive( bool _flag )
    {
        active = _flag;
    };                                                                          // /< Set the metric active or inactive.


    inline
    bool
    isGhost() const
    {
        return is_ghost == CUBE_METRIC_GHOST;
    }


    inline
    bool
    isRowWise()
    {
        return is_rowwise;
    }



    virtual bool
    asInclusiveMetric();

    virtual bool
    asExclusiveMetric();

    // / Returns true if the value is INTEGER
    bool
    isIntegerType();


    std::vector<std::string>
    split_dtype_args();                                                             // /< Strips data type parameters and stores them in a std::vector. Must be called before first call to get_data_type().
    DataType
    get_data_type();                                                                // /< Returns the data type of this metric.
    virtual IndexFormat
    get_index_format();                                                             // /< Returns format of the index, used in this metric (used ... where?)

    void
    set_adv_sev( Cnode*  cnode,
                 Thread* thrd,
                 double  value );                                                   // /< Saves the double value into the underlying AdvancedMatrix.
    double
    get_adv_sev( Cnode*  cnode,
                 Thread* thrd );              // /< Returns the double representation of the value in the underlyoung AdvancedMatrix

    void
    set_adv_sev_adv( Cnode*  cnode,
                     Thread* thrd,
                     Value*  value );                                               // /< Saves the Value into AdvancedMatrix. It is the main "set_" method. All another call this method after needed transformations.
    Value*
    get_adv_sev_adv( Cnode*  cnode,
                     Thread* thrd );                                                // /< returns the Value from the AdvancedMatrix. It is the main "get_" method.


// -------- iterators for optimal data saving only along the calltree, threads can be saved in any sequance---------------
    std::vector<Cnode*>
    get_optimal_sequence( const std::vector<Cnode*>& );                                  // /< Returns the sequence of the cnodes. Using this sequence the neibourghood relation for calculation of excl/incl values is optimal.


    void
    setStrategy( CubeStrategy strategy ); // /< Setting the strategy for the metric

    void
    dropRow( Cnode* cnode );  // /< Attempts to drop the row to control the memory footprint manually. Only if strategy of metric allwos it

    virtual
    void
    dropAllRows();

// ---------------------------------------------------------
// ----------- get incl/ecl severities ----------------------


    // /  Calculates inclusive value for given thread. Expands only calltree. Returns double representation of Value if possible.
    virtual double
    get_sev( Cnode*             cnode,
             CalculationFlavour cfv,
             Sysres*            sys,
             CalculationFlavour sf );


    // / Calculates inclusive value for given cnode and over all threads. Sums up over all threads. Assumed that all threads have continueos ids. Expands only calltree. Returns double representation of Value if possible.
    virtual double
    get_sev( Cnode*             cnode,
             CalculationFlavour cfv );

    // /  Calculates inclusive value for given thread. Expands only calltree.
    virtual Value*
    get_sev_adv( Cnode*             cnode,
                 CalculationFlavour cfv,
                 Sysres*            sys,
                 CalculationFlavour sf );


    // / Calculates inclusive value for given cnode and over all threads. Sums up over all threads. Assumed that all threads have continueos ids. Expands only calltree.
    virtual Value*
    get_sev_adv( Cnode*             cnode,
                 CalculationFlavour cfv );

    // -----------adv end -------------------------------

// ---------------------------------------------------------
// ----------- rowwise get incl/ecl severities ----------------------
// /  Calculates inclusive value for every thread. Expands only calltree. Returns double representation of Value if possible.
    virtual double*
    get_saved_sevs( Cnode* cnode
                    );

    // /  Calculates inclusive value for every thread. Expands only calltree. Returns double representation of Value if possible.
    virtual Value**
    get_saved_sevs_adv( Cnode* cnode
                        );

    // /  Calculates inclusive value for every thread. Expands only calltree. Returns double representation of Value if possible.
    virtual char*
    get_saved_sevs_raw( Cnode* cnode
                        );

    // /  Calculates inclusive value for every thread. Expands only calltree. Returns double representation of Value if possible.
    virtual double*
    get_sevs( Cnode*             cnode,
              CalculationFlavour cfv
              );

    // /  Calculates inclusive value for every thread. Expands only calltree. Returns double representation of Value if possible.
    virtual Value**
    get_sevs_adv( Cnode*             cnode,
                  CalculationFlavour cfv
                  );

    // /  Calculates inclusive value for every thread. Expands only calltree. Returns double representation of Value if possible.
    virtual char*
    get_sevs_raw( Cnode*             cnode,
                  CalculationFlavour cfv
                  );

    void
    set_sevs_raw( Cnode* cnode,
                  char*  val
                  );


    // -----------adv end -------------------------------



    virtual
    void
    invalidateCachedValue( Cnode*             cnode,
                           CalculationFlavour cf,
                           Sysres*            sysres = 0,
                           CalculationFlavour sf = CUBE_CALCULATE_INCLUSIVE );

    virtual
    void
    invalidateCache( void );

    void
    isUsedByOthers( bool value );


protected:
    std::string              disp_name;
    std::string              uniq_name;
    std::string              dtype;
    std::string              dtype_arg_str;
    std::vector<std::string> dtype_arg_vec;

    std::string          uom;
    std::string          val;      // optional
    std::string          url;
    std::string          descr;
    DataType             own_data_type;
    Value*               metric_value;           // /< Exemplar of the Value, which clones all others exemplats of Value for  calculation.
    FileFinder*          filefinder;
    std::string          expression;
    std::string          expression_init;
    std::string          expression_aggr_plus;
    std::string          expression_aggr_minus;
    CubePLMemoryManager* cubepl_memory_manager;
    GeneralEvaluation*   evaluation;
    GeneralEvaluation*   init_evaluation;
    GeneralEvaluation*   aggr_plus_evaluation;
    GeneralEvaluation*   aggr_minus_evaluation;
    bool                 is_rowwise;
    bool                 initialized;
    bool                 active;
    bool                 used_by_others;
    VizTypeOfMetric      is_ghost;
    RowWiseMatrix*       adv_sev_mat;             // /< Matrix, saving all values.
    Cache*               cache;

    CubeStrategy my_strategy;                     // /< Saves metrics own strategy. Used for postponed initialization.

    unsigned ncid;                                // /< Layout of the metric
    unsigned ntid;                                // /< Layout of the metric


    std::vector<signed int> calltree_local_ids; // /< Used to optimise calculation of inclusive/exclusive values.
    std::vector<signed int> system_local_ids;   // at this time no local id are needed ... coz all threads are leaves in the tree...

    std::vector<Cnode*> cnodev;                 // /< Stores the flat list of cnodes
    std::vector<Cnode*> roots_cnodev;           // /< Stores the list of roots of the call tree dimension

    std::vector<Machine*> roots_sysv;           // /< Stores the roots gof system dimension
    std::vector<Thread*>  sysv;                 // /< Stores the flat list of threads.

    void
    printLocalCallTreeIDs();                        // /< Prints the metric own enumeration of calltree. Used for debugging.


    virtual void
    post_calculation_cleanup();

    virtual void
    pre_calculation_preparation( Cnode* );

    virtual void
    pre_calculation_preparation( Cnode*,
                                 Sysres* );


    void
    setup_cubepl_memory();

    virtual void
    setup_cubepl_memory_for_itself();

    virtual void
    setup_cubepl_memory_for_cnode( Cnode* );

    virtual void
    setup_cubepl_memory_for_region( Region* );

    virtual void
    setup_cubepl_memory_for_sysres( Sysres* );

    virtual
    void
    cache_selection( uint32_t     n_cid,
                     uint32_t     n_tid,
                     TypeOfMetric type );
};


// / Comparing of two metrics by comparing of their unique names.
inline bool
operator==( const Metric& a, const Metric& b )
{
    return a.get_uniq_name() == b.get_uniq_name();
}
}


#endif
