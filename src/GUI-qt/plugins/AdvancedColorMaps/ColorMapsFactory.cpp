/****************************************************************************
**  CUBE        http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 1998-2016                                                **
**  Forschungszentrum Juelich GmbH, Juelich Supercomputing Centre          **
**                                                                         **
**  This software may be modified and distributed under the terms of       **
**  a BSD-style license.  See the COPYING file in the package base         **
**  directory for details.                                                 **
****************************************************************************/



#include <stdexcept>

#include "maps/SequentialColorMap.h"
#include "maps/DivergentColorMap.h"
#include "maps/CubehelixColorMap.h"
#include "maps/ImprovedRainbowColorMap.h"
#include "ColorMapsFactory.h"

using std::runtime_error;

const ColorMapsFactory::ColorMapsNames ColorMapsFactory::mapNames = ColorMapsFactory::initializeMap();

// ColorMapsFactory & ColorMapsFactory::getInstance()
// {
//	static ColorMapsFactory    instance;
//	return instance;
// }

ColorMapsFactory::~ColorMapsFactory()
{
    for ( std::map<ColorMaps, ColorMapExtended*>::iterator it = mapObjects.begin();
          it != mapObjects.end(); ++it )
    {
        delete ( *it ).second;
    }
}

ColorMapsFactory::ColorMapsNames
ColorMapsFactory::initializeMap()
{
    ColorMapsFactory::ColorMapsNames ret;
    ret[ SEQUENTIAL ]       = "Sequential";
    ret[ DIVERGENT ]        = "Divergent";
    ret[ CUBEHELIX ]        = "Cubehelix";
    ret[ IMPROVED_RAINBOW ] = "Improved rainbow";
    return ret;
}

ColorMapExtended*
ColorMapsFactory::getColorMap( ColorMaps map )
{
    std::map<ColorMaps, ColorMapExtended*>::iterator it = mapObjects.find( map );
    if ( it != mapObjects.end() )
    {
        // filtering values may have been changed in other widget
        return ( *it ).second;
    }
    // map currently doesn't exist, create the instance
    ColorMapExtended* ret = NULL;
    switch ( map )
    {
        case SEQUENTIAL:
            ret = new SequentialColorMap();
            break;
        case DIVERGENT:
            ret = new DivergentColorMap();
            break;
        case CUBEHELIX:
            ret = new CubehelixColorMap();
            break;
        case IMPROVED_RAINBOW:
            ret = new ImprovedRainbowColorMap();
            break;
        default:
            throw runtime_error( "Illegal value of enum ColorMapsFactory::ColorMaps!" );
            break;
    }
    mapObjects[ map ] = ret;
    return ret;
}

/**
 * @param color map instance
 * @return enum value corresponding to this color map
 */
ColorMapsFactory::ColorMaps
ColorMapsFactory::getColorMapIndex( ColorMapExtended* map )
{
    if ( dynamic_cast<DivergentColorMap*>( map ) )
    {
        return DIVERGENT;
    }
    else if ( dynamic_cast<SequentialColorMap*>( map ) )
    {
        return SEQUENTIAL;
    }
    else if ( dynamic_cast<ImprovedRainbowColorMap*>( map ) )
    {
        return IMPROVED_RAINBOW;
    }
    else
    {
        return CUBEHELIX;
    }
}
