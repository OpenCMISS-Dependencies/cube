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


#ifndef TESTPLUGIN1_H
#define TESTPLUGIN1_H
#include <QWidget>
#include <QLabel>
#include "PluginServices.h"
#include "CubePlugin.h"
#include "TabInterface.h"

class SimpleExample : public QObject, public CubePlugin, TabInterface
{
    Q_OBJECT
                             Q_INTERFACES( CubePlugin )

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA( IID "SimpleExamplePlugin" ) // unique plugin name
#endif

public:
    SimpleExample();
    // CubePlugin implementation
    virtual bool
    cubeOpened( PluginServices* service );
    virtual void
    cubeClosed();
    virtual QString
    name() const;
    virtual void
    version( int& major,
             int& minor,
             int& bugfix ) const;
    virtual QString
    getHelpText() const;

    // TabInterface implementation
    virtual QString
    label() const;
    virtual QWidget*
    widget();

private slots:
    void
    treeItemIsSelected( TreeType  type,
                        TreeItem* item );

private:
    QWidget*        widget_;
    QLabel*         qlabel_;
    PluginServices* service;

    // TabInterface interface
};

#endif // TESTPLUGIN1_H
