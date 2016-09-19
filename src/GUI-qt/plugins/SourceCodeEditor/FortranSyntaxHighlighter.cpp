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




#ifndef _FORTRAN_SYNTAX_HIGHLIGHTER_CPP
#define _FORTRAN_SYNTAX_HIGHLIGHTER_CPP
#include <iostream>

using namespace std;

 #include <QtGui>

 #include "FortranSyntaxHighlighter.h"

FortranSyntaxHighlighter::FortranSyntaxHighlighter( QTextDocument* parent )
    : QSyntaxHighlighter( parent )
{
    HighlightingRule rule;


    setCurrentBlockState( 0 );
    quotationFormat.setForeground( Qt::darkRed );
    quotationFormat.setFontItalic( true );
    rule.pattern = QRegExp( "\".*\"" );
    rule.format  = quotationFormat;
    highlightingRules.append( rule );

    functionFormat.setFontItalic( true );
    functionFormat.setForeground( Qt::blue );
    rule.pattern = QRegExp( "\\b[A-Za-z0-9_]+(?=\\()" );
    rule.format  = functionFormat;
    highlightingRules.append( rule );


    keywordFormat.setForeground( Qt::darkBlue );
    keywordFormat.setFontWeight( QFont::Bold );
    parallelFormat.setForeground( Qt::darkGreen );
    parallelFormat.setFontWeight( QFont::Normal );
    preprocessorFormat.setForeground( Qt::darkGray );
    preprocessorFormat.setFontWeight( QFont::Normal );
    preprocessorFormat.setFontItalic( true );
    QStringList keywordPatterns;
    QStringList parallelPatterns;
    QStringList preprocessorPatterns;
    keywordPatterns << "\\bcharacter\\b"
                    << "\\bdouble\\b"
                    << "\\binteger\\b"
                    << "\\bparameter\\b"
                    << "\\bexternal\\b"
                    << "\\bprecision\\b"
                    << "\\blogical\\b"
                    << "\\bcommon\\b"
                    << "\\bfor\\b"
                    << "\\bif\\b" << "\\bthen\\b" << "\\belse\\b" << "\\belseif\\b"
                    << "\\bendif\\b"  << "\\bswitch\\b"  << "\\bdo\\b"
                    << "\\bend\\s*do\\b" << "\\bend\\b"
                    << "\\bcase\\b" << "\\bcall\\b"  << "\\bsubroutine\\b"
                    << "\\bcontinue\\b"  << "\\bgoto\\b"
    ;
    parallelPatterns << "\\bmpi_[^(,\\s]*\\b"
    ;
    preprocessorPatterns << "^\\s*include\\s+\"[^\n]*\""
                         << "^\\s*include\\s+<[^\n]*>"
                         << "^\\s*include\\s+'[^\n]*'"
                         << "^\\s*define\\b"
    ;

    foreach( const QString &pattern, keywordPatterns )
    {
        rule.pattern = QRegExp( pattern, Qt::CaseInsensitive    );
        rule.format  = keywordFormat;
        highlightingRules.append( rule );
    }
    foreach( const QString &pattern, parallelPatterns )
    {
        rule.pattern = QRegExp( pattern );
        rule.format  = parallelFormat;
        highlightingRules.append( rule );
    }
    foreach( const QString &pattern, preprocessorPatterns )
    {
        rule.pattern = QRegExp( pattern );
        rule.format  = preprocessorFormat;
        highlightingRules.append( rule );
    }


    singleLineCommentFormat.setForeground( Qt::lightGray );
    singleLineCommentExpression1 = QRegExp( "^\\s*[cC]\\W+[^\n]*" );
    singleLineCommentExpression2 = QRegExp( "^\\s*![^$][^\n]*" );

    OpenMPFormat.setForeground( Qt::green );
    OpenMPFormatExpression = QRegExp( "\\s*!\\$OMP[^\n]*" );
}

void
FortranSyntaxHighlighter::highlightBlock( const QString& text )
{
    int commentLength;
    int startIndex = -1;
    startIndex = singleLineCommentExpression1.indexIn( text );
    if ( startIndex >= 0 )
    {
        commentLength = text.length() - startIndex;
        setFormat( startIndex, commentLength, singleLineCommentFormat );
    }
    else
    {
        startIndex = singleLineCommentExpression2.indexIn( text );
        if ( startIndex >= 0 )
        {
            commentLength = text.length() - startIndex;
            setFormat( startIndex, commentLength, singleLineCommentFormat );
        }
        else
        {
            startIndex = OpenMPFormatExpression.indexIn( text );
            if ( startIndex >= 0 )
            {
                commentLength = text.length() - startIndex;
                setFormat( startIndex, commentLength, OpenMPFormat );
            }
            else
            {
                foreach( const HighlightingRule &rule, highlightingRules )
                {
                    QRegExp expression( rule.pattern );
                    int     index = expression.indexIn( text );
                    while ( index >= 0 )
                    {
                        int length = expression.matchedLength();
                        setFormat( index, length, rule.format );
                        index = expression.indexIn( text, index + length );
                    }
                }
            }
            setCurrentBlockState( 0 );
        }
    }
}

#endif
