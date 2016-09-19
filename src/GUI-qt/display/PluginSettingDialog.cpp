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


#include "PluginSettingDialog.h"
#include "CubePlugin.h"
#include "PluginList.h"
#include "PluginManager.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QDebug>

PluginSettingDialog::PluginSettingDialog( QWidget* parent ) :
    QDialog( parent )
{
    showSettings();
}

void
PluginSettingDialog::showSettingsDialog( QWidget* parent )
{
    PluginSettingDialog* diag = new PluginSettingDialog( parent );
    diag->setWindowTitle( "Plugin settings" );
    diag->setWhatsThis( "In this dialog you may define which plugins should be loaded at startup."
                        " You may enable or disable all plugins or make an individual selection."
                        " " );

    int ret = diag->exec();

    if ( ret == QDialog::Accepted )
    {
        bool all  = diag->all->isChecked();
        bool none = diag->none->isChecked();

        QHash<QString, bool> plugins;
        foreach( QCheckBox * check, diag->pluginChecks )
        {
            plugins.insert( check->text(), check->isChecked() );
        }
        PluginManager::getInstance()->setPluginStatus( all, none, plugins );
    }
}

void
PluginSettingDialog::showSettings()
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    setLayout( mainLayout );

    bool                 allStatus;    // all plugins are activated
    bool                 noneStatus;   // all plugins are deactivated
    QHash<QString, bool> pluginStatus; // pluginname -> isActivated
    PluginManager::getInstance()->getPluginStatus( allStatus, noneStatus,
                                                   pluginStatus );

    all = new QRadioButton( tr( "enable all plugins" ), this );
    all->setChecked( allStatus );
    mainLayout->addWidget( all );

    none = new QRadioButton( tr( "disable all plugins" ), this );
    none->setChecked( noneStatus );
    mainLayout->addWidget( none );

    QRadioButton* sel = new QRadioButton( tr( "select plugins to enable" ), this );
    sel->setChecked( !noneStatus && !allStatus );
    mainLayout->addWidget( sel );

    connect( sel, SIGNAL( toggled( bool ) ), this, SLOT( selectPlugins( bool ) ) );

    int pluginCount = PluginList::getCubePluginCount();
    if ( pluginCount > 0 )
    {
        groupBox = new QGroupBox( tr( "Available plugins" ) );
        QVBoxLayout* groupLayout = new QVBoxLayout();
        groupBox->setLayout( groupLayout );
        mainLayout->addWidget( groupBox );
        for ( int index = 0; index < pluginCount; ++index )
        {
            QString    plugin = PluginList::getCubePlugin( index )->name();
            QCheckBox* check  = new QCheckBox( plugin, this );
            pluginChecks.append( check );
            bool enabled = true;
            if ( pluginStatus.contains( plugin ) )
            {
                enabled = pluginStatus.value( plugin );
            }
            check->setChecked( enabled );

            groupLayout->addWidget( check );
        }

        if ( allStatus || noneStatus )
        {
            groupBox->setEnabled( false );
        }

        QDialogButtonBox* buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );

        connect( buttonBox, SIGNAL( accepted() ), this, SLOT( accept() ) );
        connect( buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );
        mainLayout->addWidget( buttonBox );
    }
}

void
PluginSettingDialog::selectPlugins( bool enabled )
{
    groupBox->setEnabled( enabled );
}
