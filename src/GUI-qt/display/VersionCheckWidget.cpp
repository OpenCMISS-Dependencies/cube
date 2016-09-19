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




#include "VersionCheckWidget.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

#include <QFontMetrics>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QDialog>
#include <QString>
#include <QIcon>
#include <QDebug>
#include <math.h>
#include <QTimer>
#include <QToolTip>
#include <QDesktopServices>

#include "Constants.h"
#include "Globals.h"
#include "Environment.h"

using namespace std;


VersionCheckWidget::VersionCheckWidget( QWidget* parent )
    : QToolButton( parent )
{
    recentVersion   = QIcon(  ":images/recentVersion.png" );
    updateAvailable = QIcon(  ":images/updateAvailable.png" );
    noCheckPossible = QIcon(  ":images/noCheckPossible.png" );

    shownIcon = noCheckPossible;
    setIcon( shownIcon );

    url              = QUrl( UPDATE_CHECK_URL );
    download         = NULL;
    update_Available = false;
    bool no_http = env_str2bool( getenv( "CUBE_DISABLE_HTTP_DOCS" ) );
    if ( !no_http )
    {
        updateDescription =   tr( "Check for update is not performed yet." );
        Globals::setStatusMessage( tr( "Check for new version...(I)" ) );
        download = new DownloadThread( url );
        connect( download, SIGNAL( downloadFinished( QUrl, QByteArray ) ),
                 this, SLOT( check( QUrl, QByteArray ) ) );
        QTimer::singleShot( 10000, download, SLOT( loadList() ) );
    }
    else
    {
        QString _status = tr( "Check for new version is disabled via environment variable CUBE_DISABLE_HTTP_DOCS=\"" ) + QString( getenv( "CUBE_DISABLE_HTTP_DOCS" ) ) + "\"";
        updateDescription =   _status;
        Globals::setStatusMessage( _status );
    }
}


void
VersionCheckWidget::mousePressEvent( QMouseEvent* event )
{
    QToolTip::showText( mapToGlobal( event->pos() ), updateDescription );
    if ( ( update_Available ) && event->button() == Qt::LeftButton )
    {
        QDesktopServices::openUrl( QUrl( UPDATE_URL, QUrl::StrictMode ) );
    }
}



void
VersionCheckWidget::check( const QUrl&, const QByteArray& buffer )
{
    Globals::setStatusMessage( tr( "Check for new version..." ) );
    updateDescription =   tr( "Check for latest CUBE version failed." );
    if ( !buffer.isNull() )
    {
        QString pattern( tr( CUBE_VERSION ) );

        QStringList currentVersionParts = pattern.split( QRegExp( "[\\.-]" ) );
        QString     currentMajor        = currentVersionParts.at( 0 ).trimmed().toLower();
        QString     currentMidle        = currentVersionParts.at( 1 ).trimmed().toLower();
        QString     currentMinor        = ( currentVersionParts.size() >= 3 ) ? currentVersionParts.at( 2 ).trimmed().toLower() : "0";

        QString content( buffer );

        QStringList list = content.split( "\n" );

        QStringListIterator javaStyleIterator( list );
        while ( javaStyleIterator.hasNext() )
        {
            QString     line = javaStyleIterator.next();
            QStringList pair = line.split( QRegExp( "\\s*:\\s*" ) );
            if ( pair.size() < 2 ) // ignore malformed strings
            {
                continue;
            }
            QString field = pair.at( 0 ).trimmed().toLower();
            QString value = pair.at( 1 ).trimmed().toLower();

            if ( field == "version" )
            {
                QStringList versionParts = value.split( QRegExp( "\\." ) );
                QString     major        = ( versionParts.size() > 0 ) ? versionParts.at( 0 ).trimmed().toLower() : "";
                QString     midle        = ( versionParts.size() > 1 ) ? versionParts.at( 1 ).trimmed().toLower() : "";
                QString     minor        = ( versionParts.size() > 2 ) ? versionParts.at( 2 ).trimmed().toLower() : "0";

                bool ok     = true;
                int  _major = major.toInt( &ok );
                if ( !ok )
                {
                    return;
                }
                int _midle = midle.toInt( &ok );
                if ( !ok )
                {
                    return;
                }
                int _minor = minor.toInt( &ok );
                if ( !ok )
                {
                    return;
                }
                int _currentMajor = currentMajor.toInt( &ok );
                if ( !ok )
                {
                    return;
                }
                int _currentMidle = currentMidle.toInt( &ok );
                if ( !ok )
                {
                    return;
                }
                int _currentMinor = currentMinor.toInt( &ok );
                if ( !ok )
                {
                    return;
                }

                if ( ( _major > _currentMajor ) ||
                     (
                         ( _major == _currentMajor ) &&
                         ( _midle > _currentMidle )
                     ) ||
                     (
                         ( _major == _currentMajor ) &&
                         ( _midle == _currentMidle ) &&
                         ( _minor > _currentMinor )
                     )
                     )
                {
                    shownIcon = updateAvailable;
                    setIcon( shownIcon );
                    updateDescription =  content;
                    update_Available  = true;
                    if (    ( _major > _currentMajor ) ||
                            (
                                ( _major == _currentMajor ) &&
                                ( _midle > _currentMidle )
                            )
                            )
                    {
                        QString _new_version = tr( "New major version of CUBE is available. Please visit <a href=\"http://www.vi-hps.org/projects/score-p/\"><nobr>http://www.vi-hps.org/projects/score-p/</nobr> </a> for download." );
                        Globals::setStatusMessage( _new_version );
                    }
                }
                else
                {
                    shownIcon = recentVersion;
                    setIcon( shownIcon );
                    updateDescription =   tr( "You have latest version of CUBE. No update is needed." );
                    Globals::setStatusMessage( updateDescription );
                }
            }
        }
    }
    else
    {
        updateDescription =  tr( "Cannot download information about the latest CUBE release. " );
        Globals::setStatusMessage( updateDescription, Warning );
    }
    download->deleteLater();
}
