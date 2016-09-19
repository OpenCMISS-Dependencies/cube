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


#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include "InfoWidget.h"

InfoWidget::InfoWidget( QWidget* parent ) :
    QDialog( parent, Qt::Window )
{
    QWidget*          centralWidget;
    QVBoxLayout*      verticalLayout;
    QDialogButtonBox* buttonBox;

    setWindowTitle( "Cube Info Display" );

    centralWidget  = this;
    verticalLayout = new QVBoxLayout( centralWidget );
    verticalLayout->setSpacing( 6 );
    verticalLayout->setContentsMargins( 11, 11, 11, 11 );

    tabWidget = new QTabWidget( centralWidget );
    tabWidget->setTabsClosable( true );
    tabWidget->setMovable( true );
    connect( tabWidget, SIGNAL( tabCloseRequested( int ) ), this, SLOT( deleteTab( int ) ) );
    verticalLayout->addWidget( tabWidget );

    buttonBox = new QDialogButtonBox( centralWidget );
    buttonBox->setStandardButtons( QDialogButtonBox::Close );
    connect( buttonBox, SIGNAL( clicked( QAbstractButton* ) ), this, SLOT( closeWidget() ) );

    verticalLayout->addWidget( buttonBox );

    // set initial size and position
    resize( 500, 200 );
    move( QApplication::desktop()->screen()->rect().center() - rect().center() ); // center
    setVisible( true );
}

void
InfoWidget::closeEvent( QCloseEvent* event )
{
    (void) event;
    closeWidget();
}

void
InfoWidget::closeWidget()
{
    lastGeometry = this->geometry();

    while ( tabWidget->count() > 0 )
    {
        deleteTab( 0 );
    }
    this->setVisible( false );
}

/**
   showText displays a text in a tabbed widget.
   It creates a new tab with the given title or updates an existing tab.
 */
void
InfoWidget::showText( const QString& titel, const QString& text )
{
    if ( !isVisible() )
    {
        if ( lastGeometry.isValid() )
        {
            setGeometry( lastGeometry );
        }
        setVisible( true );
    }

    QTextBrowser* textBrowser = 0;
    // check if tab already exists => update existing
    for ( int i = 0; i < tabWidget->count(); i++ )
    {
        if ( tabWidget->tabText( i ) == titel )
        {
            textBrowser = dynamic_cast<QTextBrowser*> ( tabWidget->widget( i ) );
            break;
        }
    }

    // create new tab, if not already exists
    if ( !textBrowser )
    {
        textBrowser = new QTextBrowser( this );
    }

    textBrowser->setText( text );
    tabWidget->insertTab( 0, textBrowser, titel );
    tabWidget->setCurrentIndex( 0 );

    // limit number of tabs
    if ( tabWidget->count() > 8 )
    {
        tabWidget->removeTab( tabWidget->count() - 1 );
    }
}

void
InfoWidget::deleteTab( int index )
{
    QWidget* w = tabWidget->widget( index );
    tabWidget->removeTab( index );
    delete w;
}
