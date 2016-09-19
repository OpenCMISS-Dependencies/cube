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



#ifndef STATISTICS_H
#define STATISTICS_H



#include "Cube.h"
#include "BoxPlot.h"
#include "CubeServices.h"
#include "StatisticalInformation.h"
#include "PluginServices.h"

#include <utility>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>

class Statistics
{
public:
    Statistics( PluginServices* service );
    bool
    existsStatFile() const;
    QString
    getStatFileName() const;
    bool
    existsStatistics( cube::Metric const* metric ) const;
    QStringList
    getStatistics( cube::Metric const* metric ) const;
    StatisticalInformation
    getInfo( cube::Metric const* metric ) const;

    bool
    existsMaxSeverity( cube::Metric const* metric,
                       cube::Cnode const*  cnode = 0 ) const;
    bool
    existsMaxSeverity( cube::Metric const* metric,
                       cube::Cnode const*  cnode,
                       double&             enter,
                       double&             exit ) const;

    uint32_t
    findMaxSeverityId( cube::Metric const* metric ) const;

    void
    showMaxSeverityText( QWidget*            parent,
                         const QString&      label,
                         const cube::Metric* metric,
                         const cube::Cnode*  cnode = 0 ) const;

    ~Statistics();

private:
    struct SevereEvent
    {
        SevereEvent( std::ifstream& theFile );
        double   enter, exit;
        double   wastedTime;
        int      rank;
        uint32_t cnode_id;
    };
    typedef std::map<std::string,
                     std::pair<StatisticalInformation,
                               std::vector<SevereEvent>
                               >
                     > severeEventsType;

    std::map<std::string,
             std::pair<StatisticalInformation, std::vector<SevereEvent> >
             > severeEvents;

    SevereEvent const*
    findMaxSeverity( cube::Metric const* metric,
                     cube::Cnode const*  cnode ) const;

    QString         fileName;
    bool            fileExists;
    bool            verbose;
    PluginServices* service;
};

#endif
