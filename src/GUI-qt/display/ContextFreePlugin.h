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


#ifndef CONTEXTFREEPLUGIN_H
#define CONTEXTFREEPLUGIN_H

#include <QTextStream>
#include "PluginInterface.h"

class ContextFreeServices;

/** @brief Plugins that are subclass of ContextFreePlugin don't depend on a cube file. They only
 * can be activated if no cube file is loaded.
 */
class ContextFreePlugin : public PluginInterface
{
public:
    /**
     * @brief opened is called if the plugin is activated. All Resources should be allocated here,
     * not in a constructor.
     */
    virtual void
    opened( ContextFreeServices* service ) = 0;

    /**
     * @brief closed is called if the plugin is closed by the user. If resources have been allocated,
     * they should be freed here.
     */
    virtual void
    closed()
    {
    };

    virtual void
    version( int& major,
             int& minor,
             int& bugfix ) const = 0;

    virtual QString
    name() const = 0;

    virtual QString
    getHelpText() const = 0;

    virtual
    ~ContextFreePlugin()
    {
    }
};

Q_DECLARE_INTERFACE( ContextFreePlugin, PLUGIN_VERSION );


#endif // CONTEXTFREEPLUGIN_H
