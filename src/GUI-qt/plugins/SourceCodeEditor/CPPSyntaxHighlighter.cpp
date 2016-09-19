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


#ifndef _CPP_SYNTAX_HIGHLIGHTER_CPP
#define _CPP_SYNTAX_HIGHLIGHTER_CPP


 #include <QtGui>

 #include "CPPSyntaxHighlighter.h"

CPPSyntaxHighlighter::CPPSyntaxHighlighter( QTextDocument* parent )
    : QSyntaxHighlighter( parent )
{
    HighlightingRule rule;



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
    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                    << "\\bvoid\\b" << "\\bvolatile\\b"  << "\\bfor\\b"
                    << "\\bif\\b" << "\\belse\\b"  << "\\bswitch\\b"
                    << "\\bcase\\b"   << "\\bdefault\\b"  << "\\bbreak\\b"
                    << "\\bcontinue\\b"
    ;
    parallelPatterns << "\\bMPI_[^(,\\s]*\\b"
    ;
    preprocessorPatterns << "^\\s*#\\s*include\\s+\"[^\n]*\""
                         << "^\\s*#\\s*include\\s+<[^\n]*>"
                         << "^\\s*#\\s*define\\b"
                         << "^\\s*#\\s*ifdef\\b"
                         << "^\\s*#\\s*ifndef\\b"
                         << "^\\s*#\\s*if\\b"
                         << "^\\s*#\\s*defined\\b"
                         << "^\\s*#\\s*endif\\b"
                         << "^\\s*#\\s*else\\b"
    ;

    foreach( const QString &pattern, keywordPatterns )
    {
        rule.pattern = QRegExp( pattern );
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
    singleLineCommentExpression = QRegExp( "//[^\n]*" );
    OpenMPFormat.setForeground( Qt::green );
    OpenMPFormatExpression = QRegExp( "\\s*#\\s*pragma\\s+[p]?omp[^\n]*" );
//      rule.format = singleLineCommentFormat;
//      highlightingRules.append(rule);

    multiLineCommentFormat.setForeground( Qt::lightGray );


    commentStartExpression = QRegExp( "/\\*" );
    commentEndExpression   = QRegExp( "\\*/" );
}

void
CPPSyntaxHighlighter::highlightBlock( const QString& text )
{
    int commentLength;
    int startIndex = 0;
    if ( previousBlockState() != 1 )
    {
        startIndex = singleLineCommentExpression.indexIn( text );
    }
    if ( startIndex >= 0 )
    {
        commentLength = text.length() - startIndex;
        setFormat( startIndex, commentLength, singleLineCommentFormat );
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
                if ( currentBlockState() != 1 )
                {
                    setFormat( index, length, rule.format );
                }
                index = expression.indexIn( text, index + length );
            }
        }
        setCurrentBlockState( 0 );

        if ( previousBlockState() != 1 )
        {
            startIndex = OpenMPFormatExpression.indexIn( text );
        }
        if ( startIndex >= 0 )
        {
            commentLength = text.length() - startIndex;
            setFormat( startIndex, commentLength, OpenMPFormat );
        }
    }

    if ( previousBlockState() != 1 )
    {
        startIndex = commentStartExpression.indexIn( text );
    }

    while ( startIndex >= 0 )
    {
        int endIndex = commentEndExpression.indexIn( text, startIndex );

        if ( endIndex == -1 )
        {
            setCurrentBlockState( 1 );
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat( startIndex, commentLength, multiLineCommentFormat );
        startIndex = commentStartExpression.indexIn( text, startIndex + commentLength );
    }
}

#endif
