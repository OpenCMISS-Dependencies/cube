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


#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QDebug>
#include <QFileDialog>
#include <cassert>
#include <string>
#include "VampirConnectionDialog.h"
#include "CubeServices.h"

using namespace std;

class VampirConnectionThread : public QThread
{
public:
    VampirConnectionThread( string const&            host,
                            int                      port,
                            string const&            file,
                            VampirConnectionDialog*  dia,
                            QList<VampirConnecter*>* connecterList_ );
    virtual void
    run();

private:
    string                   hostName;
    string                   fileName;
    int                      portNumber;
    VampirConnectionDialog*  dialog;
    QList<VampirConnecter*>* connecterList;

    QString
    connectToVampir();
};

VampirConnectionThread::VampirConnectionThread( string const&            host,
                                                int                      port,
                                                string const&            file,
                                                VampirConnectionDialog*  dia,
                                                QList<VampirConnecter*>* connecterList_  )
    : hostName( host ), fileName( file ), portNumber( port ), dialog( dia ), connecterList( connecterList_ )
{
}

void
VampirConnectionThread::run()
{
    QString result = connectToVampir();
    if ( result != "" )
    {
        dialog->setError( "Vampir connection", result );
    }
}


QString
VampirConnectionThread::connectToVampir()
{
    // ( string const& host, int port, string const& fileName )
    QList<VampirConnecter*>& connectors = *connecterList;

    bool    verbose  = true;
    bool    replaced = false;
    QString message;
    for ( int i = 0; i < connectors.size(); ++i )
    {
        if ( !VampirConnecter::ExistsVampirWithBusName( VampirConnecter::GetVampirBusName( i ) ) )
        {
            delete connectors[ i ];

            VampirConnecter* con = new VampirConnecter( VampirConnecter::GetVampirBusName( i ), hostName, portNumber, fileName, verbose );
            message         = QString::fromStdString( con->InitiateAndOpenTrace() );
            connectors[ i ] = con;
            replaced        = true;
        }
    }

    if ( !replaced )
    {
        if ( connectors.size() < VampirConnecter::GetMaxVampirNumber() )
        {
            VampirConnecter* con = new VampirConnecter( VampirConnecter::GetVampirBusName( connectors.size() ), hostName, portNumber, fileName, verbose );
            message = QString::fromStdString( con->InitiateAndOpenTrace() );
            if ( !message.isEmpty() )
            {
                delete con;
            }
            else
            {
                connectors.append( con );
            }
        }
        else
        {
            return "Maximal number of vampir clients already reached.";
        }
    }
    return message;
}

// =======================================================================================

VampirConnectionDialog::VampirConnectionDialog( PluginServices*          service_,
                                                QWidget*                 par,
                                                const QString&           cubeFileName,
                                                QList<VampirConnecter*>* connecterList_ )
    : QDialog( par ), service( service_ ), connectionThread( 0 ), connecterList( connecterList_ )
{
    int const spacing = 20;

    setWindowTitle( "Connect to vampir" );

    QFontMetrics fm( font() );
    int          h = 10 * fm.ascent();
    int          w = 4 * fm.width( "Connect to vampir:" );
    setMinimumSize( w, h );

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing( spacing );

    QGridLayout* layout1 = new QGridLayout();
    layout1->setSpacing( spacing );

    // define host label and editor
    QLabel* hostLabel = new QLabel();
    hostLabel->setText( "Host:" );
    hostLabel->setDisabled( true );
    layout1->addWidget( hostLabel, 1, 0 );

    hostLine = new QLineEdit( "localhost" );
    hostLine->setDisabled( true );
    layout1->addWidget( hostLine, 1, 1 );

    // define port label and editor
    QLabel* portLabel = new QLabel();
    portLabel->setText( "Port:" );
    portLabel->setDisabled( true );
    layout1->addWidget( portLabel, 2, 0 );

    portLine = new QSpinBox();
    portLine->setRange( 0, 1000000 );
    portLine->setSingleStep( 1 );
    portLine->setValue( 30000 );
    portLine->setDisabled( true );
    layout1->addWidget( portLine, 2, 1 );

    // set checkbox to change between a local or remote trace file
    fromServerCheckbox = new QCheckBox( "Open local file" );
    fromServerCheckbox->setChecked( true );
    layout1->addWidget( fromServerCheckbox, 0, 0, 1, 2 );
    connect( fromServerCheckbox, SIGNAL( toggled( bool ) ), portLine, SLOT( setDisabled( bool ) ) );
    connect( fromServerCheckbox, SIGNAL( toggled( bool ) ), hostLine, SLOT( setDisabled( bool ) ) );
    connect( fromServerCheckbox, SIGNAL( toggled( bool ) ), portLabel, SLOT( setDisabled( bool ) ) );
    connect( fromServerCheckbox, SIGNAL( toggled( bool ) ), hostLabel, SLOT( setDisabled( bool ) ) );

    layout->addLayout( layout1 );

    // define file label and editor

    QHBoxLayout* layout2 = new QHBoxLayout();
    layout2->setSpacing( spacing );

    QLabel* fileLabel = new QLabel();
    fileLabel->setText( "File:" );
    layout2->addWidget( fileLabel );

    fileLine = new QLineEdit( getDefaultVampirFileName( cubeFileName ) );
    layout2->addWidget( fileLine );
    fileButton = new QPushButton( "Browse" );
    layout2->addWidget( fileButton );
    connect( fileButton, SIGNAL( clicked() ), this, SLOT( getTraceFileName() ) );

    layout->addLayout( layout2 );

    QDialogButtonBox* buttonBox = new QDialogButtonBox();
    okButton     = buttonBox->addButton( QDialogButtonBox::Ok );
    cancelButton = buttonBox->addButton( QDialogButtonBox::Cancel );
    connect( okButton, SIGNAL( clicked() ), this, SLOT( establishVampirConnection() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    layout->addWidget( buttonBox );

    setLayout( layout );
}

void
VampirConnectionDialog::getTraceFileName()
{
    QString openFileName =
        QFileDialog::getOpenFileName( this, tr( "Choose a file to open" ),
                                      fileLine->displayText(),
                                      tr( "Trace files (*.elg *.esd *.otf *.otf2 *.prv);;All files (*.*);;All files (*)" ) );
    if ( openFileName.length() > 0 )
    {
        fileLine->setText( openFileName );
    }
}

void
VampirConnectionDialog::setError( QString const& title, QString const& message )
{
    errorTitle   = title;
    errorMessage = message;
}

void
VampirConnectionDialog::printError()
{
    if ( errorMessage != "" )
    {
        QString message = errorTitle.append( "\n" ).append( errorMessage );
        service->setMessage( message, Error );
    }
    setAttribute( Qt::WA_DeleteOnClose );
    accept();
}

void
VampirConnectionDialog::establishVampirConnection()
{
    okButton->setEnabled( false );
    cancelButton->setEnabled( false );
    fileButton->setEnabled( false );

    if ( fromServerCheckbox->isChecked() )
    {
        connectionThread =
            new VampirConnectionThread( "", 0, fileLine->displayText().toStdString(), this, connecterList );
    }
    else
    {
        connectionThread =
            new VampirConnectionThread( hostLine->displayText().toStdString(),
                                        portLine->value(), fileLine->displayText().toStdString(), this, connecterList );
    }
    connect( connectionThread, SIGNAL( finished() ), this, SLOT( printError() ) );
    connectionThread->start();
}

VampirConnectionDialog::~VampirConnectionDialog()
{
    delete connectionThread;
}

QString
VampirConnectionDialog::getDefaultVampirFileName( const QString& cubeFileName ) const
{
    string pathToTrace = cube::services::dirname( cubeFileName.toStdString() );
    if ( cube::services::is_cube3_name( cubeFileName.toStdString() ) )
    {
        pathToTrace = pathToTrace + "epik.esd";
    }
    if ( cube::services::is_cube4_name( cubeFileName.toStdString() ) )
    {
        pathToTrace = pathToTrace + "traces.otf2";
    }
    return QString::fromStdString( pathToTrace );
}
