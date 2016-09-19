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


#include "Globals.h"
#include "Coloring.h"
#include "ColorMap.h"
#include "PrecisionWidget.h"
#include "MainWidget.h"
#include "ColorMap.h"
#include "TabManager.h"

MainWidget*      Globals::mainWidget      = 0;
TabManager*      Globals::tabManager      = 0;
ColorMap*        Globals::defaultColorMap = 0;
ColorMap*        Globals::colorMap        = 0;
PrecisionWidget* Globals::precisionWidget = 0;
bool             Globals::verbose         = 0;
bool             Globals::experimental    = 0;
QTextStream*     Globals::outStream       = 0;


class DebugIODevice : public QIODevice
{
public:
    DebugIODevice()
    {
        open( QIODevice::WriteOnly | QIODevice::Text );
        enabled = true;
    }

    void
    setEnabled( bool enabled )
    {
        this->enabled = enabled;
    }
protected:
    qint64
    readData( char*, qint64 )
    {
        return 0;
    }
    qint64
    writeData( const char* data, qint64 maxSize )
    {
        if ( enabled )
        {
            QString msg = QString( data );
            msg = msg.remove( QRegExp( "\\s+$" ) );
            Globals::setStatusMessage( msg, Verbose );
        }
        return maxSize;
    }
private:
    bool enabled;
};

QTextStream&
Globals::debug( const QString& sender )
{
    if ( outStream == 0 )
    {
        DebugIODevice* device = new DebugIODevice();
        device->setEnabled( verbose );
        outStream = new QTextStream( device );
    }
    *outStream << sender << ": ";
    return *outStream;
}

PrecisionWidget*
Globals::getPrecisionWidget()
{
    return precisionWidget;
}

QColor
Globals::getColor( double value, double minValue, double maxValue, bool whiteForZero )
{
    return colorMap->getColor( value, minValue, maxValue, whiteForZero );
}

void
Globals::setDefaultColorMap()
{
    setColorMap( defaultColorMap );
}

void
Globals::setStatusMessage( const QString& msg, MessageType type )
{
    if ( mainWidget )
    {
        mainWidget->setMessage( msg, type );
    }
}

QString
Globals::formatNumber( double value, bool integerType, PrecisionFormat format )
{
    return precisionWidget->numberToQString( value, integerType, format );
}

/** converts the number using the given precision format; in scientific format, the same exponent as
 * the reference value is used
 * @param value the number to convert
 * @param referenceValue the number that is used to determine the format
 * @param integerType the type of the number is integer if "integerType" is true, otherwise the type is double
 * @param format FORMAT_TREES or FORMAT_DEFAULT
 * @param scientificSmallValue use scientificSmallValue use scientific notations for values below 0.1
 */
QString
Globals::formatNumber( double value, double referenceValue, bool integerType, bool scientificSmall, PrecisionFormat format )
{
    return precisionWidget->numberToQString( value, referenceValue, integerType, scientificSmall, format );
}

double
Globals::getRoundThreshold( PrecisionFormat format )
{
    return precisionWidget->getRoundThreshold( format );
}

double
Globals::getRoundNumber( PrecisionFormat format )
{
    return precisionWidget->getRoundNr( format );
}

QMainWindow*
Globals::getMainWindow()
{
    return mainWidget;
}

TabManager*
Globals::getTabManager()
{
    return tabManager;
}

void
Globals::setExperimental( bool value )
{
    experimental = value;
}

bool
Globals::isExperimental()
{
    return experimental;
}

void
Globals::setVerbose( bool value )
{
    verbose = value;
}

// -------------- private ---------------------

ColorMap*
Globals::getColorMap()
{
    return colorMap;
}

void
Globals::setColorMap( ColorMap* map )
{
    colorMap = map;
    tabManager->updateTreeItemProperties(); // update trees
    mainWidget->update();                   // update colormap widget
}

void
Globals::initialize( MainWidget* m, TabManager* t )
{
    mainWidget      = m;
    tabManager      = t;
    precisionWidget = new PrecisionWidget();
    defaultColorMap = new DefaultColorMap();
    colorMap        = defaultColorMap;
}
