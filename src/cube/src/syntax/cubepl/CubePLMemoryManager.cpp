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


#ifndef __CUBEPL_MEMORY_MANAGER_CPP
#define __CUBEPL_MEMORY_MANAGER_CPP 0

#include <vector>
#include <iostream>
#include <sstream>
#include <float.h>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <string>

#include "CubePLMemoryManager.h"
#include "CubePLMemoryInitializer.h"
#include "CubeError.h"


using namespace std;
using namespace cube;


CubePLMemoryManager::~CubePLMemoryManager()
{
    delete cubepl_memory_initializer;
};


#endif
