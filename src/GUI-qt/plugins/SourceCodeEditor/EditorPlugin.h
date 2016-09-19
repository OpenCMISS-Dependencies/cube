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


#ifndef SourceCodeEditorPlugin_H
#define SourceCodeEditorPlugin_H
#include <QtGui>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include "CubePlugin.h"
#include "PluginServices.h"

class EditorPlugin : public QObject, CubePlugin
{
    Q_OBJECT
                            Q_INTERFACES( CubePlugin )
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA( IID "SourceCodeEditorPlugin" )
#endif


public:
    virtual QString
    name() const;

    virtual bool
    cubeOpened( PluginServices* service );

    void
    version( int& major,
             int& minor,
             int& bugfix ) const;

    virtual QString
    getHelpText() const;

private slots:
    void
    contextMenuIsShown( TreeType  type,
                        TreeItem* item );

    void
    onChangeFont();
    void
    onSaveFile();
    void
    onSaveFileAs();
    void
    onToggleReadOnly( bool checked );
    void
    sourceSlot();
    
    void
    search( const QString &);
    void
    searchForward();
    void 
    searchBackward();
    
    

private:
    PluginServices* service;
    // this is the editor to display and edit source code files
    QTextEdit* textEdit;
    // Selected Font
    QFont fontSourceCode;

    // these are two buttons that get enabled and disabled when toggling the readOnly status
    QPushButton* textEditSaveButton, * textEditSaveAsButton;
    
    // "Find" - input field
    QLineEdit * findEdit;
    
    // these are two buttons for search forwards or backwards
    QPushButton* findNextButton, * findPrevButton;
    
    // holds "search" text
    QString searchText;
    
    
    // this is the name of the last opened/stored source code file
    QString fileName;
    // this is the name of the last source code file opened
    QString lastSourceCodeName;
    // the first time a source code is not found in the given location,
    // we ask if the user wishes to open another file;
    // after this first time, we do not ask any more but directly open
    // an open file dialog
    bool firstSourceNotFound;
    // source code dialog
    QDialog* dialog;

    QString fileNameRegion; // file name of the source code which corresponds to the selected region
    int     startLine;      // begin of the region
    int     endLine;        // end of the region

    void
    sourceCode( const QString& fileName,
                int            startLine,
                int            endLine );
};

#endif // SourceCodeEditorPlugin_H
