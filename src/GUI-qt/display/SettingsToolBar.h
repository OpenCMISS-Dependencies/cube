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


#ifndef SETTINGSTOOLBAR_H
#define SETTINGSTOOLBAR_H

#include <QToolBar>
#include <QSettings>
#include <QMenu>

/**
 * @brief The SettingsToolBar class provides a tool bar to load or save user defined settings for cube files
 */
class SettingsToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit
    SettingsToolBar();

    void
    cubeOpened( QSettings& settings );

signals:
    void
    saveExperimentSettings( const QString& );
    void
    loadExperimentSettings( const QString& );

private slots:
    void
    settingSelected();
    void
    saveSettings();

private:
    QMenu*  menu;
    QString lastName;
};

#endif // SETTINGSTOOLBAR_H
