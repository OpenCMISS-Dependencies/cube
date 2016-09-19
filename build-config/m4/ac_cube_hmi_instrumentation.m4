## -*- mode: autoconf -*-
##*************************************************************************##
##  CUBE        http://www.score-p.org/                                    ##
##  SCALASCA    http://www.scalasca.org/                                   ##
##*************************************************************************##
##  Copyright (c) 2015                                                     ##
##  Forschungszentrum Juelich GmbH, Juelich Supercomputing Centre          ##
##                                                                         ##
##  This software may be modified and distributed under the terms of       ##
##  a BSD-style license.  See the COPYING file in the package base         ##
##  directory for details.                                                 ##
##*************************************************************************##

AC_DEFUN([AC_CUBE_HMI_INSTRUMENTATION],
[
AC_ARG_WITH(instrumentation, [AS_HELP_STRING([--with-instrumentation], [ Enables HMI Instrumenter macros to enable determenistic instrumentation])],[ HMI_INSTRUMENTATION="-DMARKER"; TRY_SCOREP=yes],[HMI_INSTRUMENTATION=""])

AC_CUBE_FIND_SCOREP

AC_MSG_NOTICE([HMI Instrumentation : $HMI_INSTRUMENTATION])

AC_SUBST([HMI_INSTRUMENTATION])

])


AC_DEFUN([AC_CUBE_FIND_SCOREP],
[
AC_ARG_WITH( scorep, [AS_HELP_STRING([--with-scorep=path], [ Defined path to Score-P binary])],[ CUBE_SCOREP=$withval],[CUBE_SCOREP=])


AS_IF([test "x$TRY_SCOREP" == "xyes"],[

AS_IF([ test "x$CUBE_SCOREP" == "x"], [

AC_PATH_PROG([CUBE_SCOREP], [scorep], [], [$PATH])

AS_IF([ test "x$CUBE_SCOREP" == "x"], [
HMI_INSTRUMENTATION=""
])
])
AC_MSG_NOTICE([Using ScoreP : $CUBE_SCOREP ]) 
])

AC_SUBST([CUBE_SCOREP])

])
