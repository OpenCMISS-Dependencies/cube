##*************************************************************************##
##  CUBE        http://www.scalasca.org/                                   ##
##*************************************************************************##
##  Copyright (c) 1998-2016                                                ##
##  Forschungszentrum Juelich GmbH, Juelich Supercomputing Centre          ##
##                                                                         ##
##  Copyright (c) 2009-2015                                                ##
##  German Research School for Simulation Sciences GmbH,                   ##
##  Laboratory for Parallel Programming                                    ##
##                                                                         ##
##  This software may be modified and distributed under the terms of       ##
##  a BSD-style license.  See the COPYING file in the package base         ##
##  directory for details.                                                 ##
##*************************************************************************##


CONFIG	+= qt thread rtti exceptions warn_on  release
TEMPLATE = lib
TARGET = 
CUBE +=
CUBE_SRC = ../../../src/cube
GUI_SRC = ../../../src/GUI-qt
DEPENDPATH += $${GUI_SRC} $${GUI_SRC}/display
INCLUDEPATH += $${GUI_SRC} $${GUI_SRC}/display $${CUBE_SRC}/../../vpath/src/ $${CUBE_SRC}/../ \
     $${CUBE_SRC}/include/service\
	 $${CUBE_SRC}/include/service/cubelayout \
	 $${CUBE_SRC}/include/service/cubelayout/layout \
	 $${CUBE_SRC}/include/service/cubelayout/readers \
	 $${CUBE_SRC}/include/dimensions/metric \
	 $${CUBE_SRC}/include/dimensions/metric/strategies \
	 $${CUBE_SRC}/include/dimensions/metric/data \
	 $${CUBE_SRC}/include/dimensions/metric/cache \
	 $${CUBE_SRC}/include/dimensions/metric/data/rows \
	 $${CUBE_SRC}/include/dimensions/metric/matrix \
	 $${CUBE_SRC}/include/dimensions/metric/index \
	 $${CUBE_SRC}/include/dimensions/metric/value \
	 $${CUBE_SRC}/include/dimensions/metric/value/trafo \
	 $${CUBE_SRC}/include/dimensions/metric/value/trafo/single_value \
	 $${CUBE_SRC}/include/dimensions/calltree \
	 $${CUBE_SRC}/include/dimensions/system \
	 $${CUBE_SRC}/include/dimensions \
	 $${CUBE_SRC}/include/syntax \
	 $${CUBE_SRC}/include/syntax/cubepl/evaluators \
	 $${CUBE_SRC}/include/syntax/cubepl/evaluators/nullary \
	 $${CUBE_SRC}/include/syntax/cubepl/evaluators/nullary/statements \
	 $${CUBE_SRC}/include/syntax/cubepl/evaluators/unary \
	 $${CUBE_SRC}/include/syntax/cubepl/evaluators/unary/functions/single_argument \
	 $${CUBE_SRC}/include/syntax/cubepl/evaluators/unary/functions/two_arguments \
	 $${CUBE_SRC}/include/syntax/cubepl/evaluators/unary/functions/lambda_calcul \
	 $${CUBE_SRC}/include/syntax/cubepl/evaluators/binary \
	 $${CUBE_SRC}/include/syntax/cubepl \
	 $${CUBE_SRC}/include/derivated \
	 $${CUBE_SRC}/include/topologies \
	 $${CUBE_SRC}/include/ \
	 C:/Work/regex-2.7-bin/include 
	 
LIBS += -L../cube/release -lcube 
RESOURCES = resources.qrc

DEFINES+= CUBE_PREFIX=\\\"unknown\\\" 
DEFINES+= LIBDIR=\\\".\\\"
DEFINES+= NOCROSS_BUILD
DEFINES+= BACKEND_BUILD_NOMPI
DEFINES+= FRONTEND_CUBE_COMPRESSED_READONLY=yes
DEFINES+= __MINGW32__

QMAKE_CXXFLAGS=-fexceptions

QT += network

# Input{
HEADERS += \
	$${GUI_SRC}/display/SettingsToolBar.h \
	$${GUI_SRC}/display/Globals.h \
	$${GUI_SRC}/display/DimensionOrderDialog.h \
	$${GUI_SRC}/display/TreeModel.h \
	$${GUI_SRC}/display/TreeModelProxy.h \
	$${GUI_SRC}/display/ColorScale.h \
	$${GUI_SRC}/display/BoxPlot.h \
	$${GUI_SRC}/display/Coloring.h \
	$${GUI_SRC}/display/Constants.h \
	$${GUI_SRC}/display/CubePlugin.h \
	$${GUI_SRC}/display/DownloadThread.h \
	$${GUI_SRC}/display/Environment.h \
	$${GUI_SRC}/display/FontWidget.h \
	$${GUI_SRC}/display/HelpBrowser.h \
	$${GUI_SRC}/display/HtmlWidget.h \
	$${GUI_SRC}/display/MainWidget.h \
	$${GUI_SRC}/display/StatusBar.h \
	$${GUI_SRC}/display/InfoWidget.h \
	$${GUI_SRC}/display/PrecisionWidget.h \
	$${GUI_SRC}/display/Pretty.h \
	$${GUI_SRC}/display/Settings.h \
	$${GUI_SRC}/display/AggregatedTreeItem.h \
	$${GUI_SRC}/display/TreeItem.h \
	$${GUI_SRC}/display/ScrollArea.h \
	$${GUI_SRC}/display/DerivedMetricEditor.h \
	$${GUI_SRC}/display/TableWidget.h \
	$${GUI_SRC}/display/StatisticalInformation.h \
	$${GUI_SRC}/display/TabWidget.h \
	$${GUI_SRC}/display/PluginList.h \
	$${GUI_SRC}/display/TabInterface.h \
	$${GUI_SRC}/display/ValueWidget.h \
	$${GUI_SRC}/display/NewDerivatedMetricWidget.h   \
	$${GUI_SRC}/display/CubePLSyntaxHighlighter.h   \
	$${GUI_SRC}/display/VersionCheckWidget.h \
	$${GUI_SRC}/display/PluginServices.h \
	$${GUI_SRC}/display/ContextFreeServices.h \
	$${GUI_SRC}/display/PluginManager.h \
	$${GUI_SRC}/display/PluginSettingDialog.h \
	$${GUI_SRC}/display/HtmlHelpCollection.h \
	$${GUI_SRC}/display/HmiInstrumentation.h \
	$${GUI_SRC}/display/DerivedMetricsCollection.h \
	$${GUI_SRC}/display/Tree.h \
	$${GUI_SRC}/display/FlatTree.h \
	$${GUI_SRC}/display/SystemTree.h \
	$${GUI_SRC}/display/MetricTree.h \
	$${GUI_SRC}/display/CallTree.h \
	$${GUI_SRC}/display/TreeView.h \
	$${GUI_SRC}/display/TreeItemMarkerDialog.h \
	$${GUI_SRC}/display/MetricTreeView.h \
	$${GUI_SRC}/display/CallTreeView.h \
	$${GUI_SRC}/display/FlatTreeView.h \
	$${GUI_SRC}/display/SystemTreeView.h \
	$${GUI_SRC}/display/TabManager.h \
	$${GUI_SRC}/display/ColorMap.h 

SOURCES += \
	$${GUI_SRC}/display/SettingsToolBar.cpp \
	$${GUI_SRC}/display/ScrollArea.cpp \
	$${GUI_SRC}/display/DerivedMetricEditor.cpp \
	$${GUI_SRC}/display/BoxPlot.cpp \
	$${GUI_SRC}/display/Coloring.cpp \
	$${GUI_SRC}/display/DownloadThread.cpp \
	$${GUI_SRC}/display/FontWidget.cpp \
	$${GUI_SRC}/display/Environment.cpp \
	$${GUI_SRC}/display/HelpBrowser.cpp \
	$${GUI_SRC}/display/HtmlWidget.cpp \
	$${GUI_SRC}/display/MainWidget.cpp \
	$${GUI_SRC}/display/PrecisionWidget.cpp \
	$${GUI_SRC}/display/Pretty.cpp \
	$${GUI_SRC}/display/AggregatedTreeItem.cpp \
	$${GUI_SRC}/display/TreeItem.cpp \
	$${GUI_SRC}/display/Settings.cpp \
	$${GUI_SRC}/display/StatisticalInformation.cpp \
	$${GUI_SRC}/display/TableWidget.cpp \
	$${GUI_SRC}/display/TabWidget.cpp \
	$${GUI_SRC}/display/PluginList.cpp \
	$${GUI_SRC}/display/ValueWidget.cpp \
	$${GUI_SRC}/display/StatusBar.cpp \
	$${GUI_SRC}/display/InfoWidget.cpp \
	$${GUI_SRC}/display/NewDerivatedMetricWidget.cpp \
	$${GUI_SRC}/display/CubePLSyntaxHighlighter.cpp  \
	$${GUI_SRC}/display/VersionCheckWidget.cpp \
	$${GUI_SRC}/display/PluginServices.cpp \
	$${GUI_SRC}/display/ContextFreeServices.cpp \
	$${GUI_SRC}/display/PluginSettingDialog.cpp \
	$${GUI_SRC}/display/PluginManager.cpp \
	$${GUI_SRC}/display/HtmlHelpCollection.cpp  \
	$${GUI_SRC}/display/DerivedMetricsCollection.cpp \
	$${GUI_SRC}/display/DimensionOrderDialog.cpp \
	$${GUI_SRC}/display/ColorScale.cpp \
	$${GUI_SRC}/display/Globals.cpp \
	$${GUI_SRC}/display/TabManager.cpp \
	$${GUI_SRC}/display/TreeView.cpp \
	$${GUI_SRC}/display/TreeItemMarkerDialog.cpp \
	$${GUI_SRC}/display/MetricTreeView.cpp \
	$${GUI_SRC}/display/CallTreeView.cpp \
	$${GUI_SRC}/display/FlatTreeView.cpp \
	$${GUI_SRC}/display/SystemTreeView.cpp \
	$${GUI_SRC}/display/Tree.cpp \
	$${GUI_SRC}/display/TreeModel.cpp \
	$${GUI_SRC}/display/TreeModelProxy.cpp \
	$${GUI_SRC}/display/MetricTree.cpp \
	$${GUI_SRC}/display/CallTree.cpp \
	$${GUI_SRC}/display/FlatTree.cpp \
	$${GUI_SRC}/display/SystemTree.cpp \
	$${GUI_SRC}/display/TreeItemMarker.cpp
	
# Additional dependencies
# PRE_TARGETDEPS += $CUBE/lib/libcube4.a



# Hack for IBM AIX
aix-g++ {
  QMAKE_CXXFLAGS_RELEASE += -O0
}
aix-g++-64 {
  QMAKE_CXXFLAGS_RELEASE += -O0
}
