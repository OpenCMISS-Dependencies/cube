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
 * \file CubeError.cpp
 * \brief Define the output functions for every kind of error.
 */
#include "CubeError.h"

using namespace std;
using namespace cube;

namespace cube
{
ostream&
operator <<( ostream& out, Error& exception )
{
    out << exception.get_msg() << endl;
    return out;
}

ostream&
operator <<( ostream& out, RuntimeError& exception )
{
    out << exception.get_msg() << endl;
    return out;
}

ostream&
operator <<( ostream& out, FatalError& exception )
{
    out << exception.get_msg() << endl;
    return out;
}
}
