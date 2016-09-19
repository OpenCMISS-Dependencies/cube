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



#ifndef _HTML_HELP_COLLECTION_H
#define _HTML_HELP_COLLECTION_H

#include <QString>

#define NUM_HELP_TEXTES 12
enum HELP_KEY { NO_HELP                                   = 0,
                CUBEPL_HELP                               = 1,
                DERIVED_METRIC_HELP                       = 2,
                DERIVED_METRIC_FROM_FILE_HELP             = 3,
                DERIVED_METRIC_EXPRESSION_HELP            = 4,
                DERIVED_METRIC_INIT_EXPRESSION_HELP       = 5,
                DERIVED_METRIC_AGGR_PLUS_EXPRESSION_HELP  = 6,
                DERIVED_METRIC_AGGR_MINUS_EXPRESSION_HELP = 7,
                WHAT_IS_CLIPBOARD                         = 8,
                WHAT_IS_FROM_FILE                         = 9,
                WHAT_IS_TO_FILE                           = 10,
                WHAT_IS_DERIVED_METRICS_LIBRARY           = 11 };

/*-------------------------------------------------------------------------*/
/**
 * @file  HtmlHelpCollection.h
 * @brief Declaration of the class HelpCollection
 *
 * This header file provides the declaration of the class HelpCollection.
 */
/*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/
/**
 * @class HelpCollection
 * @brief Provides a collection of Help textes.
 *
 * This class provides a simple collection of help textes, formatted in HTML.
 * Under Windows we cannot use path for  installed files (missing  configure)
 */
/*-------------------------------------------------------------------------*/

class HelpCollection
{
public:

    // / returns help text
    static QString& getHelpText( HELP_KEY );



private:

    static QString textes[ NUM_HELP_TEXTES ];
};


#endif
