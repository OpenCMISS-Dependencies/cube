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


#include "Launch.h"
#include <QDebug>
#include <iostream>
#include "PluginServices.h"

/** executes the given command cmd and writes arguments to stdout */
void
Launch::launch( const QString& cmdline )
{
    QStringList args = cmdline.split( " ", QString::SkipEmptyParts );
    QString     cmd  = args.takeFirst();

    if ( cmd.isEmpty() )
    {
        return;
    }
    connect( this, SIGNAL( readyReadStandardOutput() ), this, SLOT( readStdout() ) );
    connect( this, SIGNAL( error( QProcess::ProcessError ) ), this, SLOT( error( QProcess::ProcessError ) ) );
    connect( this, SIGNAL( finished( int ) ), this, SLOT( launchFinished( int ) ) );
    this->start( cmd, args );
}

void
Launch::launchFinished( int exitCode )
{
    if ( exitCode != 0 )
    {
        std::cerr << "LaunchPlugin: execution finished with error: " << exitCode << std::endl;
    }
    this->deleteLater();
}

void
Launch::error( QProcess::ProcessError err )
{
    std::cerr << "LaunchPlugin: launch execution finished with error: " << err << std::endl;
}

void
Launch::readStdout()
{
    char   buf[ 1024 ];
    qint64 lineLength;
    do
    {
        lineLength = this->readLine( buf, sizeof( buf ) );
        if ( lineLength != -1 )
        {
            if ( ( buf[ 0 ] == '%' ) && ( isalpha( buf[ 1 ] ) ) && ( buf[ 2 ] == '=' ) )
            {
                QString str = QString( &buf[ 3 ] );
                QString var = QString( '%' ).append( buf[ 1 ] );
                QString val = str.trimmed();
                emit    receivedVar( QPair<QString, QString>( var, val ) );
            }
        }
    }
    while ( lineLength > 0 );
}
