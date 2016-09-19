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



#ifndef _HTMLWIDGET_H
#define _HTMLWIDGET_H


#include <QTextBrowser>
#include <QBuffer>

/*-------------------------------------------------------------------------*/
/**
 * @file  HtmlWidget.h
 * @brief Declaration of the class HtmlWidget
 *
 * This header file provides the declaration of the class HtmlWidget.
 */
/*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/
/**
 * @class HtmlWidget
 * @brief Provides a simple HTML-browser widget
 *
 * This class provides a simple HTML-browser widget based on Qt's
 * QTextBrowser class. Its main extension over QTextBrowser is the
 * ability to load HTML pages as well as referenced resources (i.e.,
 * images in particular) directly from a web server or a file,
 * respectively.
 *
 * This widget is not intended to be used as a full-featured web
 * browser, but more as an easy way to display online help messages
 * in HTML format. When writing the documents to be displayed, keep
 * in mind that this widget only supports a limited subset of HTML
 * (see Qt's reference documentation for details).
 */
/*-------------------------------------------------------------------------*/

class HtmlWidget : public QTextBrowser
{
    Q_OBJECT

public:
    HtmlWidget( QWidget* parent = 0 );

    void
    loadUrl( const QList<QUrl>& mirrorList );

    /**
        for internal use: loads all referenced resources
     */
    virtual QVariant
    loadResource( int         type,
                  const QUrl& name );

signals:
    /**
     * @brief loadingFinished is emitted after the html resource has been loaded
     * @param url the URL which had to be loaded
     * @param ok true, if loading was successfull
     */
    void
    loadingFinished( const QUrl& url,
                     bool        ok );

private slots:
    void
    urlLoaded( const QUrl&       url,
               const QByteArray& buffer );

    void
    resourceLoaded( const QUrl&       url,
                    const QByteArray& buffer );

private:
    QHash<QString, QVariant> resourceHash;
    QUrl                     lastUrl;
};


#endif   /* !_HTMLWIDGET_H */
