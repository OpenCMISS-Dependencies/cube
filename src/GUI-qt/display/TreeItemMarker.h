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


#ifndef TREEITEMMARKER_H
#define TREEITEMMARKER_H

#include <QColor>
#include <QPixmap>

class MarkerAttributes
{
public:
    MarkerAttributes();

    MarkerAttributes( const QColor& c );

    const QColor&
    getColor() const;

    void
    release();

    static MarkerAttributes
    getMarkerAttributes();

    friend class TreeItemMarkerDialog;
private:
    void
    setColor( const QColor& color );

    static QList<MarkerAttributes > definedMarker;
    QColor                          color;
};

/**
 * TreeItemMarker is used to visually mark tree items in the view.
 */
class TreeItemMarker
{
    friend class PluginServices;
    friend class TreeItemMarkerDialog;
public:
    ~TreeItemMarker();
    const MarkerAttributes&
    getAttributes() const;
    const QString&
    getLabel() const;
    const QPixmap
    getIcon() const;
    void
    setSize( int iconHeight );

    bool
    isColorVisible() const
    {
        return showColor;
    }
    bool
    isIconVisible() const
    {
        return showIcon;
    }
    static bool
    isGrayedOut()
    {
        return grayItems;
    }

private:
    TreeItemMarker( const QString&        label,
                    const QList<QPixmap>& icons = QList<QPixmap>( ) );
    void
    setColorVisible( bool visible );
    void
    setIconVisible( bool visible );

    QString          label;
    MarkerAttributes attribute;
    QList<QPixmap>   icons; // different sizes of the displayed icon
    QPixmap          currentIcon;
    bool             showColor;
    bool             showIcon;
    static bool      grayItems; // if true, gray out all items without markers
};

#endif // TREEITEMMARKER_H
