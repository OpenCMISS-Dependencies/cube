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



#ifndef _FORTRAN_SYNTAX_HIGHLIGHTER_H
#define _FORTRAN_SYNTAX_HIGHLIGHTER_H


#include <QSyntaxHighlighter>
#include <QTextEdit>

/*-------------------------------------------------------------------------*/
/**
 * @file  FortranSyntaxHighlighter.h
 * @brief Declaration of the class FortranSyntaxHighlighter
 *
 * This header file provides the declaration of the class Source Code Editor.
 */
/*-------------------------------------------------------------------------*/


class FortranSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    FortranSyntaxHighlighter( QTextDocument* parent = 0 );

protected:
    void
    highlightBlock( const QString& text );

private:
    struct HighlightingRule
    {
        QRegExp         pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;
    QRegExp singleLineCommentExpression1;
    QRegExp singleLineCommentExpression2;
    QRegExp OpenMPFormatExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat parallelFormat;
    QTextCharFormat preprocessorFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat OpenMPFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

#endif
