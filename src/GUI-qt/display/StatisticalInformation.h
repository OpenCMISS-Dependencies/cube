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



#ifndef STATISTICAL_H
#define STATISTICAL_H

#include "Constants.h"

#include <utility>
#include <fstream>
#include <string>
#include <list>
#include <QStringList>
class StatisticalInformation
{
public:
    StatisticalInformation( std::ifstream& theFile );
    StatisticalInformation();
    StatisticalInformation( int         theCount,
                            double      theSum,
                            double      theMean,
                            double      min,
                            double      q25,
                            double      theMedian,
                            double      q75,
                            double      max,
                            double      theVariance,
                            std::string theName = "",
                            std::string theUom = "" );
    QStringList
    Print( std::string const& patternName = std::string( "" ),
           std::string const  uom = std::string( "" ),
           PrecisionFormat    format = FORMAT_DOUBLE
           ) const;
    bool
    isStatisticalInformationValid();

public:
    int         count;
    bool        statInformation;
    double      sum, mean, minimum, q1, median, q3, maximum, variance;
    bool        valid_values;
    QStringList data_status_description;
    std::string name;
    std::string uom; // unit of measurement
};



#endif
