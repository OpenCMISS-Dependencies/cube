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



#ifndef _HELPBROWSER_H
#define _HELPBROWSER_H

#include <QTextBrowser>
#include <QDialog>
#include <QUrl>
#include <QBuffer>
#include "DownloadThread.h"
#include "HtmlWidget.h"

/*-------------------------------------------------------------------------*/
/**
 * @file  HelpBrowser.h
 * @brief Declaration of the class HelpBrowser
 *
 * This header file provides the declaration of the class HelpBrowser.
 */
/*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/
/**
 * @class HelpBrowser
 * @brief Provides a simple dialog window for displaying HTML help texts
 *
 * This class provides a simple non-modal dialog window for displaying
 * HTML help texts. The displayed text (including referenced image data)
 * will be cached until either a new document is loaded, in which case its
 * data will be cached, or the instance is destroyed (closing the dialog
 * will only hide it).
 */
/*-------------------------------------------------------------------------*/

class HelpBrowser : public QDialog
{
    Q_OBJECT

public:
    /// @name Constructors &amp; destructor
    /// @{

    HelpBrowser( const QString& title,
                 QWidget*       parent = 0 );

    /// @}

    /// @name Displaying a help text
    /// @{
    void
    showUrl( const QUrl& url,
             QString     errorMessage );

    /// @}
    /**
     * @param urlList List of url to search for
     */
    void
    showUrl( const QList<QUrl>& mirrorList,
             QString            errorMessage );

    /// @name Displaying a help text from  QString  and scrolling to anchor if given
    /// @{
    void
    showHelpAsString( QString&,
                      QString anchor = QString( "" )  );


private slots:
    void
    loadingFinished( const QUrl&,
                     bool ok );


private:
    /// Browser widget used to display the help text
    HtmlWidget* html;
    QBuffer     buffer;           // buffer for contents of URL
    QString     errorMessage;     // message to show, if URL could not be loaded
    QList<QUrl> mirrorList;
};


#endif
