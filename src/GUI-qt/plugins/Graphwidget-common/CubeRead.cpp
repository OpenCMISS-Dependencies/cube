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


#include "CubeRead.h"
#include "CubeMetric.h"
#include "CubeRegion.h"

#include <algorithm>
#include <string>
#include <QDebug>

#include <algorithm>
#include <iostream>
using namespace std;


CubeRead::CubeRead( cube::Cube* cube ) :  cube( cube )
{
    if ( cube == NULL )
    {
        throw string( "Cube is not loaded yet." );       // /< temporary exception to signalize that one cannot operate. Should be replaces.
    }
    generateData();
}

QVector<double>
CubeRead::getProcessesV( int metNum, int iterNum )
{
    return values.at( metNum ).at( iterNum );
}
QVector<double>
CubeRead::getThreadsV( int iterNum ) // REY
{
    return values_subItr.at( iterNum );
}

QString
CubeRead::getMetricName( int metNum )
{
    return metricsNames.at( metNum );
}
void
CubeRead::setLastMetricUName( int metNum )       // REY
{
    lastMetricUName = metricsNames.at( metNum ); // .toLower();
}
QString
CubeRead::getLastMetricUName() // REY
{
    return lastMetricUName;
}
unsigned int
CubeRead::getMetricNameSize()
{
    return metricsNames.size();
}

int
CubeRead::getIterations( QString metricName )
{
    for ( int i = 0; i < metricsNames.size(); i++ )
    {
        if ( metricsNames.at( i ).compare( metricName ) == 0 )
        {
            return values.at( i ).size();
        }
    }
    return -1;
}

int
CubeRead::getIterations( int metricNum )
{
    return getIterations( getMetricName( metricNum ) );
}
int
CubeRead::getIterations() // REY
{
    int tempSize =  values_subItr.size();
    if ( tempSize > 0 )
    {
        return tempSize;
    }
    return -1;
}


QVector<QVector<double> >
CubeRead::getAllProcesses( int metNum ) // REY
{
    return values.at( metNum );
}
int
CubeRead::getThreadsSize() // REY
{
    return values_subItr[ 0 ].size();
}
QVector<QVector<double> >
CubeRead::getAllResult_Itr() // REY
{
    return values_subItr;
}
pair<double, double>
CubeRead::getMinMaxValue_Itr() // REY
{
    return globalMinMax_Itr;
}
// -------------- protected methods --------------------


void
CubeRead::generateData()
{
    createListOfMetrics();
    createListOfCnodes();
    createListOfProcesses();
    createListOfThreads();

    calcAndFill();
}

void
CubeRead::createListOfMetrics()
{
    metricsNames.clear();
    metrics = cube->get_metv();
    for ( std::vector<cube::Metric*>::iterator m_iter = metrics.begin(); m_iter != metrics.end(); m_iter++ )
    {
        // metricsNames.push_back( QString::fromStdString( ( *m_iter )->get_uniq_name() ) );
        metricsNames.push_back( QString::fromStdString( ( *m_iter )->get_disp_name() ) );
    }
}
void
CubeRead::createListOfCnodes()
{
    iterations.clear();
    std::vector<cube::Cnode*> _all_cnodes = cube->get_cnodev();


    regex_t     rgT;
    std::string _iteration_regex = "iteration=";
//      QString _str_number = QString("%1").arg(loop_number);
//      _iteration_regex += _str_number.toUtf8().data();
    _iteration_regex += "[0-9]+$";


    int reti = regcomp( &rgT, _iteration_regex.c_str(), REG_EXTENDED );
    if ( reti != 0 )
    {
        throw string( "Couldn't compile regular expression: "  + _iteration_regex );  // Stupid regex;
    }
    for ( std::vector<cube::Cnode*>::iterator c_iter =  _all_cnodes.begin(); c_iter !=  _all_cnodes.end(); c_iter++ )
    {
        //   string s = ( *c_iter )->get_callee()->get_name() ;
        //  qDebug()<<s.c_str()<<"  "<< ( *c_iter )->get_id();


        if ( isIterationCnode( &rgT, ( *c_iter )->get_callee()->get_name() ) )
        {
            iterations.push_back( *c_iter );
        }
    }
    regfree( &rgT );
    if ( iterations.empty() )
    {
        setCubeHasItr( false );
        // throw string( QString( "Cannot find iterations" ).toUtf8().data() );     // didnt find any regions.
    }
    else
    {
        setCubeHasItr( true );
    }
}
void
CubeRead::createListOfSubCnodes( const QList<cube::Cnode*>& func )
{
    iterations_func.clear();
    iterations_func.assign( func.begin(), func.end() );
}
void
CubeRead::createListOfProcesses()
{
    processes = cube->get_procv();
}

void
CubeRead::createListOfThreads()
{
    threadsNames.clear();
    threads = cube->get_thrdv();
    for ( std::vector<cube::Thread*>::iterator t_iter = threads.begin(); t_iter != threads.end(); t_iter++ )
    {
        threadsNames.push_back( QString::fromStdString( ( *t_iter )->get_name() ) );
    }
    // qDebug()<<QString::fromStdString( ( *t_iter )->get_name() ) ;
}

void
CubeRead::calcAndFill()
{
    //  cout << "GENERATE DATA " << endl;

    // resize the arrays accordingly
    values.resize( metrics.size() );
    for ( size_t m = 0; m < metrics.size(); m++ )
    {
        values[ m ].resize( iterations.size() );
        for ( size_t i = 0; i < iterations.size(); i++ )
        {
            values[ m ][ i ].resize( processes.size() );
        }
    }


    unsigned i = 0;
    for ( std::vector<cube::Metric*>::iterator m_iter = metrics.begin(); m_iter != metrics.end(); m_iter++, i++ )
    {
        unsigned j = 0;
        for ( std::vector<cube::Cnode*>::iterator c_iter =  iterations.begin(); c_iter !=  iterations.end(); c_iter++, j++ )
        {
            unsigned k = 0;
            //    cout << " *************************************** " << endl;
            for ( std::vector<cube::Process*>::iterator p_iter = processes.begin(); p_iter != processes.end(); p_iter++, k++ )
            {
                cube::Value* _v = cube->get_sev_adv(
                    *m_iter, cube::CUBE_CALCULATE_INCLUSIVE,
                    *c_iter, cube::CUBE_CALCULATE_INCLUSIVE,
                    ( cube::Sysres* )( *p_iter ), cube::CUBE_CALCULATE_INCLUSIVE
                    );
                if ( _v != NULL )
                {
                    values[ i ][ j ][ k ] = _v->getDouble();
                }
                else
                {
                    values[ i ][ j ][ k ] = 0.;
                }
                //    cout << values[ i ][ j ][ k ] << endl;

                delete _v;
            }
        }
        //   cout << endl;
    }
}
void
CubeRead::calcAndFill_subItr( cube::CalculationFlavour calcType )
{
    vector <cube::Metric*> x =  cube->get_metv();
    cube::Metric*          m = NULL;

    for ( std::vector<cube::Metric*>::iterator m_iter = x.begin(); m_iter != x.end(); m_iter++ )
    {
        if ( ( *m_iter )->get_disp_name()  ==  getLastMetricUName().toStdString() )
        {
            m = cube->get_met( ( *m_iter )->get_uniq_name() );
            break;
        }
    }
    if ( m == NULL )
    {
        qDebug() << " metric not found!";
        return;
    }

    // resize the arrays accordingly
    values_subItr.resize( iterations_func.size() );
    for ( size_t i = 0; i < iterations_func.size(); i++ )
    {
        values_subItr[ i ].resize( threads.size() );
    }


    bool     flag = false;
    unsigned j    = 0;
    for ( std::vector<cube::Cnode*>::iterator c_iter =  iterations_func.begin(); c_iter !=  iterations_func.end(); c_iter++, j++ )
    {
        unsigned k = 0;
        for ( std::vector<cube::Thread*>::iterator p_iter = threads.begin(); p_iter != threads.end(); p_iter++, k++ )
        {
            cube::Value* _v = cube->get_sev_adv(
                m, calcType,
                *c_iter, calcType,
                ( cube::Location* )( *p_iter ), calcType
                );

            if ( _v != NULL )
            {
                values_subItr[ j ][ k ] = _v->getDouble();
            }
            else
            {
                values_subItr[ j ][ k ] = 0.;
            }

// cout<<values_subItr[ j ][ k ]<<endl;
            if ( flag == false )   // first comparison
            {
                globalMinMax_Itr.first = globalMinMax_Itr.second = values_subItr[ j ][ k ];
                flag                   = true;
            }
            else
            {
                if ( values_subItr[ j ][ k ] > globalMinMax_Itr.second  )
                {
                    globalMinMax_Itr.second = values_subItr[ j ][ k ];
                }

                if ( values_subItr[ j ][ k ] < globalMinMax_Itr.first  )
                {
                    globalMinMax_Itr.first = values_subItr[ j ][ k ];
                }
            }

            delete _v;
        }
    }
}

bool
CubeRead::isIterationCnode( regex_t* rgT, std::string region_name )
{
    char msgbuf[ 255 ];
    int  reti = regexec( rgT, region_name.c_str(), 0, NULL, 0 );
    if ( reti == 0 )
    {
        return true;
    }
    else
    if ( reti == REG_NOMATCH )
    {
        return false;
    }
    else
    {
        regerror( reti, rgT, msgbuf, sizeof( msgbuf ) );
        cerr << " Regex match failed" << msgbuf << endl;
        return false;
    };
}
bool
CubeRead::isIterationFuncCnode( vector<int> listID, int testID )
{
    for ( std::vector<int>::iterator iter = listID.begin(); iter !=  listID.end(); iter++ )
    {
        if ( ( *iter ) == testID )
        {
            return true;
        }
    }
    return false;
}
bool
CubeRead::getCubeHasItr()
{
    return cubeHasItr;
}
void
CubeRead::setCubeHasItr( bool temp )
{
    cubeHasItr = temp;
}
// QList<cube::Cnode*>
// CubeRead::getIterationsFunc()
// {
//   return iterations_func;
// }
// void
// CubeRead::setIterationsFunc(QList<cube::Cnode*>  iterations_funcTemp)
// {
//    iterations_func =  iterations_funcTemp;
// }
void
CubeRead::setCalcType( cube::CalculationFlavour calcTypeTemp )
{
    calcType = calcTypeTemp;
}

cube::CalculationFlavour
CubeRead::getCalcType()
{
    return calcType;
}
