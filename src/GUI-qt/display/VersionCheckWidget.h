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




#ifndef VERSION_CHECK_H
#define VERSION_CHECK_H

#include <QWidget>
#include <QPainter>
#include <QString>
#include <QToolButton>
#include <utility>
#include <vector>
#include <string>

#include <QUrl>
#include <QBuffer>

#include "DownloadThread.h"

// #define UPDATE_CHECK_URL "http://www.vi-hps.org/upload/packages/cube/CUBE_LATEST_RELEASE"
#define UPDATE_CHECK_URL "http://apps.fz-juelich.de/scalasca/releases/cube/CUBE_LATEST_RELEASE"
// #define UPDATE_URL "http://www.vi-hps.org/projects/score-p/"

// this url is not 100% valid, coz i dont know yet precise url for the page, where CUBE get published
#define UPDATE_URL "http://www.scalasca.org/software/cube-4.x/cube.html"




class VersionCheckWidget : public QToolButton
{
    Q_OBJECT
public:
    VersionCheckWidget( QWidget* parent = 0 );

private:
    virtual void
    mousePressEvent( QMouseEvent* event );



    QIcon recentVersion;
    QIcon updateAvailable;
    QIcon noCheckPossible;

    QIcon shownIcon;

    QUrl            url;
    DownloadThread* download;
    QString         updateDescription;
    bool            update_Available;
private slots:
    void
    check( const QUrl&       url,
           const QByteArray& buffer );
};

#endif
