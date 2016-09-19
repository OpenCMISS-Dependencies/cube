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


#include "SettingsToolBar.h"
#include "PluginManager.h"
#include <QAction>
#include <QInputDialog>
#include <QDebug>
#include <QSettings>

SettingsToolBar::SettingsToolBar()
{
    this->setParent( PluginManager::getInstance()->getMainWindow() );

    // create menu to load previouly saved experiment settings
    menu = new QMenu( this );
    QAction* restoreAct = new QAction( tr( "Restore Setting..." ), this );
    restoreAct->setMenu( menu );
    addAction( restoreAct );

    // create button to save experiment settings
    QAction* saveAct = new QAction( tr( "Save Settings" ), this );
    addAction( saveAct );
    connect( saveAct, SIGNAL( triggered() ), this, SLOT( saveSettings() ) );
}

void
SettingsToolBar::cubeOpened( QSettings& settings )
{
    menu->clear();
    // fill menu to load previouly saved experiment settings
    settings.beginGroup( "experiment" );
    QStringList settingsList = settings.childGroups();
    foreach( QString name, settingsList )
    {
        QAction* action = menu->addAction( name );
        connect( action, SIGNAL( triggered() ), this, SLOT( settingSelected() ) );
    }
    settings.endGroup();
}

void
SettingsToolBar::settingSelected()
{
    QAction* action = qobject_cast<QAction*>( sender() );

    QString settingsName = action->text();
    emit    loadExperimentSettings( settingsName );

    if ( settingsName != "last" )
    {
        lastName = settingsName;
    }
    else
    {
        lastName = "";
    }
}

void
SettingsToolBar::saveSettings()
{
    QString name = QInputDialog::getText( this, "Select settings name", "Settings name", QLineEdit::Normal, lastName );
    if ( !name.isEmpty() )
    {
        emit saveExperimentSettings( name );
    }
    lastName = name;
}
