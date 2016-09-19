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



#ifndef _MAINWIDGET_H
#define _MAINWIDGET_H

#include <QSplitter>
#include <QComboBox>
#include <QLineEdit>
#include <QMainWindow>
#include <QStatusBar>
#include <QMenuBar>
#include <QPushButton>
#include <QSettings>
#include <QStackedWidget>
#include <QTextBrowser>
#include "Cube.h"
#include "Constants.h"
#include "Settings.h"
#include "Globals.h"
#include "StatusBar.h"

class TabWidget;
class ColorWidget;
class ValueWidget;
class PrecisionWidget;
class FontWidget;
class ColorScale;
class Settings;
class TabManager;
class PluginServices;

namespace cube
{
class Cube;
}

// this class defines the main window of the GUI

class MainWidget : public QMainWindow, public InternalSettingsHandler
{
    Q_OBJECT

public:
    enum CubeContext { CONTEXT_CUBE, CONTEXT_INIT, CONTEXT_FREE };



    /********************public methods *****************/

    MainWidget( cube::CubeStrategy strategy = cube::CUBE_ALL_IN_MEMORY_STRATEGY );
    ~MainWidget();

    // load the contents of a cube fie into a cube object
    // and display the contents
    bool
    readFile( const QString openFileName );

    // sets the status bar text
    void
    setMessage( const QString& message,
                MessageType    type = Information );

    // InternalSettingsHandler interface
    void
    loadExperimentSettings( QSettings& );
    void
    saveExperimentSettings( QSettings& );
    void
    saveGlobalStartupSettings( QSettings& settings );
    void
    loadGlobalStartupSettings( QSettings& settings );
    void
    saveGlobalSettings( QSettings& settings );
    void
    loadGlobalSettings( QSettings& settings );
    QString
    settingName();
    void
    setContext( const CubeContext& context );

private slots:
    void
    openCube( cube::Cube*    cube,
              const QString& filename = "" );

    // input a file name to open and open it via readFile(...)
    void
    openFile();

    // re-open the last file, the second last file, etc.
    void
    openLastFiles();

    // remove loaded data
    void
    closeFile();


    // save a copy of cube
    void
    saveAs();

    // input a file name to open as external file for external percentage
    // and open it via readExternalFile(...)
    bool
    openExternalFile();

    // remove loaded external data for external percentage
    void
    closeExternalFile();

    // closes the application after eventually saving settings
    void
    closeApplication();

    // set the order of metric/call/system splitter elements
    void
    setDimensionOrder();

    // displays a short intro to cube
    void
    introduction();

    // shows the about message
    void
    about();

    // list all mouse and keyboard control
    void
    keyHelp();

    // saves a screenshot in a png file
    // (it is without the shell frame, I could not find any possibilities to include that frame within qt)
    void
    screenshot();

    // sets the file size threshold above which dynamic loading of metric data is enabled
    void
    setDynloadThreshold();

    // sets the busy cursor
    void
    setBusy( bool busy );

    void
    updateColormapMenu();

    // causes to open a color dialog to allow the user to change color-specific settings
    void
    editColorMap();

    // allow to select one of the registered colormaps
    void
    selectColorMap();

    // react on signal ColorMap::colorMapChanged()
    void
    updateColorMap( ColorMap* map = 0 );

    // causes to open a precision dialog to allow the user to change precision-specific settings
    void
    setPrecision();

    // causes to open a font dialog to allow the user to change
    // font and spacing for tree widgets
    void
    setFont();

    // trigers status of flattree
    void
    triggerStatusOfFlattree();

    // slot to apply the settings of the font widget
    void
    applyFont( FontWidget* fontWidget );

    // distribute the available width between the 3 tabs such that
    // from each scroll area the same percentual amount is visible
    void
    distributeWidth();

    // adapt the size of the main window such that
    // all informations are visible
    // and resize the splitter widget accordingly
    void
    adaptWidth();

    void
    selectExternalValueMode( TabWidget* widget );

    void
    onRegionInfo();

    void
    recentFileSelected( const QUrl& link );

signals:

    /******************** signals *****************/

    void
    enableClose( bool ); // cube is loaded -> close buttons become active
    void
    enableExtClose( bool );

protected:
    virtual void
    closeEvent( QCloseEvent* event );
    virtual void
    dragEnterEvent( QDragEnterEvent* event );
    virtual void
    dropEvent( QDropEvent* event );

private:
    /******************** private fields **********************/
    StatusBar*      statusBar;
    QStackedWidget* stackedWidget;
    TabManager*     tabManager;
    ColorScale*     colorScale;

    Settings* settings;
    bool      settingsAreInitialized;
    QString   lastColorMapName;
    bool      fileLoaded;       // true, if cube data is loaded from a file

    // flag to select load strategy...
    cube::CubeStrategy strategy;

    // the cube database for loaded data
    cube::Cube* cube;
    // the cube database for loaded external data for external percentage
    cube::Cube* cubeExternal;

    // stores at most 5 last opened files
    QStringList openedFiles;
    // stores the name of the current external file
    QString lastExternalFileName;
    // stores the actions for opening the last at most 5 files
    std::vector<QAction*> lastFileAct;
    // file menu containing u.o. the last file actions
    QMenu*   fileMenu;
    QMenu*   settingsMenu;
    QMenu*   mapMenu;            // color map selection menu
    QAction* colorsAct;          // color map edit action
    QAction* performanceInfoAct; // help menu item
    QAction* regionInfoAct;      // help menu item
    QAction* triggerStatusOfFlatTree;

    // Saving cube actions
    QAction* saveAsAct;

    QAction*      selection1Act, * selection2Act;
    QAction*      splitterOrderAct;
    QTextBrowser* recentFilesText;

    // threshold for file size above which dynamic metric loading is enabled
    int dynloadThreshold;

    // the font for the tree display, user-definable
    QFont   treeFont;
    QString treeFontFamily;
    int     treeFontSize;
    QColor  treeSelectionColor;

    /******************** private methods **********************/

    // read the contents of a cube file into a temporary cube object for
    // the external percentage modus and compute the relevant values for the display
    bool
    readExternalFile( const QString fileName );

    // remember the names of the last 5 files that were opened;
    // they can be re-opened via the display menu
    void
    rememberFileName( QString fileName );

    // creates the pull-down menu
    void
    createMenu();

    // update the title of the application to show infos to the loaded file and external file
    void
    updateWidgetTitle();
    void
    setColorMap( ColorMap* map );

    /**
     * Go through color maps loaded in plugins in check whether one of them fits to name remembered in variable lastColorMapName
     * If yes, then set this map as active for Cube.
     */
    void
    loadColorMap();
    void
    createInitialScreen( QWidget* mainWidget );
};



#endif
