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


#
# This macro invalidates the headers cache variables created by previous AC_CHECK_HEADER/AC_CHECK_HEADERS checks.
#
AC_DEFUN([AC_SCOREP_RESET_LIB_CACHE], [
    AS_FOR([AX_var], [ax_var], [$1], [
        dnl You can replace "ac_cv_lib_" with any prefix from http://www.gnu.org/software/autoconf/manual/html_node/Cache-Variable-Index.html
        AS_VAR_PUSHDEF([ax_Var], [ac_cv_lib_${ax_var}_$2])
        AS_UNSET([ax_Var])
        AS_VAR_POPDEF([ax_Var])
    ])
]) # AX_RESET_LIB_CACHE

AC_DEFUN([AC_SCOREP_RESET_HEADERS_CACHE], [
    AS_FOR([AX_var], [ax_var], [$1], [
        dnl You can replace "ac_cv_lib_" with any prefix from http://www.gnu.org/software/autoconf/manual/html_node/Cache-Variable-Index.html
        AS_VAR_PUSHDEF([ax_Var], [ac_cv_header_${ax_var}])
        AS_UNSET([ax_Var])
        AS_VAR_POPDEF([ax_Var])
    ])
]) # AX_RESET_LIB_CACHE


AC_DEFUN([AC_SCOREP_LIBZ_DETECTION],
[
    dnl If after run of this macro is still empty - lib wasn't found
    LIB_RETURN=""

    if test -d $1; then
      PATH_CANDIDATES=`find $1 -type d `
      for zlib_path in $PATH_CANDIDATES; do

	  dnl Save the current state
	  ax_probe_library_save_LDFLAGS=${LDFLAGS}

	  LDFLAGS="-L$zlib_path"
	  dnl $LDFLAGS 
	  AC_MSG_CHECKING([zlib in $zlib_path])
	  AS_ECHO()
	  _AS_ECHO_LOG([LDFLAGS="${LDFLAGS}"])

	  AC_CHECK_LIB(z, zlibVersion, [ZLIB_FOUND="yes"], [ZLIB_FOUND="no"])

	  dnl Restore the state to original in case of unsuccessful attempt
	  LDFLAGS=${ax_probe_library_save_LDFLAGS}
	  AC_SCOREP_RESET_LIB_CACHE([z], [zlibVersion])

	  dnl We have found the location, leave the loop:
	  if test "x$ZLIB_FOUND" == "xyes"; then
		  AC_MSG_NOTICE([FOUND zlib at $1])
		  LIB_RETURN="$zlib_path"
		  break;
	  fi
      done
    else
      AC_MSG_ERROR(["Error: $1 doesn't exist"])
      LIB_RETURN=""
    fi

])


AC_DEFUN([AC_SCOREP_LIBZ_HEADERS_DETECTION],
[
    dnl If after run of this macro is still empty - lib wasn't found
    HEADER_RETURN=""

    if test -d $1; then
      PATH_CANDIDATES=`find $1 -type d `
      for zlib_path in $PATH_CANDIDATES; do

	  dnl Save the current state
	  ax_probe_header_save_CXXFLAGS=${CXXFLAGS}
	  ax_probe_header_save_CFLAGS=${CFLAGS}

	  CXXFLAGS="-I$zlib_path"
	  CFLAGS="-I$zlib_path"
	  dnl $LDFLAGS 
	  AC_MSG_CHECKING([zlib.h in $zlib_path])
	  AS_ECHO()
	  _AS_ECHO_LOG([CXXFLAGS="${CXXFLAGS}"])
	  _AS_ECHO_LOG([CFLAGS="${CFLAGS}"])

	  AC_CHECK_HEADER(zlib.h, [ZLIB_HEADER_FOUND="yes"], [ZLIB_HEADER_FOUND="no"])

	  dnl Restore the state to original in case of unsuccessful attempt
	  CXXFLAGS=${ax_probe_header_save_CXXFLAGS}
	  CFLAGS=${ax_probe_header_save_CFLAGS}
	  AC_SCOREP_RESET_HEADERS_CACHE([zlib.h])

	  dnl We have found the location, leave the loop:
	  if test "x$ZLIB_HEADER_FOUND" == "xyes"; then
		  AC_MSG_NOTICE([FOUND zlib.h at $1])
		  HEADER_RETURN="$zlib_path"
		  break;
	  fi
      done
    else
      AC_MSG_ERROR(["Error: $1 doesn't exist"])
      HEADER_RETURN=""
    fi

])



AC_DEFUN([AC_SCOREP_LIBZ_FRONTEND_SEARCH], [
    dnl if variable is empty, we do not change FRONTEND_LIBZ_LD
    AC_SCOREP_RESET_LIB_CACHE([z], [zlibVersion])
    AC_SCOREP_RESET_HEADERS_CACHE([zlib.h])
    if test "x$FRONTEND_PATH" != "x"; then
	AC_SCOREP_LIBZ_DETECTION([$FRONTEND_PATH])
	AC_SCOREP_LIBZ_HEADERS_DETECTION([$FRONTEND_PATH])
	
	if test "x$HEADER_RETURN"  != "x" && test "x$LIB_RETURN"  != "x"; then
	    FRONTEND_LIBZ_LD="-L$LIB_RETURN -lz"
	    FRONTEND_LIBZ_HEADERS="-I$HEADER_RETURN"

	    if test "x$FRONTEND_COMPRESSION_ON" = "xfull"; then
	      COMPRESSION="$COMPRESSION -DFRONTEND_CUBE_COMPRESSED=yes "
	      MSG_FRONTEND_COMPRESSION=full
	    fi
	    if test "x$FRONTEND_COMPRESSION_ON" = "xro"; then
	      COMPRESSION="$COMPRESSION -DFRONTEND_CUBE_COMPRESSED_READONLY=yes "
	      MSG_FRONTEND_COMPRESSION=readonly
	    fi
	else
	    AC_MSG_WARN([ Cannot find zlib under $FRONTEND_PATH. CUBE uses zlib compression and this library is needed, if you create a frontend application.])
	    FRONTEND_LIBZ_LD=""
	    FRONTEND_LIBZ_HEADERS=""
	    MSG_FRONTEND_COMPRESSION=no
	    COMPRESSION="$COMPRESSION"
	fi
    else
	AC_CHECK_LIB( z, zlibVersion, [FRONTEND_LIBZ_LD="-lz" ;  if test "x$FRONTEND_COMPRESSION_ON" = "xfull"; then MSG_FRONTEND_COMPRESSION="full" ; COMPRESSION="$COMPRESSION -DFRONTEND_CUBE_COMPRESSED=yes "; fi ;  if test "x$FRONTEND_COMPRESSION_ON" = "xro"; then MSG_FRONTEND_COMPRESSION="readonly" ; COMPRESSION="$COMPRESSION -DFRONTEND_CUBE_COMPRESSED_READONLY=yes "; fi], [
		AC_MSG_WARN([ Cannot find zlib. CUBE uses zlib compression and this library is needed, if you create a frontend application.]) ;  MSG_FRONTEND_COMPRESSION=no
	    ])
    fi
    AC_SCOREP_RESET_LIB_CACHE([z], [zlibVersion])
    AC_SCOREP_RESET_HEADERS_CACHE([zlib.h])
    
    
   
])


AC_DEFUN([AC_SCOREP_LIBZ_BACKEND_SEARCH], [
    dnl if variable is empty, we do not change BACKEND_LIBZ_LD
    AC_SCOREP_RESET_LIB_CACHE([z], [zlibVersion])
    AC_SCOREP_RESET_HEADERS_CACHE([zlib.h])
    if test "x$BACKEND_PATH" != "x"; then
	AC_SCOREP_LIBZ_DETECTION([$BACKEND_PATH])
	AC_SCOREP_LIBZ_HEADERS_DETECTION([$BACKEND_PATH])
	if test "x$HEADER_RETURN"  != "x" && test "x$LIB_RETURN"  != "x";then
	    BACKEND_LIBZ_LD="-L$LIB_RETURN -lz"
	    BACKEND_LIBZ_HEADERS="-I$HEADER_RETURN"
	    MSG_BACKEND_COMPRESSION=yes; 
	    COMPRESSION="$COMPRESSION -DBACKEND_CUBE_COMPRESSED=yes "
	else
	    AC_MSG_WARN([ Cannot find zlib under $BACKEND_PATH. CUBE uses zlib compression and this library is needed, if you create a backend application.])
	    BACKEND_LIBZ_LD=""
	    BACKEND_LIBZ_HEADERS=""
	    MSG_BACKEND_COMPRESSION=no
	    COMPRESSION="$COMPRESSION"
	fi

    else
	AC_CHECK_LIB(z,zlibVersion,  [BACKEND_LIBZ_LD="-lz" ;  MSG_BACKEND_COMPRESSION="yes" ; COMPRESSION="$COMPRESSION -DBACKEND_CUBE_COMPRESSED=yes "], [
		AC_MSG_WARN([ Cannot find zlib. CUBE uses zlib compression and this library is needed, if you create a backend application.])  ;  MSG_BACKEND_COMPRESSION=no
	    ])
    fi
    AC_SCOREP_RESET_LIB_CACHE([z], [zlibVersion])
    AC_SCOREP_RESET_HEADERS_CACHE([zlib.h])
    
])





AC_DEFUN([AC_CUBE_LIBZ_FRONTEND_SELECTION], [

AC_ARG_WITH(frontend-zlib, [AS_HELP_STRING([--with-frontend-zlib="path to frontrend zlib"], [Defines the zlib library, used by cube c++ library])],[FRONTEND_PATH=$withval ; FRONTEND_LIBZ_LD="-L$withval/lib -lz" ; FRONTEND_LIBZ_HEADERS="-I$withval/include"],[FRONTEND_PATH= ; FRONTEND_LIBZ_LD="-lz" ; FRONTEND_LIBZ_HEADERS=""])
#AC_ARG_WITH(frontend-compression, [AS_HELP_STRING([--with-frontend-compression], [Enables reading and writing of compressed cube files by C++ library(tools, GUI) and c-writer on frontend.)])],[FRONTEND_COMPRESSION_ON=$withval ],[])

if test "x${ac_scorep_cross_compiling}" = "xyes"; then
AC_ARG_WITH(compression, [AS_HELP_STRING([--with-compression=full|ro|none], [Setup the level of compression support:  "full" - all parts (tools, GUI and c-writer)  create compressed cubes; "ro" - tools and GUI can still read compressed cube files, but write uncompressed version. "none" - neither read not write of compressed cube files is possible. ])],[ BACKEND_COMPRESSION_ON=$withval ; FRONTEND_COMPRESSION_ON=$withval ],[])
fi


dnl "looks for libz.a or libz.so in pathes, wich are under the $BACKEND_PATH or $FRONTEND_PATH"
if test "x$FRONTEND_COMPRESSION_ON" = "xfull" || test "x$FRONTEND_COMPRESSION_ON" = "xro"; then
AC_SCOREP_LIBZ_FRONTEND_SEARCH
else
COMPRESSION=""
FRONTEND_LIBZ_HEADERS=""
FRONTEND_LIBZ_LD=""
MSG_FRONTEND_COMPRESSION="no"
fi




AC_MSG_NOTICE([ Compression support in tools and GUI : $MSG_FRONTEND_COMPRESSION])
AS_IF([ test "x$MSG_FRONTEND_COMPRESSION" != "xno" ],[ 
AC_MSG_NOTICE([ Frontend zlib get linked  as: $FRONTEND_LIBZ_LD])
AC_MSG_NOTICE([ Frontend zlib headers included as: $FRONTEND_LIBZ_HEADERS])
])
AC_SUBST([FRONTEND_LIBZ_LD])
AC_SUBST([FRONTEND_LIBZ_HEADERS])
AC_SUBST([COMPRESSION])
AC_SUBST([MSG_FRONTEND_COMPRESSION])

])



AC_DEFUN([AC_CUBE_LIBZ_BACKEND_SELECTION], [

 BACKEND_COMPRESSION_ON="ro" ; FRONTEND_COMPRESSION_ON="ro"
AC_ARG_WITH(compression, [AS_HELP_STRING([--with-compression=full|ro|none], [Setup the level of compression support:  "full" - all parts (tools, GUI and c-writer)  create compressed cubes; "ro" - tools and GUI can still read compressed cube files (default), but write uncompressed version. "none" - neither read not write of compressed cube files is possible. ])],[ BACKEND_COMPRESSION_ON=$withval ; FRONTEND_COMPRESSION_ON=$withval ],[])


AC_ARG_WITH(backend-zlib, [AS_HELP_STRING([--with-backend-zlib="path to backend zlib"], [Defines the zlib library, used by cube c-writer])],[BACKEND_PATH=$withval ; BACKEND_LIBZ_LD="-L$withval/lib -lz" ; BACKEND_LIBZ_HEADERS="-I$withval/include"],[BACKEND_PATH= ; BACKEND_LIBZ_LD="-lz" ;  BACKEND_LIBZ_HEADERS=""])
#AC_ARG_WITH(backend-compression, [AS_HELP_STRING([--with-backend-compression], [Enables writing of compressed cube files by  c-writer on backend.)])],[BACKEND_COMPRESSION_ON=$withval ],[])
if test "x$BACKEND_COMPRESSION_ON" = "xfull"; then
BACKEND_COMPRESSION_ON="yes";
else
BACKEND_COMPRESSION_ON="no";
fi

dnl "looks for libz.a or libz.so in pathes, wich are under the $BACKEND_PATH or $FRONTEND_PATH"
if test "x$BACKEND_COMPRESSION_ON" = "xyes"; then
AC_SCOREP_LIBZ_BACKEND_SEARCH
else
COMPRESSION=""
BACKEND_LIBZ_HEADERS=""
BACKEND_LIBZ_LD=""
MSG_BACKEND_COMPRESSION="no"
fi



AC_MSG_NOTICE([ CUBE C-Writer library creates compressed files : $MSG_BACKEND_COMPRESSION])
AS_IF([ test "x$MSG_FRONTEND_COMPRESSION" = "xyes" ],[ 

AC_MSG_NOTICE([ Back-end zlib get linked  as: $BACKEND_LIBZ_LD])
AC_MSG_NOTICE([ Back-end zlib headers included  as: $BACKEND_LIBZ_HEADERS])
])
AC_SUBST([BACKEND_LIBZ_LD])
AC_SUBST([BACKEND_LIBZ_HEADERS])
AC_SUBST([COMPRESSION])
AC_SUBST([MSG_BACKEND_COMPRESSION])
])

