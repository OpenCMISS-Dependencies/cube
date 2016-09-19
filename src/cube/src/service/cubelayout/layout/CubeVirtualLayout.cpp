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
 * \file CubeVirtualLayout.cpp
 * \brief Implements common parts of the interface for embedded layout of CUBE report.
 */
#ifndef __VIRTUAL_LAYOUT_CPP
#define __VIRTUAL_LAYOUT_CPP

#include "CubeTypes.h"
#include "CubeSysres.h"
#include "CubeMetric.h"
#include "CubeError.h"
#include "CubeVirtualLayout.h"

using namespace cube;
using namespace std;

std::string
VirtualLayout::getPathToAnchor()
{
    throw NoFileError("");
    return "";
}
std::string
VirtualLayout::getPathToData()
{
    throw NoFileError("");
    return "";
}

std::string
VirtualLayout::getPathToMetricData( cube::Metric* met )
{
    throw NoFileError("");
    return "";
}

std::string
VirtualLayout::getPathToMetricIndex( cube::Metric* met )
{
    throw NoFileError("");
    return "";
}

std::string
VirtualLayout::getAnchorName()
{
    throw NoFileError("");
    return "anchor" + getOwnAnchorExtension();
}

std::string
VirtualLayout::getMetricDataName( cube::Metric* met )
{
    throw NoFileError("");
    return "";
}

std::string
VirtualLayout::getMetricIndexName( cube::Metric* met )
{
    throw NoFileError("");
    return "";
}


#endif
