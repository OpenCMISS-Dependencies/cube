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


#include "FontWidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTreeView>
#include <QDebug>
#include <QColorDialog>
#include <QGroupBox>

FontWidget::FontWidget( QWidget*       parent,
                        const QString& family,
                        int            pointSize,
                        const QColor&  selectionColor ) : QDialog( parent )
{
    setWindowTitle( "Font settings" );
    initialColor = selectionColor;

    QTreeView view;
    QPalette  treePalette = view.palette();
    defaultColor = treePalette.color( QPalette::Highlight );

    QVBoxLayout* layout = new QVBoxLayout();
    setLayout( layout );

    QLabel* fontLabel = new QLabel( this );
    fontLabel->setText( "Font: " );
    layout->addWidget( fontLabel );

    fontCombo = new QFontComboBox( this );
    layout->addWidget( fontCombo );
    if ( family != USE_DEFAULT )
    {
        fontCombo->setCurrentFont( QFont( family ) );
        fontCombo->insertItem( 0, "Use Qt default", QTreeView().font() );
    }
    else
    {
        fontCombo->insertItem( 0, "Use Qt default", QTreeView().font() );
        fontCombo->setCurrentIndex( 0 );
    }

    // font size
    QLabel* sizeLabel = new QLabel( this );
    sizeLabel->setText( "Size [pt]: " );
    layout->addWidget( sizeLabel );

    sizeCombo = new  QComboBox();
    sizeCombo->addItem( "Use Qt default" );
    for ( int i = 6; i <= 20; i++ )
    {
        sizeCombo->addItem( QString::number( i ) );
        if ( i == pointSize )
        {
            sizeCombo->setCurrentIndex( sizeCombo->count() - 1 );
        }
    }
    if ( pointSize == -1 )
    {
        sizeCombo->setCurrentIndex( 0 );
    }
    layout->addWidget( sizeCombo );

    // selection color
    QGroupBox* box = new QGroupBox();
    //box->setTitle( "Set selection background color" );
    QVBoxLayout* colorLayout = new QVBoxLayout();
    box->setLayout( colorLayout );
    layout->addWidget( box );

    QLabel* label = new QLabel( this );
    label->setText( "Change background color for selected tree items" );
    colorLayout->addWidget( label );
    QWidget* horizontal = new QWidget();
    colorLayout->addWidget( horizontal );
    QHBoxLayout* hlayout = new QHBoxLayout();
    horizontal->setLayout( hlayout );
    QRadioButton* useDefault = new QRadioButton( "Use Qt default" );
    selectColor = new QRadioButton( "Select color" );
    if ( selectionColor == defaultColor )
    {
        useDefault->setChecked( true );
    }
    else
    {
        selectColor->setChecked( true );
    }
    connect( useDefault, SIGNAL( toggled( bool ) ), this, SLOT( setDefaulSelectionColor( bool ) ) );
    hlayout->addWidget( useDefault );
    hlayout->addWidget( selectColor );

    colorBut = new QPushButton( "Selection Color " );
    colorBut->setFlat( true );
    colorBut->setAutoFillBackground( true );

    QPalette palette = colorBut->palette();
    color = selectionColor;
    palette.setColor( colorBut->backgroundRole(), color );
    colorBut->setPalette( palette );
    connect( colorBut, SIGNAL( pressed() ), this, SLOT( setSelectionColor() ) );
    colorLayout->addWidget( colorBut );

    QDialogButtonBox* buttonBox = new QDialogButtonBox();
    buttonBox->addButton( QDialogButtonBox::Ok );
    QPushButton* applyButton = buttonBox->addButton( QDialogButtonBox::Apply );
    buttonBox->addButton( QDialogButtonBox::Cancel );

    connect( buttonBox, SIGNAL( accepted() ), this, SLOT( onOk() ) );
    connect( applyButton, SIGNAL( clicked() ),  this, SLOT( onApply() ) );
    connect( buttonBox, SIGNAL( rejected() ), this, SLOT( onCancel() ) );

    layout->addWidget( buttonBox );

    setWhatsThis( "Opens a dialog to specify the font, the font size (in pt), and the line spacing for the tree displays.  The \"Ok\" button applies the settings to the display and closes the dialog, the \"Apply\" button applies the settings to the display, and \"Cancel\" cancels all changes since the dialog was opened (even if \"Apply\" was pressed in between) and closes the dialog." );

    familyIndex = fontCombo->currentIndex();
    sizeIndex   = sizeCombo->currentIndex();
}

void
FontWidget::setDefaulSelectionColor( bool on )
{
    if ( on )
    {
        color = defaultColor;
        QPalette palette = colorBut->palette();
        palette.setColor( colorBut->backgroundRole(), color );
        colorBut->setPalette( palette );
    }
}

void
FontWidget::setSelectionColor()
{
    selectColor->setChecked( true );
    QColor newColor = QColorDialog::getColor( color, this );
    if ( newColor.isValid() )
    {
        this->color = newColor;
    }
    QPalette palette = colorBut->palette();
    palette.setColor( colorBut->backgroundRole(), color );
    colorBut->setPalette( palette );
}

QFont
FontWidget::getFont()
{
    QFont font = ( fontCombo->currentIndex() > 0 ) ? fontCombo->currentFont() : QTreeView().font();
    int   size = ( sizeCombo->currentIndex() > 0 ) ? sizeCombo->currentText().toInt() : QTreeView().font().pointSize();
    font.setPointSize( size );
    return font;
}

QString
FontWidget::getFontFamily()
{
    return ( fontCombo->currentIndex() > 0 ) ? fontCombo->currentFont().family() : USE_DEFAULT;
}

int
FontWidget::getFontSize()
{
    return ( sizeCombo->currentIndex() > 0 ) ? sizeCombo->currentText().toInt() : -1;
}

QColor
FontWidget::getSelectionColor()
{
    return color;
}

void
FontWidget::onOk()
{
    onApply();
    accept();
}

void
FontWidget::onApply()
{
    emit apply( this );
}

void
FontWidget::onCancel()
{
    fontCombo->setCurrentIndex( familyIndex );
    sizeCombo->setCurrentIndex( sizeIndex );
    color = initialColor;
    onApply();
    reject();
}
