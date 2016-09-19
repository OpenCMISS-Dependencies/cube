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



#include "StatusBar.h"
#include <QDebug>
#include <QSplitter>
#include <iostream>

StatusBar::StatusBar()
{
    oneRow    = new QWidget();
    rowLayout = new QHBoxLayout( oneRow );
    rowLayout->setContentsMargins( 0, 0, 0, 0 );
    oneRow->setLayout( rowLayout );
    label = new QLabel();
    label->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    rowLayout->addWidget( label );

    oneRow->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    this->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum );

    textView = new QTextEdit();
    textView->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    textView->setReadOnly( true );
    textView->setFocusPolicy( Qt::NoFocus );

    setLayout( new QHBoxLayout() );
    layout()->setContentsMargins( 0, 0, 0, 0 );
    layout()->addWidget( oneRow );

    label->setWhatsThis( "The status bar shows the most recent log message. By clicking on it, the complete log becomes visible." );

    this->setFocusPolicy( Qt::StrongFocus );
}

void
StatusBar::addLine( const QString& line, MessageType type )
{
    QString message;

    if ( type == Verbose )
    {
        std::cout << line.toStdString() << std::endl;
        message = QString( "<font color=gray>%1</font>" ).arg( line );
    }
    else if ( type == Information )
    {
        message = line;
    }
    else if ( type == Warning )
    {
        message = QString( "<font color=red>%1</font>" ).arg( line );
    }
    else if ( type == Error || type == Critical )
    {
        message = QString( "<font color=red>%1</font>" ).arg( line );
    }
    if ( type != Verbose )
    {
        label->setText( message );
    }

    textView->append( message );
}

void
StatusBar::addPermanentWidget( QWidget* widget )
{
    rowLayout->addWidget( widget );
}

/** show multiline log if the user clicks on the status bar label */
void
StatusBar::mousePressEvent( QMouseEvent* )
{
    layout()->removeWidget( oneRow );
    oneRow->setVisible( false );
    textView->setVisible( true );
    layout()->addWidget( textView );
    emit minimized( false );
}

/** revert to single line view, if status bar looses the focus */
void
StatusBar::focusOutEvent( QFocusEvent* )
{
    if ( !isVisible() )
    {
        return; // don't react if StatusBarWidget rearranges child widgets
    }

    layout()->removeWidget( textView );
    textView->setVisible( false );
    oneRow->setVisible( true );
    layout()->addWidget( oneRow );
    this->parentWidget()->setFocus();
    emit minimized( true );
}

// =========================================================================================================
// =========================================================================================================

StatusBarWidget::StatusBarWidget()
{
    isMini    = true;
    statusBar = new StatusBar();
    splitter  = new QSplitter( Qt::Vertical );

    VersionCheckWidget* _version_check = new VersionCheckWidget( this );
    statusBar->addPermanentWidget( _version_check );

    setLayout( new QVBoxLayout() );
    layout()->setContentsMargins( 0, 0, 0, 0 );

    connect( statusBar, SIGNAL( minimized( bool ) ), this, SLOT( statusBarIsMinimized( bool ) ) );
}

void
StatusBarWidget::setWidget( QWidget* widget )
{
    this->widget = widget;
    layout()->addWidget( widget );
    layout()->addWidget( statusBar );
}

StatusBar*
StatusBarWidget::getStatusBar()
{
    return statusBar;
}

void
StatusBarWidget::statusBarIsMinimized( bool mini )
{
    if ( isMini == mini )
    {
        return; // if menu is opened, this method is called double => invalid splitterSizes
    }
    isMini = mini;

    if ( mini ) // status bar gets minimal size
    {
        splitterSizes = splitter->saveState();
        layout()->removeWidget( splitter );
        layout()->addWidget( widget );
        layout()->addWidget( statusBar );
    }
    else
    {
        layout()->removeWidget( widget );
        layout()->removeWidget( statusBar );
        layout()->addWidget( splitter );
        splitter->addWidget( widget );
        splitter->addWidget( statusBar );
        splitter->restoreState( splitterSizes );
        statusBar->setFocus();
    }
}
