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


#include <cassert>
#include <QMessageBox>
#include <QFileDialog>
#include <QDialog>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QApplication>
#include <QFontDialog>

#include "EditorPlugin.h"
#include "CPPSyntaxHighlighter.h"
#include "FortranSyntaxHighlighter.h"

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( SourceCodeEditorPlugin, EditorPlugin ); // ( PluginName, ClassName )
#endif

void
EditorPlugin::version( int& major, int& minor, int& bugfix ) const
{
    major  = 1;
    minor  = 0;
    bugfix = 0;
}

QString
EditorPlugin::name() const
{
    return "SourceCodeEditor";
}

QString
EditorPlugin::getHelpText() const
{
    return "Simple source code editor with syntax highlighting";
}

bool
EditorPlugin::cubeOpened( PluginServices* service )
{
    this->service = service;

    connect( service, SIGNAL( contextMenuIsShown( TreeType, TreeItem* ) ),
             this, SLOT( contextMenuIsShown( TreeType, TreeItem* ) ) );

    // the first time a source code is not found in the given location,
    // we ask if the user wishes to open another file;
    // after this first time, we do not ask any more but directly open
    // an open file dialog
    firstSourceNotFound = true;
    lastSourceCodeName  = "";

    return true;
}

void
EditorPlugin::contextMenuIsShown( TreeType type, TreeItem* item )
{
    if ( type == CALLTREE  || type == CALLFLAT )
    {
        QAction* action = service->addContextMenuItem( type, "Show source code" );
        connect( action, SIGNAL( triggered() ), this, SLOT( sourceSlot() ) );

        item->getSourceInfo( fileNameRegion, startLine, endLine );
        if ( fileNameRegion.isEmpty() )
        {
            action->setEnabled( false );
        }
    }
}

void
EditorPlugin::sourceSlot()
{
    sourceCode( fileNameRegion, startLine, endLine );
}

// source code editor, copied from TreeWidget.cpp
void
EditorPlugin::sourceCode( const QString& sourceFile, int startLine, int endLine )
{
    QString fileName = sourceFile;
    /*
       std::string stdFileName = "";
       if ( type == CALLTREE )
       {
        if ( !takeCallee )
        {
            stdFileName = ( ( cube::Cnode* )( item->cubeObject ) )->get_mod();
        }
        else
        {
            stdFileName = ( ( cube::Cnode* )( item->cubeObject ) )->get_callee()->get_mod();
        }
       }
       else if ( type == CALLFLAT && item->cubeObject != NULL )
       {
        stdFileName = ( ( cube::Region* )( item->cubeObject ) )->get_mod();
       }
       if ( strcmp( stdFileName.c_str(), "MPI" ) == 0 ||
         strcmp( stdFileName.c_str(), "INTERNAL" ) == 0 ||
         strcmp( stdFileName.c_str(), "OMP" ) == 0 )
       {
        stdFileName = "";
       }

       fileName = QString::fromStdString( stdFileName );
     */
    QFile* file = new QFile( fileName );

    while ( !file->exists() || !file->open( QFile::ReadWrite | QFile::Text ) )
    {
        if ( firstSourceNotFound )
        {
            QString message;
            if ( !file->exists() )
            {
                message = tr( "File \" %1 \" does not exist." ).arg( fileName );
            }
            else
            {
                message = tr( "Cannot read file %1:\n%2." ).arg( fileName ).arg( file->errorString() );
            }
            message.append( "\n\nDo you want to open another file?" );
            if ( QMessageBox::question( service->getParentWidget(),
                                        tr( "Source file" ),
                                        message,
                                        QMessageBox::Open | QMessageBox::Cancel ) == QMessageBox::Cancel )
            {
                delete file;
                return;
            }
            firstSourceNotFound = false;
        }

        // take the path of the last opened source code file
        // and append the current file name to it;
        // that will be the default name for the open file dialog
        QString name = lastSourceCodeName;
        if ( name.compare( QString( "" ) ) != 0 )
        {
            int pos = name.lastIndexOf( "/" );
            if ( pos >= 0 )
            {
                name = name.left( pos );
            }
        }
        else
        {
            name = QDir::homePath();
        }

        QString name2 = fileName;
        {
            int pos = name2.lastIndexOf( "/" );
            if ( pos >= 0 )
            {
                name2 = name2.right( pos );
            }
        }

        name.append( "/" );
        name.append( name2 );

        QString caption = "";
        fileName = QFileDialog::getOpenFileName( service->getParentWidget(), caption, name );
        if ( fileName.isEmpty() )
        {
            delete file;
            return;
        }
        delete file;
        file               = new QFile( fileName );
        lastSourceCodeName = fileName;
    }

    /* todo
       std::string stdFileName = fileName.toStdString();
       std::string correct_place            = fileName.toUtf8().data();
       size_t      last_slash_pos_corrected = correct_place.rfind( "/" );
       size_t      last_slash_pos_original  = stdFileName.rfind( "/" );
       if ( correct_place.compare( stdFileName ) != 0 )   // we perform paths correction only if file name is different than stored one.
       {
        if ( last_slash_pos_original != string::npos ) // we found last slash
        {
            if ( stdFileName.substr( last_slash_pos_original ).compare( correct_place.substr( last_slash_pos_corrected ) ) == 0 )
            {
                // we are here, means, file names are same and user seems to open same source, just on different place
                // ... so, we try to correct paths in the regions.
                size_t last_slash_pos_corrected1 = last_slash_pos_corrected;
                size_t last_slash_pos_original1  =  last_slash_pos_original;
                while ( stdFileName.substr( last_slash_pos_original1 ).compare( correct_place.substr( last_slash_pos_corrected1 ) ) == 0 )
                {
                    // we are here, means, file names are same and user seems to open same source, just on different place
                    // ... so, we try to correct paths in the regions.
                    last_slash_pos_corrected1--;
                    last_slash_pos_original1--;
                }
                // we step back to the last position, where substrings matched
                last_slash_pos_corrected1++;
                last_slash_pos_original1++;

                string _original_prefix =  stdFileName.substr( 0, last_slash_pos_original1 );
                string _correct_prefix  =    correct_place.substr( 0, last_slash_pos_corrected1 );
                // now we run over all regions in cube and replace all matching pathprefices by corrected one
                const vector<cube::Region*> regv = cube->get_regv();
                for ( vector<cube::Region*>::const_iterator iter = regv.begin(); iter != regv.end(); ++iter )
                {
                    string _mod = ( *iter )->get_mod();
                    if ( _mod.substr( 0, _original_prefix.length() ).compare( _original_prefix ) == 0 )
                    {
                        // it means, there is a wrong prefix
                        _mod.replace( 0, _original_prefix.length(), _correct_prefix );
                        ( *iter )->set_mod( _mod );
                    }
                }
            }
        }
       }*/



    {
        QTextStream in( file );
        textEdit = new QTextEdit();
        textEdit->setLineWrapMode( QTextEdit::NoWrap );
        textEdit->setLineWrapColumnOrWidth( 80 );

        textEdit->setFont( fontSourceCode );
        textEdit->setPlainText( in.readAll() );
        delete file;
    }


/*
    // get the position of the item in the source code
    int startLine  = -1;
    int endLine = -1;
    if ( item->type == CALLITEM )
    {
        if ( !takeCallee )
        {
            startLine = ( ( cube::Cnode* )( item->cubeObject ) )->get_line();
        }
        else
        {
            startLine  = ( ( cube::Cnode* )( item->cubeObject ) )->get_callee()->get_begn_ln();
            endLine = ( ( cube::Cnode* )( item->cubeObject ) )->get_callee()->get_end_ln();
        }
    }
    else if ( item->type == REGIONITEM && item->cubeObject != NULL )
    {
        startLine  = ( ( cube::Region* )( item->cubeObject ) )->get_begn_ln();
        endLine = ( ( cube::Region* )( item->cubeObject ) )->get_end_ln();
    }
 */

    // mark the start line of the item's code by a dark green background
    QList<QTextEdit::ExtraSelection> extras;

    if ( endLine != -1 )
    {
        QTextEdit::ExtraSelection highlight2;
        highlight2.format.setBackground( QColor( 0, 100, 0, 100 ) );
        highlight2.cursor = textEdit->textCursor();
        highlight2.cursor.movePosition( QTextCursor::Start );
        for ( int i = 1; i < endLine; i++ )
        {
            highlight2.cursor.movePosition( QTextCursor::Down );
        }
        highlight2.cursor.movePosition( QTextCursor::Down, QTextCursor::KeepAnchor );
        extras << highlight2;
    }

    // mark the final line of the item's code by a green background
    if ( startLine > 0 )
    {
        QTextEdit::ExtraSelection highlight;
        highlight.format.setBackground( QColor( 0, 200, 0, 100 ) );
        highlight.cursor = textEdit->textCursor();
        highlight.cursor.movePosition( QTextCursor::Start );
        for ( int i = 1; i < startLine; i++ )
        {
            highlight.cursor.movePosition( QTextCursor::Down );
        }
        highlight.cursor.movePosition( QTextCursor::StartOfLine );
        textEdit->setTextCursor( highlight.cursor );
        highlight.cursor.movePosition( QTextCursor::Down, QTextCursor::KeepAnchor );

        extras << highlight;
    }

    textEdit->setExtraSelections( extras );

    bool    cppcode     = true;
    bool    fortrancode = false;
    QRegExp fortranSourceCode( "\\.[fF][:digit:]{0,2}$" );
    QRegExp cSourceCode( "\\.c|h$" );
    QRegExp cppSourceCode( "\\.cpp|hpp$" );

    if ( fortranSourceCode.indexIn( fileName ) != -1 )
    {
        fortrancode = true;
        cppcode     = false;
    }
    else if (
        cSourceCode.indexIn( fileName ) != -1  ||
        cppSourceCode.indexIn( fileName ) != -1
        )
    {
        fortrancode = false;
        cppcode     = true;
    }

    if ( cppcode )
    {
        new CPPSyntaxHighlighter( textEdit->document() );
    }
    else if ( fortrancode )
    {
        new FortranSyntaxHighlighter( textEdit->document() );
    }

    dialog = new QDialog();
    QFontMetrics fmetric( textEdit->font() );
    int          wsize = fmetric.boundingRect( "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW" ).width();
    dialog->resize( std::max( wsize, 500 ), 500 );

    dialog->setWindowTitle( fileName );
    QVBoxLayout* layout = new QVBoxLayout();

    // add the text editor to the dialog
    layout->addWidget( textEdit );
    textEdit->setReadOnly( true );

    // add save/save as /close buttons to the dialog
    textEditSaveButton   = new QPushButton( "Save" );
    textEditSaveAsButton = new QPushButton( "Save as" );
    QPushButton* closeButton = new QPushButton( "Close" );
    
    findNextButton       = new QPushButton( "Next" );
    findPrevButton       = new QPushButton( "Previous" );
    findEdit             = new QLineEdit();
    

    //    QFontDialog * fontSelection = new QFontDialog(textEdit->font(), 0);
    // Select some another font
    QPushButton* fontButton = new QPushButton( "Font..." );

    QCheckBox* readOnlyAct = new QCheckBox( tr( "Read only" ) );
    readOnlyAct->setChecked( true );
    onToggleReadOnly( true );
    connect( readOnlyAct, SIGNAL( toggled( bool ) ), this, SLOT( onToggleReadOnly( bool ) ) );

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget( readOnlyAct );
    buttonLayout->addWidget( textEditSaveButton );
    buttonLayout->addWidget( textEditSaveAsButton );
    
    buttonLayout->addWidget( findEdit );
    buttonLayout->addWidget( findPrevButton );
    buttonLayout->addWidget( findNextButton );
    
    connect( findPrevButton, SIGNAL(clicked( bool ) ), this, SLOT( searchBackward()));
    connect( findNextButton, SIGNAL(clicked( bool ) ), this, SLOT( searchForward()));
    connect( findEdit, SIGNAL(textEdited(const QString& ) ), this, SLOT( search( const QString&)));
    
    
    
    
    buttonLayout->addWidget( fontButton );
    buttonLayout->addWidget( closeButton );

    layout->addLayout( buttonLayout );
    dialog->setLayout( layout );

    connect( textEditSaveButton, SIGNAL( clicked( bool ) ), this, SLOT( onSaveFile() ) );
    connect( textEditSaveAsButton, SIGNAL( clicked( bool ) ), this, SLOT( onSaveFileAs() ) );
    connect( closeButton, SIGNAL( clicked( bool ) ), dialog, SLOT( close() ) );
    connect( fontButton, SIGNAL( clicked( bool ) ), this, SLOT( onChangeFont() ) );


    // execute the dialog
    dialog->exec();

    disconnect( textEditSaveButton, SIGNAL( clicked( bool ) ), this, SLOT( onSaveFile() ) );
    disconnect( textEditSaveAsButton, SIGNAL( clicked( bool ) ), this, SLOT( onSaveFileAs() ) );
    disconnect( closeButton, SIGNAL( clicked( bool ) ), dialog, SLOT( close() ) );
    disconnect( fontButton, SIGNAL( clicked( bool ) ), this, SLOT( onChangeFont() ) );

    delete dialog;
}
// end of sourceCode()

// Selection of another font in source code dialog.
void
EditorPlugin::onChangeFont()
{
    fontSourceCode = QFontDialog::getFont( 0, textEdit->font() );
    textEdit->setFont( fontSourceCode );
}

// this slot is called by the editor for source code;
// it stores the eventually modified source code
//
void
EditorPlugin::onSaveFile()
{
    // "fileName" stores the name for the last source code opening/storing
    QFile file( fileName );

    // if the file cannot be opened for writing
    // display a warning message and return without storing
    if ( !file.open( QFile::WriteOnly | QFile::Text ) )
    {
        QString message = tr( "Cannot write file %1:\n%2." ).arg( fileName ).arg( file.errorString() );
        service->setMessage( message, Error );
        return;
    }

    {
        // write source code into the opened file
        QTextStream out( &file );
        QApplication::setOverrideCursor( Qt::WaitCursor );
        out << textEdit->toPlainText();
        QApplication::restoreOverrideCursor();
        out.flush();
    }
}


// this slot is called by the editor opened for source code;
// it stores the eventually modified source code under a new name
//
void
EditorPlugin::onSaveFileAs()
{
    // get the new file name by user input
    QString tmpName = QFileDialog::getSaveFileName( service->getParentWidget() );
    // tmpName can be empty, if the user canceled the input;
    // in this case just return without storing
    if ( tmpName.isEmpty() )
    {
        return;
    }
    // otherwise store the new name in the object field "fileName"
    fileName = tmpName;
    // and call the slot for storing source code
    onSaveFile();
    // update the window title of the dialog widget containing the text editor
    dialog->setWindowTitle( fileName );
}




void
EditorPlugin::onToggleReadOnly( bool checked )
{
    if ( !checked )
    {
        textEdit->setReadOnly( false );
        textEditSaveButton->setEnabled( true );
        textEditSaveAsButton->setEnabled( true );
    }
    else
    {
        textEdit->setReadOnly( true );
        textEditSaveButton->setEnabled( false );
        textEditSaveAsButton->setEnabled( false );
    }
}



void
EditorPlugin::search(const QString & text)
{
    searchText = text;
    searchBackward();
    searchForward();
}

void
EditorPlugin::searchForward()
{
    textEdit->find( searchText );
}

void
EditorPlugin::searchBackward()
{
    textEdit->find( searchText, QTextDocument::FindBackward );
}

