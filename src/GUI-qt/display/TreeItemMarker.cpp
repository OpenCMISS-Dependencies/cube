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


#include "TreeItemMarker.h"

QList<MarkerAttributes> MarkerAttributes::definedMarker = QList<MarkerAttributes>();

MarkerAttributes::MarkerAttributes()
{
}

MarkerAttributes::MarkerAttributes( const QColor& c )
{
    color = c;
}

const QColor&
MarkerAttributes::getColor() const
{
    return color;
}

void
MarkerAttributes::setColor( const QColor& color )
{
    this->color = color;
}

void
MarkerAttributes::release()
{
    definedMarker.insert( 0, *this );
}

MarkerAttributes
MarkerAttributes::getMarkerAttributes()
{
    if ( definedMarker.isEmpty() )
    {
        definedMarker.append( MarkerAttributes( Qt::green ) );
        definedMarker.append( MarkerAttributes( Qt::cyan ) );
        definedMarker.append( MarkerAttributes( Qt::darkYellow ) );
        definedMarker.append( MarkerAttributes( Qt::darkMagenta ) );
        definedMarker.append( MarkerAttributes( Qt::blue ) );
        definedMarker.append( MarkerAttributes( Qt::red ) );
    }
    else if ( definedMarker.size() == 1 )
    {
        definedMarker.append( MarkerAttributes( Qt::red ) );
    }
    return definedMarker.takeFirst();
}

// =========================================================
bool TreeItemMarker::grayItems = false;

TreeItemMarker::TreeItemMarker( const QString& label, const QList<QPixmap>& icons )
{
    this->label = label;
    this->icons = icons;
    attribute   = MarkerAttributes::getMarkerAttributes();
    currentIcon = QPixmap();
    showColor   = true;
    showIcon    = true;
}

TreeItemMarker::~TreeItemMarker()
{
    attribute.release();
}

const MarkerAttributes&
TreeItemMarker::getAttributes() const
{
    return attribute;
}

const QString&
TreeItemMarker::getLabel() const
{
    return label;
}

const QPixmap
TreeItemMarker::getIcon() const
{
    return currentIcon;
}

void
TreeItemMarker::setSize( int iconHeight )
{
    if ( icons.size() == 0 || !showIcon )
    {
        currentIcon = QPixmap();
        return;
    }

    foreach( QPixmap p, icons )
    {
        if ( p.size().height() > iconHeight )
        {
            currentIcon = p;
            break;
        }
    }

    currentIcon = currentIcon.scaledToHeight( iconHeight );
}

void
TreeItemMarker::setColorVisible( bool visible )
{
    showColor = visible;
}

void
TreeItemMarker::setIconVisible( bool visible )
{
    showIcon = visible;
}


