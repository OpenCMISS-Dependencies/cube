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




AC_DEFUN([AC_SCOREP_CONFIG_SELECTION_REPORT], [

XEND=$1


AFS_SUMMARY([Cube C Writer Library], [$WITH_C_WRITER
])

AS_IF([test "x$XEND" = "xfrontend" || test "x$CROSS_BUILD" != "xyes" ],[

AFS_SUMMARY([Cube C++ Library], [$WITH_CPP_LIB
])
AFS_SUMMARY([Cube Tools], [$WITH_TOOLS
])
AFS_SUMMARY([Cube GUI], [$WITH_GUI
])


AS_IF( [test "x$WITH_GUI" = "xyes" ], [
AFS_SUMMARY([  With qmake], [$QMAKE] )
AFS_SUMMARY([  With Qt version], [$QT_VERSION])
AFS_SUMMARY([  With Qt specs], [$QT_SPECS])
AFS_SUMMARY([  With Qt defines], [$QT_DEFINES])
AFS_SUMMARY([  With Qt include path], [$QT_INCPATH])
AFS_SUMMARY([  With Qt compiler flags], [$QT_CXXFLAGS])
AFS_SUMMARY([  With Qt linker flags], [$QT_LDFLAGS])
AFS_SUMMARY([  With Qt libraries], [$QT_LIBS])
AFS_SUMMARY([  With moc], [$MOC] )
AFS_SUMMARY([  With uic], [$UIC] )
AFS_SUMMARY([  With supported plugins], [] )
AFS_SUMMARY([    Plugin "Barplot"], [$WITH_GUI_PLUGIN_BARPLOT] )
AFS_SUMMARY([    Plugin "Heatmap"], [$WITH_GUI_PLUGIN_HEATMAP] )
AFS_SUMMARY([    Plugin "Launch" ], [$WITH_GUI_PLUGIN_LAUNCH] )
AFS_SUMMARY([    Plugin "Paraver" ], [$WITH_GUI_PLUGIN_PARAVER] )
AFS_SUMMARY([    Plugin "SourceEditor" ], [$WITH_GUI_PLUGIN_SOURCE_EDITOR] )
AFS_SUMMARY([    Plugin "Statistics" ], [$WITH_GUI_PLUGIN_STATISTICS] )
AFS_SUMMARY([    Plugin "Vampir" ], [$WITH_GUI_PLUGIN_VAMPIR] )
AFS_SUMMARY([    Plugin "AdvancedColorMaps" ], [$WITH_GUI_PLUGIN_ADVANCEDCM] )

AS_IF([ test "x$CUBE_DEBUG" == "x"], [
AFS_SUMMARY([  Disable qDebug() and qWarning()],[$CUBE_DEBUG_FLAGS])
],
[
AFS_SUMMARY([  Enabled qDebug() and qWarning()],[yes])

AS_IF( [test "x$HMI_INSTRUMENTATION" != "x" ], [
AFS_SUMMARY([ HMI Instrumentation ], [ yes ] )
])
])

])

])  # XEND


AS_IF( [test "x$WITH_CPP_LIB" = "xyes"  || test "x$WITH_C_WRITER" = "xyes"], [
AS_IF([ test "x$CROSS_BUILD" = "xyes" ],[

AS_IF([test "x$XEND" = "xfrontend" ], [
AFS_SUMMARY([Frontend zlib compression ], [$MSG_FRONTEND_COMPRESSION] )
AS_IF([test "x$MSG_FRONTEND_COMPRESSION" != "xno" ],[
AFS_SUMMARY([  zlib headers ], [$FRONTEND_LIBZ_HEADERS] )
AFS_SUMMARY([  zlib library ], [$FRONTEND_LIBZ_LD] )
])

AS_IF([ test "x$MSG_FRONTEND_COMPRESSION" = "xreadonly" ],[
AFS_SUMMARY([  Notice], [Front end part of cube (c++ cube library, tools and GUI) can read compressed cube files, but write only uncompressed cube files.])
])

AS_IF([ test "x$MSG_FRONTEND_COMPRESSION" = "xfull" ],[
AFS_SUMMARY([  Notice],[Front end part of cube (c++ cube library, tools and GUI) can read and write compressed cube files.])
])
])

AS_IF([test "x$XEND" = "xbackend" ], [
AFS_SUMMARY([Backend zlib compression ], [$MSG_BACKEND_COMPRESSION] )
AS_IF([test "x$MSG_BACKEND_COMPRESSION" = "xyes" ],[
AFS_SUMMARY([  zlib headers ], [$BACKEND_LIBZ_HEADERS] )
AFS_SUMMARY([  zlib library ], [$BACKEND_LIBZ_LD] )
AFS_SUMMARY([  Notice],[Back end part of cube (c-writer library) produces compressed cube report.])
])
])
], [

AFS_SUMMARY([zlib compression ], [$MSG_FRONTEND_COMPRESSION] )
AS_IF([test "x$MSG_FRONTEND_COMPRESSION" != "xno" ],[
AFS_SUMMARY([  zlib headers ], [$FRONTEND_LIBZ_HEADERS] )
AFS_SUMMARY([  zlib library ], [$FRONTEND_LIBZ_LD] )
])


AS_IF([ test "x$MSG_FRONTEND_COMPRESSION" = "xreadonly" ],[
AFS_SUMMARY([  Notice], [Cube tools, GUI  can read compressed cube files, but write only uncompressed cube files.])
])


AS_IF([ test "x$MSG_FRONTEND_COMPRESSION" = "xfull" ],[
AFS_SUMMARY([  Notice],[Cube tools, GUI  can read and write compressed cube files.])
])

])
AFS_SUMMARY([  zlib compression flags ], [$COMPRESSION] )


])

AS_IF([test "x$CUBE_DUMP_R_SELECTED" = "xyes"],
[
	AFS_SUMMARY([Use R for cube_dump], [$CUBE_DUMP_R_AVAILABLE])
	AS_IF( [test "x$CUBE_DUMP_R_AVAILABLE" = "xyes" ],
		[
		AFS_SUMMARY([  With R], [$R_PATH])
		AFS_SUMMARY([  With Rscript], [$RSCRIPT_PATH])
		AFS_SUMMARY([  R compiler flags], [$R_CPPFLAGS])
		AFS_SUMMARY([  R linker flags], [$R_LDFLAGS])
		AFS_SUMMARY([  RInside path ], [$R_INSIDE_PATH] )
		AFS_SUMMARY([  Rcpp path], [$R_CPP_PATH] )
		],
		[AFS_SUMMARY([  Notice],[R, Rscript, RInside and Rcpp are necessary])]
	)
])

AS_IF([ test "x$WITH_CPP_LIB" = "xyes" ],[
AS_IF([ test "x$ENABLE_MAKEFILE_RULES_FOR_REGENERATION" = "xyes" ],[
AFS_SUMMARY([Regenerate parsers:], [yes])
])
AS_IF( [test "x$CUBE_DEFAULT_ALL_IN_MEMORY" != "x" ],
		[
		AFS_SUMMARY([Data loading strategy ], [Keep all in memory, load on demand])
		])
AS_IF( [test "x$CUBE_DEFAULT_ALL_IN_MEMORY_PRELOAD" != "x" ],
		[
		AFS_SUMMARY([Data loading strategy ], [Keep all in memory, preloaded])
		])
AS_IF( [test "x$CUBE_DEFAULT_MANUAL" != "x" ],
		[
		AFS_SUMMARY([Data loading strategy ], [Manual loading])
		])
AS_IF( [test "x$CUBE_DEFAULT_LAST_N" != "x" ],
		[
		AFS_SUMMARY([Data loading strategy ], [Keep last $CUBE_N_LAST_ROWS rows in memory])
		])
])



AS_IF([test "x$XEND" = "xbackend" && test "x$CROSS_BUILD" == "xyes" ], [
AS_IF([test "x$WITH_C_WRITER" == "xyes"  ],[
AS_IF([ test "x$CUBE_SCOREP" != "x" ], [
AFS_SUMMARY([C99 compiler used],[$CUBE_SCOREP $CC])
],[
AFS_SUMMARY([C99 compiler used],[$CC])
])

AFS_SUMMARY([ Compiler flags used],[$CFLAGS])
])
],
[
AS_IF([ test "x$WITH_C_WRITER" == "xyes"  ],[
AS_IF([ test "x$CUBE_SCOREP" != "x" ], [
AFS_SUMMARY([C99 compiler used],[$CUBE_SCOREP $CC])
],[
AFS_SUMMARY([C99 compiler used],[$CC])
])
AFS_SUMMARY([ Compiler flags used],[$CFLAGS])
])
AS_IF([test "x$WITH_CPP_LIB" == "xyes" ],[
AS_IF([ test "x$CUBE_SCOREP" != "x" ], [
AFS_SUMMARY([C++ compiler used],[$CUBE_SCOREP $CXX])
],[
AFS_SUMMARY([C++ compiler used],[$CXX])
])
AFS_SUMMARY([ Compiler flags used],[$CXXFLAGS $FRONTEND_CXXFLAGS])
])
])



])









AC_DEFUN([AC_SCOREP_CUBE_BACKEND_SELECTION], [

AC_SCOREP_CUBE_INITIALIZE_BACKEND_SELECTION
AS_IF([ test "x$CROSS_BUILD" != "xyes"],[
AC_SCOREP_CUBE_INITIALIZE_FRONTEND_SELECTION
])

AC_SCOREP_CUBE_CWRITER_SELECTION


# put here this conditionals.... in future it has to go away
AM_CONDITIONAL([HAVE_LEX], [test "x${LEX}" != "x:"])
AM_CONDITIONAL([WITH_CPP_LIB], [test "x$WITH_CPP_LIB" == "xyes"])
AM_CONDITIONAL([WITH_TOOLS], [test "x$WITH_TOOLS" == "xyes"])
AM_CONDITIONAL([WITH_GUI], [test "x$WITH_GUI" == "xyes" ])
AM_CONDITIONAL([WITH_GUI_PLUGIN_LAUNCH], [test x$WITH_GUI_PLUGIN_LAUNCH == "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_GRAPHWIDGET_COMMON], [test x$WITH_GUI_PLUGIN_GRAPHWIDGETCOMMON == "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_BARPLOT], [test x$WITH_GUI_PLUGIN_BARPLOT == "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_HEATMAP], [test x$WITH_GUI_PLUGIN_HEATMAP == "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_PARAVER], [test "x$WITH_GUI_PLUGIN_PARAVER" = "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_SOURCE_EDITOR], [test "x$WITH_GUI_PLUGIN_SOURCE_EDITOR" = "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_STATISTICS], [test "x$WITH_GUI_PLUGIN_STATISTICS" = "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_VAMPIR], [test "x$WITH_GUI_PLUGIN_VAMPIR" = "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_ADVANCEDCM], [test "x$WITH_GUI_PLUGIN_ADVANCEDCM" = "xyes"])
AM_CONDITIONAL([CUBE_DUMP_WITH_R], [test "x$CUBE_DUMP_R_AVAILABLE" = "xyes"])
AM_CONDITIONAL([CUBE_COMPRESSION_BACKEND], [test "x$MSG_BACKEND_COMPRESSION" = "xyes"])
AM_CONDITIONAL([CUBE_COMPRESSION_FRONTEND], [test "x$MSG_FRONTEND_COMPRESSION" = "xyes"])
AFS_AM_CONDITIONAL([ENABLE_MAKEFILE_RULES_FOR_REGENERATION], [test "x$ENABLE_MAKEFILE_RULES_FOR_REGENERATION" = "xyes"], [false])

])



AC_DEFUN([AC_SCOREP_CUBE_ACCELERATOR_SELECTION], [

AC_SCOREP_CUBE_INITIALIZE_BACKEND_SELECTION
AC_SCOREP_CUBE_CWRITER_SELECTION

# put here this conditionals.... in future it has to go away
AM_CONDITIONAL([HAVE_LEX], [test "x${LEX}" != "x:"])
AM_CONDITIONAL([WITH_CPP_LIB], [test "x$WITH_CPP_LIB" == "xyes"])
AM_CONDITIONAL([WITH_TOOLS], [test "x$WITH_TOOLS" == "xyes"])
AM_CONDITIONAL([WITH_GUI], [test "x$WITH_GUI" == "xyes" ])
AM_CONDITIONAL([WITH_GUI_PLUGIN_LAUNCH], [test x$WITH_GUI_PLUGIN_LAUNCH == "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_GRAPHWIDGET_COMMON], [test x$WITH_GUI_PLUGIN_GRAPHWIDGETCOMMON == "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_BARPLOT], [test x$WITH_GUI_PLUGIN_BARPLOT == "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_HEATMAP], [test x$WITH_GUI_PLUGIN_HEATMAP == "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_VAMPIR], [test "x$WITH_GUI_PLUGIN_VAMPIR" = "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_SOURCE_EDITOR], [test "x$WITH_GUI_PLUGIN_SOURCE_EDITOR" = "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_STATICTICS], [test "x$WITH_GUI_PLUGIN_STATISTICS" = "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_PARAVER], [test "x$WITH_GUI_PLUGIN_PARAVER" = "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_ADVANCEDCM], [test "x$WITH_GUI_PLUGIN_ADVANCEDCM" = "xyes"])

AM_CONDITIONAL([CUBE_DUMP_WITH_R], [test "x$CUBE_DUMP_R_AVAILABLE" = "xyes"])
AFS_AM_CONDITIONAL([ENABLE_MAKEFILE_RULES_FOR_REGENERATION], [test "x$ENABLE_MAKEFILE_RULES_FOR_REGENERATION" = "xyes"], [false])
])



AC_DEFUN([AC_SCOREP_CUBE_FRONTEND_SELECTION], [
AS_IF([ test "x$CROSS_BUILD" = "xyes" ],[
AC_SCOREP_CUBE_INITIALIZE_FRONTEND_SELECTION
AC_SCOREP_CUBE_CWRITER_SELECTION
])


AC_SCOREP_CUBE_CUBELIB_SELECTION
AC_SCOREP_CUBE_TOOLS_SELECTION
AC_SCOREP_CUBE_GUI_SELECTION
AC_SCOREP_CUBE_R_SELECTION
AC_CUBE_DEBUG
# this is here and not in gui coz one can specify scorep for not gui parts 
AC_CUBE_HMI_INSTRUMENTATION 

# put here this conditionals.... in future it has to go away
AM_CONDITIONAL([HAVE_LEX], [test "x${LEX}" != "x:"])
AM_CONDITIONAL([WITH_CPP_LIB], [test "x$WITH_CPP_LIB" == "xyes"])
AM_CONDITIONAL([WITH_TOOLS], [test "x$WITH_TOOLS" == "xyes"])
AM_CONDITIONAL([WITH_GUI], [test "x$WITH_GUI" == "xyes" ])
AM_CONDITIONAL([WITH_GUI_PLUGIN_LAUNCH], [test x$WITH_GUI_PLUGIN_LAUNCH == "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_GRAPHWIDGET_COMMON], [test x$WITH_GUI_PLUGIN_GRAPHWIDGETCOMMON == "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_BARPLOT], [test x$WITH_GUI_PLUGIN_BARPLOT == "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_HEATMAP], [test x$WITH_GUI_PLUGIN_HEATMAP == "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_PARAVER], [test "x$WITH_GUI_PLUGIN_PARAVER" = "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_SOURCE_EDITOR], [test "x$WITH_GUI_PLUGIN_SOURCE_EDITOR" = "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_STATISTICS], [test "x$WITH_GUI_PLUGIN_STATISTICS" = "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_VAMPIR], [test "x$WITH_GUI_PLUGIN_VAMPIR" = "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_ADVANCEDCM], [test "x$WITH_GUI_PLUGIN_ADVANCEDCM" = "xyes"])
AM_CONDITIONAL([CUBE_DUMP_WITH_R], [test "x$CUBE_DUMP_R_AVAILABLE" = "xyes"])
AM_CONDITIONAL([CUBE_COMPRESSION_FRONTEND], [test "x$MSG_FRONTEND_COMPRESSION" = "xyes"])

AFS_AM_CONDITIONAL([ENABLE_MAKEFILE_RULES_FOR_REGENERATION], [test "x$ENABLE_MAKEFILE_RULES_FOR_REGENERATION" = "xyes"], [false])

])


AC_DEFUN([AC_SCOREP_CUBE_CWRITER_SELECTION], [

AC_ARG_WITH(cwriter, [AS_HELP_STRING([--with-cwriter | --without-cwriter], [Enables (default) or disables building and installation of the C cube writer.])],[],[])

AS_IF( [ test x$with_cwriter = "xyes" ], [
WITH_C_WRITER="yes"
])

AS_IF( [ test x$with_cwriter = "xno" || test "x$without_cwriter" = "xyes" ], [
WITH_C_WRITER="no"
])
AC_SUBST(WITH_C_WRITER)
AM_CONDITIONAL([WITH_C_WRITER], [test x$WITH_C_WRITER == "xyes"])
])


AC_DEFUN([AC_SCOREP_CUBE_CUBELIB_SELECTION], [
AC_ARG_WITH(cubelib, [AS_HELP_STRING([--with-cubelib | --without-cubelib], [Enables (default) or disables building and installation of the cube c++ library.])],[],[])

AS_IF([ test "x$with_cubelib" = "xyes" ], [
WITH_CPP_LIB="yes"
])

AS_IF( [ test "x$with_cubelib" = "xno" || test "x$without_cubelib" = "xyes" ], [
WITH_CPP_LIB="no"
WITH_TOOLS="no"
WITH_GUI="no"
])


AS_IF([test "x$WITH_CPP_LIB" = "xyes"],[
AM_PROG_LEX
AC_PROG_YACC
AC_SCOREP_PARSER_SELECTION
])
AM_CONDITIONAL([HAVE_LEX], [test "x${LEX}" != "x:"])
AC_SUBST(WITH_CPP_LIB)
AM_CONDITIONAL([WITH_CPP_LIB], [test "x$WITH_CPP_LIB" == "xyes"])
])



AC_DEFUN([AC_SCOREP_CUBE_TOOLS_SELECTION], [
AC_ARG_WITH(tools, [AS_HELP_STRING([--with-tools | --without-tools], [Enables (default) or disables building and installation of cube tools.])],[],[])


AS_IF([ test "x$with_tools" = "xyes" ], [
WITH_CPP_LIB="yes"
WITH_TOOLS="yes"
])

AS_IF([ test "x$with_tools" = "xno"  || test "x$without_tools" = "xyes" ], [
WITH_TOOLS="no"
])
AC_SUBST(WITH_TOOLS)
AM_CONDITIONAL([WITH_TOOLS], [test "x$WITH_TOOLS" == "xyes"])
])



AC_DEFUN([AC_SCOREP_CUBE_GUI_SELECTION], [
AC_ARG_WITH([gui],
    [AS_HELP_STRING([--with-gui | --without-gui],
        [Enables (default) or disables  building and installation of the Cube GUI.])],
    [AS_IF([test "x${with_gui}" = xyes && test "x${enable_shared}" = xno],
        [AC_MSG_ERROR([Cannot build GUI components when --disable-shared is requested.])])],
    [AS_IF([test "x${enable_shared}" = xno],
        [with_gui=no
         AC_MSG_WARN([Disabling GUI components because --disable-shared is requested.])],
        [])])

AS_IF([test "x$with_gui" = "xyes" ],
    [WITH_CPP_LIB="yes"
     WITH_GUI="yes"])

AS_IF([test "x$with_gui" = "xno"],
    [WITH_GUI="no"
     WITH_PARAVER=
     WITH_VAMPIR=])

AS_IF([test "x$WITH_GUI" = "xyes" ],
    [# Qt with AutoTroll
     AT_WITH_QT([+network +widgets +printsupport], [+plugin], [], [WITH_GUI="no"])
     AS_IF([test "x$WITH_GUI" = "xyes"],
         [AC_CUBE_SUPPORTED_PLUGINS])
    AC_CUBE_HMI_INSTRUMENTATION
])

AC_SUBST(WITH_GUI)
AM_CONDITIONAL([WITH_GUI], [test "x$WITH_GUI" == "xyes" ])
AM_CONDITIONAL([WITH_GUI_PLUGIN_LAUNCH], [test x$WITH_GUI_PLUGIN_LAUNCH == "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_GRAPHWIDGET_COMMON], [test x$WITH_GUI_PLUGIN_GRAPHWIDGETCOMMON == "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_BARPLOT], [test x$WITH_GUI_PLUGIN_BARPLOT == "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_HEATMAP], [test x$WITH_GUI_PLUGIN_HEATMAP == "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_VAMPIR], [test "x$WITH_GUI_PLUGIN_VAMPIR" = "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_SOURCE_EDITOR], [test "x$WITH_GUI_PLUGIN_SOURCE_EDITOR" = "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_STATISTICS], [test "x$WITH_GUI_PLUGIN_STATISTICS" = "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_PARAVER], [test "x$WITH_GUI_PLUGIN_PARAVER" = "xyes"])
AM_CONDITIONAL([WITH_GUI_PLUGIN_ADVANCEDCM], [test "x$WITH_GUI_PLUGIN_ADVANCEDCM" = "xyes"])

])


AC_DEFUN([AC_SCOREP_CUBE_R_SELECTION], [
AC_ARG_WITH(cube_dump_r, [AS_HELP_STRING([--with-cube-dump-r | --without-cube-dump-r], [Enables (default) or disables support for R matrix output in Cube dump tool])], [], [with_cube_dump_r=yes])

AS_IF( [ test "x$with_cube_dump_r" = "xyes" ], [
CUBE_DUMP_R_SELECTED="yes"
])
AS_IF( [ test "x$with_cube_dump_r" = "xno" || test "x$without_cube_dump_r" = "xyes" ], [
CUBE_DUMP_R_SELECTED="no"
])

AC_SCOREP_R_OPTION

AM_CONDITIONAL([CUBE_DUMP_WITH_R], [test "x$CUBE_DUMP_R_AVAILABLE" = "xyes"])
])






AC_DEFUN([AC_SCOREP_CUBE_INITIALIZE_BACKEND_SELECTION], [

WITH_CPP_LIB="no"
WITH_TOOLS="no"
WITH_GUI="no"
WITH_C_WRITER="yes"

])


AC_DEFUN([AC_SCOREP_CUBE_INITIALIZE_FRONTEND_SELECTION], [

WITH_CPP_LIB="yes"
WITH_TOOLS="yes"
WITH_GUI="yes"
WITH_C_WRITER="yes"

])






AC_DEFUN([AC_SCOREP_PARSER_SELECTION], [

AC_ARG_WITH(system-parser, [AS_HELP_STRING([--with-system-parser ],
	      [Generates all parsers of cube library using system own template (if found flex&bison) instead of using the own shipped version of pregenerated parsers. Apply this option if you have some errors in Cube4Parser.cpp, Cube4Scanner.cpp, CubePL1Parser.cpp, CubePL1Scanner.cpp, CubePL0Parser.cpp or CubePL0Scanner.cpp])], [], [])


WITH_SYSTEM_PARSER="no"
AS_IF( [ test "x$with_system_parser" = "xyes" ], [
WITH_SYSTEM_PARSER="yes"
])

AC_SCOREP_CUBE_PARSER_SELECTION


])




AC_DEFUN([AC_SCOREP_CUBE_PARSER_SELECTION], [

AM_PROG_LEX
AC_PROG_YACC


ENABLE_MAKEFILE_RULES_FOR_REGENERATION=no
AS_IF([ test "x$WITH_SYSTEM_PARSER" = "xyes" ],[
AC_MSG_NOTICE([ Remove pregenerated sources to invoke syntax parsers generation. ])
rm  $srcdir/../src/cube/src/syntax/Cube4Scanner.cpp
rm  $srcdir/../src/cube/src/syntax/cubepl/CubePL1Scanner.cpp
rm  $srcdir/../src/cube/src/syntax/cubepl/CubePL0Scanner.cpp
rm  $srcdir/../src/tools/tools/0031.ReMap2/ReMapScanner.cc
rm  $srcdir/../src/cube/src/syntax/Cube4Parser.cpp
rm  $srcdir/../src/cube/src/syntax/cubepl/CubePL1Parser.cpp
rm  $srcdir/../src/cube/src/syntax/cubepl/CubePL0Parser.cpp
rm  $srcdir/../src/tools/tools/0031.ReMap2/ReMapParser.cc
ENABLE_MAKEFILE_RULES_FOR_REGENERATION=yes
])



AC_MSG_NOTICE([ Use $LEX as a lexical analyzer. ])
AC_MSG_NOTICE([ Use $YACC as a syntax parser. ])


])








AC_DEFUN([AC_SCOREP_CUBE_BACKEND_EXECUTION_DETECTION], [
. $srcdir/../vendor/common/build-config/platforms/platform-backend-exec-${ac_scorep_platform}
. $srcdir/../vendor/common/build-config/platforms/platform-mpi-exec-${ac_scorep_platform}

 AC_MSG_NOTICE([ To run a serial application on backend we use prefix : $BACKENDEXEC ])

 if test "x$BATCHSYSTEMS" != "x"; then
 AC_CHECK_PROGS(BATCHSYS, $BATCHSYSTEMS, "")
 else
  BATCHSYS="sh"
 fi
 AC_MSG_NOTICE([ To run a job use  : exec.$BATCHSYS])
AC_MSG_NOTICE([ Copy   : exec.$BATCHSYS])
cp $srcdir/../vendor/common/build-config/platforms/exec.$BATCHSYS.in exec.$BATCHSYS.in
if test "x$BATCHSYS" != "xsh"; then
   AC_MSG_NOTICE([ Copy   : exec.sh])
cp $srcdir/../vendor/common/build-config/platforms/exec.sh.in exec.sh.in
fi

])



AC_DEFUN([AC_SCOREP_CUBE_FRONTEND_EXECUTION_DETECTION], [


. $srcdir/../vendor/common/build-config/platforms/platform-frontend-exec-${ac_scorep_platform}

 AC_MSG_NOTICE([ To run a serial application on frontend we use prefix: $FRONTENDEXEC])

])



AC_DEFUN([AC_SCOREP_CUBE_EXECUTION_SUBSTS], [


AC_SUBST([FRONTENDEXEC])
AC_SUBST([BACKENDEXEC])
AC_SUBST([NTHREADS])
AC_SUBST([EXECUTION_PREFIX])

])


AC_DEFUN([AC_SCOREP_CONSTUCT_LIST_OF_LIB_DIRS], [

sufix=""
   case "x${build_cpu}" in
    "xx86")
        sufix="32"
        ;;
    "xx86_64")
        sufix="64"
        ;;
    "xppc32")
        sufix="32"
        ;;
    "xppc64")
        sufix="64"
        ;;
    "xia64")
        sufix="64"
        ;;
    "xia32")
        sufix="32"
        ;;
    "xpowerpc64")
        sufix="64"
        ;;
    "xpowerpc32")
        sufix="32"
        ;;
    "xmips64")
        sufix="64"
        ;;
    "xmips32")
        sufix="32"
        ;;
   esac
if test "x$sufix" = "x"; then
LIBDIRS_LIST="/lib/ /usr/lib/ "
else
LIBDIRS_LIST="/lib/ /lib$sufix/ /usr/lib/ /usr/lib$sufix/ "
fi


])



AC_DEFUN([AC_SCOREP_CONFIGURE_STATUS_REPORT], [
AS_IF([test "x${afs_externally_triggered}" != xyes],
    [AC_MSG_NOTICE([----------------------------------------------------------------------------])
     AC_MSG_NOTICE([])
     AC_MSG_NOTICE([For further steps you can do:])
     AC_MSG_NOTICE(["make"            - to compile and to build selected components of Cube.])
     AC_MSG_NOTICE([])
     AC_MSG_NOTICE(["make install"    - to install selected components of Cube. ])
     AC_MSG_NOTICE([])
     AC_MSG_NOTICE([])
     AC_MSG_NOTICE([----------------------------------------------------------------------------])
     AC_MSG_NOTICE([])
     AC_MSG_NOTICE([===========================================================================])
    ])
])
