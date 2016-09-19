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

#include "config.h"


#include <QString>
#include <QPixmap>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QAction>
#include <QWhatsThis>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QScrollArea>
#include <QProgressDialog>

#include "MainWidget.h"

#include "CubeParseContext.h"
#include "CubeError.h"
#include "CubeServices.h"
#include "CubeTypes.h"
#include "Cube.h"
#include "CubePlatformsCompat.h"

#include "Globals.h"
#include "PluginManager.h"
#include "PluginServices.h"
#include "Constants.h"
#include "FontWidget.h"
#include "Settings.h"
#include "TabManager.h"
#include "DimensionOrderDialog.h"
#include "TreeView.h"
#include "MetricTree.h"
#include "ColorScale.h"
#include "ContextFreeServices.h"
#include "PluginServices.h"
#include "PrecisionWidget.h"

#include <cassert>
#include <iostream>
// #ifndef CUBE_COMPRESSED
#include <fstream>
// #else
#include "CubeZfstream.h"
// #endif

#include "HmiInstrumentation.h"

using namespace std;

#define CUBE_CONTEXT 0
#define NO_CONTEXT 1

MainWidget::MainWidget( cube::CubeStrategy _strategy ) : strategy( _strategy )
{
    cube                   = NULL;
    lastExternalFileName   = "";
    cubeExternal           = NULL;
    dynloadThreshold       = 1000000000;
    settingsAreInitialized = false;
    lastColorMapName       = "";
    fileLoaded             = false;
    recentFilesText        = NULL;

    this->setAcceptDrops( true );

    QTreeView view;
    QPalette  treePalette = view.palette();
    treeSelectionColor = treePalette.color( QPalette::Highlight );

    resize( 800, 600 );
    setWindowIcon( QIcon( ":images/CubeIcon.xpm" ) );

    stackedWidget = new QStackedWidget();
    stackedWidget->setWindowIcon( QIcon( ":images/CubeIcon.xpm" ) );

    StatusBarWidget* central = new StatusBarWidget();
    statusBar = central->getStatusBar();
    central->setWidget( stackedWidget );
    setCentralWidget( central );

    tabManager = new TabManager();
    tabManager->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    colorScale = new ColorScale();
    colorScale->setVisible( false );

    QWidget*     mainWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    // LAYOUT_MARGIN and LAYOUT_SPACING are defined in constants.h
    mainLayout->setMargin( LAYOUT_MARGIN );
    mainLayout->setSpacing( LAYOUT_SPACING );
    mainWidget->setLayout( mainLayout );
    mainLayout->addWidget( tabManager );
    mainLayout->addWidget( colorScale );

    Globals::initialize( this, tabManager );

    PrecisionWidget* precisionWidget = Globals::getPrecisionWidget();
    connect( precisionWidget, SIGNAL( apply() ), tabManager, SLOT( updateTreeItemProperties() ) );

    /* initialize cube dependent plugins */
    PluginManager::getInstance()->setMainWindow( this );

    /* initialize context free plugins */
    ContextFreeServices* cfs           = ContextFreeServices::getInstance();
    QWidget*             initialScreen = new QWidget( this );
    QWidget*             contextFree   = new QWidget( this );
    cfs->setWidget( contextFree );
    connect( cfs, SIGNAL( openCubeRequest( cube::Cube* ) ), this, SLOT( openCube( cube::Cube* ) ) );

    stackedWidget->addWidget( mainWidget );    // loaded cube (CONTEXT_CUBE)
    stackedWidget->addWidget( initialScreen ); // start screen (CONTEXT_INIT)
    stackedWidget->addWidget( contextFree );   // context free plugin (CONTEXT_FREE)
    stackedWidget->setCurrentIndex( CONTEXT_INIT );

    createInitialScreen( initialScreen );

    // menu creation
    // note: system tab widget must be created before createMenu() gets called
    createMenu();

    setWhatsThis( CUBE_FULL_NAME " is a presentation component suitable for displaying performance data for parallel programs including MPI and OpenMP applications. Program performance is represented in a multi-dimensional space including various program and system resources. The tool allows the interactive exploration of this space in a scalable fashion and browsing the different kinds of performance behavior with ease. Cube also includes a library to read and write performance data as well as operators to compare, integrate, and summarize data from different experiments." );

    menuBar()->setWhatsThis( "The menu bar consists of three menus, a file menu, a display menu, and a help menu. Some menu functions have also a keyboard shortcut, which is written beside the menu item's name in the menu. E.g., you can open a file with Ctrl+O without going into the menu.  A short description of the menu items is visible in the status bar if you stay for a short while with the mouse above a menu item." );

    QList<InternalSettingsHandler*> list;
    list.append( this );
    list.append( PluginManager::getInstance() );

    settings = new Settings( settingsMenu, list );

    PluginManager::getInstance()->initializePlugins();
    setColorMap( Globals::getColorMap() );

    tabManager->setFocusPolicy( Qt::StrongFocus );
    tabManager->setFocus();

    // set all splitter childs to same size
    int        width = this->width();
    QList<int> sizes;
    sizes << width / 3 << width / 3 << width / 3;
    tabManager->setSizes( sizes );
}
// end of constructor

void
MainWidget::setContext( const CubeContext& context )
{
    stackedWidget->setCurrentIndex( context );
}

#include <QApplication>
void
MainWidget::createInitialScreen( QWidget* mainWidget )
{
    QHBoxLayout* main = new QHBoxLayout();
    mainWidget->setLayout( main );

    QVBoxLayout* vbox   = new QVBoxLayout();
    QWidget*     widget = new QWidget( mainWidget );
    widget->setLayout( vbox );
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding );
    main->addSpacerItem( spacer );
    main->addWidget( widget );
    spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding );
    main->addSpacerItem( spacer );

    QPushButton* open = new QPushButton( QApplication::style()->standardIcon( QStyle::SP_DirOpenIcon ), " Open Cube File " );
    //QPushButton* open = new QPushButton( " Open Cube File " );
    QFont        font = open->font();
    font.setPointSize( font.pointSize() + 2 );
    //open->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed );
    open->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    connect( open, SIGNAL( clicked( bool ) ), this, SLOT( openFile() ) );
    open->setFont( font );

    // recent cube files ---------------
    int columns           = 100;
    int fontSizeIncrement = 2;

    QLabel* label = new QLabel( "Recent Cube Files" );
    label->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed );
    font = label->font();
    font.setPointSize( font.pointSize() + 2 );
    label->setFont( font );

    recentFilesText = new QTextBrowser();
    recentFilesText->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    recentFilesText->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    recentFilesText->setOpenLinks( false );
    font = recentFilesText->font();
    font.setPointSize( font.pointSize() + fontSizeIncrement );
    recentFilesText->setFont( font );
    recentFilesText->setLineWrapMode( QTextEdit::FixedColumnWidth );
    recentFilesText->setLineWrapColumnOrWidth( columns );
    QColor   color   = mainWidget->palette().color( QPalette::Background );
    QPalette palette = recentFilesText->palette();
    palette.setColor( QPalette::Base, color );
    recentFilesText->setPalette( palette );
    //text->setFrameStyle(QFrame::NoFrame);
    connect( recentFilesText, SIGNAL( anchorClicked( QUrl ) ), this, SLOT( recentFileSelected( QUrl ) ) );

    // recent cube files ---------------

    QSpacerItem* vspace = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    vbox->addSpacerItem( vspace );
    vbox->addWidget( open );
    vbox->addSpacing( 10 );
    vbox->addWidget( label );
    vbox->addWidget( recentFilesText );
    vspace = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    vbox->addSpacerItem( vspace );
}

void
MainWidget::recentFileSelected( const QUrl& link )
{
    int      idx    = link.toString().toInt();
    QAction* action = lastFileAct.at( idx );
    action->trigger();
}

MainWidget::~MainWidget()
{
    delete settings; // saves global settings on deletation
    delete cube;
}

/**********************************************************/
/****************** menu **********************************/
/**********************************************************/

// creates the pull-down menu
//
void
MainWidget::createMenu()
{
    // file menu

    fileMenu = menuBar()->addMenu( tr( "&File" ) );
    fileMenu->setStatusTip( tr( "Ready" ) );

    QAction* openAct = new QAction( tr( "&Open..." ), this );
    openAct->setShortcut( tr( "Ctrl+O" ) );
    openAct->setStatusTip( tr( "Opens a cube file" ) );
    connect( openAct, SIGNAL( triggered() ), this, SLOT( openFile() ) );
    fileMenu->addAction( openAct );
    openAct->setWhatsThis( "Offers a selection dialog to open a Cube file. In case of an already opened file, it will be closed before a new file gets opened. If a file got opened successfully, it gets added to the top of the recent files list (see below). If it was already in the list, it is moved to the top." );

    QMenu* contextFreeMenu = PluginManager::getInstance()->getContextFreePluginMenu();
    fileMenu->addMenu( contextFreeMenu );
    connect( this, SIGNAL( enableClose( bool ) ), contextFreeMenu, SLOT( setDisabled( bool ) ) );

    saveAsAct = new QAction( tr( "&Save as..." ), this );
    saveAsAct->setShortcut( tr( "Ctrl+S" ) );
    saveAsAct->setStatusTip( tr( "Save the current cube under another name..." ) );
    saveAsAct->setEnabled( false );
    connect( saveAsAct, SIGNAL( triggered() ), this, SLOT( saveAs() ) );
    fileMenu->addAction( saveAsAct );
    saveAsAct->setWhatsThis( "Offers a selection dialog to select a new name for a Cube file." );


    QAction* closeAct = new QAction( tr( "&Close" ), this );
    closeAct->setShortcut( tr( "Ctrl+W" ) );
    closeAct->setStatusTip( tr( "Closes the open file" ) );
    closeAct->setEnabled( false );
    connect( closeAct, SIGNAL( triggered() ), this, SLOT( closeFile() ) );
    connect( this, SIGNAL( enableClose( bool ) ), closeAct, SLOT( setEnabled( bool ) ) );
    fileMenu->addAction( closeAct );
    closeAct->setWhatsThis( "Closes the currently opened Cube file. Disabled if no file is opened." );

    QAction* openExtAct = new QAction( tr( "Open &external..." ), this );
    openExtAct->setStatusTip( tr( "Opens a second cube file for external percentage" ) );
    connect( openExtAct, SIGNAL( triggered() ), this, SLOT( openExternalFile() ) );
    fileMenu->addAction( openExtAct );
    openExtAct->setWhatsThis( "Opens a file for the external percentage value mode (see further help on the value modes)." );

    QAction* closeExtAct = new QAction( tr( "Close e&xternal" ), this );
    closeExtAct->setStatusTip( tr( "Closes the second cube file for external percentage" ) );
    closeExtAct->setEnabled( false );
    connect( closeExtAct, SIGNAL( triggered() ), this, SLOT( closeExternalFile() ) );
    connect( this, SIGNAL( enableExtClose( bool ) ), closeExtAct, SLOT( setEnabled( bool ) ) );
    connect( this, SIGNAL( enableExtClose( bool ) ), closeExtAct, SLOT( setEnabled( bool ) ) );
    fileMenu->addAction( closeExtAct );
    closeExtAct->setWhatsThis( "Closes the current external file and removes all corresponding data. Disabled if no external file is opened." );

    settingsMenu = fileMenu->addMenu( "Settings" );

    fileMenu->addSeparator();

#ifdef WITH_DYNLOAD
    QAction* dynloadAct = new QAction( tr( "D&ynamic loading threshold..." ), this );
    dynloadAct->setStatusTip( tr( "Sets the threshold for dynamic loading of data" ) );
    connect( dynloadAct, SIGNAL( triggered() ), this, SLOT( setDynloadThreshold() ) );
    fileMenu->addAction( dynloadAct );
    dynloadAct->setWhatsThis( "By default, Cube always loads the whole amount of data when you open a Cube file. However, Cube offers also a possibility to load only those data which is needed for the current display. To be more precise, the data for the selected metric(s) and, if a selected metric is expanded, the data for its children are loaded. If you change the metric selection, possibly some new data is needed for the display that is dynamically loaded on demand. Currently not needed data gets unloaded.\n\nThis functionality is useful most of all for large files. Under this menu item you can define a file size threshold (in bytes) above which Cube offers you dynamic data loading. If a file being opened is larger than this threshold, Cube will ask you if you wish dynamic loading." );
#endif

    QAction* screenshotAct = new QAction( tr( "Sc&reenshot..." ), this );
    screenshotAct->setStatusTip( tr( "Saves a screenshot into the file cube.png" ) );
    connect( screenshotAct, SIGNAL( triggered() ), this, SLOT( screenshot() ) );
    fileMenu->addAction( screenshotAct );
    screenshotAct->setWhatsThis( "The function offers you to save a screenshot in a png file. Unfortunately the outer frame of the main window is not saved, only the application itself." );

    fileMenu->addSeparator();

    QAction* quitAct = new QAction( tr( "&Quit" ), this );
    quitAct->setShortcut( tr( "Ctrl+Q" ) );
    quitAct->setStatusTip( tr( "Exits the application" ) );
    connect( quitAct, SIGNAL( triggered() ), this, SLOT( closeApplication() ) );
    fileMenu->addAction( quitAct );
    quitAct->setWhatsThis( "Closes the application." );

    fileMenu->addSeparator();

    // display menu

    QMenu* displayMenu = menuBar()->addMenu( tr( "&Display" ) );
    displayMenu->setStatusTip( tr( "Ready" ) );

    splitterOrderAct = new QAction( tr( "D&imension order..." ), this );
    splitterOrderAct->setStatusTip( tr( "Sets the order of the dimensions metric, call chain, and system." ) );
    connect( splitterOrderAct, SIGNAL( triggered() ), this, SLOT( setDimensionOrder() ) );
    displayMenu->addAction( splitterOrderAct );
    splitterOrderAct->setWhatsThis( "As explained above, Cube has three resizable panes. Initially the metric pane is on the left, the call pane is in the middle, and the system pane is on the right-hand-side. However, sometimes you may be interested in other orders, and that is what this menu item is about. It offers all possible pane orderings.  For example, assume you would like to see the metric and call values for a certain thread.  In this case, you should place the system pane on the left, the metric pane in the middle, and the call pane on the right. Note that in panes left-hand-side of the metric pane we have no meaningful values, since they miss a reference metric; in this case we specify the values to be undefined, denoted by a \"-\" sign." );

    mapMenu = displayMenu->addMenu( "Choose colormap" );
    mapMenu->setStatusTip( tr( "Choose a colormap" ) );
    connect( mapMenu, SIGNAL( aboutToShow() ), this, SLOT( updateColormapMenu() ) );
    mapMenu->setEnabled( false );

    colorsAct = new QAction( tr( "Edit colormap..." ), this );
    colorsAct->setStatusTip( tr( "Edit the selected colormap" ) );
    connect( colorsAct, SIGNAL( triggered() ), this, SLOT( editColorMap() ) );
    displayMenu->addAction( colorsAct );
    colorsAct->setWhatsThis( "Opens a dialog that allows the user to edit the selected colormap" );
    colorsAct->setEnabled( false );

    QAction* precisionAct = new QAction( tr( "&Precision..." ), this );
    precisionAct->setStatusTip( tr( "Defines the precision of the display for numbers." ) );
    connect( precisionAct, SIGNAL( triggered() ), this, SLOT( setPrecision() ) );
    precisionAct->setWhatsThis( "Activating this menu item opens a dialog for precision settings. See also help on the dialog itself." );
    displayMenu->addAction( precisionAct );

    QMenu* treeMenu = displayMenu->addMenu( tr( "Trees" ) );
    treeMenu->setStatusTip( tr( "Ready" ) );

    QAction* markerAction = PluginManager::getInstance()->getMarkerConfigAction();
    treeMenu->addAction( markerAction );

    triggerStatusOfFlatTree = new QAction( tr( "Disable/Enable Flattree..." ), this );
    triggerStatusOfFlatTree->setStatusTip( tr( "Defines font type and size, and spacing for the tree widgets." ) );
    triggerStatusOfFlatTree->setShortcut( tr( "Ctrl+D" ) );
    connect( triggerStatusOfFlatTree, SIGNAL( triggered() ), this, SLOT( triggerStatusOfFlattree() ) );
    //   triggerStatusOfFlatTree->setWhatsThis("Opens a dialog to specify the font, the font size (in pt), and the line spacing for the tree displays.  The \"Ok\" button applies the settings to the display and closes the dialog, the \"Apply\" button applies the settings to the display, and \"Cancel\" cancels all changes since the dialog was opened (even if \"Apply\" was pressed in between) and closes the dialog.");
    treeMenu->addAction( triggerStatusOfFlatTree );
    triggerStatusOfFlatTree->setEnabled( false );

    QAction* fontAct = new QAction( tr( "Fo&nt and colors..." ), this );
    fontAct->setStatusTip( tr( "Defines font type and size, and selection color for the tree widgets." ) );
    connect( fontAct, SIGNAL( triggered() ), this, SLOT( setFont() ) );
    fontAct->setWhatsThis( "Opens a dialog to specify the font, the font size (in pt), and the selection color for the tree displays.  The \"Ok\" button applies the settings to the display and closes the dialog, the \"Apply\" button applies the settings to the display, and \"Cancel\" cancels all changes since the dialog was opened (even if \"Apply\" was pressed in between) and closes the dialog." );
    treeMenu->addAction( fontAct );

    QMenu* widthMenu = displayMenu->addMenu( tr( "Optimize width" ) );
    widthMenu->setStatusTip( tr( "Ready" ) );
    widthMenu->setWhatsThis( "Under this menu item Cube offers widget rescaling such that the amount of information shown is maximized, i.e., Cube optimally distributes the available space between its components. You can chose if you would like to stick to the current main window size, or if you allow to resize it." );

    QAction* width1Act = new QAction( tr( "&Keep main window size" ), this );
    width1Act->setStatusTip( tr( "Optimize widths of tabs in order to show equally large percentages." ) );
    connect( width1Act, SIGNAL( triggered() ), this, SLOT( distributeWidth() ) );
    width1Act->setWhatsThis( "Under this menu item Cube offers widget rescaling with keeping the main window size, such that the amount of information shown is maximized, i.e., Cube optimally distributes the available space between its components." );
    widthMenu->addAction( width1Act );

    QAction* width2Act = new QAction( tr( "&Adapt main window size" ), this );
    width2Act->setStatusTip( tr( "Resizes the main window width and adapt tab widths in order to show all informations" ) );
    connect( width2Act, SIGNAL( triggered() ), this, SLOT( adaptWidth() ) );
    width2Act->setWhatsThis( "Under this menu item Cube offers widget rescaling possibly changing the main window size, such that the amount of information shown is maximized, i.e., Cube optimally distributes the available space between its components." );
    widthMenu->addAction( width2Act );

    QMenu* pluginMenu = PluginManager::getInstance()->getPluginMenu();
    menuBar()->addMenu( pluginMenu );

    // help menu

    QMenu* helpMenu = menuBar()->addMenu( tr( "&Help" ) );
    helpMenu->setStatusTip( tr( "Ready" ) );

    QAction* introAct = new QAction( tr( "Getting started" ), this );
    introAct->setStatusTip( tr( "Displayes a short introduction to Cube." ) );
    connect( introAct, SIGNAL( triggered() ), this, SLOT( introduction() ) );
    helpMenu->setWhatsThis( "Opens a dialog with some basic informations on the usage of Cube." );
    helpMenu->addAction( introAct );

    QAction* keysAct = new QAction( tr( "Mouse and keyboard control" ), this );
    keysAct->setStatusTip( tr( "Shows all supported mouse and keyboard controls" ) );
    connect( keysAct, SIGNAL( triggered() ), this, SLOT( keyHelp() ) );
    keysAct->setWhatsThis( "List all control possibilities for keyboard and mouse." );
    helpMenu->addAction( keysAct );

    QAction* whatsThisAct = QWhatsThis::createAction();
    whatsThisAct->setStatusTip( "Change into help mode for display components" );
    whatsThisAct->setWhatsThis( "Here you can get more specific information on parts of the Cube GUI. If you activate this menu item, you switch to the \"What's this?\" mode. If you now click on a widget an appropriate help text is shown. The mode is left when help is given or when you press Esc.\n\nAnother way to ask the question is to move the focus to the relevant widget and press Shift+F1." );
    helpMenu->addAction( whatsThisAct );

    QAction* aboutAct = new QAction( tr( "About" ), this );
    aboutAct->setStatusTip( tr( "Shows CUBE's About box" ) );
    connect( aboutAct, SIGNAL( triggered() ), this, SLOT( about() ) );
    aboutAct->setWhatsThis( "Opens a dialog with some release information." );
    helpMenu->addAction( aboutAct );

    helpMenu->addSeparator();

    QMenu* pluginHelp = PluginManager::getInstance()->getPluginHelpMenu();
    helpMenu->addMenu( pluginHelp );
    helpMenu->addSeparator();

    performanceInfoAct = new QAction( tr( "Selected metrics description" ), this );
    performanceInfoAct->setStatusTip( tr( "Shows the online description of the selected metrics." ) );
    performanceInfoAct->setWhatsThis( "Shows some (usually more extensive) online description for the selected metrics. For metrics it might point to an online documentation explaining their semantics." );
    helpMenu->addAction( performanceInfoAct );
    performanceInfoAct->setEnabled( false );

    regionInfoAct = new QAction( tr( "Selected regions description" ), this );
    regionInfoAct->setStatusTip( tr( "Shows the online description of the selected regions in a  program." ) );
    regionInfoAct->setWhatsThis( "Shows some (usually more extensive) online description for the clicked region in program. For regions representing library functions it might point to the corresponding library documentation." );
    helpMenu->addAction( regionInfoAct );
    regionInfoAct->setEnabled( false );
}

/*************************************************************/
/************ widget title, status bar ***********************/
/*************************************************************/

// update the title of the application to show infos to the loaded file and external file
//
void
MainWidget::updateWidgetTitle()
{
    // CUBE_NAME is defined in constants.h
    QString widgetTitle( CUBE_FULL_NAME );

    // append the name of the currently opened file
    if ( cube != NULL && fileLoaded )
    {
        QFileInfo openFileName  =  QFileInfo( openedFiles[ 0 ] );
        QString   fileName      = openFileName.fileName();
        QString   lastDirectory = openFileName.dir().dirName();

        widgetTitle.append( ": " );
        widgetTitle.append( lastDirectory );
        widgetTitle.append( QDir::separator() );
        widgetTitle.append( fileName );
    }

    // append the name of the current external file
    QString externalName = lastExternalFileName;
    if ( externalName != "" )
    {
        QFileInfo externalFile  =  QFileInfo( externalName );
        QString   fileName      = externalFile.fileName();
        QString   lastDirectory = externalFile.dir().dirName();

        widgetTitle.append( " external: " );
        widgetTitle.append( lastDirectory );
        widgetTitle.append( QDir::separator() );
        widgetTitle.append( fileName );
    }

    setWindowTitle( widgetTitle );
}

// sets the status bar text
//
void
MainWidget::setMessage( const QString& message, MessageType type )
{
    statusBar->addLine( message, type );
    // statusBar()->showMessage( tr( message.toStdString().c_str() ) );
}


/*************************************************************/
/******************** file handling **************************/
/*************************************************************/


// remember the names of the last 5 files that were opened;
// they can be re-opened via the display menu
//
void
MainWidget::rememberFileName( QString fileName )
{
    QFileInfo fi( fileName );
    fileName = fi.absoluteFilePath();

    // clear the QActions from the display menu to re-open files
    for ( int i = 0; i < ( int )lastFileAct.size(); i++ )
    {
        fileMenu->removeAction( lastFileAct.at( i ) );
    }
    lastFileAct.clear();

    // insert the new fileName at the beginning of the openedFiles vector
    // and remove eventual duplicates
    openedFiles.push_back( QString( "" ) );
    for ( int i = openedFiles.size() - 1; i > 0; i-- )
    {
        openedFiles[ i ] = openedFiles[ i - 1 ];
    }
    openedFiles[ 0 ] = fileName;
    for ( int i = 1; i < ( int )openedFiles.size(); i++ )
    {
        if ( openedFiles[ i ].compare( openedFiles[ 0 ] ) == 0 )
        {
            for ( int j = i; j < ( int )openedFiles.size() - 1; j++ )
            {
                openedFiles[ j ] = openedFiles[ j + 1 ];
            }
            openedFiles.pop_back();
            break;
        }
    }

    // we remember at most 5 file names
    if ( openedFiles.size() > 5 )
    {
        openedFiles.pop_back();
    }

    foreach( QString filename, openedFiles )
    {
        QString  name = QFileInfo( filename ).fileName(); // returns the name of the file, excluding the path
        QAction* act  = fileMenu->addAction( name );
        act->setData( filename );
        connect( act, SIGNAL( triggered() ), this, SLOT( openLastFiles() ) );

        act->setStatusTip( filename );
        act->setWhatsThis( "At the bottom of the File menu the last 5 opened files are offered for re-opening, the top-most being the most recently opened one. A full path to the file is visible in the status bar if you move the mouse above one of the recent file items in the menu." );
        lastFileAct.push_back( act );
    }

    if ( recentFilesText ) // update initial screen contents and size
    {
        QString str = "";
        for ( int i = 0; i < ( int )lastFileAct.size(); i++ )
        {
            QAction* action = lastFileAct.at( i );
            if ( i > 0 )
            {
                str += "<br>";
            }
            str += "<a href=" + QString::number( i ) + ">" + action->text() + "</a> <br>&nbsp;&nbsp;" + openedFiles[ i ];
        }
        recentFilesText->setText( str );

        QSize size = recentFilesText->document()->size().toSize();
        size.setWidth( recentFilesText->document()->idealWidth() );
        size += QSize( 10, 10 );
        recentFilesText->setMinimumSize( size );
    }
}

// end of rememberFileName(...)

// private slot connected to a file menu item which shows one of the recently opened files
void
MainWidget::openLastFiles()
{
    QAction* action = qobject_cast<QAction*>( sender() );
    if ( action != 0 )
    {
        QString filename = action->data().toString();
        readFile( filename );
    }
}

// input a file name to open and open it via readFile(...)
//
void
MainWidget::saveAs()
{
    QString saveAsFileName = QFileDialog::getSaveFileName( this,
                                                           tr( "Choose a file to open" ),
                                                           "",
                                                           tr( "Cube4 files (*.cubex);;All files (*.*);;All files (*)" ) );

    if ( saveAsFileName.length() == 0 )
    {
        setMessage( tr( "Ready" ) );
        return;
    }
    // here is the saving name selecetd

    setMessage( tr( "Saving " ).append( saveAsFileName ).append( "..." ) );
    setCursor( Qt::WaitCursor );
    cube::Cube* _new_cube = new cube::Cube( *cube, cube::CUBE_DEEP_COPY );
    _new_cube->writeCubeReport( saveAsFileName.toUtf8().data() );
    delete _new_cube;
    setCursor( Qt::ArrowCursor );
    setMessage( tr( "Ready" ) );
}



// input a file name to open and open it via readFile(...)
//
void
MainWidget::openFile()
{
    QString lastName     = ( openedFiles.size() == 0 ? "." : openedFiles[ 0 ] );
    QString openFileName = QFileDialog::getOpenFileName( this,
                                                         tr( "Choose a file to open" ),
                                                         lastName,
                                                         tr( "Cube3/4 files (*cube *cube.gz *.cubex);;Cube4 files (*.cubex);;Cube3 files (*.cube.gz *.cube);;All files (*.*);;All files (*)" ) );

    if ( openFileName.length() == 0 )
    {
        setMessage( tr( "Ready" ) );
        return;
    }
    setMessage( tr( "Open " ).append( openFileName ).append( "..." ) );
    setCursor( Qt::WaitCursor );
    readFile( openFileName );
    setCursor( Qt::ArrowCursor );
    setMessage( tr( "Ready" ) );
}


// load the contents of a cube file into a cube object
// and display the contents
//
bool
MainWidget::readFile( const QString fn )
{
    QDir    fileDir( fn );
    QString fileName = fileDir.absolutePath();
    try
    {
        if ( cube::services::check_file( fileName.toUtf8().data() ) != 0 )
        {
            return false;
        }

        // setMessage( tr( "Closing previous data..." ) );
        if ( cube != NULL )
        {
            closeFile();
        }

        setMessage( tr( "Opened " ) + fileName + ". Parsing..." );
        setCursor( Qt::BusyCursor );

        assert( cube == NULL );
        cube = new cube::Cube();
        cube->openCubeReport( fileName.toStdString() );
        cube->setGlobalMemoryStrategy( strategy );
        fileLoaded = true;
    }
    catch ( const cube::OpenFileError& e  )
    {
        setMessage( QString( "Error. \n" ).append( QString::fromStdString( e.get_msg() ) ), Error );
        setCursor( Qt::ArrowCursor );
        setMessage( tr( "Error during opening cube file...." ), Error );
        delete cube;
        cube = NULL;
        return false;
    }
    catch ( const cube::RuntimeError& e )
    {
        setMessage( QString( "Error. \n" ).append( QString::fromStdString( e.get_msg() ) ), Error );
        setCursor( Qt::ArrowCursor );
        setMessage( tr( "Error during opening cube file...." ) );
        delete cube;
        cube = NULL;
        return false;
    }
    // remember the file name for opening recent files
    rememberFileName( fileName ); 
    openCube( cube, fileName );
    return true;
}

void
MainWidget::openCube( cube::Cube* cube, const QString& fileName )
{
    this->cube = cube;

    // set initial plugin data
    PluginManager::getInstance()->opened( cube, fileName, tabManager );

    // initialize the displays in their display order

    tabManager->cubeOpened( cube );
    tabManager->show();
    foreach( TabWidget * widget, tabManager->getTabWidgets() )
    {
        connect( widget, SIGNAL( externalValueModusSelected( TabWidget* ) ),
                 this, SLOT( selectExternalValueMode( TabWidget* ) ) );
    }

    setMessage( tr( "Computing values..." ) );

    updateWidgetTitle();
    emit enableClose( true );

    setCursor( Qt::ArrowCursor );

    // add local documentation paths from CUBE_DOCPATH environment variable
    // as additional "mirrors"
    char* ptr = getenv( "CUBE_DOCPATH" );
    if ( ptr )
    {
        QString     docpath( ptr );
        QStringList paths = docpath.split( ':', QString::SkipEmptyParts );

        QStringList::const_iterator it = paths.begin();
        while ( it != paths.end() )
        {
            QString mirror( "file://" + *it );
            if ( !mirror.endsWith( '/' ) )
            {
                mirror.append( '/' );
            }

            cube->def_mirror( mirror.toStdString() );

            ++it;
        }
    }

    // open all plugins, in a final step all plugin tabs will be added
    PluginManager::getInstance()->openPlugins();

    // load experiment specific settings
    settings->cubeLoaded( fileName );

    // activate loaded experiment specific settings or set initial status
    tabManager->initialize();

    // enable menu items
    colorsAct->setEnabled( true );
    mapMenu->setEnabled( true );
    colorScale->setVisible( true );
    triggerStatusOfFlatTree->setEnabled( true );
    saveAsAct->setEnabled( true );

    performanceInfoAct->setEnabled( true );
    regionInfoAct->setEnabled( true );

    stackedWidget->setCurrentIndex( CONTEXT_CUBE );

    // connect help menu items to slots of the current views
    connect( performanceInfoAct, SIGNAL( triggered() ),
             tabManager->getView( METRICTREE ), SLOT( onShowDocumentation() ) );
    connect( regionInfoAct, SIGNAL( triggered() ), this, SLOT( onRegionInfo() ) );

    setMessage( tr( "Ready" ) );
}

// end of readFile(...)


// remove loaded data
//
void
MainWidget::closeFile()
{
    if ( cube == 0 )
    {
        return;              // already closed
    }
    fileLoaded = false;
    settings->cubeClosed();
    lastColorMapName = Globals::getColorMap()->getMapName();

    PluginManager::getInstance()->closed();

    tabManager->cubeClosed();

    delete cube;
    cube = NULL;
    updateWidgetTitle();

    emit enableClose( false );
    setMessage( tr( "Ready" ) );

    // disable menu items
    saveAsAct->setEnabled( false );
    colorsAct->setEnabled( false ); // disable colormap selection
    colorScale->setVisible( false );
    mapMenu->setEnabled( false );
    triggerStatusOfFlatTree->setEnabled( false );
    performanceInfoAct->setEnabled( false );
    regionInfoAct->setEnabled( false );

    performanceInfoAct->disconnect(); // disconnect all slots
    regionInfoAct->disconnect();

    stackedWidget->setCurrentIndex( CONTEXT_INIT );
}

/**
 * This method is called, if the user selects EXTERNAL value modus. If no external cube file is loaded,
 * a file dialog is opened.
 */
void
MainWidget::selectExternalValueMode( TabWidget* widget )
{
    bool ok = false;
    if ( lastExternalFileName.size() > 0 )
    {
        ok = true;
    }
    else
    {
        ok = openExternalFile();
    }
    if ( ok )
    {
        widget->setValueModus( EXTERNAL );
    }
    else
    {
        widget->setValueModus( ABSOLUTE );
    }
}

// input a file name to open as external file for external percentage
// and open it via readExternalFile(...)
//
bool
MainWidget::openExternalFile()
{
    QString fileName = QFileDialog::getOpenFileName( this,
                                                     tr( "Choose a file for external percentage" ),
                                                     lastExternalFileName,
                                                     tr( "Cube3/4 files (*cube *cube.gz *.cubex);;Cube4 files (*.cubex);;Cube3 files (*.cube.gz *.cube);;All files (*.*);;All files (*)" ) );

    if ( fileName.length() == 0 )
    {
        setMessage( tr( "Ready" ) );
        return false;
    }
    bool result = readExternalFile( fileName );
    if ( result )
    {
        updateWidgetTitle();
        emit enableExtClose( true );
    }

    return result;
}


// read the contents of a cube file into a temporary cube object for
// the external percentage modus and compute the relevant values for the display
//
bool
MainWidget::readExternalFile( const QString fileName )
{
    setMessage( tr( "Parsing..." ) );
    setCursor( Qt::BusyCursor );

    // #ifndef CUBE_COMPRESSED
    //     std::ifstream in( fileName.toStdString().c_str() );
    // #else
    //     gzifstream    in( fileName.toStdString().c_str(), ios_base::in | ios_base::binary );
    // #endif

    lastExternalFileName = fileName;

    // read in external data into the external cube object
    assert( cubeExternal == NULL );
    cubeExternal = new cube::Cube();
    try
    {
        cubeExternal->openCubeReport( fileName.toStdString() );
        cubeExternal->setGlobalMemoryStrategy( strategy );
        // compute external reference values for metric tree
        MetricTree* metricTree = static_cast<MetricTree*> ( tabManager->getTree( METRICTREE ) );
        metricTree->computeExternalReferenceValues( cubeExternal );
    }
    catch ( const cube::RuntimeError& e )
    {
        setMessage( e.get_msg().c_str(), Error );
    }

    // the relevant values got stores in the metric tab widget
    // and we can delete the external cube object
    delete cubeExternal;
    cubeExternal = NULL;

    setCursor( Qt::ArrowCursor );
    setMessage( tr( "Ready" ) );

    return true;
}

// remove loaded external data for external percentage
//
void
MainWidget::closeExternalFile()
{
    // check if external modus is set in one of the tabs;

    foreach( TabWidget * widget, tabManager->getTabWidgets() )
    {
        if ( widget->getValueModus() == EXTERNAL )
        {
            widget->setValueModus( ABSOLUTE );
        }
    }

    lastExternalFileName = "";
    updateWidgetTitle();
    emit enableExtClose( false );
}


/*********************************************************/
/****************** miscellaneous ************************/
/*********************************************************/

void
MainWidget::closeApplication()
{
    close(); // closeEvent is called
}

// clean-up before close
// closes the application after eventually saving settings


// for calling scorep_finalize(), should be function-name in C++ have 'C' linkage : extern "C" void scorep_finalize(void);
DEFINE_SCOREP_FINALIZE();

void
MainWidget::closeEvent( QCloseEvent* event )
{
    // Before closing the window by choosing "Close" from the menu, or clicking the X button, scorep_finalize() should be called.
    CALL_SCOREP_FINALIZE();

    Q_UNUSED( event );

    closeFile();


    close();
}

// set the order of metric/call/system splitter elements
//
void
MainWidget::setDimensionOrder()
{
    DimensionOrderDialog dialog( this, tabManager->getOrder() );
    if ( dialog.exec() )
    {
        QList<DisplayType> order = dialog.getOrder();
        tabManager->setOrder( order );
    }
}

void
MainWidget::updateColormapMenu()
{
    mapMenu->clear();

    QList<ColorMap*> maps = PluginManager::getInstance()->getColorMaps();
    maps.prepend( Globals::defaultColorMap );

    QActionGroup* selectionActionGroup = new QActionGroup( this );
    selectionActionGroup->setExclusive( true );
    QString currentMapName = Globals::getColorMap()->getMapName();
    foreach( ColorMap * map, maps )
    {
        QAction* action = mapMenu->addAction( map->getMapName() );
        action->setCheckable( true );
        selectionActionGroup->addAction( action );
        action->setData( qVariantFromValue( ( void* )map ) );
        connect( action, SIGNAL( triggered() ), this, SLOT( selectColorMap() ) );
        if ( map->getMapName() == currentMapName )
        {
            action->setChecked( true );
        }
    }
}

// causes to open a color dialog to allow the user to change the colormap
void
MainWidget::editColorMap()
{
    ColorMap* map = Globals::getColorMap();
    map->showDialog();
}

// slot for color map menu item
void
MainWidget::selectColorMap()
{
    QAction*  action = qobject_cast<QAction*>( sender() );
    ColorMap* map    = static_cast<ColorMap*> ( action->data().value<void*>() );
    setColorMap( map );
}

void
MainWidget::setColorMap( ColorMap* map )
{
    ColorMap* old = Globals::getColorMap();
    if ( old == map )
    {
        return;
    }
    disconnect( old, SIGNAL( colorMapChanged() ), this, SLOT( updateColorMap() ) );
    connect( map, SIGNAL( colorMapChanged() ), this, SLOT( updateColorMap() ) );
    // remember current map name to store in settings
    lastColorMapName = map->getMapName();
    updateColorMap( map );
}

// slot, called if apply is pressed in edit dialog
void
MainWidget::updateColorMap( ColorMap* map )
{
    if ( map == 0 )
    {
        map = Globals::getColorMap();
    }
    Globals::setColorMap( map );
    if ( cube ) // notify plugin tabs
    {
        tabManager->updateColors();
    }
}



// void MainWidget::applyColorMapChange()
// {
// we need to redraw color bars, nothing more
//       update();
//       tabManager->updateTreeItemProperties();
// }


// causes to open a precision dialog to allow the user to change precision-specific settings
//
void
MainWidget::setPrecision()
{
    PrecisionWidget* precisionWidget = Globals::getPrecisionWidget();
    precisionWidget->exec();
    setMessage( tr( "Ready" ) );
}

//
//
void
MainWidget::triggerStatusOfFlattree()
{
    TabWidget* tab  = tabManager->getTab( CALL );
    TreeView*  view = tabManager->getView( CALLFLAT );
    tab->enableTab( view, !view->isEnabled() );
}


// causes to open a font dialog to allow the user to change
// font and spacing for tree widgets
//
void
MainWidget::setFont()
{
    FontWidget fontWidget( this, treeFontFamily, treeFontSize, treeSelectionColor );
    connect( &fontWidget, SIGNAL( apply( FontWidget* ) ), this, SLOT( applyFont( FontWidget* ) ) );
    fontWidget.exec();
    disconnect( &fontWidget, SIGNAL( apply( FontWidget* ) ), this, SLOT( applyFont( FontWidget* ) ) );
}


// slot to apply the settings of the font widget
//
void
MainWidget::applyFont( FontWidget* fontWidget )
{
    treeFont           = fontWidget->getFont();
    treeFontFamily     = fontWidget->getFontFamily();
    treeFontSize       = fontWidget->getFontSize();
    treeSelectionColor = fontWidget->getSelectionColor();

    tabManager->setTreeFont( treeFont, treeSelectionColor );
}

void
MainWidget::onRegionInfo()
{
    assert( cube != NULL );

    TreeType  type = tabManager->getActiveTree( CALL )->getTreeType(); // CALLTREE or CALLFLAT
    TreeView* view = tabManager->getView( type );
    view->onShowDocumentation();
}

void
MainWidget::introduction()
{
    QString str = "Cube (<b>C</b>UBE <b>U</b>niform <b>B</b>ehavioral <b>E</b>ncoding) is a presentation<br>"
                  "component suitable for displaying a wide variety of performance<br>"
                  "data for parallel programs including MPI and OpenMP applications.<br>"
                  "Cube allows interactive exploration of the performance data in a scalable fashion.<br>"
                  "Scalability is achieved in two ways: hierarchical decomposition of<br>"
                  "individual dimensions and aggregation across different dimensions. All<br>"
                  "metrics are uniformly accommodated in the same display and thus<br>"
                  "provide the ability to easily compare the effects of different kinds<br>"
                  "of program behavior.<br>"
                  "<br>"
                  "Cube has been designed around a high-level data model of program<br>"
                  "behavior called the Cube performance space. The Cube<br>"
                  "performance space consists of three dimensions: a metric dimension, a<br>"
                  "program dimension, and a system dimension.  The metric dimension<br>"
                  "contains a set of metrics, such as communication time or<br>"
                  "cache misses.  The program dimension contains the program's<br>"
                  "call tree, which includes all the call paths onto which metric values<br>"
                  "can be mapped. The system dimension contains the items<br>"
                  "executing in parallel, which can be processes or threads depending on<br>"
                  "the parallel programming model.  Each point (m, c, s) of the space<br>"
                  "can be mapped onto a number representing the actual measurement for<br>"
                  "metric m while the control flow of process/thread s was executing<br>"
                  "call path c.  This mapping is called the severity of the<br>"
                  "performance space.<br>"
                  "<br>"
                  "Each dimension of the performance space is organized in a<br>"
                  "hierarchy. First, the metric dimension is organized in an<br>"
                  "inclusion hierarchy where a metric at a lower level is a subset of its<br>"
                  "parent. For example, communication time is below execution time.<br>"
                  "Second, the program dimension is organized in a call-tree hierarchy.<br>"
                  "However, sometimes it can be advantageous to abstract away from the<br>"
                  "hierarchy of the call tree, for example if one is interested in the<br>"
                  "severities of certain methods, independently of the position of their<br>"
                  "invocations.  For this purpose Cube supports also flat call<br>"
                  "profiles, that are represented as a flat sequence of all <br>"
                  "methods.  Finally, the system dimension is organized in a multi-level<br>"
                  "hierarchy consisting of the levels: machine, SMP node, process,<br>"
                  "and thread.<br>"
                  "<br>"
                  "Cube serves with a library to read and write instances of the<br>"
                  "previously described data model in the form of an XML file. The<br>"
                  "file representation is divided into a metadata part and a<br>"
                  "data part. The metadata part describes the structure of the three<br>"
                  "dimensions plus the definitions of various program and system<br>"
                  "resources. The data part contains the actual severity numbers to be<br>"
                  "mapped onto the different elements of the performance space.<br>"
                  "<br>"
                  "The display component can load such a file and display the<br>"
                  "different dimensions of the performance space using three coupled tree<br>"
                  "browsers. The browsers are connected such a way<br>"
                  "that you can view one dimension with respect to another dimension. The<br>"
                  "connection is based on selections: in each tree you can select<br>"
                  "one or more nodes.<br>"
                  "<br>"
                  "If the Cube file contains topological information, the<br>"
                  "distribution of the performance metric across the topology can be<br>"
                  "examined using the topology view.  Furthermore, the display is<br>"
                  "augmented with a source-code display that shows the position<br>"
                  "of a call site in the source code.<br>"
                  "<br>"
                  "As performance tuning of parallel applications usually involves<br>"
                  "multiple experiments to compare the effects of certain optimization<br>"
                  "strategies, Cube includes a feature designed to simplify<br>"
                  "cross-experiment analysis. The Cube algebra is an extension of<br>"
                  "the framework for multi-execution performance<br>"
                  "tuning by Karavanic and Miller and offers a set<br>"
                  "of operators that can be used to compare, integrate, and summarize<br>"
                  "multiple Cube data sets. The algebra allows the combination of<br>"
                  "multiple Cube data sets into a single one that can be displayed like<br>"
                  "the original ones.<br>"
                  "<br>"
                  "If you need further help, press Ctrl+F1 and evtl. select the component<br>"
                  "for which you need help.";


    //     QLabel*      label  = new QLabel( str );
    QTextEdit* text = new QTextEdit( str );
    text->setReadOnly( true );
    //     QVBoxLayout* layout = new QVBoxLayout();
    //     layout->addWidget( text );
    //     QWidget*     widget = new QWidget();
    //     widget->setWindowIcon( QIcon( ":images/CubeIcon.xpm" ) );
    //     widget->setLayout( layout );
    // //     widget->resize(500,600);
    //
    //     QScrollArea* scrollArea = new QScrollArea();
    //     scrollArea->setWidget( widget );

    QDialog dialog;
    dialog.setWindowTitle( QObject::tr( "Introduction to " ).append( CUBE_FULL_NAME ) );
    QVBoxLayout* layout2 = new QVBoxLayout();
    layout2->addWidget( text );
    dialog.setLayout( layout2 );
    dialog.resize( 500, 600 );

    QPushButton* _close = new QPushButton( QObject::tr( "&Close" ) );
    _close->setDefault( true );
    layout2->addWidget( _close );
    connect( _close, SIGNAL( pressed() ), &dialog, SLOT( accept() ) );

    dialog.exec();
}


// shows the about message
//
void
MainWidget::about()
{
    QDialog dialog( this );
    dialog.setWindowIcon( QIcon( ":images/CubeIcon.xpm" ) );
    dialog.setWindowTitle( QObject::tr( "About " ).append( CUBE_FULL_NAME ) );

    QLabel* cube_title    = new QLabel( "<b>This is " CUBE_FULL_NAME " (rev. " CUBE_REVISION ")</b>) (c) 2009-2016" );
    QLabel* qt_version    = new QLabel( "Build with <b>Qt v" QT_VERSION_CUBE " </b>" );
    QLabel* qmake         = new QLabel( "Build with qmake: <b>" QT_PATH "</b>" );
    QLabel* cube_homepage = new QLabel( "Home page        : <a href=\"http://www.scalasca.org\"> " PACKAGE_URL " </a>" );
    cube_homepage->setOpenExternalLinks( true );
    QLabel* cube_support = new QLabel( "Technical support: <a href=\"mailto:scalasca@fz-juelich.de?subject=" CUBE_FULL_NAME " Feedback.&body=Dear Scalasca Team, \n\nwe would like to inform you, that " CUBE_FULL_NAME " (rev. " CUBE_REVISION ") is awesome and we would like to have additional feature: a Teleport and an X-Wing fighter support. \n\n\n\n\n Sincerely,\n User of " CUBE_FULL_NAME "\">scalasca@fz-juelich.de</a>" );
    cube_support->setOpenExternalLinks( true );


    QLabel* fz_copyright = new QLabel( "Juelich Supercomputing Centre,\nForschungszentrum Juelich GmbH" );

    QLabel* fz_logo = new QLabel();
    fz_logo->setPixmap( QPixmap( ":/images/fzjlogo.xpm" ).scaledToWidth( 200 / 1.3,  Qt::SmoothTransformation ) );
    QLabel* cube_logo = new QLabel();
    cube_logo->setPixmap( QPixmap( ":/images/CubeLogo.xpm" ).scaledToWidth( 200 / 1.3,  Qt::SmoothTransformation ) );

    QVBoxLayout* main_layout        = new QVBoxLayout();
    QHBoxLayout* fz_layout          = new QHBoxLayout();
    QHBoxLayout* cube_layout        = new QHBoxLayout();
    QVBoxLayout* fz_labels_layout   = new QVBoxLayout();
    QVBoxLayout* cube_labels_layout = new QVBoxLayout();
    QVBoxLayout* fz_logos_layout    = new QVBoxLayout();
    QVBoxLayout* cube_logos_layout  = new QVBoxLayout();

    main_layout->addLayout( cube_layout );
    main_layout->addLayout( fz_layout );

    fz_layout->addLayout( fz_logos_layout );
    fz_layout->addLayout( fz_labels_layout );

    cube_layout->addLayout( cube_logos_layout );
    cube_layout->addLayout( cube_labels_layout );


    main_layout->setAlignment( Qt::AlignHCenter );

    fz_layout->setAlignment( Qt::AlignLeading );
    fz_labels_layout->setAlignment( Qt::AlignLeading );

    cube_labels_layout->setAlignment( Qt::AlignLeading );
    cube_labels_layout->setAlignment( cube_title,  Qt::AlignHCenter );
    cube_labels_layout->setAlignment( qt_version,  Qt::AlignHCenter );
    cube_labels_layout->setAlignment( qmake,  Qt::AlignHCenter );

    /*  QVBoxLayout * layout = new QVBoxLayout();
       QVBoxLayout * logos_layout = new QVBoxLayout();*/

    main_layout->setSpacing( 5 );
    cube_layout->setSpacing( 5 );
    fz_layout->setSpacing( 5 );

    cube_labels_layout->setSpacing( 1 );
    // logos_layout->setSpacing(25);



    //   QHBoxLayout *layout2 = new QHBoxLayout();

    fz_labels_layout->addWidget( fz_copyright );
    fz_logos_layout->addWidget( fz_logo );

    cube_labels_layout->addWidget( cube_title );
    cube_labels_layout->addWidget( cube_homepage );
    cube_labels_layout->addWidget( cube_support );
    cube_labels_layout->addWidget( qt_version );
    cube_labels_layout->addWidget( qmake );




    cube_logos_layout->addWidget( cube_logo );

    dialog.setLayout( main_layout );

    QPushButton* _close = new QPushButton( QObject::tr( "&Close" ) );
    _close->setDefault( true );
    main_layout->addWidget( _close );
    connect( _close, SIGNAL( pressed() ), &dialog, SLOT( accept() ) );
    dialog.exec();

    setMessage( tr( "Ready" ) );
}


void
MainWidget::keyHelp()
{
    QString key1, role1;
    key1.append( "Shift+F1\n\n" );
    role1.append( "Help: What\'s this?\n\n" );
    key1.append( "Ctrl+O\n" );
    role1.append( "Shortcut for menu File -> Open\n" );
    key1.append( "Ctrl+W\n" );
    role1.append( "Shortcut for menu File -> Close\n" );
    key1.append( "Ctrl+Q\n" );
    role1.append( "Shortcut for menu File -> Quit \n" );
    key1.append( "Left click\n" );
    role1.append( "over menu/tool bar: activate menu/function \n" );
    key1.append( "\n" );
    role1.append( "over value mode combo: select value mode\n" );
    key1.append( "\n" );
    role1.append( "over tab: switch to tab \n" );
    key1.append( "\n" );
    role1.append( "in tree: select/deselect/expand/collapse items\n" );
    key1.append( "\n" );
    role1.append( "in topology: select item\n" );
    key1.append( "Right click\n" );
    role1.append( "in tree: context menu\n" );
    key1.append( "\n" );
    role1.append( "in topology: context information \n" );
    key1.append( "Ctrl+Left click\n" );
    role1.append( "in tree: multiple selection/deselection \n" );
    key1.append( "Left drag\n" );
    role1.append( "over scroll bar: scroll \n" );
    key1.append( "\n" );
    role1.append( "in topology: rotate topology \n" );
    key1.append( "Ctrl+Left drag\n" );
    role1.append( "in topology: increase plane distance \n" );
    key1.append( "Shift+Left drag\n" );
    role1.append( "in topology: move topology \n" );
    key1.append( "Mouse wheel\n" );
    role1.append( "in topology: zoom in/out \n" );
    key1.append( "Up arrow\n" );
    role1.append( "in tree: move selection one item up (single-selection only)\n" );
    key1.append( "\n" );
    role1.append( "in topology/scroll area: scroll one unit up \n" );
    key1.append( "Down arrow\n" );
    role1.append( "in tree: move selection one item down (single-selection only) \n" );
    key1.append( "\n" );
    role1.append( "in topology/scroll area:: scroll one unit down \n" );
    key1.append( "Left arrow\n" );
    role1.append( "in scroll area: scroll to the left \n" );
    key1.append( "Right arrow\n" );
    role1.append( "in scroll area: scroll to the right\n" );
    key1.append( "Page up\n" );
    role1.append( "in tree/topology/scroll area: scroll one page up \n" );
    key1.append( "Page down\n" );
    role1.append( "in tree/topology/scroll area: scroll one page down \n" );


    QString key2, role2;
    key2.append( "Up Arrow\n" );
    role2.append( "Move one line up\n" );
    key2.append( "Down Arrow\n" );
    role2.append( "Move one line down\n" );
    key2.append( "Left Arrow\n" );
    role2.append( "Scroll one character to the left (if horizontally scrollable)\n" );
    key2.append( "Right Arrow\n" );
    role2.append( "Scroll one character to the right (if horizontally scrollable)\n" );
    key2.append( "Page Up\n" );
    role2.append( "Move one (viewport) page up \n" );
    key2.append( "PageDown\n" );
    role2.append( "Move one (viewport) page down \n" );
    key2.append( "Home\n" );
    role2.append( "Move to the beginning of the text \n" );
    key2.append( "End\n" );
    role2.append( "Move to the end of the text \n" );
    key2.append( "Mouse wheel\n" );
    role2.append( "Scroll the page vertically\n" );
    key2.append( "Alt+Mouse wheel\n" );
    role2.append( "Scroll the page horizontally (if horizontally scrollable)\n" );
    key2.append( "Ctrl+Mouse wheel\n" );
    role2.append( "Zoom the text \n" );
    key2.append( "Ctrl+A\n" );
    role2.append( "Select all text \n " );


    QString key3, role3;
    key3.append( "Left Arrow\n" );
    role3.append( "Move one character to the left \n" );
    key3.append( "Right Arrow\n" );
    role3.append( "Move one character to the right\n" );
    key3.append( "Backspace\n" );
    role3.append( "Delete the character to the left of the cursor\n" );
    key3.append( "Delete\n" );
    role3.append( "Delete the character to the right of the cursor\n" );
    key3.append( "Ctrl+C\n" );
    role3.append( "Copy the selected text to the clipboard\n" );
    key3.append( "Ctrl+Insert\n" );
    role3.append( "Copy the selected text to the clipboard\n" );
    key3.append( "Ctrl+K\n" );
    role3.append( "Delete to the end of the line\n" );
    key3.append( "Ctrl+V\n" );
    role3.append( "Paste the clipboard text into text edit\n" );
    key3.append( "Shift+Insert\n" );
    role3.append( "Paste the clipboard text into text edit\n" );
    key3.append( "Ctrl+X\n" );
    role3.append( "Delete the selected text and copy it to the clipboard\n" );
    key3.append( "Shift+Delete\n" );
    role3.append( "Delete the selected text and copy it to the clipboard\n" );
    key3.append( "Ctrl+Z\n" );
    role3.append( "Undo the last operation\n" );
    key3.append( "Ctrl+Y\n" );
    role3.append( "Redo the last operation\n" );
    key3.append( "Ctrl+Left arrow\n" );
    role3.append( "Move the cursor one word to the left\n" );
    key3.append( "Ctrl+Right arrow\n" );
    role3.append( "Move the cursor one word to the right\n" );
    key3.append( "Ctrl+Home\n" );
    role3.append( "Move the cursor to the beginning of the text\n" );
    key3.append( "Ctrl+End\n" );
    role3.append( "Move the cursor to the end of the text\n" );
    key3.append( "Hold Shift + some movement\n" );
    role3.append( "Select region \n" );
    key3.append( "(e.g. Right arrow)\n" );


    QLabel* key1Label  = new QLabel( key1 );
    QLabel* role1Label = new QLabel( role1 );
    QLabel* key2Label  = new QLabel( key2 );
    QLabel* role2Label = new QLabel( role2 );
    QLabel* key3Label  = new QLabel( key3 );
    QLabel* role3Label = new QLabel( role3 );

    QHBoxLayout* layout1 = new QHBoxLayout();
    layout1->addWidget( key1Label );
    layout1->addWidget( role1Label );
    layout1->setAlignment( key1Label, Qt::AlignTop | Qt::AlignLeft );
    layout1->setAlignment( role1Label, Qt::AlignTop | Qt::AlignLeft );
    QHBoxLayout* layout2 = new QHBoxLayout();
    layout2->addWidget( key2Label );
    layout2->addWidget( role2Label );
    layout2->setAlignment( key2Label, Qt::AlignTop | Qt::AlignLeft );
    layout2->setAlignment( role2Label, Qt::AlignTop | Qt::AlignLeft );
    QHBoxLayout* layout3 = new QHBoxLayout();
    layout3->addWidget( key3Label );
    layout3->addWidget( role3Label );
    layout3->setAlignment( key3Label, Qt::AlignTop | Qt::AlignLeft );
    layout3->setAlignment( role3Label, Qt::AlignTop | Qt::AlignLeft );

    QLabel* descr1 = new QLabel( "<b>General control:</b>" );
    QLabel* descr2 = new QLabel( "<b>For source code editor in read-only mode:</b>" );
    QLabel* descr3 = new QLabel( "<b>For source code editor in read-and-write mode additionally:</b>" );

    QVBoxLayout* widgetLayout = new QVBoxLayout();
    widgetLayout->addWidget( descr1 );
    widgetLayout->addLayout( layout1 );
    widgetLayout->addWidget( descr2 );
    widgetLayout->addLayout( layout2 );
    widgetLayout->addWidget( descr3 );
    widgetLayout->addLayout( layout3 );

    widgetLayout->setAlignment( descr1, Qt::AlignTop | Qt::AlignLeft );
    widgetLayout->setAlignment( descr2, Qt::AlignTop | Qt::AlignLeft );
    widgetLayout->setAlignment( descr3, Qt::AlignTop | Qt::AlignLeft );
    widgetLayout->setAlignment( layout1, Qt::AlignTop | Qt::AlignLeft );
    widgetLayout->setAlignment( layout2, Qt::AlignTop | Qt::AlignLeft );
    widgetLayout->setAlignment( layout3, Qt::AlignTop | Qt::AlignLeft );

    QWidget* widget = new QWidget();
    widget->setWindowIcon( QIcon( ":images/CubeIcon.xpm" ) );

    widget->setLayout( widgetLayout );

    QScrollArea* scrollWidget = new QScrollArea();
    scrollWidget->setBackgroundRole( QPalette::Base );
    scrollWidget->setWidget( widget );

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget( scrollWidget );

    QDialog dialog( this );
    dialog.setWindowTitle( QObject::tr( "Controls in " ).append( CUBE_FULL_NAME ) );
    dialog.resize( 500, 600 );
    dialog.setLayout( layout );

    QPushButton* _close = new QPushButton( QObject::tr( "&Close" ) );
    _close->setDefault( true );
    layout->addWidget( _close );
    connect( _close, SIGNAL( pressed() ), &dialog, SLOT( accept() ) );

    dialog.exec();

    setMessage( tr( "Ready" ) );
}


// sets the file size threshold above which dynamic loading of metric data is enabled
//
void
MainWidget::setDynloadThreshold()
{
    bool ok;
    int  value = QInputDialog::getInt( this, "Dynamic loading threshold", "File size threshold (byte) above which files are dynamically loaded: ", dynloadThreshold, 0, 1000000000, 50000000, &ok );
    if ( ok )
    {
        dynloadThreshold = value;
    }
}


// saves a screenshot in a png file
// (it is without the shell frame, I could not find any possibilities to include that frame within qt)
//
void
MainWidget::screenshot()
{
    // this is based on repainting the window into the pixmap
    QPixmap pixmap = QPixmap::grabWidget( this );
    // alternatively direct shot from pixels on the screen:
    // QPixmap pixmap = QPixmap::grabWindow(winId());

    QImage image = pixmap.toImage();

    QString format      = "png";
    QString initialPath = QDir::currentPath() + tr( "/cube." ) + format;

    QString fileName = QFileDialog::getSaveFileName( this, tr( "Save As" ),
                                                     initialPath,
                                                     tr( "%1 Files (*.%2);;All Files (*)" )
                                                     .arg( format.toUpper() )
                                                     .arg( format ) );
    if ( !fileName.isEmpty() )
    {
        image.save( fileName, format.toLocal8Bit() );
    }

    setMessage( tr( "Ready" ) );
}

// sets the busy cursor
//
void
MainWidget::setBusy( bool busy )
{
    setCursor( busy ? Qt::BusyCursor : Qt::ArrowCursor );
}



/** distribute the available width between the 3 tabs such that from each scroll area the same percentual
 * amount is visible
 */
void
MainWidget::distributeWidth()
{
    // get the current sizes of the splitter's widgets
    QList<int> sizes   = tabManager->sizes();
    int        current = 0;
    foreach( int val, sizes )
    {
        current += val;
    }

    // get optimal widths for the splitter's widgets
    int preferred = 0;
    int idx       = 0;
    foreach( TabWidget * widget, tabManager->getTabWidgets() )
    {
        sizes[ idx ] = widget->sizeHint().width();
        preferred   += sizes[ idx++ ];
    }
    // distribute the available width
    for ( int i = 0; i < 3; i++ )
    {
        sizes[ i ] = ( int )( ( double )current * ( double )sizes[ i ] / ( double )preferred );
    }
    // resize
    tabManager->setSizes( sizes );
}

/** adapt the size of the main window such that all informations are visible and resize the splitter widget
 *  accordingly */
void
MainWidget::adaptWidth()
{
    // get optimal widths for the splitter's widgets
    QList<int> sizes     = tabManager->sizes();
    int        current   = 0; // sum of current splitter sizes
    int        preferred = 0; // sum of preferred content size
    int        idx       = 0;
    foreach( TabWidget * widget, tabManager->getTabWidgets() )
    {
        current += sizes.at( idx );
        int preferredWidth = widget->sizeHint().width();
        sizes[ idx ] = preferredWidth;
        preferred   += preferredWidth;
        idx++;
    }

    int pad = this->width() - current; // get space without tab widget contents, e.g. splitter control, paddings
    pad += 10;
    resize( preferred + pad, height() );
    tabManager->setSizes( sizes );
}

// ------------ setting handler implementation ----------------------------------

QString
MainWidget::settingName()
{
    return "MainWindow";
}

/** saves settings that do not depend on an experiment */
void
MainWidget::saveGlobalSettings( QSettings& settings )
{
    // save position and size of the main window
    settings.setValue( "coords/size", size() );
    settings.setValue( "coords/pos", pos() );

    // save widths of the widgets in the splitter widget
    /*
       QList<int>      splitterSizes = splitterWidget->sizes();
       QList<QVariant> variantList;
       for ( int i = 0; i < splitterSizes.size(); i++ )
       {
        QVariant v( splitterSizes[ i ] );
        variantList.push_back( v );
       }
       settings.setValue( "splitterSizes", variantList );
     */

    // tree widget display
    settings.setValue( "treeFontFamily", treeFontFamily );
    settings.setValue( "treeFontPointSize", treeFontSize );

    QTreeView view;
    QPalette  treePalette           = view.palette();
    QColor    defaultSelectionColor = treePalette.color( QPalette::Highlight );
    QColor    color;
    if ( treeSelectionColor != defaultSelectionColor )
    {
        color = treeSelectionColor;
    }
    settings.setValue( "selectionColor", color );

    // save precision values
    PrecisionWidget* precisionWidget = Globals::getPrecisionWidget();
    settings.setValue( "precision/prec0", precisionWidget->getPrecision( FORMAT_TREES ) );
    settings.setValue( "precision/round0", precisionWidget->getRoundNr( FORMAT_TREES ) );
    settings.setValue( "precision/exp0", precisionWidget->getUpperExpNr( FORMAT_TREES ) );
    settings.setValue( "precision/prec1", precisionWidget->getPrecision( FORMAT_DEFAULT ) );
    settings.setValue( "precision/round1", precisionWidget->getRoundNr( FORMAT_DEFAULT ) );
    settings.setValue( "precision/exp1", precisionWidget->getUpperExpNr( FORMAT_DEFAULT ) );

    // save external file name
    settings.setValue( "lastExternalFileName", lastExternalFileName );
    // save file size threshold above which dynamic loading is enabled
    settings.setValue( "dynloadThreshold", dynloadThreshold );

    settings.setValue( "lastColorMapName", lastColorMapName );
}

void
MainWidget::loadGlobalStartupSettings( QSettings& settings )
{
    // restore names of recent files
    QStringList files = settings.value( "openedFiles" ).toStringList();
    for ( int i = files.size() - 1; i >= 0; --i )
    {
        rememberFileName( files[ i ] );
    }
}

void
MainWidget::saveGlobalStartupSettings( QSettings& settings )
{
    settings.setValue( "openedFiles", openedFiles );
}

void
MainWidget::loadGlobalSettings( QSettings& settings )
{
    QSettings settingSettings; // global settings without group
    bool      restoreGeometry = settingSettings.value( "RestoreExperimentGeometry", false ).toBool();

    // restore window geometry only on startup or if settings "restoreGeometry" is set
    if ( !settingsAreInitialized || restoreGeometry )
    {
        settingsAreInitialized = true;
        // restore position and size of the main window
        resize( settings.value( "coords/size", QSize( 600, 500 ) ).toSize() );
        move( settings.value( "coords/pos", QPoint( 200, 200 ) ).toPoint() );

        // restoring the width of the widgets in the splitter widget
        /* todo settings splitter size
           QList<QVariant> v = settings.value( "splitterSizes", QList<QVariant>() ).toList();
           if ( v.size() > 0 )
           {
            QList<int> splitterSizes;
            for ( int i = 0; i < v.size(); i++ )
            {
                splitterSizes.push_back( v[ i ].toInt() );
            }
            splitterWidget->setSizes( splitterSizes );
           }
         */
    }

    // tree widget display
    // spacing = settings.value( "spacing", 5 ).toInt();
    QFont defaultFont =  QTreeView().font();
    treeFontFamily = settings.value( "treeFontFamily", USE_DEFAULT ).toString();
    treeFontSize   = settings.value( "treeFontPointSize", -1 ).toInt();
    treeFont.setFamily( treeFontFamily != USE_DEFAULT ? treeFontFamily : defaultFont.family() );
    treeFont.setPointSize( treeFontSize > 0 ? treeFontSize : defaultFont.pointSize() );
    QColor color = settings.value( "selectionColor", QColor() ).value<QColor>();
    treeSelectionColor = color.isValid() ? color : treeSelectionColor;
    tabManager->setTreeFont( treeFont, treeSelectionColor );

    // restore precision values
    PrecisionWidget* precisionWidget = Globals::getPrecisionWidget();
    precisionWidget->setPrecision( settings.value( "precision/prec0", 2 ).toInt(), FORMAT_TREES );
    precisionWidget->setUpperExpNr( settings.value( "precision/exp0", 4 ).toInt(), FORMAT_TREES );
    precisionWidget->setRoundNr( settings.value( "precision/round0", 7 ).toInt(), FORMAT_TREES );
    precisionWidget->setPrecision( settings.value( "precision/prec1", 2 ).toInt(), FORMAT_DEFAULT );
    precisionWidget->setUpperExpNr( settings.value( "precision/exp1", 4 ).toInt(), FORMAT_DEFAULT );
    precisionWidget->setRoundNr( settings.value( "precision/round1", 7 ).toInt(), FORMAT_DEFAULT );

    // restore the threshold above which dynamic metric loading is enabled
    dynloadThreshold = settings.value( "dynloadThreshold", 1000000000 ).toInt();

    lastColorMapName = settings.value( "lastColorMapName", "" ).toString();
    // should be safe - the call to settings is AFTER initializing tabs
    loadColorMap();
}

void
MainWidget::saveExperimentSettings( QSettings& experimentSettings )
{
    tabManager->saveExperimentSettings( experimentSettings );
}

void
MainWidget::loadExperimentSettings( QSettings& experimentSettings )
{
    tabManager->loadExperimentSettings( experimentSettings );
    setMessage( tr( "Ready" ) );
}

void
MainWidget::loadColorMap()
{
    QList<ColorMap*> maps = PluginManager::getInstance()->getColorMaps();
    foreach( ColorMap * map, maps )
    {
        if ( lastColorMapName == map->getMapName() )
        {
            setColorMap( map );
            break;
        }
    }
}

void
MainWidget::dragEnterEvent( QDragEnterEvent* event )
{
    const QMimeData* mime = event->mimeData();
    if ( mime->hasUrls() &&  mime->text().startsWith( "file://" ) && ( mime->text().endsWith( ".cube" ) || mime->text().endsWith( ".cubex" ) ) )
    {
        event->acceptProposedAction();
    }
}

void
MainWidget::dropEvent( QDropEvent* event )
{
    QString file = event->mimeData()->text();
    file.remove( "file://" );
    readFile( file );
    event->acceptProposedAction();
}
