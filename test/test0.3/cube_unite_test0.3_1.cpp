/****************************************************************************
**  CUBE        http://www.score-p.org/                                    **
**  SCALASCA    http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 1998-2015                                                **
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
 * Opens a big created cube report and reqds some values.
 */

#include <iostream>
#include <cmath>
#include <iomanip>

#include "Cube.h"

using namespace std;
using namespace cube;



int
main( int argc, char** argv )
{
    const double epsilon = 1e-11;
    Cube         cube;

    vector<Metric*> metrics;
    vector<Cnode*>  cnodes;
    vector<Thread*> threads;
    cout << " Open cube values_support.cubex..." << endl;
    cube.openCubeReport( "values_support.cubex" );
//     cube.openCubeReport("tau_example.cubex");


    metrics = cube.get_metv();
    cnodes  = cube.get_cnodev();
    threads = cube.get_thrdv();



    for ( vector<Metric*>::iterator m_iter = metrics.begin(); m_iter != metrics.end(); m_iter++ )
    {
        cout <<  "List of the saved values in the metric " << ( *m_iter )->get_uniq_name() << endl;
        for ( vector<Cnode*>::iterator c_iter = cnodes.begin(); c_iter != cnodes.end(); c_iter++ )
        {
            cout <<  "List of the saved values in the cnode " << ( *c_iter )->get_callee()->get_name() << endl;
            for ( vector<Thread*>::iterator s_iter = threads.begin(); s_iter != threads.end(); s_iter++ )
            {
                Value* _v = cube.get_sev_adv( *m_iter, *c_iter, *s_iter );
                cout <<   ( *s_iter )->get_name() << " : " << _v->getString() << endl;
                delete _v;
            }
        }
    }



    Cube* outCube = new Cube( cube, CUBE_DEEP_COPY );

    outCube->writeCubeReport( "values_support2" );
    delete outCube;



    return 0;
}
