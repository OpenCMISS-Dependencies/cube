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





#ifndef _TABWIDGET2
#define _TABWIDGET2

#include <QSettings>
#include <QTabWidget>
#include <QComboBox>
#include "Constants.h"

namespace cube
{
class Cube;
}

class TabInterface;
class ValueWidget;
class TreeView;
class Tree;

/**
   A TabWidget consists of three graphical elements which are vertically aligned:
   - a combo box to choose the value mode
   - a tabbar with a group of tabs, e.g. TreeViews or other classes that derive from TabInterface
   - a value widget

   Cube creates three instances of TabWidget with different types (TabWidgetType):
   - the metric tab contains only the metric tree
   - the call tab contains the call tree and a flat profile
   - the system tab contains the system tree and may contain tabs from plugins
 */

class TabWidget : public QWidget
{
    Q_OBJECT

signals:
    void
    externalValueModusSelected( TabWidget* );

public slots:

    /************ public slots for value computations and display **********/

    // display the current tab element
    void
    valuesChanged();


private slots:

    // signal currentChanged() is emitted when the visible tab element has changed;
    // that signal gets connected to this slot
    void
    onCurrentChanged( int index );

    void
    setValueModus( int index );

public:

    TabWidget( DisplayType type );
    ~TabWidget();

    // initialize the tab widget
    void
    initialize( const QList<DisplayType>& order );

    void
    setOrder( const QList<DisplayType>& order );

    void
    addPluginTab( TabInterface*,
                  int addAtIndex = -1 );

    void
    removePluginTab( TabInterface* );

    int
    addTreeTab( TreeView* );

    TreeView*
    getActiveTreeView();

    /************ get methods **********/

    // return the current value modus
    ValueModus
    getValueModus();

    // return the precision widget
    ValueWidget*
    getValueWidget();

    /*********** initialization / clean up **************/

    // called when files are closed;
    // clear the tab, no data is shown
    void
    cubeClosed();

    /************* settings **********/

    // save tab settings
    // void saveSettings( QSettings& settings );

    // load tab settings
    // bool loadSettings( QSettings& settings );

    /************** miscellaneous ****************/

    // disable/enable the box plot tab, if system widget is 1st
    void
    enableTab( TabInterface* tab,
               bool          enabled );

    // return the minimal width necessary to display all informations
    virtual
    QSize
    sizeHint() const;

    DisplayType
    getType() const;

    void
    updateValueWidget();

    void
    setValueModus( ValueModus modus );

private:
    /*********************/
    // the 3 components of this widget
    QTabWidget*  tabWidget;
    ValueWidget* valueWidget;
    QComboBox*   valueCombo;

    DisplayType type;                             // type: metric, call or system tab, see constants.h for TabWidgetType
    cube::Cube* cube;                             // the cube database
    ValueModus  valueModus;                       // value modus, see constants.h

    QHash<QWidget*, TabInterface*> tabHash;       // contains all tab widgets
    QHash<QWidget*, TreeView*>     treeHash;      // contains only TreeWidgets
    QList<QWidget*>                pluginWidgets; // contains all registered plugins with tab component
    QList<DisplayType>             order;
    QHash<TabInterface*, bool>     changeInfo;    // true, if tree values have been changed since tab has been inactive

    /*********************/
    void
    updateValueCombo();

    bool
    isOrder( DisplayType left,
             DisplayType right );
    int
    addTab( TabInterface* tc,
            int           addAtIndex = -1 );

    TabInterface*
    currentTabInterface() const;

    TabInterface*
    tabInterface( int idx ) const;
};

#endif
