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


#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QColorDialog>
#include <QGridLayout>
#include <QCheckBox>
#include <QSpacerItem>
#include <QDialogButtonBox>
#include "TreeItemMarkerDialog.h"
#include "PluginManager.h"
#include "TreeItemMarker.h"
#include "TabManager.h"

TreeItemMarkerDialog::TreeItemMarkerDialog( TabManager* tab, QWidget* parent ) : QDialog( parent ), tabManager( tab )
{
    if ( !tabManager )
    {
        return; // if no cube file is loaded (=> TabManager is null) no markers are defined
    }
    this->setWindowTitle( "Tree Item Marker" );
    itemMarker = PluginManager::getInstance()->getTreeItemMarker();

    QVBoxLayout* main = new QVBoxLayout();
    setLayout( main );
    QWidget* table = new QWidget();
    main->addWidget( new QLabel( "Configure Tree Item Marker" ) );
    main->addWidget( table );

    QCheckBox* gray = new QCheckBox( "gray out items without marker" );
    connect( gray, SIGNAL( toggled( bool ) ), this, SLOT( grayItems( bool ) ) );
    main->addWidget( gray );
    gray->setChecked( TreeItemMarker::isGrayedOut() );

    QGridLayout* gridLayout = new QGridLayout();
    int          col        = 0;
    int          row        = 0;
    foreach( TreeItemMarker * marker, itemMarker )
    {
        QPushButton* choose = new QPushButton(  " " + marker->getLabel() + " " );
        choose->setFlat( true );
        choose->setAutoFillBackground( true );
        QPalette bp = choose->palette();
        bp.setColor( QPalette::Button, marker->getAttributes().getColor() );
        choose->setPalette( bp );
        QVariant var = qVariantFromValue( ( void* )marker );
        choose->setProperty( "TreeItemMarker", var );
        choose->setProperty( "index", row );
        connect( choose, SIGNAL( pressed() ), this, SLOT( chooseColor() ) );
        buttons.append( choose );
        colors.append( marker->getAttributes().getColor() );

        QLabel* icon    = new QLabel();
        bool    visible = marker->isIconVisible();
        marker->setIconVisible( true );
        marker->setSize( choose->fontMetrics().height() + choose->fontMetrics().descent() );
        icon->setPixmap( marker->getIcon() );
        marker->setIconVisible( visible );

        QCheckBox* showColor = new QCheckBox( "color" );
        showColor->setChecked( marker->isColorVisible() );
        colorIsVisible.append( marker->isColorVisible() );
        showColor->setProperty( "TreeItemMarker", var );
        showColor->setProperty( "index", row );
        connect( showColor, SIGNAL( toggled( bool ) ), this, SLOT( enableColor( bool ) ) );

        QCheckBox* showIcon = new QCheckBox( "icon" );
        showIcon->setChecked( marker->isIconVisible() );
        iconIsVisible.append( marker->isIconVisible() );
        showIcon->setProperty( "TreeItemMarker", var );
        showIcon->setProperty( "index", row );
        connect( showIcon, SIGNAL( toggled( bool ) ), this, SLOT( enableIcon( bool ) ) );
        if ( marker->getIcon().isNull() )
        {
            showIcon->setEnabled( false );
            showIcon->setChecked( false );
        }

        gridLayout->addWidget( choose, row, col++ );
        gridLayout->addWidget( icon,  row, col++ );
        gridLayout->addItem( new QSpacerItem( 30, 1 ),  row, col++ );
        gridLayout->addWidget( showColor, row, col++ );
        gridLayout->addWidget( showIcon, row, col++ );

        col = 0;
        row++;
    }
    table->setLayout( gridLayout );
    QDialogButtonBox* buttonBox = new QDialogButtonBox( QDialogButtonBox::Cancel | QDialogButtonBox::Apply | QDialogButtonBox::Ok );
    QPushButton*      apply     = buttonBox->button( QDialogButtonBox::Apply );
    connect( buttonBox, SIGNAL( accepted() ), this, SLOT( accept() ) );
    connect( buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );
    connect( apply,     SIGNAL( pressed() ), this, SLOT( apply() ) );
    main->addWidget( buttonBox );
}

void
TreeItemMarkerDialog::accept()
{
    QDialog::accept();
    tabManager->updateTreeItems();
}

void
TreeItemMarkerDialog::reject()
{
    QDialog::reject();
    int idx = 0;
    foreach( TreeItemMarker * marker, itemMarker )
    {
        marker->setColorVisible( colorIsVisible.at( idx ) );
        marker->setIconVisible( iconIsVisible.at( idx ) );
        marker->attribute.setColor( colors.at( idx ) );
        idx++;
    }

    tabManager->updateTreeItems();
}

void
TreeItemMarkerDialog::apply()
{
    tabManager->updateTreeItems();
}

void
TreeItemMarkerDialog::grayItems( bool enabled )
{
    TreeItemMarker::grayItems = enabled;
}

void
TreeItemMarkerDialog::enableColor( bool enabled )
{
    TreeItemMarker* ma = ( TreeItemMarker* )( sender()->property( "TreeItemMarker" ) ).value<void*>();
    ma->setColorVisible( enabled );
}

void
TreeItemMarkerDialog::enableIcon( bool enabled )
{
    TreeItemMarker* ma = ( TreeItemMarker* )( sender()->property( "TreeItemMarker" ) ).value<void*>();
    ma->setIconVisible( enabled );
}

void
TreeItemMarkerDialog::chooseColor()
{
    TreeItemMarker* ma    = ( TreeItemMarker* )( sender()->property( "TreeItemMarker" ) ).value<void*>();
    QColor          color = QColorDialog::getColor( ma->getAttributes().getColor(), this );

    if ( color.isValid() )
    {
        ma->attribute.setColor( color );

        int          index = sender()->property( "index" ).toInt();
        QPushButton* but   = buttons.at( index );
        QPalette     p     = but->palette();
        p.setColor( QPalette::Button, color );
        but->setPalette( p );
    }
}
