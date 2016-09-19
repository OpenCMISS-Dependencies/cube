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



#include "StatisticalInformation.h"
#include "Globals.h"
#include "Environment.h"
#include "CubeMetric.h"
#include "CubeCnode.h"
#include "CubeServices.h"

#include <math.h>
#include <iomanip>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include <stdexcept>
#include <limits>

#include <iostream>

using namespace std;

namespace
{
void
ReadWhitespaces( istream& theFile )
{
    char check = theFile.peek();
    while ( check == ' ' || check == '\n' )
    {
        theFile.ignore();
        check = theFile.peek();
    }
}
}

StatisticalInformation::StatisticalInformation( ifstream& theFile )
{
    mean = median = minimum = maximum = sum = variance = q1 = q3 = 0;
    theFile >> count;
    ReadWhitespaces( theFile );
    if ( theFile.peek() != '-' )
    {
        theFile >> mean;
        theFile >> median;
        theFile >> minimum;
        theFile >> maximum;
        theFile >> sum;
        if ( count >= 2 )
        {
            theFile >> variance;
        }
        if ( count >= 5 )
        {
            theFile >> q1;
            theFile >> q3;
        }
        statInformation = true;
    }
    else
    {
        statInformation = false;
    }

    valid_values = isStatisticalInformationValid();
}

StatisticalInformation::StatisticalInformation()
{
    count           = 0;
    sum             = 0;
    mean            = 0;
    minimum         = 0;
    q1              = 0;
    median          = 0;
    q3              = 0;
    maximum         = 0;
    variance        = 0;
    name            = "";
    uom             = "";
    statInformation = false;
    valid_values    = false;
    data_status_description.clear();
}


StatisticalInformation::StatisticalInformation( int         theCount,
                                                double      theSum,
                                                double      theMean,
                                                double      min,
                                                double      q25,
                                                double      theMedian,
                                                double      q75,
                                                double      max,
                                                double      theVariance,
                                                std::string theName,
                                                std::string theUom )
    : count( theCount ), statInformation( true ), sum( theSum ), mean( theMean ), minimum( min ), q1( q25 ),
      median( theMedian ), q3( q75 ), maximum( max ), variance( theVariance ), valid_values( true ), data_status_description(),
      name( theName ), uom( theUom )
{
    valid_values = isStatisticalInformationValid();
}

/**
 * Performs a sequence of checks, whether statistical data looks meaningfull or not.
 */
bool
StatisticalInformation::StatisticalInformation::isStatisticalInformationValid()
{
    data_status_description.clear();
    bool _return = true;
    if ( minimum > maximum )
    {
        data_status_description.append( "Min value is larger than Max." );
        _return = false;                   // the world is up side down
    }
    if ( mean > maximum && mean < minimum )
    {
        data_status_description.append( "Mean value is out of range [minimum, maximum]." );
        _return = false;
    } // mean is out of range
    if ( median > maximum && median < minimum )
    {
        data_status_description.append( "Median value is out of range [minimum, maximum]." );
        _return = false;
    } // median is out of range
    if ( maximum * count  < sum )
    {
        data_status_description.append( "Sum is larger than \"count * maximum\"." );
        _return = false;
    } // sum is bigger that count times maximum
    if ( ( minimum * count )  > sum )
    {
        data_status_description.append( "Sum is smaller than \"count * minimum\"." );
        _return = false;
    } // sum is lower that count times minimum
    if ( count >= 2 )
    {
        if ( variance < 0 )
        {
            data_status_description.append( "Variance is negative." );
            _return = false;
        } // varianvve cannot be negativ

        if ( count >= 5 )
        {
            if ( q1 > q3 )
            {
                data_status_description.append( "25% quantile is larger than 75% quantile." );
                _return = false;
            } // quantile 25% is bigger than 75%
            if ( median > q3 )
            {
                data_status_description.append( "Medium (50% quantile) is larger than 75% quantile." );
                _return = false;
            } // definition of medium : 50% and it cannot be bigger than 75%
            if ( median < q1 )
            {
                data_status_description.append( "Medium (50% quantile) is smaller than 25% quantile." );
                _return = false;
            } // definition of medium : 50% and it cannot be smaller  than 25%
        }
    }
    return _return;
}


#define OUTPUT_PERCENT( P ) ( fabs( maximum ) > 10e-6 ) ? ( percentStream << " " << ( int )( ( P ) * 100 / maximum + .5 ) << "%" << endl ) : percentStream << endl;
QStringList
StatisticalInformation::Print( string const& patternName, string const uom, PrecisionFormat format ) const
{
    stringstream percentStream;
    stringstream theStream;
    string       captionString;
    if ( patternName.length() > 0 )
    {
        captionString += "Pattern: \n";
        theStream << patternName.c_str() << endl;
        percentStream << endl;
    }

    if ( valid_values )
    {
        if ( statInformation )
        {
            captionString += "Sum: \n";
            captionString += "Count: \n";
            captionString += "Mean: \n";
            theStream << Globals::formatNumber( sum, false, format ).toStdString().c_str();
            if ( uom.length() != 0 )
            {
                theStream << " " << uom.c_str();
            }
            theStream << endl;
            theStream << Globals::formatNumber( count, true, format ).toStdString().c_str() << endl;
            theStream << Globals::formatNumber( mean, false, format ).toStdString().c_str();
            if ( uom.length() != 0 )
            {
                theStream << " " << uom.c_str();
            }
            theStream << endl;
            percentStream << endl << endl;
            OUTPUT_PERCENT( mean );

            if ( count >= 2 )
            {
                captionString += "Standard deviation: \n";
                theStream << Globals::formatNumber( sqrt( variance ), false, format ).toStdString().c_str();
                if ( uom.length() != 0 )
                {
                    theStream << " " << uom.c_str();
                }
                theStream << endl;
                OUTPUT_PERCENT( sqrt( variance ) );
            }

            captionString += "Maximum: \n";
            theStream << Globals::formatNumber( maximum, false, format ).toStdString().c_str();
            if ( uom.length() != 0 )
            {
                theStream << " " << uom.c_str();
            }
            theStream << endl;
            OUTPUT_PERCENT( maximum );

            if ( count >= 5 )
            {
                captionString += "Upper quartile (Q3): \n";
                theStream << Globals::formatNumber( q3, false, format ).toStdString().c_str();
                if ( uom.length() != 0 )
                {
                    theStream << " " << uom.c_str();
                }
                theStream << endl;
                OUTPUT_PERCENT( q3 );
            }
            captionString += "Median: \n";
            theStream << Globals::formatNumber( median, false, format ).toStdString().c_str();
            if ( uom.length() != 0 )
            {
                theStream << " " << uom.c_str();
            }
            theStream << endl;
            OUTPUT_PERCENT( median );
            if ( count >= 5 )
            {
                captionString += "Lower quartile (Q1): \n";
                theStream << Globals::formatNumber( q1, false, format ).toStdString().c_str();
                if ( uom.length() != 0 )
                {
                    theStream << " " << uom.c_str();
                }
                theStream << endl;
                OUTPUT_PERCENT( q1 );
            }
            captionString += "Minimum: ";
            theStream << Globals::formatNumber( minimum, false, format ).toStdString().c_str();
            if ( uom.length() != 0 )
            {
                theStream << " " << uom.c_str();
            }
            OUTPUT_PERCENT( minimum );
        }
        else
        {
            captionString += "Count: \n";
            theStream << Globals::formatNumber( count, true, format ).toStdString().c_str() << endl;
            percentStream << endl;
        }
    }
    else
    {
        theStream << "Statistical data seems to be bogus. Several errors are found (listed below): \n" << endl
                  << data_status_description.join( "\n" ).toUtf8().data() << endl;
    }
    QStringList list;
    list.append( QString::fromStdString( captionString ) );
    list.append( QString::fromStdString( theStream.str() ) );
    list.append( QString::fromStdString( percentStream.str() ) );
    return list;
}
