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

#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include "HelpBrowser.h"
#include "Globals.h"

/*--- Constructors & destructor -------------------------------------------*/

/**
 * @brief Constructs a new help browser dialog
 *
 * This constructor creates a new help browser dialog with the given window
 * @p title and @p parent widget.
 *
 * @param  title   Window title
 * @param  parent  Parent widget
 */
HelpBrowser::HelpBrowser( const QString& title,
                          QWidget*       parent )
    : QDialog( parent )
{
    setAttribute( Qt::WA_DeleteOnClose, false );

    // Dialog defaults
    setWindowTitle( title );
    resize( 600, 600 );

    // Set up layout manager
    QVBoxLayout* layout = new QVBoxLayout();
    setLayout( layout );

    // Create HTML display
    html = new HtmlWidget( this );
    layout->addWidget( html );

    QPushButton* _close = new QPushButton( QObject::tr( "&Close" ) );
    layout->addWidget( _close );
    connect( _close, SIGNAL( pressed() ), this, SLOT( close() ) );
}


/*--- Displaying a help text ----------------------------------------------*/

/**
 * @brief Displays the help text with the given URL
 *
 * This method displays the help text with the given @p url. If the base
 * URL of @p url is identical to the one of the current document, this
 * method will only scroll to the new anchor (if provided). Otherwise, the
 * new document will be loaded.
 *
 * @param  url  URL of the help text to be shown
 *
 * @return @b true if successful, @b false otherwise (e.g., in case of a
 *         download error).
 */

void
HelpBrowser::showUrl( const QUrl& url, QString errorMessage )
{
    QList<QUrl> list;
    list.append( url );
    showUrl( list, errorMessage );
}

void
HelpBrowser::showUrl( const QList<QUrl>& mirrorList, QString errorMessage )
{
    this->errorMessage = errorMessage;
    this->mirrorList   = mirrorList;
    connect( html, SIGNAL( loadingFinished( QUrl, bool ) ), this, SLOT( loadingFinished( QUrl, bool ) ) );
    html->loadUrl( mirrorList );
}

/**
 * @brief HelpBrowser::loadingFinished is called, after document has been loaded
 */
void
HelpBrowser::loadingFinished( const QUrl& url, bool ok )
{
    (void) url; // to suppress not used warning
    if ( ok )
    {
        setWindowTitle( html->documentTitle() );
        show();
    }
    else
    {
        foreach( QUrl mirror, mirrorList )
        {
            Globals::setStatusMessage( QString( "Tried mirror: " ) + mirror.path(), Information );
        }
        Globals::setStatusMessage( QString( "Warning: " ) + errorMessage, Warning );
    }
}


/// @name Displaying a help text from  QString
/// @{
void
HelpBrowser::showHelpAsString( QString& data, QString anchor )
{
    html->setHtml( data );
    if ( !anchor.isEmpty() )
    {
        html->scrollToAnchor( anchor );
    }
    setWindowTitle( html->documentTitle() );
    show();
}
