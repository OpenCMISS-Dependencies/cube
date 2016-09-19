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



#ifndef _NEW_DERIVATED_METRIC_WIDGET_H
#define _NEW_DERIVATED_METRIC_WIDGET_H


#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QString>
#include <QLabel>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QTextStream>
#include <QKeyEvent>
#include <QMenu>
#include "Cube.h"
#include "CubeMetric.h"
#include "CubePL1Driver.h"
#include "CubePLSyntaxHighlighter.h"
#include "HelpBrowser.h"
#include "DerivedMetricEditor.h"
#include "StatusBar.h"

/*-------------------------------------------------------------------------*/
/**
 * @file  NewDerivatedMetricWidget.h
 * @brief Declaration of the widget, used to create a new derivated metric
 *
 * This header file provides the declaration of the class HelpBrowser.
 */
/*-------------------------------------------------------------------------*/

class NewDerivatedMetricWidget : public QDialog
{
    Q_OBJECT

public:
    // / @name Constructors &amp; destructor
    // / @{

    NewDerivatedMetricWidget( cube::Cube*   cube,
                              cube::Metric* to_edit_metric = 0,
                              cube::Metric* met_parent = 0,
                              QWidget*      parent = 0 );

    // / @}
    ~NewDerivatedMetricWidget()
    {
        delete error_highlighter;
    };

public slots:
    // / @name Different slots to set up the different fields of metric:
    // / @{

    void
    selectMetricFromLibrary( int );

    void
    setMetricType( int );

    void
    setDisplayName( const QString& );
    void
    setUniqName( const QString& );

//     void setDType(QString&);
    void
    setUom( const QString& );
    void
    setValue( const QString& );
    void
    setURL( const QString& );
    void
    setDescription( void );
    void
    setCubePLExpression( void );
    void
    setCubePLInitExpression( void );

    void
    setCubePLAggrPlusExpression( void );

    void
    setCubePLAggrMinusExpression( void );

    void
    showCubePLHelp( void );

    void
    showKindsOfDerivedMetricsHelp( void );


    void
    showMetricExpressionHelp( void );

    QStringList
    getCompletionList();

    void
    createMetricFromFile( void );

    void
    saveMetricToFile( void );

    void
    createMetricFromClipboard( void );


    void
    createMetric();

    cube::Metric*
    get_created_metric( void )
    {
        return working_metric;
    };

    void
    selectFileForFillingForm( void );




    // / @}

protected:
    void
    dragEnterEvent( QDragEnterEvent* event );
    void
    dropEvent( QDropEvent* event );
    void
    fillTheFormFromUrl( QUrl& );

    void
    fillTheFormFromClipboard();

    void
    fillTheForm( QTextStream& );

    void
    enableShareLink( bool );

    QString
    packDataToString();

private:

    bool calculation_ok;
    bool calculation_init_ok;
    bool calculation_plus_ok;
    bool calculation_minus_ok;


    cube::Cube*        cube;
    cube::Metric*      parent_metric;
    cube::Metric*      working_metric;
    cube::TypeOfMetric metric_type;

    std::string uniq_name;
    std::string disp_name;
//     std::string dtype;
    std::string uom;
    std::string val;
    std::string url;
    std::string kind;
    std::string descr;
    std::string calculation;
    std::string init_calculation;
    std::string calculation_plus;
    std::string calculation_minus;


    QComboBox* metric_type_selection;
    QComboBox* metric_library_selection;
    QLineEdit* display_name_input;
    QLineEdit* unique_name_input;
//     QLineEdit * data_type_input;
    QLineEdit* uom_input;
    QLineEdit* value_input;
    QLineEdit* url_input;
    QTextEdit* description_input;

    QTabWidget* calculations_container;

    DerivedMetricEditor* calculation_input;
    DerivedMetricEditor* calculation_init_input;
    DerivedMetricEditor* calculation_aggr_plus;
    DerivedMetricEditor* calculation_aggr_minus;


    QPushButton* create_metric;
    QPushButton* _whatis_help;
    QPushButton* _metric_kinds_help;
    QPushButton* _metric_expt_help;
    QPushButton* _metric_lib_help;
    QPushButton* metric_from_file;
    QPushButton* metric_to_file;
    QPushButton* metric_from_clipboard;


    QLabel* share_metric;

    CubePLSyntaxHighlighter* error_highlighter;
    CubePLSyntaxHighlighter* error_init_highlighter;
    CubePLSyntaxHighlighter* error_aggr_plus_highlighter;
    CubePLSyntaxHighlighter* error_aggr_minus_highlighter;

    HelpBrowser* help_browser;
    StatusBar*   statusBar; // for error messages
};


#endif
