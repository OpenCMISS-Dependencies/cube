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


AC_PREREQ([2.69])
AC_INIT([Cube], m4_esyscmd([../vendor/common/build-config/generate-package-version.sh ../build-config/VERSION]), [scalasca@fz-juelich.de], [cube],[http://www.scalasca.org])

## Initialize configuration summary
AFS_PACKAGE_BUILD_INIT([backend])
AFS_SUMMARY_INIT

AC_SUBST([LIBRARY_INTERFACE_VERSION], m4_esyscmd([../vendor/common/build-config/generate-library-version.sh ../build-config/VERSION]))
AC_DEFINE_UNQUOTED([LIBRARY_INTERFACE_VERSION], ["$LIBRARY_INTERFACE_VERSION"], [Interface version number])


## Where to find auxiliary build tools & local macros
AC_CONFIG_AUX_DIR([../build-config])
## See also ACLOCAL_AMFLAGS in Makefile.am.
AC_CONFIG_MACRO_DIRS([../build-config/m4 ../vendor/common/build-config/m4])

## Set (non-)cross compilation mode
AS_IF([test "x${ac_scorep_cross_compiling}" != "xyes" && \
       test "x${ac_scorep_cross_compiling}" != "xno"],
      [AC_MSG_ERROR([invalid value '$ac_scorep_cross_compiling' for variable \$ac_scorep_cross_compiling.], [1])])
AM_CONDITIONAL([CROSS_BUILD], [test "x${ac_scorep_cross_compiling}" = "xyes"])
cross_compiling=${ac_scorep_cross_compiling}
CROSS_BUILD=$cross_compiling
if test "x${ac_scorep_cross_compiling}" = "xyes"; then
   AC_SUBST([FRONTEND_CXX], [${CXX_FOR_BUILD}])
else
   AC_SUBST([FRONTEND_CXX], [${CXX}])
fi
AC_SUBST([CROSS_BUILD])
# On crosscompile machines, know where to look for backend stuff.
#On crosscompile machines, install backend/ /mic stuff into
# $(bin|lib)dir/(backend|mic).
backend_suffix=""
AS_IF([test "x${ac_scorep_cross_compiling}" = "xyes"],
      [AS_IF([test "x${ac_scorep_platform}" = "xmic"],
 		                 [backend_suffix="/mic"],
 		             [backend_suffix="/backend"])
])
AC_SUBST([backend_suffix])
# used for EXTRA_DIST in Makefile.am
AM_CONDITIONAL([PLATFORM_MIC], [test "x${ac_scorep_platform}" = "xmic"])

## Set convenience variables for macros
ac_scorep_backend=yes
AM_COND_IF([CROSS_BUILD],
           [ac_scorep_frontend=no],
           [ac_scorep_frontend=yes])


AC_SCOREP_CONSTUCT_LIST_OF_LIB_DIRS
AC_SCOREP_CUBE_BACKEND_SELECTION


AS_IF([ test "x$CROSS_BUILD" != "xyes" ],[
AC_SCOREP_CUBE_FRONTEND_SELECTION
CUBEW_CROSS_BUILD="-DCROSS_BUILD"
],[
CUBEW_CROSS_BUILD="-DNOCROSS_BUILD"
])

AC_SUBST(CUBEW_CROSS_BUILD)


## Platform configuration


##--- Package-specific configuration ----------------------------------------
## Set up Automake
AM_INIT_AUTOMAKE([foreign color-tests 1.13.4 -Wall tar-pax])
AM_SILENT_RULES([yes])
AM_PROG_AR

## Set up Libtool
LT_PREREQ([2.4.6])
LT_INIT([])
AC_SUBST([LIBTOOL_DEPS])


AS_IF([ test "x$WITH_C_WRITER" = "xyes" ],[
## Consistency checks for compilers
AC_SCOREP_CHECK_COMPILER_VAR_SET([CC])
## Checks for programs
AC_PROG_CC

AC_SCOREP_PROG_CC_C99
AC_C_BIGENDIAN
## trunc and ceil needed by the cube4 writer
AC_SEARCH_LIBS([trunc], [m])
AC_SEARCH_LIBS([ceil], [m])
])

AS_IF([ test "x$CROSS_BUILD" != "xyes" ],[
AS_IF([ test "x$WITH_CPP_LIB" = "xyes" ],[
AC_SCOREP_CHECK_COMPILER_VAR_SET([CXX])

AFS_DEBUG_OPTION
AC_SCOREP_PROG_CC_C99
AC_PROG_CXX
AM_PROG_CC_C_O
AC_LANG([C++])
AC_CUBE_IO_BUFFERSIZE
AC_CUBE_STRATEGY_SELECTION
AC_CUBELIB_ERROR_CODES

])
])

AC_SCOREP_COMPILER_CHECKS
AC_SCOREP_POSIX_FUNCTIONS

AS_IF([ test "x$WITH_CPP_LIB" == "xyes" || test "x$WITH_C_WRITER" == "xyes"  ],[

AC_CUBE_LIBZ_BACKEND_SELECTION
AS_IF([ test "x$CROSS_BUILD" != "xyes" ],[
AC_CUBE_LIBZ_FRONTEND_SELECTION
])


])

## Workaround for linking issues on Cray
AS_IF([test "x${ac_scorep_platform}" = "xcrayxt" && \
       test "x${enable_shared}" = "xno"],
      [AC_CONFIG_COMMANDS([libtool-cray-fix],
          [sed -e '1,/prefer_static_libs=/ s/prefer_static_libs=.*$/prefer_static_libs=yes/' \
               libtool > libtool.fix && mv libtool.fix libtool])])


# On crosscompile machines, install backend/mic stuff into
# $(bin|lib)dir/(backend|mic).
backend_suffix=""
AS_IF([test "x${ac_scorep_cross_compiling}" = "xyes"],
        [AS_IF([test "x${ac_scorep_platform}" = "xmic"],
                 [backend_suffix="/mic"],
             [backend_suffix="/backend"])
        ])
AC_SUBST([backend_suffix])


AC_SCOREP_CONFIG_SELECTION_REPORT([backend])


#check for examples
AC_CUBE_EXAMPLES

#AFS_DETECT_MIC
#
#IF([ test "x$afs_mic_enabled" == "xyes"],
#[## Handle own package's subdir configures
#AC_CONFIG_SUBDIR_CUSTOM([build-accel],
#                        [${args} ac_scorep_platform=${ac_scorep_platform} ac_scorep_cross_compiling=${ac_scorep_cross_compiling} ac_mic_enabled=yes])
#
#])


## Generate config files
AC_CONFIG_HEADERS([../src/config-backend.h])


AC_CONFIG_FILES([../examples/Makefile])
AC_CONFIG_FILES([../examples/Makefile.backend])

if test "x${ac_scorep_cross_compiling}" != "xyes"; then
AC_CONFIG_FILES([../examples/Makefile.frontend])
fi




CC="$CUBE_SCOREP $CC"
CXX="$CUBE_SCOREP $CXX"



AC_CONFIG_FILES([Makefile])

AC_CONFIG_FILES([../bin/cube-config-backend
                 ], \
                [chmod +x ../bin/cube-config-backend
                ])
if test "x$CROSS_BUILD" != "xyes"; then

AC_CONFIG_FILES([../examples/GUI/plugins/simple/example-simple.pro
                 ../examples/GUI/plugins/demo/demo-plugin.pro
                 ../examples/GUI/plugins/colormap/colormap.pro
                 ../examples/GUI/plugins/context-free/context-free.pro ])

AC_CONFIG_FILES([../bin/cube-config-frontend
                 ], \
                [chmod +x ../bin/cube-config-frontend
                ])
AC_CONFIG_FILES([../test/test0.0/run_test_0.0.sh
                 ], \
                [chmod +x ../test/test0.0/run_test_0.0.sh
                ])
AC_CONFIG_FILES([../test/test0.0/cube_unite_test0.0.cpp
                 ])
AC_CONFIG_FILES([../test/test0.1/run_test_0.1.sh
                 ], \
                [chmod +x ../test/test0.1/run_test_0.1.sh
                ])
AC_CONFIG_FILES([../test/test0.2/run_test_0.2.sh
                 ], \
                [chmod +x ../test/test0.2/run_test_0.2.sh
                ])
AC_CONFIG_FILES([../test/test0.3/run_test_0.3.sh
                 ], \
                [chmod +x ../test/test0.3/run_test_0.3.sh
                ])
AC_CONFIG_FILES([../test/test0.4/run_test_0.4.sh
                 ], \
                [chmod +x ../test/test0.4/run_test_0.4.sh
                ])
AC_CONFIG_FILES([../test/test1.1/run_test_1.1.sh
                 ], \
                [chmod +x ../test/test1.1/run_test_1.1.sh
                ])
AC_CONFIG_FILES([../test/test1.2/run_test_1.2.sh
                 ], \
                [
                 chmod +x ../test/test1.2/run_test_1.2.sh
                ])
AC_CONFIG_FILES([../test/test1.3/run_test_1.3.sh
                 ], \
                [
                 chmod +x ../test/test1.3/run_test_1.3.sh
                ])
AC_CONFIG_FILES([../test/test2/run_test_2.sh
                 ], \
                [
                 chmod +x ../test/test2/run_test_2.sh
                ])
AC_CONFIG_FILES([../test/test3/run_test_3.sh
                 ], \
                [
                 chmod +x ../test/test3/run_test_3.sh
                ])
AC_CONFIG_FILES([../test/test4/run_test_4.sh
                 ], \
                [
                 chmod +x ../test/test4/run_test_4.sh
                ])
AC_CONFIG_FILES([../test/test5/run_test_5.sh
                 ], \
                [
                 chmod +x ../test/test5/run_test_5.sh
                ])
AC_CONFIG_FILES([../test/test7/run_test_7.sh
                 ], \
                [
                 chmod +x ../test/test7/run_test_7.sh
                ])
AC_CONFIG_FILES([../test/test8/run_test_8.sh
                 ], \
                [
                 chmod +x ../test/test8/run_test_8.sh
                ])
AC_CONFIG_FILES([../test/test9/run_test_9.sh
                 ], \
                [
                 chmod +x ../test/test9/run_test_9.sh
                ])
AC_CONFIG_FILES([../test/test10/run_test_10.sh
                 ], \
                [
                 chmod +x ../test/test10/run_test_10.sh
                ])
AC_CONFIG_FILES([../test/test11/run_test_11.sh
                 ], \
                [
                 chmod +x ../test/test11/run_test_11.sh
                ])
AC_CONFIG_FILES([../test/test12/run_test_12.sh
                 ], \
                [
                 chmod +x ../test/test12/run_test_12.sh
                ])
AC_CONFIG_FILES([../test/test13/run_test_13.sh
                 ], \
                [
                 chmod +x ../test/test13/run_test_13.sh
                ])
AC_CONFIG_FILES([../test/test14/run_test_14.sh
                 ], \
                [
                 chmod +x ../test/test14/run_test_14.sh
                ])
AC_CONFIG_FILES([../test/test15/run_test_15.sh
                 ], \
                [
                 chmod +x ../test/test15/run_test_15.sh
                ])
AC_CONFIG_FILES([../test/test16/run_test_16.sh
                 ], \
                [
                 chmod +x ../test/test16/run_test_16.sh
                ])
AC_CONFIG_FILES([../test/test17/run_test_17.sh
                 ], \
                [
                 chmod +x ../test/test17/run_test_17.sh
                ])
AC_CONFIG_FILES([../test/test18/run_test_18.sh
                 ], \
                [
                 chmod +x ../test/test18/run_test_18.sh
                ])
AC_CONFIG_FILES([../test/test19/run_test_19.sh
                 ], \
                [
                 chmod +x ../test/test19/run_test_19.sh
                ])
AC_CONFIG_FILES([../test/test20/run_test_20.sh
                 ], \
                [
                 chmod +x ../test/test20/run_test_20.sh
                ])
AC_CONFIG_FILES([../test/test21/run_test_21.sh
                 ], \
                [
                 chmod +x ../test/test21/run_test_21.sh
                ])
AC_CONFIG_FILES([../test/test22/run_test_22.sh
                 ], \
                [
                 chmod +x ../test/test22/run_test_22.sh
                ])
AC_CONFIG_FILES([../test/test23/run_test_23.sh
                 ], \
                [
                 chmod +x ../test/test23/run_test_23.sh
                ])
AC_CONFIG_FILES([../test/test24/run_test_24.sh
                 ], \
                [
                 chmod +x ../test/test24/run_test_24.sh
                ])
AC_CONFIG_FILES([../test/test25/run_test_25.sh
                 ], \
                [
                 chmod +x ../test/test25/run_test_25.sh
                ])
AC_CONFIG_FILES([../test/test26/run_test_26.sh
                 ], \
                [
                 chmod +x ../test/test26/run_test_26.sh
                ])
AC_CONFIG_FILES([../test/test27/run_test_27.sh
                 ], \
                [
                 chmod +x ../test/test27/run_test_27.sh
                ])
AC_CONFIG_FILES([../test/test28/run_test_28.sh
                 ], \
                [
                 chmod +x ../test/test28/run_test_28.sh
                ])
AC_CONFIG_FILES([../test/test29/run_test_29.sh
                 ], \
                [
                 chmod +x ../test/test29/run_test_29.sh
                ])
AC_CONFIG_FILES([../test/test30/run_test_30.sh
                 ], \
                [
                 chmod +x ../test/test30/run_test_30.sh
                ])
AC_CONFIG_FILES([../test/test31/run_test_31.sh
                 ], \
                [
                 chmod +x ../test/test31/run_test_31.sh
                ])

AS_IF([ test "x$CUBE_DUMP_R_SELECTED" == "xyes" ],[
	AC_CONFIG_FILES([../test/test32/run_test_32.sh
                 ], \
                [
                 chmod +x ../test/test32/run_test_32.sh
                ])
])

fi



AC_OUTPUT
