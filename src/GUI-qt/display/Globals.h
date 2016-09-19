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


#ifndef GLOBALS_H
#define GLOBALS_H

#include <QColor>
#include <QMainWindow>
#include <QTextStream>
#include "Constants.h"

class ColorMap;
class PrecisionWidget;
class MainWidget;
class TabManager;

enum MessageType { Verbose, Information, Warning, Error, Critical };

class Globals
{
    friend class MainWidget;
    friend class PluginManager;
public:
    /** returns the corresponding color of the currently selected colormap */
    static QColor
    getColor( double value,
              double minValue,
              double maxValue,
              bool   whiteForZero = true );

    /** prints the given text into the status line below the main widget */
    static void
    setStatusMessage( const QString& msg,
                      MessageType    type = Information );

    static QString
    formatNumber( double          value,
                  bool            integerType = false,
                  PrecisionFormat format = FORMAT_TREES );

    static QString
    formatNumber( double          value,
                  double          referenceValue,
                  bool            scientificSmall = false,
                  bool            integerType = false,
                  PrecisionFormat format = FORMAT_TREES );

    static double
    getRoundNumber( PrecisionFormat format );

    static double
    getRoundThreshold( PrecisionFormat format );

    static QMainWindow*
    getMainWindow();

    static TabManager*
    getTabManager();

    static void
    setVerbose( bool value );

    static QTextStream&
    debug( const QString& sender );

    static bool
    isExperimental();

    static void
    setExperimental( bool value );

private:
    static PrecisionWidget* precisionWidget;
    static ColorMap*        defaultColorMap;
    static ColorMap*        colorMap;
    static MainWidget*      mainWidget;
    static TabManager*      tabManager;
    static bool             verbose;
    static bool             experimental;
    static QTextStream*     outStream;

    static void
    initialize( MainWidget* m,
                TabManager* t );
    static PrecisionWidget*
    getPrecisionWidget();
    static void
    setColorMap( ColorMap* map );
    static void
    setDefaultColorMap();
    static ColorMap*
    getColorMap();
};

#endif // GLOBALS_H
