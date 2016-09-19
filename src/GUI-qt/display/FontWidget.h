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


#ifndef _FONTWIDGET_H
#define _FONTWIDGET_H

#include <QDialog>
#include <QFontComboBox>
#include <QSpinBox>
#include <QRadioButton>

#define USE_DEFAULT "USE_DEFAULT"

// FontWidget is used to allow the user to define a font, font size, and
// line spacing for tree widgets

class FontWidget : public QDialog
{
    Q_OBJECT

signals:

    void
    apply( FontWidget* fontWidget );

private slots:

    void
    onOk();
    void
    onApply();
    void
    onCancel();
    void
    setSelectionColor();
    void
    setDefaulSelectionColor( bool on );

public:
    FontWidget( QWidget*       parent,
                const QString& family,
                int            pointSize,
                const QColor&  selectionColor );

    QFont
    getFont();
    QString
    getFontFamily();
    int
    getFontSize();
    QColor
    getSelectionColor();

private:

    QFontComboBox* fontCombo;
    QComboBox*     sizeCombo;
    QPushButton*   colorBut;
    QColor         color;
    QColor         initialColor;
    QColor         defaultColor;
    QRadioButton*  selectColor;
    int            familyIndex; // stores initial selected index of combo box, used for "Cancel"
    int            sizeIndex;
};

#endif
