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


CONFIG	+= qt thread rtti exceptions warn_on  release plugin
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = lib
TARGET = 
CUBE +=
CUBE_SRC = ../../../src/cube
GUI_SRC = ../../../src/GUI-qt
DEPENDPATH += $${GUI_SRC} $${GUI_SRC}/display
INCLUDEPATH += $${GUI_SRC} $${GUI_SRC}/display  $${CUBE_SRC}/../\
        $${CUBE_SRC}/../../vpath/src/ \
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
        C:/Scalasca-Soft/Misc/regex-2.7-bin/include 
	 
LIBS += -L../cube/release -lcube -L../GUI-qt-core/release -lcube4gui -LC:/Scalasca-Soft/Misc/regex-2.7-bin/lib -lregex

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
$${GUI_SRC}/plugins/SourceCodeEditor/EditorPlugin.h \
$${GUI_SRC}/plugins/SourceCodeEditor/FortranSyntaxHighlighter.h \
$${GUI_SRC}/plugins/SourceCodeEditor/CPPSyntaxHighlighter.h 


SOURCES += \
$${GUI_SRC}/plugins/SourceCodeEditor/CPPSyntaxHighlighter.cpp \
$${GUI_SRC}/plugins/SourceCodeEditor/FortranSyntaxHighlighter.cpp \
$${GUI_SRC}/plugins/SourceCodeEditor/EditorPlugin.cpp 



# Hack for IBM AIX
aix-g++ {
  QMAKE_CXXFLAGS_RELEASE += -O0
}
aix-g++-64 {
  QMAKE_CXXFLAGS_RELEASE += -O0
}
