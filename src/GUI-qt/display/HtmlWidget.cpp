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



#include <iostream>
#include <QBuffer>
#include <QDebug>

#include "HtmlWidget.h"
#include "DownloadThread.h"

using namespace std;

/*--- Constructors & destructor -------------------------------------------*/

/**
 * @brief Constructs an empty instance
 *
 * Constructs a new, empty HtmlWidget instance width the given @p parent.
 *
 * @param  parent  Parent widget
 */
HtmlWidget::HtmlWidget( QWidget* parent )
    : QTextBrowser( parent )
{
    setOpenExternalLinks( true );
}


/*--- Loading of contents -------------------------------------------------*/

/**
 * @brief Loads and displays the document with the given URL
 *
 * This method tries to load the document with the given @p url.
 * On success, it displays the contents of the document within the
 * widget.
 * After the data has been loaded
 *
 * @param mirrorList list of mirror URLs of the document to be displayed
 *
 */
void
HtmlWidget::loadUrl( const QList<QUrl>& mirrorList )
{
    // check if current URL base is already loaded
    bool    alreadyLoaded = false;
    QString lastBase      = lastUrl.toString( QUrl::RemoveFragment );
    if ( !lastBase.isEmpty() )
    {
        foreach( QUrl url, mirrorList )
        {
            QString base = url.toString( QUrl::RemoveFragment );
            if ( base == lastBase )
            {
                alreadyLoaded = true;
                break;
            }
        }
    }
    if ( !alreadyLoaded )     // new URL requested
    {
        clear();              // Clear the document
        resourceHash.clear(); // clear old resources
        DownloadThread* download = new DownloadThread( mirrorList );
        connect( download, SIGNAL( downloadFinished( QUrl, const QByteArray & ) ), this, SLOT( urlLoaded( QUrl, const QByteArray & ) ) );
        download->loadList();
    }
    else
    {
        // same document => scroll to anchor
        QString anchor = mirrorList.isEmpty() ? "" : mirrorList.first().fragment();
        if ( !anchor.isEmpty() )
        {
            scrollToAnchor( anchor );
        }
        emit loadingFinished( lastUrl, true );
    }
}

/**
 * @brief HelpBrowser::urlLoaded called, after document has been loaded using DownloadThread
 */
void
HtmlWidget::urlLoaded( const QUrl& url, const QByteArray& buffer )
{
    if ( !buffer.isNull() )
    {
        lastUrl = url;
        // Display document
        setHtml( QString( buffer ) );

        QString anchor = url.fragment();
        if ( !anchor.isEmpty() )
        {
            scrollToAnchor( anchor );
        }
        emit loadingFinished( lastUrl, true );
    }
    else
    {
        emit loadingFinished( url, false );
    }
    DownloadThread* download = qobject_cast<DownloadThread*>( sender() );
    download->deleteLater();
}



/**
 * @brief Loads referenced resources
 *
 * This method is called whenever an additional resource (e.g., an image or
 * an CSS style sheet) ir referenced from the document. It reimplements the
 * implementation provided by Qt's QTextBrowser class.
 *
 * @param  type  Type of the resource
 * @param  name  Resource URL
 *
 * @return Resource data as QByteArray (encapsulated in a QVariant) on
 *         success, an empty QVariant object otherwise.
 */
QVariant
HtmlWidget::loadResource( int type, const QUrl& name )
{
    Q_UNUSED( type );

    // Sanity check
    if ( !name.isValid() )
    {
        cerr << "Invalid URL: " << name.toString().toStdString().c_str() << endl;
        return QVariant();
    }

    // Determine absolute URL
    QUrl absoluteUrl;
    if ( name.isRelative() )
    {
        absoluteUrl = lastUrl.resolved( name );
    }
    else
    {
        absoluteUrl = name;
    }


    if ( !resourceHash.contains( absoluteUrl.toString() ) )                // 1st try to load resource
    {
        QVariant result = QTextBrowser::loadResource( type, absoluteUrl ); // try default resource loader
        if ( result.isNull() )
        {
            resourceHash.insert( absoluteUrl.toString(), QVariant() ); // avoid multiple load tasks on for the resource

            DownloadThread* download = new DownloadThread( absoluteUrl );
            connect( download, SIGNAL( downloadFinished( QUrl, const QByteArray & ) ), this, SLOT( resourceLoaded( QUrl, const QByteArray & ) ) );
            download->loadList();
        }
        return result;
    }
    else
    {
        QVariant result = resourceHash.value( absoluteUrl.toString() );
        return result;
    }

    return QTextBrowser::loadResource( type, absoluteUrl );
}


void
HtmlWidget::resourceLoaded( const QUrl& url, const QByteArray& buffer )
{
    resourceHash.insert( url.toString(), buffer );
    DownloadThread* download = qobject_cast<DownloadThread*>( sender() );
    download->deleteLater();
}
