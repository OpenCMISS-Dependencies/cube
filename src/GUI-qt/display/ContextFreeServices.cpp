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


#include "ContextFreeServices.h"

ContextFreeServices* ContextFreeServices::single = 0;

ContextFreeServices::ContextFreeServices()
{
}

ContextFreeServices*
ContextFreeServices::getInstance()
{
    if ( single == 0 )
    {
        single = new ContextFreeServices();
    }
    return single;
}

void
ContextFreeServices::openCube( cube::Cube* cube )
{
    emit openCubeRequest( cube );
}

QWidget*
ContextFreeServices::getWidget()
{
    return widget;
}

void
ContextFreeServices::setWidget( QWidget* w )
{
    widget = w;
}
