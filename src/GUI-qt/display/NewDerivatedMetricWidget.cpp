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


#include <sstream>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QWhatsThis>
#include <QToolTip>
#include <QUrl>
#include <QFileDialog>
#include <QTextStream>
#include <QClipboard>
#include <QApplication>
#include <QMimeData>
#include <QDebug>
#include "NewDerivatedMetricWidget.h"
#include "CubePLSyntaxHighlighter.h"
#include "HtmlHelpCollection.h"
#include "DerivedMetricsCollection.h"
#include "Globals.h"

using namespace std;

/*--- Constructors & destructor -------------------------------------------*/

/**
 * @brief Constructs a widget for construction of a new derivated metric
 *
 * This constructor creates a new help browser dialog with the given window
 * @p title and @p parent widget.
 *
 * @param  title   Window title
 * @param  parent  Parent widget
 */
NewDerivatedMetricWidget::NewDerivatedMetricWidget( cube::Cube*   _cube,
                                                    cube::Metric* _working_metric,
                                                    cube::Metric* _met_parent,
                                                    QWidget*      parent )
    : QDialog( parent ), cube( _cube ), parent_metric( _met_parent ), working_metric( _working_metric )
{
    metric_type_selection    = NULL;
    metric_library_selection = NULL;
    display_name_input       = NULL;
    unique_name_input        = NULL;
    uom_input                = NULL;
    value_input              = NULL;
    url_input                = NULL;
    description_input        = NULL;

    calculations_container = NULL;

    calculation_input      = NULL;
    calculation_init_input = NULL;
    calculation_aggr_plus  = NULL;
    calculation_aggr_minus = NULL;


    create_metric         = NULL;
    _whatis_help          = NULL;
    _metric_kinds_help    = NULL;
    _metric_expt_help     = NULL;
    _metric_lib_help      = NULL;
    metric_from_file      = NULL;
    metric_to_file        = NULL;
    metric_from_clipboard = NULL;


    share_metric = NULL;

    error_highlighter            = NULL;
    error_init_highlighter       = NULL;
    error_aggr_plus_highlighter  = NULL;
    error_aggr_minus_highlighter = NULL;

    help_browser = NULL;
    statusBar    = new StatusBar();

    setAcceptDrops( true );
    calculation_ok      = true;
    calculation_init_ok = true;
    if ( parent_metric != NULL )
    {
        uniq_name = parent_metric->get_uniq_name();
        disp_name = parent_metric->get_disp_name();
        uom       = parent_metric->get_uom();
        val       = parent_metric->get_val();
        url       = parent_metric->get_url();
        descr     = parent_metric->get_descr();
        kind      = parent_metric->get_metric_kind();
    }

    if ( working_metric == NULL )
    {
        setWindowTitle( tr( "Create new metric as a child of metric " ).append( QString::fromStdString( disp_name ) ) );
    }
    else
    {
        setWindowTitle( tr( "Edit metric " ).append( QString::fromStdString( working_metric->get_disp_name() ) ) );
    }
    QVBoxLayout* top_layout = new QVBoxLayout();
    top_layout->setSpacing( 5 );
    QHBoxLayout* layout1 = new QHBoxLayout();

    layout1->addWidget( new QLabel( tr( "<b>Select metric from collection :</b>" ) ) );
    metric_library_selection = new QComboBox();
    metric_library_selection->addItems( DerivedMetricsCollection::getDerivedMetricsNames() );
    metric_library_selection->setCurrentIndex( 0 );
    metric_library_selection->setWhatsThis( HelpCollection::getHelpText( WHAT_IS_DERIVED_METRICS_LIBRARY ) );
    layout1->addWidget( metric_library_selection );
    connect( metric_library_selection, SIGNAL( currentIndexChanged( int  ) ), this, SLOT( selectMetricFromLibrary( int  ) ) );

    if ( working_metric != NULL )
    {
        metric_library_selection->setEnabled( false );
    }

    layout1->addSpacing( 15 );


    metric_to_file = new QPushButton( QIcon( ":images/savetofile.png" ), "" );
    metric_to_file->setWhatsThis( HelpCollection::getHelpText( WHAT_IS_TO_FILE ) );
    layout1->addWidget( metric_to_file );
    connect( metric_to_file, SIGNAL( pressed() ), this, SLOT( saveMetricToFile() ) );
    layout1->addSpacing( 3 );

    metric_from_file = new QPushButton( QIcon( ":images/fromtextfile.png" ), "" );
    metric_from_file->setWhatsThis( HelpCollection::getHelpText( WHAT_IS_FROM_FILE ) );
    layout1->addWidget( metric_from_file );
    connect( metric_from_file, SIGNAL( pressed() ), this, SLOT( createMetricFromFile() ) );

    metric_from_clipboard = new QPushButton( QIcon( ":images/paste-icon.png" ), "" );
    metric_from_clipboard->setWhatsThis( HelpCollection::getHelpText( WHAT_IS_CLIPBOARD ) );
    layout1->addWidget( metric_from_clipboard );
    connect( metric_from_clipboard, SIGNAL( pressed() ), this, SLOT( createMetricFromClipboard() ) );


    layout1->addStretch();

    top_layout->addLayout( layout1 );
    QWidget* horizontalLineWidget = new QWidget;
    horizontalLineWidget->setFixedHeight( 2 );
    horizontalLineWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
    horizontalLineWidget->setStyleSheet( QString( "background-color: #c0c0c0;" ) );
    top_layout->addWidget( horizontalLineWidget  );

    QGridLayout* grid_layout2 = new QGridLayout();

    grid_layout2->addWidget( new QLabel( tr( "<b>Derived metric type :</b>" ) ), 0, 0 );
    metric_type_selection = new QComboBox();
    metric_type_selection->addItem( " --- please select type of derived metric ---- " );
    metric_type_selection->addItem( "Postderived metric" );
    metric_type_selection->addItem( "Prederived inclusive metric" );
    metric_type_selection->addItem( "Prederived exclusive metric" );
    grid_layout2->addWidget( metric_type_selection, 0, 1 );
    connect( metric_type_selection, SIGNAL( currentIndexChanged( int  ) ), this, SLOT( setMetricType( int  ) ) );


    _metric_kinds_help = new QPushButton( QIcon( ":images/whatishelp.png" ), "" );
    grid_layout2->addWidget( _metric_kinds_help, 0, 2 );
    connect( _metric_kinds_help, SIGNAL( pressed() ), this, SLOT( showKindsOfDerivedMetricsHelp() ) );






    grid_layout2->addWidget( new QLabel( tr( "<b>Display name :</b>" ) ), 1, 0 );
    display_name_input = new QLineEdit();
    grid_layout2->addWidget( display_name_input, 1, 1 );
    if ( working_metric != NULL )
    {
        display_name_input->setText( working_metric->get_disp_name().c_str() );
        setDisplayName( working_metric->get_disp_name().c_str() );
        display_name_input->setEnabled( false );
    }
    connect( display_name_input, SIGNAL( textChanged( const QString & ) ), this, SLOT( setDisplayName( const QString & ) ) );

    grid_layout2->addWidget( new QLabel( tr( "<b>Unique name :</b>" ) ), 2, 0 );
    unique_name_input =  new QLineEdit();
    grid_layout2->addWidget( unique_name_input, 2, 1 );
    if ( working_metric != NULL )
    {
        unique_name_input->setText( working_metric->get_uniq_name().c_str() );
        setUniqName( working_metric->get_uniq_name().c_str() );
        unique_name_input->setEnabled( false );
    }
    connect( unique_name_input, SIGNAL( textChanged( const QString & ) ), this, SLOT( setUniqName( const QString & ) ) );

    grid_layout2->addWidget( new QLabel( tr( "<b>Data type :</b>" ) ), 3, 0 );
    QLabel* _label = NULL;
    if ( working_metric != NULL )
    {
        _label = new QLabel( working_metric->get_dtype().c_str()  );
        _label->setEnabled( false );
    }
    else
    {
        _label = new QLabel( ( parent_metric == NULL ) ? QString::fromStdString( "DOUBLE" ) : QString::fromStdString( parent_metric->get_dtype() ) );
    }
    grid_layout2->addWidget( _label, 3, 1 );

    grid_layout2->addWidget( new QLabel( tr( "<b>Unit of measurement :</b>" ) ), 4, 0 );
    uom_input = new QLineEdit();
    grid_layout2->addWidget( uom_input, 4, 1 );
    if ( working_metric != NULL )
    {
        uom_input->setText( working_metric->get_uom().c_str() );
        setUom( working_metric->get_uom().c_str() );
        uom_input->setEnabled( false );
    }
    connect( uom_input, SIGNAL( textChanged( const QString & ) ), this, SLOT( setUom( const QString & ) ) );

    grid_layout2->addWidget( new QLabel( tr( "<b>URL :</b>" ) ), 5, 0 );
    url_input = new QLineEdit();
    grid_layout2->addWidget( url_input, 5, 1 );
    if ( working_metric != NULL )
    {
        url_input->setText( working_metric->get_url().c_str() );
        setURL( working_metric->get_url().c_str() );
        url_input->setEnabled( false );
    }
    connect( url_input, SIGNAL( textChanged( const QString & ) ), this, SLOT( setURL( const QString & ) ) );

    top_layout->addLayout( grid_layout2 );
    QVBoxLayout* layout3 = new QVBoxLayout();


    layout3->addWidget( new QLabel( tr( "<b>Description :</b>" ) ) );
    description_input = new QTextEdit();
    layout3->addWidget( description_input );
    if ( working_metric != NULL )
    {
        description_input->setText( working_metric->get_descr().c_str() );
        setDescription();
        description_input->setEnabled( false );
    }
    connect( description_input, SIGNAL( textChanged() ), this, SLOT( setDescription() ) );


    top_layout->addLayout( layout3 );

    QGridLayout* exp_layout = new QGridLayout();

    calculations_container = new QTabWidget();

    _metric_expt_help = new QPushButton( QIcon( ":images/whatishelp.png" ), "" );
    calculations_container->setCornerWidget( _metric_expt_help );
    connect( _metric_expt_help, SIGNAL( pressed() ), this, SLOT( showMetricExpressionHelp() ) );


    exp_layout->addWidget( calculations_container );

    calculation_input = new DerivedMetricEditor();
    calculations_container->addTab( calculation_input, tr( "&Calculation" ) );
    calculations_container->setTabIcon( 0, QIcon( ":images/source.png" ) );


    calculation_init_input = new DerivedMetricEditor();
    calculations_container->addTab( calculation_init_input, tr( "Calculation &Init" ) );
    calculations_container->setTabIcon( 1, QIcon( ":images/source.png" ) );
    calculation_aggr_plus = new DerivedMetricEditor();
    calculations_container->addTab( calculation_aggr_plus, tr( "Aggregation \"&+\"" ) );
    calculations_container->setTabIcon( 2, QIcon( ":images/source.png" ) );
    calculation_aggr_minus = new DerivedMetricEditor();
    calculations_container->addTab( calculation_aggr_minus, tr( "Aggregation \"&-\"" ) );
    calculations_container->setTabIcon( 3, QIcon( ":images/source.png" ) );



    QStringList list = getCompletionList();
    connect( calculation_input, SIGNAL( textChanged() ), this, SLOT( setCubePLExpression() ) );
    calculation_input->setKeywords( list );
    error_highlighter = new CubePLSyntaxHighlighter( calculation_input->document() );
    error_highlighter->rehighlight();

    connect( calculation_init_input, SIGNAL( textChanged() ), this, SLOT( setCubePLInitExpression() ) );
    calculation_init_input->setKeywords( list );
    error_init_highlighter = new CubePLSyntaxHighlighter( calculation_init_input->document() );
    error_init_highlighter->rehighlight();

    connect( calculation_aggr_plus, SIGNAL( textChanged() ), this, SLOT( setCubePLAggrPlusExpression() ) );
    calculation_aggr_plus->setKeywords( list );
    error_aggr_plus_highlighter = new CubePLSyntaxHighlighter( calculation_aggr_plus->document() );
    error_aggr_plus_highlighter->rehighlight();

    connect( calculation_aggr_minus, SIGNAL( textChanged() ), this, SLOT( setCubePLAggrMinusExpression() ) );
    calculation_aggr_minus->setKeywords( list );
    error_aggr_minus_highlighter = new CubePLSyntaxHighlighter( calculation_aggr_minus->document() );
    error_aggr_minus_highlighter->rehighlight();



    if ( working_metric != NULL )
    {
        if ( working_metric->get_type_of_metric() == cube::CUBE_METRIC_POSTDERIVED )
        {
            setMetricType( 1 );
        }
        else
        if ( working_metric->get_type_of_metric() == cube::CUBE_METRIC_PREDERIVED_INCLUSIVE )
        {
            setMetricType( 2 );
        }
        else
        if ( working_metric->get_type_of_metric() == cube::CUBE_METRIC_PREDERIVED_EXCLUSIVE )
        {
            setMetricType( 3 );
        }
        else
        {
            setMetricType( 0 );
        }
        metric_type_selection->setEnabled( false );




        calculation_input->setText( working_metric->get_expression().c_str() );
        calculation_init_input->setText( working_metric->get_init_expression().c_str() );
        calculation_aggr_plus->setText( working_metric->get_aggr_plus_expression().c_str() );
        calculation_aggr_minus->setText( working_metric->get_aggr_minus_expression().c_str() );

        setCubePLExpression();
        setCubePLInitExpression();
        setCubePLAggrPlusExpression();
        setCubePLAggrMinusExpression();
    }






    top_layout->addLayout( exp_layout );

    QHBoxLayout* button_layout = new  QHBoxLayout();

    create_metric = new QPushButton( ( working_metric != NULL ) ? tr( "Edit &metric" ) : tr( "Create &metric" ) );
    connect( create_metric, SIGNAL( pressed() ), this, SLOT( createMetric() ) );
    button_layout->addWidget( create_metric );
    create_metric->setEnabled( unique_name_input->text() != "" );

    QPushButton* _close = new QPushButton( tr( "&Cancel" ) );
    connect( _close, SIGNAL( pressed() ), this, SLOT( close() ) );
    button_layout->addWidget( _close );



    top_layout->addLayout( button_layout );
    QWidget* horizontalLineWidget2 = new QWidget;
    horizontalLineWidget2->setFixedHeight( 2 );
    horizontalLineWidget2->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
    horizontalLineWidget2->setStyleSheet( QString( "background-color: #c0c0c0;" ) );
    top_layout->addWidget( horizontalLineWidget2  );

    share_metric = new QLabel( ""  );
    enableShareLink( false );
    top_layout->addWidget( share_metric );

    top_layout->addWidget( statusBar );

    setLayout( top_layout );
}

void
NewDerivatedMetricWidget::setMetricType(  int type )
{
    metric_type_selection->setCurrentIndex( type );
    calculations_container->setTabEnabled( 0, true );
    calculations_container->setTabEnabled( 1, true );
    calculations_container->setTabEnabled( 2, true );
    calculations_container->setTabEnabled( 3, true );
    switch ( type )
    {
        case 1:
            metric_type = cube::CUBE_METRIC_POSTDERIVED;
            calculations_container->setTabEnabled( 2, false );
            calculations_container->setTabEnabled( 3, false );
            break;
        case 2:
            metric_type = cube::CUBE_METRIC_PREDERIVED_INCLUSIVE;
            break;
        case 3:
            metric_type = cube::CUBE_METRIC_PREDERIVED_EXCLUSIVE;
            calculations_container->setTabEnabled( 3, false );
            break;
        default:
            metric_type = cube::CUBE_METRIC_SIMPLE; // just value to set the widget in unenabled state
            break;
    }
    if ( create_metric == NULL )
    {
        return;
    }
    create_metric->setEnabled(  ( metric_type == cube::CUBE_METRIC_POSTDERIVED || metric_type == cube::CUBE_METRIC_PREDERIVED_EXCLUSIVE || metric_type == cube::CUBE_METRIC_PREDERIVED_INCLUSIVE   )
                                && ( disp_name != "" )
                                && ( uniq_name != "" )
                                && ( calculation != "" )
                                && ( calculation_ok )
                                && ( calculation_init_ok )
                                && ( calculation_plus_ok )
                                && ( calculation_minus_ok )
                                );
}

void
NewDerivatedMetricWidget::setDisplayName(  const QString& new_disp_name )
{
    disp_name = new_disp_name.toStdString();
    if ( create_metric == NULL )
    {
        return;
    }
    create_metric->setEnabled(  ( metric_type == cube::CUBE_METRIC_POSTDERIVED || metric_type == cube::CUBE_METRIC_PREDERIVED_EXCLUSIVE || metric_type == cube::CUBE_METRIC_PREDERIVED_INCLUSIVE   )
                                && ( disp_name != "" )
                                && ( uniq_name != "" )
                                && ( calculation != "" )
                                && ( calculation_ok )
                                && ( calculation_init_ok )
                                && ( calculation_plus_ok )
                                && ( calculation_minus_ok )
                                );
}


void
NewDerivatedMetricWidget::setUniqName( const QString& new_uniq_name )
{
    uniq_name = new_uniq_name.toStdString();
    if ( create_metric == NULL )
    {
        return;
    }
    create_metric->setEnabled(  ( metric_type == cube::CUBE_METRIC_POSTDERIVED || metric_type == cube::CUBE_METRIC_PREDERIVED_EXCLUSIVE || metric_type == cube::CUBE_METRIC_PREDERIVED_INCLUSIVE   )
                                && ( disp_name != "" )
                                && ( uniq_name != "" )
                                && ( calculation != "" )
                                && ( calculation_ok )
                                && ( calculation_init_ok )
                                && ( calculation_plus_ok )
                                && ( calculation_minus_ok )
                                );
}


void
NewDerivatedMetricWidget::setUom( const QString& new_uom )
{
    uom = new_uom.toStdString();
}

void
NewDerivatedMetricWidget::setValue( const QString& new_value )
{
    val = new_value.toStdString();
}

void
NewDerivatedMetricWidget::setURL( const QString& new_url )
{
    url = new_url.toStdString();
}

void
NewDerivatedMetricWidget::setDescription( void )
{
    descr = description_input->toPlainText().toStdString();
}

void
NewDerivatedMetricWidget::setCubePLExpression()
{
    calculation = calculation_input->toPlainText().toStdString();

    std::string _to_test = std::string( "<cubepl>" ) + calculation + std::string( "</cubepl>" );

    std::string error_message = "";
    if ( cube->test_cubepl_expression( _to_test, error_message ) )
    {
        calculations_container->setTabIcon( 0, ( calculation == "" ) ? QIcon( ":images/source.png" ) : QIcon( ":images/syntax_ok.png" ) );
        error_highlighter->setErrorPlace( -1 );
        calculation_ok = true;
        statusBar->addLine( "Ok", Information );
    }
    else
    {
        QString error_string = QString::fromStdString( error_message );
        calculations_container->setTabIcon( 0, QIcon( ":images/syntax_error.png" ) );

        statusBar->addLine(  error_string, Error );

        QStringList list1        = error_string.split( ":" );
        QString     locations    = list1.at( 0 );
        QStringList list2        = locations.split( "." );
        QString     row          = list2.at( 0 );
        QString     columns      = ( list2.size() > 1 ) ? list2.at( 1 ) : row;
        QStringList list3        = columns.split( "-" );
        QString     column_start = list3.at( 0 );

        QString column_end = ( list3.size() > 1 ) ? list3.at( 1 ) : column_start;



//         int errorplace    = column_start.toInt();
        int errorplace_to = column_end.toInt();
        error_highlighter->setErrorPlace( errorplace_to );
        calculation_ok = false;
    }

    if ( create_metric == NULL )
    {
        return;
    }

    create_metric->setEnabled(  ( metric_type == cube::CUBE_METRIC_POSTDERIVED || metric_type == cube::CUBE_METRIC_PREDERIVED_EXCLUSIVE || metric_type == cube::CUBE_METRIC_PREDERIVED_INCLUSIVE   )
                                && ( disp_name != "" )
                                && ( uniq_name != "" )
                                && ( calculation != "" )
                                && ( calculation_ok )
                                && ( calculation_init_ok )
                                && ( calculation_plus_ok )
                                && ( calculation_minus_ok )
                                );
    enableShareLink( create_metric->isEnabled() );
}


void
NewDerivatedMetricWidget::setCubePLInitExpression()
{
    init_calculation = calculation_init_input->toPlainText().toStdString();

    std::string _to_test = std::string( "<cubepl>" ) + init_calculation + std::string( "</cubepl>" );

    std::string error_message = "";
    if ( cube->test_cubepl_expression( _to_test, error_message ) )
    {
        calculations_container->setTabIcon( 1, ( init_calculation == "" ) ? QIcon( ":images/source.png" ) : QIcon( ":images/syntax_ok.png" ) );
        error_init_highlighter->setErrorPlace( -1 );
        calculation_init_ok = true;
        statusBar->addLine( "Ok", Information );
    }
    else
    {
        QString error_string = QString::fromStdString( error_message );
        calculations_container->setTabIcon( 1, QIcon( ":images/syntax_error.png" ) );
        statusBar->addLine(  error_string, Error );

        QStringList list1        = error_string.split( ":" );
        QString     locations    = list1.at( 0 );
        QStringList list2        = locations.split( "." );
        QString     row          = list2.at( 0 );
        QString     columns      = ( list2.size() > 1 ) ? list2.at( 1 ) : row;
        QStringList list3        = columns.split( "-" );
        QString     column_start = list3.at( 0 );

        QString column_end = ( list3.size() > 1 ) ? list3.at( 1 ) : column_start;



//         int errorplace    = column_start.toInt();
        int errorplace_to = column_end.toInt();
        error_init_highlighter->setErrorPlace( errorplace_to );
        calculation_init_ok = false;
    }

    if ( create_metric == NULL )
    {
        return;
    }


    create_metric->setEnabled(  ( metric_type == cube::CUBE_METRIC_POSTDERIVED || metric_type == cube::CUBE_METRIC_PREDERIVED_EXCLUSIVE || metric_type == cube::CUBE_METRIC_PREDERIVED_INCLUSIVE   )
                                && ( disp_name != "" )
                                && ( uniq_name != "" )
                                && ( calculation != "" )
                                && ( calculation_ok )
                                && ( calculation_init_ok )
                                && ( calculation_plus_ok )
                                && ( calculation_minus_ok )
                                );
    enableShareLink( create_metric->isEnabled() );
}





void
NewDerivatedMetricWidget::setCubePLAggrPlusExpression( void )
{
    calculation_plus = calculation_aggr_plus->toPlainText().toStdString();

    std::string _to_test = std::string( "<cubepl>" ) + calculation_plus + std::string( "</cubepl>" );

    std::string error_message = "";
    if ( cube->test_cubepl_expression( _to_test, error_message ) )
    {
        calculations_container->setTabIcon( 2, ( calculation_plus == "" ) ? QIcon( ":images/source.png" ) : QIcon( ":images/syntax_ok.png" ) );
        error_highlighter->setErrorPlace( -1 );
        calculation_plus_ok = true;
        statusBar->addLine( "Ok", Information );
    }
    else
    {
        QString error_string = QString::fromStdString( error_message );
        calculations_container->setTabIcon( 2, QIcon( ":images/syntax_error.png" ) );
        statusBar->addLine(  error_string, Error );

        QStringList list1        = error_string.split( ":" );
        QString     locations    = list1.at( 0 );
        QStringList list2        = locations.split( "." );
        QString     row          = list2.at( 0 );
        QString     columns      = ( list2.size() > 1 ) ? list2.at( 1 ) : row;
        QStringList list3        = columns.split( "-" );
        QString     column_start = list3.at( 0 );

        QString column_end = ( list3.size() > 1 ) ? list3.at( 1 ) : column_start;



//         int errorplace    = column_start.toInt();
        int errorplace_to = column_end.toInt();
        error_highlighter->setErrorPlace( errorplace_to );
        calculation_plus_ok = false;
    }
    if ( create_metric == NULL )
    {
        return;
    }

    create_metric->setEnabled(  ( metric_type == cube::CUBE_METRIC_POSTDERIVED || metric_type == cube::CUBE_METRIC_PREDERIVED_EXCLUSIVE || metric_type == cube::CUBE_METRIC_PREDERIVED_INCLUSIVE   )
                                && ( disp_name != "" )
                                && ( uniq_name != "" )
                                && ( calculation != "" )
                                && ( calculation_ok )
                                && ( calculation_init_ok )
                                && ( calculation_plus_ok )
                                && ( calculation_minus_ok )
                                );
    enableShareLink( create_metric->isEnabled() );
}









void
NewDerivatedMetricWidget::setCubePLAggrMinusExpression( void )
{
    calculation_minus = calculation_aggr_plus->toPlainText().toStdString();

    std::string _to_test = std::string( "<cubepl>" ) + calculation_minus + std::string( "</cubepl>" );

    std::string error_message = "";
    if ( cube->test_cubepl_expression( _to_test, error_message ) )
    {
        calculations_container->setTabIcon( 3, ( calculation_minus == "" ) ? QIcon( ":images/source.png" ) : QIcon( ":images/syntax_ok.png" ) );
        error_highlighter->setErrorPlace( -1 );
        calculation_minus_ok = true;
        statusBar->addLine( "Ok", Information );
    }
    else
    {
        QString error_string = QString::fromStdString( error_message );
        calculations_container->setTabIcon( 3, QIcon( ":images/syntax_error.png" ) );

        statusBar->addLine(  error_string, Error );

        QStringList list1        = error_string.split( ":" );
        QString     locations    = list1.at( 0 );
        QStringList list2        = locations.split( "." );
        QString     row          = list2.at( 0 );
        QString     columns      = ( list2.size() > 1 ) ? list2.at( 1 ) : row;
        QStringList list3        = columns.split( "-" );
        QString     column_start = list3.at( 0 );

        QString column_end = ( list3.size() > 1 ) ? list3.at( 1 ) : column_start;



//         int errorplace    = column_start.toInt();
        int errorplace_to = column_end.toInt();
        error_highlighter->setErrorPlace( errorplace_to );
        calculation_minus_ok = false;
    }
    if ( create_metric == NULL )
    {
        return;
    }

    create_metric->setEnabled(  ( metric_type == cube::CUBE_METRIC_POSTDERIVED || metric_type == cube::CUBE_METRIC_PREDERIVED_EXCLUSIVE || metric_type == cube::CUBE_METRIC_PREDERIVED_INCLUSIVE   )
                                && ( disp_name != "" )
                                && ( uniq_name != "" )
                                && ( calculation != "" )
                                && ( calculation_ok )
                                && ( calculation_init_ok )
                                && ( calculation_plus_ok )
                                && ( calculation_minus_ok )
                                );
    enableShareLink( create_metric->isEnabled() );
}














void
NewDerivatedMetricWidget::createMetric( void )
{
    if ( working_metric != NULL ) // editing an existing metric
    {
        cubeplparser::CubePLDriver* driver = cube->get_cubepl_driver();
        std::string                 expression;
        std::string                 cubepl_program;
        std::string                 derived_metric_error;
        cube::GeneralEvaluation*    formula            = NULL;
        cube::GeneralEvaluation*    init_formula       = NULL;
        cube::GeneralEvaluation*    aggr_plus_formula  = NULL;
        cube::GeneralEvaluation*    aggr_minus_formula = NULL;

        if ( calculation != "" )
        {
            expression     = calculation;
            cubepl_program = string( "<cubepl>" ) + expression + string( "</cubepl>" );

            stringstream strin( cubepl_program );
            formula = driver->compile( &strin, &cerr );

            if ( formula != NULL )
            {
                working_metric->setEvaluation( formula );
                working_metric->set_expression( expression );
            }
        }
        if ( init_calculation != "" )
        {
            expression     = init_calculation;
            cubepl_program = string( "<cubepl>" ) + expression + string( "</cubepl>" );

            stringstream strin( cubepl_program );
            init_formula = driver->compile( &strin, &cerr );

            if ( init_formula != NULL )
            {
                working_metric->setInitEvaluation( init_formula );
                working_metric->set_init_expression( expression );
            }
        }

        if ( working_metric->get_type_of_metric() == cube::CUBE_METRIC_PREDERIVED_EXCLUSIVE || working_metric->get_type_of_metric() == cube::CUBE_METRIC_PREDERIVED_INCLUSIVE )
        {
            if ( calculation_plus != "" )
            {
                expression     = calculation_plus;
                cubepl_program = string( "<cubepl>" ) + expression + string( "</cubepl>" );

                stringstream strin( cubepl_program );
                aggr_plus_formula = driver->compile( &strin, &cerr );
                if ( aggr_plus_formula != NULL )
                {
                    working_metric->setAggrPlusEvaluation( aggr_plus_formula );
                    working_metric->set_aggr_plus_expression( expression );
                }
            }
            if ( working_metric->get_type_of_metric() == cube::CUBE_METRIC_PREDERIVED_INCLUSIVE )
            {
                if ( calculation_minus != "" )
                {
                    expression     = calculation_minus;
                    cubepl_program = string( "<cubepl>" ) + expression + string( "</cubepl>" );

                    stringstream strin( cubepl_program );
                    aggr_minus_formula = driver->compile( &strin, &cerr );

                    if ( aggr_minus_formula != NULL )
                    {
                        working_metric->setAggrMinusEvaluation( aggr_minus_formula );
                        working_metric->set_aggr_minus_expression( expression );
                    }
                }
            }
        }
    }
    else
    {
        working_metric = cube->def_met(
            disp_name, uniq_name,
            ( parent_metric == NULL ) ? "DOUBLE" :  parent_metric->get_dtype(),
            uom,
            val,
            url,
            descr,
            parent_metric,
            metric_type,
            calculation,
            init_calculation,
            calculation_plus,
            calculation_minus );
        if ( working_metric == NULL )
        {
            statusBar->addLine(  tr( "Cannot create metric. Please check the type of parent metric." ), Error );
            return;
        }
        statusBar->addLine( "Ok", Information );
    }
    close();
}




void
NewDerivatedMetricWidget::showCubePLHelp( void )
{
    if ( help_browser == NULL )
    {
        help_browser = new HelpBrowser( tr( "Help" ), this );
    }
    help_browser->showHelpAsString( HelpCollection::getHelpText( CUBEPL_HELP ) );
    QApplication::restoreOverrideCursor();
}

void
NewDerivatedMetricWidget::showKindsOfDerivedMetricsHelp( void )
{
    if ( help_browser == NULL )
    {
        help_browser = new HelpBrowser( tr( "Help" ), this );
    }
    help_browser->showHelpAsString( HelpCollection::getHelpText( DERIVED_METRIC_HELP ) );
    QApplication::restoreOverrideCursor();
}


void
NewDerivatedMetricWidget::showMetricExpressionHelp( void )
{
    if ( help_browser == NULL )
    {
        help_browser = new HelpBrowser( tr( "Help" ), this );
    }
    int top_tab = calculations_container->currentIndex();
    switch ( top_tab )
    {
        default:
        case 0: // expression
            help_browser->showHelpAsString( HelpCollection::getHelpText( DERIVED_METRIC_EXPRESSION_HELP ) );
            break;
        case 1: // init expression
            help_browser->showHelpAsString( HelpCollection::getHelpText( DERIVED_METRIC_INIT_EXPRESSION_HELP ) );
            break;
        case 2: // plus expression
            help_browser->showHelpAsString( HelpCollection::getHelpText( DERIVED_METRIC_AGGR_PLUS_EXPRESSION_HELP ) );
            break;
        case 3: // minus expression
            help_browser->showHelpAsString( HelpCollection::getHelpText( DERIVED_METRIC_AGGR_MINUS_EXPRESSION_HELP ) );
            break;
    }
    QApplication::restoreOverrideCursor();
}

void
NewDerivatedMetricWidget::dragEnterEvent( QDragEnterEvent* event )
{
    if ( event->mimeData()->hasUrls() )
    {
        event->acceptProposedAction();
    }
}


void
NewDerivatedMetricWidget::dropEvent( QDropEvent* event )
{
    QList<QUrl>           _urls = event->mimeData()->urls();
    QList<QUrl>::iterator _iter = _urls.begin();
    QUrl                  _file = *_iter;
    event->acceptProposedAction();
    fillTheFormFromUrl( _file );
}



void
NewDerivatedMetricWidget::selectFileForFillingForm()
{
    QString _file = QFileDialog::getOpenFileName( this, tr( "Open File with definition of a derived metric" ),
                                                  QDir::currentPath(),
                                                  tr( "CubePL files (*.dm);;Text files (*.txt)" ) );
    if ( _file != "" )
    {
        QUrl _url( _file );
        fillTheFormFromUrl( _url );
    }
}

void
NewDerivatedMetricWidget::fillTheFormFromUrl( QUrl& _url )
{
    QFile _file( _url.path() );
    if ( !_file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        return;
    }

    QTextStream in( &_file );
    fillTheForm( in );
}


void
NewDerivatedMetricWidget::fillTheFormFromClipboard( void )
{
    const QClipboard* clipboard = QApplication::clipboard();
    const QMimeData*  mimeData  = clipboard->mimeData();

    if ( mimeData->hasText() )
    {
        QString     text = mimeData->text();
        QTextStream in( &text );
        fillTheForm( in );
    }
}




void
NewDerivatedMetricWidget::fillTheForm( QTextStream& in )
{
    enum TargetString
    {
        DM_PARSER_COMMENT               = -1,
        DM_PARSER_TYPE                  = 0,
        DM_PARSER_DISPLAY_NAME          = 1,
        DM_PARSER_UNIQUE_NAME           = 2,
        DM_PARSER_UOM                   = 3,
        DM_PARSER_URL                   = 4,
        DM_PARSER_DESCRIPTION           = 5,
        DM_PARSER_EXPRESSION            = 6,
        DM_PARSER_INIT_EXPRESSION       = 7,
        DM_PARSER_AGGR_PLUS_EXPRESSION  = 8,
        DM_PARSER_AGGR_MINUS_EXPRESSION = 9
    };

    TargetString switcher = DM_PARSER_COMMENT;

    QRegExp dmetric_commentar( "^\\s*#" );
    QRegExp dmetric_type( "^\\s*metric type\\s*:" );
    QRegExp dmetric_display_name( "^\\s*display name\\s*:" );
    QRegExp dmetric_unique_name( "^\\s*unique name\\s*:" );
    QRegExp dmetric_uom( "^\\s*uom\\s*:" );
    QRegExp dmetric_url( "^\\s*url\\s*:" );
    QRegExp dmetric_description( "^\\s*description\\s*:" );
    QRegExp dmetric_expression( "^\\s*cubepl\\s+expression\\s*:" );
    QRegExp dmetric_init_expression( "^\\s*cubepl\\s+init\\s+expression\\s*:" );
    QRegExp dmetric_aggr_plus_expression( "^\\s*cubepl\\s+plus\\s+expression\\s*:" );
    QRegExp dmetric_aggr_minus_expression( "^\\s*cubepl\\s+minus\\s+expression\\s*:" );

    QString metric_type;
    QString metric_display_name;
    QString metric_unique_name;
    QString metric_uom;
    QString metric_url;
    QString metric_description;
    QString metric_cubepl_expression;
    QString metric_cubepl_init_expression;
    QString metric_cubepl_plus_expression;
    QString metric_cubepl_minus_expression;

    QString parts[ 10 ];

    while ( !in.atEnd() )
    {
        QString line = in.readLine().trimmed();

        if ( line.indexOf( dmetric_commentar ) != -1 )
        {
            continue;
        }
        else if ( line.indexOf( dmetric_type ) != -1 )
        {
            switcher = DM_PARSER_TYPE;
            line.remove( dmetric_type );
        }
        else if ( line.indexOf( dmetric_display_name ) != -1 )
        {
            switcher = DM_PARSER_DISPLAY_NAME;
            line.remove( dmetric_display_name );
        }
        else if ( line.indexOf( dmetric_unique_name ) != -1 )
        {
            switcher = DM_PARSER_UNIQUE_NAME;
            line.remove( dmetric_unique_name );
        }
        else if ( line.indexOf( dmetric_uom ) != -1 )
        {
            switcher = DM_PARSER_UOM;
            line.remove( dmetric_uom );
        }
        else if ( line.indexOf( dmetric_url ) != -1 )
        {
            switcher = DM_PARSER_URL;
            line.remove( dmetric_url );
        }
        else if ( line.indexOf( dmetric_description ) != -1 )
        {
            switcher = DM_PARSER_DESCRIPTION;
            line.remove( dmetric_description );
        }
        else if ( line.indexOf( dmetric_expression ) != -1 )
        {
            switcher = DM_PARSER_EXPRESSION;
            line.remove( dmetric_expression );
        }
        else if ( line.indexOf( dmetric_init_expression ) != -1 )
        {
            switcher = DM_PARSER_INIT_EXPRESSION;
            line.remove( dmetric_init_expression );
        }
        else if ( line.indexOf( dmetric_aggr_plus_expression ) != -1 )
        {
            switcher = DM_PARSER_AGGR_PLUS_EXPRESSION;
            line.remove( dmetric_aggr_plus_expression );
        }
        else if ( line.indexOf( dmetric_aggr_minus_expression ) != -1 )
        {
            switcher = DM_PARSER_AGGR_MINUS_EXPRESSION;
            line.remove( dmetric_aggr_minus_expression );
        }

        if ( switcher != DM_PARSER_COMMENT )
        {
            parts[ switcher ].append( line ).append( "\n" );
        }
    }

    if ( working_metric == NULL )
    {
        if ( parts[ DM_PARSER_TYPE ].trimmed().toLower() == "postderived" )
        {
            metric_type_selection->setCurrentIndex( 1 );
        }
        else
        if ( parts[ DM_PARSER_TYPE ].trimmed().toLower() == "prederived_inclusive" )
        {
            metric_type_selection->setCurrentIndex( 2 );
        }
        else
        if ( parts[ DM_PARSER_TYPE ].trimmed().toLower() == "prederived_exclusive" )
        {
            metric_type_selection->setCurrentIndex( 3 );
        }
        else
        {
            metric_type_selection->setCurrentIndex( 0 );
        }
        display_name_input->setText( parts[ DM_PARSER_DISPLAY_NAME ].trimmed() );
        unique_name_input->setText( parts[ DM_PARSER_UNIQUE_NAME ].trimmed() );
        uom_input->setText( parts[ DM_PARSER_UOM ].trimmed() );
        url_input->setText( parts[ DM_PARSER_URL ].trimmed() );
        description_input->setText( parts[ DM_PARSER_DESCRIPTION ].trimmed() );
    }
    calculation_input->setText( parts[ DM_PARSER_EXPRESSION ].trimmed() );
    calculation_init_input->setText( parts[ DM_PARSER_INIT_EXPRESSION ].trimmed() );
    calculation_aggr_plus->setText( parts[ DM_PARSER_AGGR_PLUS_EXPRESSION ].trimmed() );
    calculation_aggr_minus->setText( parts[ DM_PARSER_AGGR_MINUS_EXPRESSION ].trimmed() );
}





void
NewDerivatedMetricWidget::createMetricFromFile( void )
{
    selectFileForFillingForm();
}


void
NewDerivatedMetricWidget::saveMetricToFile( void )
{
    QString _file = QFileDialog::getSaveFileName( this, tr( "Save Metric Definition into a file..." ),
                                                  QDir::currentPath(),
                                                  tr( "CubePL files(*.dm );; Text Files (*.txt)" ) );
    if ( _file != "" )
    {
        QFile file( _file + ".dm" );
        if ( file.open( QIODevice::WriteOnly ) )
        {
            QTextStream _out( &file );
            switch ( metric_type_selection->currentIndex() )
            {
                default:
                case 0:
                    break;
                case 1:
                    _out << "metric type: postderived" << endl << endl;
                    break;
                case 2:
                    _out << "metric type: prederived_inclusive" << endl << endl;
                    break;
                case 3:
                    _out << "metric type: prederived_exclusive" << endl << endl;
                    break;
            }
            _out << "display name: " << display_name_input->text().toUtf8().data() << endl << endl;
            _out << "unique name: " << unique_name_input->text().toUtf8().data() << endl << endl;
            _out << "uom: " << uom_input->text().toUtf8().data() << endl << endl;
            _out << "url: " << url_input->text().toUtf8().data() << endl << endl;
            _out << "description: " << description_input->toPlainText().toUtf8().data() << endl << endl;
            _out << "cubepl expression: " << calculation_input->toPlainText().toUtf8().data() << endl << endl;
            _out << "cubepl init expression: " << calculation_init_input->toPlainText().toUtf8().data() << endl << endl;
            if ( metric_type_selection->currentIndex() > 1 )
            {
                _out << "cubepl plus expression: " << calculation_aggr_plus->toPlainText().toUtf8().data() << endl << endl;
            }
            if ( metric_type_selection->currentIndex() == 2 )
            {
                _out << "cubepl minus expression: " << calculation_aggr_minus->toPlainText().toUtf8().data() << endl << endl;
            }
        }
    }
}

void
NewDerivatedMetricWidget::createMetricFromClipboard( void )
{
    fillTheFormFromClipboard();
}


void
NewDerivatedMetricWidget::selectMetricFromLibrary(  int type )
{
    QTextStream in( &DerivedMetricsCollection::getDerivedMetricText( type ) );
    fillTheForm( in );
}



void
NewDerivatedMetricWidget::enableShareLink( bool flag )
{
    QString _caption( tr( "Share this metric with SCALASCA group" ) );
    if ( !flag )
    {
        share_metric->setText( _caption );
        share_metric->setOpenExternalLinks( false  );
    }
    else
    {
        QString _text_intro( tr( "Dear Scalasca Team, \n\n"
                                 "this metric is very useful in my performance analysis and I would like to suggest to include it into the library of Cube:\n\n" ) );
        QString _metric = packDataToString();
        QString _text_closure( tr( "\n\n\n\n\n Sincerely," ) );
        share_metric->setText( tr( "<a href=\"mailto:scalasca@fz-juelich.de?subject=Please add this derived metric to your library&body=" ).
                               append( _text_intro ).
                               append( _metric ).
                               append( _text_closure ).
                               append( "\">" ).
                               append( _caption ).
                               append( "</a> " ) );
        share_metric->setOpenExternalLinks( true );
    }
}


QString
NewDerivatedMetricWidget::packDataToString( void )
{
    QString _to_return( tr( "Metric type:" ).append( metric_type_selection->currentText().append( "\n\n" ).append(
                                                         tr( "Display name:" ).append( display_name_input->text().append( "\n\n" ).append(
                                                                                           tr( "Unique name:" ).append( unique_name_input->text().append( "\n\n" ).append(
                                                                                                                            tr( "UoM:" ).append( uom_input->text().append( "\n\n" ).append(
                                                                                                                                                     tr( "URL:" ).append( url_input->text().append( "\n\n" ).append(
                                                                                                                                                                              tr( "Description:" ).append( description_input->toPlainText().append( "\n\n" ).append(
                                                                                                                                                                                                               tr( "CubePL Expression:" ).append( calculation_input->toPlainText().append( "\n\n" ).append(
                                                                                                                                                                                                                                                      tr( "CubePl Init Expression:" ).append( calculation_init_input->toPlainText().append( "\n\n" ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) );
    _to_return.replace( "\"", "'" );
    return _to_return;
}


QStringList
NewDerivatedMetricWidget::getCompletionList()
{
    QStringList           list;
    vector<cube::Metric*> _metrics = cube->get_metv();
    for ( vector<cube::Metric*>::iterator m_iter = _metrics.begin(); m_iter != _metrics.end(); ++m_iter )
    {
        list.append( "metric::" + QString::fromStdString( ( *m_iter )->get_uniq_name() ) );
        list.append( "metric::context::" + QString::fromStdString( ( *m_iter )->get_uniq_name() ) );
        list.append( "metric::fixed::" + QString::fromStdString( ( *m_iter )->get_uniq_name() ) );
        list.append( "metric::call::" + QString::fromStdString( ( *m_iter )->get_uniq_name() ) );
        list.append( "metric::set::" + QString::fromStdString( ( *m_iter )->get_uniq_name() ) );
        list.append( "metric::get::" + QString::fromStdString( ( *m_iter )->get_uniq_name() ) );
    }
    vector<cube::Metric*> _ghost_metrics = cube->get_ghost_metv();
    for ( vector<cube::Metric*>::iterator m_iter = _ghost_metrics.begin(); m_iter != _ghost_metrics.end(); ++m_iter )
    {
        list.append( "metric::" + QString::fromStdString( ( *m_iter )->get_uniq_name() ) );
        list.append( "metric::context::" + QString::fromStdString( ( *m_iter )->get_uniq_name() ) );
        list.append( "metric::fixed::" + QString::fromStdString( ( *m_iter )->get_uniq_name() ) );
        list.append( "metric::call::" + QString::fromStdString( ( *m_iter )->get_uniq_name() ) );
        list.append( "metric::set::" + QString::fromStdString( ( *m_iter )->get_uniq_name() ) );
        list.append( "metric::get::" + QString::fromStdString( ( *m_iter )->get_uniq_name() ) );
    }

    list.append( "${cube::#mirrors" );
    list.append( "${cube::#metrics" );
    list.append( "${cube::#root::metrics" );
    list.append( "${cube::#regions" );
    list.append( "${cube::#callpaths" );
    list.append( "${cube::#root::callpaths" );
    list.append( "${cube::#locations" );
    list.append( "${cube::#locationgroups" );
    list.append( "${cube::#stns" );
    list.append( "${cube::#rootstns" );
    list.append( "${cube::filename" );
    list.append( "${cube::metric::uniq::name" );
    list.append( "${cube::metric::disp::name" );
    list.append( "${cube::metric::url" );
    list.append( "${cube::metric::description" );
    list.append( "${cube::metric::dtype" );
    list.append( "${cube::metric::uom" );
    list.append( "${cube::metric::expression" );
    list.append( "${cube::metric::initexpression" );
    list.append( "${cube::metric::#children" );
    list.append( "${cube::metric::parent::id" );
    list.append( "${cube::metric::children" );
    list.append( "${cube::metric::enumeration" );
    list.append( "${cube::callpath::mod" );
    list.append( "${cube::callpath::line" );
    list.append( "${cube::callpath::#children" );
    list.append( "${cube::callpath::children" );
    list.append( "${cube::callpath::calleeid" );
    list.append( "${cube::callpath::parent::id" );
    list.append( "${cube::callpath::enumeration" );
    list.append( "${cube::region::name" );
    list.append( "${cube::region::mangled::name" );
    list.append( "${cube::region::paradigm" );
    list.append( "${cube::region::role" );
    list.append( "${cube::region::url" );
    list.append( "${cube::region::description" );
    list.append( "${cube::region::mod" );
    list.append( "${cube::region::begin::line" );
    list.append( "${cube::region::end::line" );
    list.append( "${cube::stn::name" );
    list.append( "${cube::stn::description" );
    list.append( "${cube::stn::class" );
    list.append( "${cube::stn::#children" );
    list.append( "${cube::stn::children" );
    list.append( "${cube::stn::#locationgroups" );
    list.append( "${cube::stn::locationgroups" );
    list.append( "${cube::stn::parent::id" );
    list.append( "${cube::locationgroup::name" );
    list.append( "${cube::locationgroup::parent::id" );
    list.append( "${cube::locationgroup::rank" );
    list.append( "${cube::locationgroup::type" );
    list.append( "${cube::locationgroup::void" );
    list.append( "${cube::locationgroup::#locations" );
    list.append( "${cube::locationgroup::locations" );
    list.append( "${cube::location::name" );
    list.append( "${cube::location::type" );
    list.append( "${cube::location::parent::id" );
    list.append( "${cube::location::rank" );
    list.append( "${cube::location::void" );
    list.append( "${cube::#locations::void" );
    list.append( "${cube::#locations::nonvoid" );
    list.append( "${cube::#locationgroups::void" );
    list.append( "${cube::#locationgroups::nonvoid" );
    list.append( "${calculation::metric::id" );
    list.append( "${calculation::callpath::id" );
    list.append( "${calculation::region::id" );
    list.append( "${calculation::sysres::id" );
    list.append( "${calculation::sysres::kind" );
    return list;
}
