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


#include "BoxPlot.h"
#include "Pretty.h"
#include "TableWidget.h"
#include "StatisticalInformation.h"
#include "Globals.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

#include <QFontMetrics>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QDialog>
#include <QString>
#include <QPen>
#include <QDebug>
#include <math.h>

using namespace std;

namespace
{
class HeightCalculator
{
public:
    HeightCalculator( int    offset,
                      int    height,
                      double max,
                      double min )
        : lowOffset( offset ), clientHeight( height ),
          maxValue( max ), minValue( min )
    {
    }
    int
    operator()( double val ) const
    {
        return static_cast<int>( lowOffset - clientHeight /
                                 ( maxValue - minValue ) * ( Clip( val ) - minValue ) );
    }
    enum Position { Inside, Above, Below };
    Position
    CalculatePosition( double value ) const
    {
        if ( value > maxValue )
        {
            return Above;
        }
        if ( value < minValue )
        {
            return Below;
        }
        return Inside;
    }
    double
    Clip( double value ) const
    {
        if ( value > maxValue )
        {
            return maxValue;
        }
        if ( value < minValue )
        {
            return minValue;
        }
        return value;
    }
private:
    int    lowOffset, clientHeight;
    double maxValue, minValue;
};
}

BoxPlot::BoxPlot( QWidget* parent )
    : QWidget( parent ), firstPosition( 0, 0 )
{
    tooltip      = 0;
    minimumValue = start;
    maximumValue = 0.0;
    start        = 0.;
    borderPad    = 10;
    tickWidth    = 3;

    // setWindowTitle("BoxPlot");
    resize( 300, 450 );
    items.resize( 0 );
    ResetMinMaxValues();
    if ( parent != 0 )
    {
        parent->adjustSize();
    }
}

void
BoxPlot::Add( StatisticalInformation const& theStatisticalInformation )
{
    items.push_back( theStatisticalInformation );
    resize( 150 + 80 * items.size(), 450 );
    if ( parentWidget() != 0 )
    {
        parentWidget()->adjustSize();
    }
    ResetView();
}

/**
   set the data of a boxplot which consists only of one StatisticalInformation
   existing StatisticalInformations will be deleted
   @param theStatisticalInformation values of the boxplot
   @param abs the corresponding absoulte values

 */
void
BoxPlot::Set( StatisticalInformation const& theStatisticalInformation,  StatisticalInformation const& absStatisticalInformation, double startValue )
{
    start = startValue;
    items.clear();
    items.push_back( theStatisticalInformation );
    absolute = absStatisticalInformation;
    ResetView();
}

void
BoxPlot::Reset()
{
    items.clear();
    ResetView();
}

void
BoxPlot::ResetMinMaxValues()
{
    minimumValue = start;
    maximumValue = 0.0;
    if ( items.size() != 0 )
    {
        double maxVal = numeric_limits<double>::min();
        for ( vector<StatisticalInformation>::const_iterator it = items.begin(); it != items.end(); ++it )
        {
            if ( it->maximum > maxVal )
            {
                maxVal = it->maximum;
            }
        }
        maximumValue = maxVal;
    }
    AdjustMinMaxValues();
}

void
BoxPlot::AdjustMinMaxValues()
{
    vector<double> values;
    values.resize( 0 );
    Pretty( minimumValue, maximumValue, numberOfAxisEntries, values );
    minimumValue = *values.begin();
    maximumValue = *values.rbegin();
}

void
BoxPlot::ResetView()
{
    ResetMinMaxValues();
    update();
}

// Calculates the value on the y-axis which belongs to a height of koord pixels on the axis
double
BoxPlot::GetValue( int koord ) const
{
    int lowOffset    = GetCanvasHeight() - GetLowerBorderHeight();
    int clientHeight = lowOffset - GetUpperBorderHeight();
    return ( lowOffset - koord ) * ( GetMaxValue() - GetMinValue() )
           / clientHeight + GetMinValue();
}

void
BoxPlot::paintEvent( QPaintEvent* )
{
    QPainter painter( this );
    if ( items.size() == 0 )
    {
        painter.drawText( rect(), Qt::AlignCenter, "Not enough values to display." );
        return;
    }
    if ( fabs( maximumValue - minimumValue ) < 1e-8 )
    {
        maximumValue = minimumValue + 0.5; // create valid box
        minimumValue = maximumValue - 1.0;
    }

    DrawLeftLegend( painter );
    DrawLowerLegend( painter );
    DrawRightLegend( painter );
    DrawBoxPlots( painter );
    painter.drawRect( GetLeftBorderWidth(), GetUpperBorderHeight(),
                      GetCanvasWidth() - GetRightBorderWidth() - GetLeftBorderWidth(),
                      GetCanvasHeight() - GetLowerBorderHeight() - GetUpperBorderHeight() );
}

void
BoxPlot::mousePressEvent( QMouseEvent* event )
{
    if ( items.size() == 0 )
    {
        return;
    }
    else
    {
        firstPosition.first  = event->x();
        firstPosition.second = event->y();

        if ( event->button() == Qt::RightButton )
        {
            StatisticalInformation const* clicked = GetClickedStatisticalInformation( event );
            if ( clicked != 0 )
            {
                ShowStatisticsTooltip( clicked );
            }
        }
    }
}

#define EPS 10
void
BoxPlot::mouseReleaseEvent( QMouseEvent* event )
{
    switch ( event->button() )
    {
        case Qt::LeftButton:
            if ( items.size() == 0 )
            {
                return;
            }
            else
            {
                double newMaxVal, newMinVal;
                if ( firstPosition.second < event->y() )
                {
                    newMaxVal = GetValue( firstPosition.second );
                    newMinVal = GetValue( event->y() );
                }
                else
                {
                    newMinVal = GetValue( firstPosition.second );
                    newMaxVal = GetValue( event->y() );
                }
                if ( event->y() - firstPosition.second < EPS )
                {
                    StatisticalInformation const* clicked = GetClickedStatisticalInformation( event );
                    if ( clicked != 0 )
                    {
                        ShowStatistics( clicked );
                    }
                }
                else
                {
                    SetMinMaxValues( newMinVal, newMaxVal );
                }
            }
            break;
        case Qt::MidButton:
            ResetView();
            break;
        default:
            break;
    }
    if ( tooltip )
    {
        tooltip->close();
    }
}

void
BoxPlot::SetMinMaxValues( double newMinVal, double newMaxVal )
{
    if ( newMaxVal < maximumValue )
    {
        maximumValue = newMaxVal;
    }
    if ( newMinVal > minimumValue )
    {
        minimumValue = newMinVal;
    }
    AdjustMinMaxValues();
    update();
}

int
BoxPlot::GetCanvasWidth() const
{
    return width();
}

int
BoxPlot::GetCanvasHeight() const
{
    return height();
}

int
BoxPlot::GetTextWidth( QString const& text ) const
{
    QFontMetrics fm( font() );
    return fm.width( text );
}

int
BoxPlot::GetTextWidth( string const& text ) const
{
    return GetTextWidth( QString::fromStdString( text ) );
}

int
BoxPlot::GetTextHeight( QString const& ) const
{
    QFontMetrics fm( font() );
    return 3 * fm.ascent() / 2;
}

int
BoxPlot::GetTextHeight( string const& text ) const
{
    return GetTextHeight( QString::fromStdString( text ) );
}

int
BoxPlot::GetLeftBorderWidth() const
{
    double value      = GetMaxValue();
    double labelWidth = GetTextWidth( Globals::formatNumber( value, value, true ) );

    return labelWidth + 3 * tickWidth + borderPad;
}

/**
   calculate height neede for DrawLowerLegend
 */
int
BoxPlot::GetLowerBorderHeight() const
{
    if ( items.size() == 1 )                   // only one boxplot StatisticalInformation
    {
        if ( items.at( 0 ).name.length() > 0 ) // has label
        {
            return GetTextHeight( items.at( 0 ).name );
        }
        else
        {
            return GetTextHeight( QString( "|" ) );
        }
    }

    string longestName;
    {
        unsigned int longestLength = 0;
        for ( vector<StatisticalInformation>::const_iterator it = items.begin(); it != items.end(); ++it )
        {
            if ( it->name.length() > longestLength )
            {
                longestLength = it->name.length();
                longestName   = it->name;
            }
        }
    }
    int textHeight  = GetTextHeight( longestName );
    int clientWidth = GetCanvasWidth() - GetRightBorderWidth()
                      - GetLeftBorderWidth();
    int textWidth       = GetTextWidth( "00:" + longestName );
    int numberOfColumns = clientWidth / textWidth;
    int numberOfRows    = ( numberOfColumns == 0
                            ? 2
                            : ( items.size() + numberOfColumns - 1 ) / numberOfColumns );
    return ( 2 + 2 * numberOfRows ) * textHeight;
}

int
BoxPlot::GetUpperBorderHeight() const
{
    return GetCanvasHeight() / 20;
}

int
BoxPlot::GetRightBorderWidth() const
{
    if ( absolute.count == 0 ) // no right legend
    {
        return GetCanvasWidth() / 20;
    }
    else
    {
        int    maxWidth = 0;
        double values[] = { absolute.median, absolute.mean, absolute.q1, absolute.q3, absolute.maximum, absolute.minimum };
        for ( int i = 0; i < 6; i++ )
        {
            maxWidth = max( maxWidth, GetTextWidth( Globals::formatNumber( values[ i ], values[ i ], true ) ) );
        }
        maxWidth += 3 * tickWidth + borderPad;
        return maxWidth;
    }
}

double
BoxPlot::GetMinValue() const
{
    return minimumValue;
}

double
BoxPlot::GetMaxValue() const
{
    return maximumValue;
}

/** draws right legend with absolute values, if absolute values are set */
void
BoxPlot::DrawRightLegend( QPainter& painter )
{
    if ( absolute.count == 0 )
    {
        return;
    }

    int        x = GetCanvasWidth() - GetRightBorderWidth();
    QList<int> oldPos;
    double     referenceValue = absolute.maximum;
    DrawMark( painter, x, items.at( 0 ).median, absolute.median, referenceValue, oldPos );
    DrawMark( painter, x, items.at( 0 ).mean, absolute.mean, referenceValue, oldPos );
    DrawMark( painter, x, items.at( 0 ).q1, absolute.q1, referenceValue, oldPos );
    DrawMark( painter, x, items.at( 0 ).q3, absolute.q3, referenceValue, oldPos );
    DrawMark( painter, x, items.at( 0 ).maximum, absolute.maximum, referenceValue, oldPos );
    DrawMark( painter, x, items.at( 0 ).minimum,  absolute.minimum, referenceValue, oldPos );
}

/** draws one right tick and corresponding label
    @param x x-position of the tick
    @param value current value which defindes the y position
    @param label absolute value corresponding to value, used as label text
    @param oldPos list of previous positions to avoid collisions
 */
void
BoxPlot::DrawMark( QPainter& painter, int x, double value, double label, double reference, QList<int>& oldPos )
{
    int clientHeight = GetCanvasHeight() - GetLowerBorderHeight()
                       - GetUpperBorderHeight();

    int y = static_cast<int>( GetCanvasHeight() - GetLowerBorderHeight()
                              - clientHeight / ( maximumValue - minimumValue ) * ( value - minimumValue ) );
    int h = GetTextHeight( QString( "8" ) );

    for ( int i = 0; i < oldPos.size(); ++i )
    {
        if ( ( y + h > oldPos[ i ] ) && ( y < oldPos[ i ] + h ) ) // collision with previous label => do not draw
        {
            return;
        }
    }
    oldPos.append( y );

    painter.drawLine( x, y, x + tickWidth, y );
    painter.drawText( x + 3 * tickWidth, y - h / 3,
                      100, h, Qt::AlignLeft, Globals::formatNumber( label, reference, true ) );
}

void
BoxPlot::DrawLeftLegend( QPainter& painter )
{
    int upperOffset  = GetUpperBorderHeight();
    int clientHeight = GetCanvasHeight() - GetLowerBorderHeight()
                       - upperOffset;
    int clientWidth = GetLeftBorderWidth();

    vector<double> values;
    values.resize( 0 );
    Pretty( GetMinValue(), GetMaxValue(), numberOfAxisEntries, values );
    double minValue = *values.begin();
    double maxValue = *values.rbegin();

    int textHeight = GetTextHeight( QString( "8" ) );

    double referenceValue = GetMaxValue();
    for ( vector<double>::const_iterator it = values.begin(); it != values.end(); ++it )
    {
        int currYPos = static_cast<int>( clientHeight + upperOffset
                                         - clientHeight / ( maxValue - minValue ) * ( *it - minValue ) );
        painter.drawLine( clientWidth, currYPos, clientWidth - tickWidth, currYPos );
        QPen oldPen = painter.pen();
        QPen dashedPen( Qt::DashLine );
        dashedPen.setColor( Qt::cyan );
        painter.setPen( dashedPen );
        painter.drawLine( clientWidth, currYPos,
                          GetCanvasWidth() - GetRightBorderWidth(), currYPos );
        painter.setPen( oldPen );
        painter.drawText( 0, currYPos - textHeight / 3, clientWidth - 3 * tickWidth,
                          textHeight, Qt::AlignRight,
                          Globals::formatNumber( *it, referenceValue, true ) );
    }
}

void
BoxPlot::DrawLowerLegend( QPainter& painter )
{
    int    leftOffset   = GetLeftBorderWidth();
    int    clientWidth  = GetCanvasWidth() - GetRightBorderWidth() - leftOffset;
    int    clientHeight = GetLowerBorderHeight();
    string longestName;
    {
        unsigned int longestLength = 0;
        for ( vector<StatisticalInformation>::const_iterator it = items.begin(); it != items.end(); ++it )
        {
            if ( it->name.length() > longestLength )
            {
                longestLength = it->name.length();
                longestName   = it->name;
            }
        }
    }
    int textWidth = GetTextWidth( "00:" + longestName ) * 4 / 3;

    int numberOfEntries = items.size();
    int numberOfColumns = clientWidth / textWidth;
    if ( numberOfColumns == 0 )
    {
        return;
    }
    int numberOfRows = ( numberOfEntries + numberOfColumns - 1 ) / numberOfColumns;

    int legendHeight      = 2 * GetTextHeight( QString( "1" ) );
    int descriptionHeight = clientHeight - legendHeight;

    int i = 0;
    for ( vector<StatisticalInformation>::const_iterator it = items.begin(); it != items.end(); ++it, ++i )
    {
        int currXOffset = leftOffset + clientWidth / ( numberOfEntries + 1 ) * ( i + 1 );
        /* no legend if only one StatisticalInformation exist */
        if ( items.size() > 1 )
        {
            painter.drawLine( currXOffset, GetCanvasHeight() - clientHeight,
                              currXOffset, GetCanvasHeight() - clientHeight + legendHeight / 4 );
            painter.drawText( currXOffset - GetTextWidth( QString::number( i + 1 ) ) / 3,
                              GetCanvasHeight() - clientHeight + legendHeight * 2 / 3,
                              QString::number( i + 1 ) );
        }
        /* no legend if text if empty */
        if ( it->name.length() > 0 )
        {
            painter.drawText( leftOffset + ( i % numberOfColumns ) * textWidth,
                              GetCanvasHeight() - descriptionHeight
                              + descriptionHeight / ( 3 * numberOfRows / 2 + 1 )
                              * ( 1 + 3 * ( i / numberOfColumns ) ) / 2,
                              QString::number( i + 1 ) + ": " + it->name.c_str() );
        }
    }
}

StatisticalInformation const*
BoxPlot::GetClickedStatisticalInformation( QMouseEvent* event ) const
{
    if ( items.size() == 1 )
    {
        return &( items[ 0 ] );
    }

    int x               = event->x();
    int numberOfEntries = items.size();
    int leftOffset      = GetLeftBorderWidth();
    int clientWidth     = GetCanvasWidth() - GetRightBorderWidth() - leftOffset;
    int boxWidth        = 2 * clientWidth / ( 3 * ( numberOfEntries + 1 ) );

    int index  = ( x - leftOffset - boxWidth / 2 ) / ( clientWidth / ( numberOfEntries + 1 ) );
    int middle = leftOffset + clientWidth / ( numberOfEntries + 1 ) * ( index + 1 );
    if ( x >= middle - boxWidth / 2 && x <= middle + boxWidth / 2 )
    {
        if ( index >= 0 && index < numberOfEntries )
        {
            return &( items[ index ] );
        }
    }
    return 0;
}

void
BoxPlot::ShowStatisticsTooltip( StatisticalInformation const* StatisticalInformation )
{
    if ( tooltip )
    {
        tooltip->close();
        delete tooltip;
        tooltip = 0;
    }

    QPoint localMouse( firstPosition.first, firstPosition.second );
    QPoint mousePos = this->mapToGlobal( localMouse );
    tooltip = TableWidget::showInToolTip( this, mousePos, StatisticalInformation->Print() );
}

void
BoxPlot::ShowStatistics( StatisticalInformation const* StatisticalInformation )
{
    TableWidget::showInWindow( this, "Statistics info", "", StatisticalInformation->Print() );
}

void
BoxPlot::DrawBoxPlots( QPainter& painter )
{
    int numberOfEntries = items.size();

    int    leftOffset   = GetLeftBorderWidth();
    int    clientWidth  = GetCanvasWidth() - GetRightBorderWidth() - leftOffset;
    int    lowOffset    = GetCanvasHeight() - GetLowerBorderHeight();
    int    clientHeight = lowOffset - GetUpperBorderHeight();
    double maxValue     = GetMaxValue();
    double minValue     = GetMinValue();
    int    boxWidth     = 2 * clientWidth / ( 3 * ( numberOfEntries + 1 ) );

    HeightCalculator        valueModifier( lowOffset, clientHeight, maxValue, minValue );
    HeightCalculator const& CalculateHeight = valueModifier;

    int i = 0;
    for ( vector<StatisticalInformation>::const_iterator it = items.begin(); it != items.end(); ++it, ++i )
    {
        int currWidth = leftOffset + clientWidth / ( numberOfEntries + 1 ) * ( i + 1 );

        // Draw the horizontal lines for minimum and maximum
        QPen oldPen = painter.pen();
        QPen thickPen;
        thickPen.setWidth( 2 );
        painter.setPen( thickPen );
        if ( valueModifier.CalculatePosition( it->minimum ) == HeightCalculator::Inside )
        {
            painter.drawLine( currWidth - boxWidth / 4, CalculateHeight( it->minimum ),
                              currWidth + boxWidth / 4, CalculateHeight( it->minimum ) );
        }
        if ( valueModifier.CalculatePosition( it->maximum ) == HeightCalculator::Inside )
        {
            painter.drawLine( currWidth - boxWidth / 4, CalculateHeight( it->maximum ),
                              currWidth + boxWidth / 4, CalculateHeight( it->maximum ) );
        }
        painter.setPen( oldPen );

        // if there are enough values to have Q75 and Q25
        if ( it->count >= 5 )
        {
            // Draw the connection lines between max/min and Q75/Q25
            painter.setPen( Qt::DashLine );
            if ( valueModifier.CalculatePosition( it->maximum ) != valueModifier.CalculatePosition( it->q3 )
                 || valueModifier.CalculatePosition( it->maximum ) == HeightCalculator::Inside )
            {
                painter.drawLine( currWidth, CalculateHeight( it->maximum ), currWidth,
                                  CalculateHeight( it->q3 ) );
            }
            if ( valueModifier.CalculatePosition( it->minimum ) != valueModifier.CalculatePosition( it->q1 )
                 || valueModifier.CalculatePosition( it->minimum ) == HeightCalculator::Inside )
            {
                painter.drawLine( currWidth, CalculateHeight( it->minimum ), currWidth,
                                  CalculateHeight( it->q1 ) );
            }
            painter.setPen( Qt::SolidLine );

            // Draw the box
            if ( valueModifier.CalculatePosition( it->q3 ) != valueModifier.CalculatePosition( it->q1 )
                 || valueModifier.CalculatePosition( it->q3 ) == HeightCalculator::Inside )
            {
                QRect myBox( currWidth - boxWidth / 2, CalculateHeight( it->q3 ),
                             boxWidth,
                             CalculateHeight( it->q1 ) - CalculateHeight( it->q3 ) );
                painter.fillRect( myBox, QBrush( Qt::white ) );
                painter.drawRect( myBox );
            }
        }
        else
        {
            // Just draw a connection between min and max and no box
            painter.setPen( Qt::DashLine );
            if ( valueModifier.CalculatePosition( it->maximum ) != valueModifier.CalculatePosition( it->minimum )
                 || valueModifier.CalculatePosition( it->maximum ) == HeightCalculator::Inside )
            {
                painter.drawLine( currWidth, CalculateHeight( it->maximum ), currWidth,
                                  CalculateHeight( it->minimum ) );
            }
            painter.setPen( Qt::SolidLine );
        }

        // Draw the median line
        if ( valueModifier.CalculatePosition( it->median ) == HeightCalculator::Inside )
        {
            thickPen.setWidth( 3 );
            painter.setPen( thickPen );
            painter.drawLine( currWidth - boxWidth / 2, CalculateHeight( it->median ),
                              currWidth + boxWidth / 2, CalculateHeight( it->median ) );
            painter.setPen( oldPen );
        }

        // Draw the mean line
        if ( valueModifier.CalculatePosition( it->mean ) == HeightCalculator::Inside )
        {
            painter.setPen( Qt::DashLine );
            painter.drawLine( currWidth - boxWidth / 2, CalculateHeight( it->mean ),
                              currWidth + boxWidth / 2, CalculateHeight( it->mean ) );
            painter.setPen( Qt::SolidLine );
        }
        // values are invalid. Paint error message
        if ( !it->valid_values )
        {
            QPen redPen;
            redPen.setColor( Qt::red );
            painter.setPen( redPen );

            QRect rect = painter.boundingRect( 0, 0, 0, 0, Qt::AlignLeft, "W" ); // get a hight of the line

            for ( int i = 0; i < it->data_status_description.size(); i++ )
            {
                painter.drawText(
                    GetLeftBorderWidth(), GetUpperBorderHeight() +  +( rect.height() + 3 ) * ( i + 1 ),
                    it->data_status_description.at( i )  );
            }
            painter.setPen( oldPen );
        }
    }
}
