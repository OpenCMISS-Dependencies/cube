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
 * Opens a big created cube report and reqds some values.
 */

#include <iostream>
#include <cmath>
#include <iomanip>

#include "Cube.h"
#include "CubeMetric.h"
#include "CubeCnode.h"
#include "CubeThread.h"
#include "CubeValue.h"

using namespace std;
using namespace cube;



int
main( int argc, char** argv )
{
    const double epsilon = 1e-11;
    try
    {
        Cube cube;

        vector<Metric*> metrics;
        vector<Cnode*>  cnodes;
        vector<Thread*> threads;

        cout << " Open cube tau_example.cubex...";
        cube.openCubeReport( "tau_example.cubex" );

        metrics = cube.get_metv();
        cnodes  = cube.get_cnodev();
        threads = cube.get_thrdv();
        cout << "done" << endl;
        Value* v1 = cube.get_saved_sev_adv( metrics[ 0 ], cnodes[ 1 ], threads[ 0 ] );
        cout << v1->getString() << endl;
        Value* v2 = cube.get_saved_sev_adv( metrics[ 0 ], cnodes[ 1 ], threads[ 1 ] );
        cout << v2->getString() << endl;
        Value* v3 = cube.get_saved_sev_adv( metrics[ 0 ], cnodes[ 2 ], threads[ 0 ] );
        cout << v3->getString() << endl;
        Value* v4 = cube.get_saved_sev_adv( metrics[ 0 ], cnodes[ 2 ], threads[ 1 ] );
        cout << v4->getString() << endl;
        Value* v5 = cube.get_saved_sev_adv( metrics[ 1 ], cnodes[ 2 ], threads[ 0 ] );
        cout << v5->getString() << endl;
        Value* v6 = cube.get_saved_sev_adv( metrics[ 2 ], cnodes[ 2 ], threads[ 0 ] );
        cout << v6->getString() << endl;
//     Value * v2 = cube.get_sev_adv(metrics[0], CUBE_CALCULATE_EXCLUSIVE, cnodes[0], CUBE_CALCULATE_INCLUSIVE, (Sysres*)threads[0], CUBE_CALCULATE_INCLUSIVE);
//     cout << v2->getString() << endl;
//     Value * v3 = cube.get_sev_adv(metrics[0], CUBE_CALCULATE_EXCLUSIVE);
//     cout << v3->getString() << endl;
//     Value * v4 = cube.get_sev_adv(metrics[1], CUBE_CALCULATE_INCLUSIVE, cnodes[0], CUBE_CALCULATE_INCLUSIVE, (Sysres*)threads[1], CUBE_CALCULATE_INCLUSIVE);
//     cout << v4->getString() << endl;
//     Value * v5 = cube.get_sev_adv(metrics[1], CUBE_CALCULATE_INCLUSIVE, cnodes[0], CUBE_CALCULATE_INCLUSIVE);
//     cout << v5->getString() << endl;


        delete v1;
        delete v2;
        delete v3;
        delete v4;
        delete v5;
        delete v6;

//     Value * vv1 = cube.get_sev_adv(metrics[0], CUBE_CALCULATE_INCLUSIVE, cnodes[0], CUBE_CALCULATE_INCLUSIVE);
//     cout << vv1->getString() << endl;

        Value* vv1 = cube.get_sev_adv( metrics[ 1 ], CUBE_CALCULATE_INCLUSIVE );
        cout << vv1->getString() << endl;
        Value* vv2 = cube.get_sev_adv( metrics[ 0 ], CUBE_CALCULATE_INCLUSIVE );
        cout << vv2->getString() << endl;
        Value* vv3 = cube.get_sev_adv( metrics[ 0 ], CUBE_CALCULATE_EXCLUSIVE );
        cout << vv3->getString() << endl;


        delete vv1;
        delete vv2;
        delete vv3;


/*    int result =  (
                    fabs(v1 - 18. ) < epsilon &&
                    fabs(v2 - 9.  ) < epsilon &&
                    fabs(v3 + 311.) < epsilon &&
                    fabs(v4 +317. ) < epsilon  &&
                    fabs(vv1 - 3691.3) < epsilon  &&
                    fabs(vv2 - 223.8 ) < epsilon &&
                    fabs(vv3 - 7469.4) < epsilon  &&
                    fabs(vv4 + 567.9 ) < epsilon &&
                    fabs(vv5 + 691.3) < epsilon
                    )?0:-1;   */
/*
    delete vv1;
    delete vv2;
    delete vv3;
    delete vv4;
    delete vv5;
 */
    }
    catch ( const RuntimeError& er )
    {
        cerr << er.get_msg() << endl;
        return -1;
    }
    return 0;
}
