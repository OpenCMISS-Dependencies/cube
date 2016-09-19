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




#ifndef _CUBEPL_SYNTAX_HIGHLIGHTER_H
#define _CUBEPL_SYNTAX_HIGHLIGHTER_H


#include <QSyntaxHighlighter>
#include <QTextEdit>

/*-------------------------------------------------------------------------*/
/**
 * @file  CubePLSyntaxHighlighter.h
 * @brief Declaration of the class CubePLSyntaxHighlighter
 *
 * This header file provides the declaration of the class HelpBrowser.
 */
/*-------------------------------------------------------------------------*/




class CubePLSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    CubePLSyntaxHighlighter( QTextDocument* parent = 0 );


    void
    setErrorPlace( int err )
    {
        err_column = err;
    };

protected:

    /// Highlighting block
    void
    highlightBlock( const QString& block );



    int err_column;

    struct HighlightingRule
    {
        QRegExp         pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat errorFormat;
    QTextCharFormat operatorFormat;
    QTextCharFormat variablesFormat;
};


#endif
