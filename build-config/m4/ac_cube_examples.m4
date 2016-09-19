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





AC_DEFUN([AC_SCOREP_FRONTEND_EXAMPLES],
[

    AS_IF([ test "x$WITH_CPP_LIB" = "xyes"],[ 
	NOMPI_CUBE_FRONTEND_EXAMPLES_SRC="cube_example.cpp "
	NOMPI_CUBE_FRONTEND_EXAMPLES_EXE="cube_example.cpp.exe"
	if test "x$MPICXX" != "x"; then
	    MPI_CUBE_FRONTEND_EXAMPLES_SRC=""
	    MPI_CUBE_FRONTEND_EXAMPLES_EXE=""
	fi
])

    AS_IF([ test "x$WITH_C_WRITER" = "xyes"],[
	NOMPI_CUBEW_FRONTEND_EXAMPLES_SRC="cubew_example_complex.c \
					   cubew_example_simple.c"
	NOMPI_CUBEW_FRONTEND_EXAMPLES_EXE="cubew_example_complex.c.exe \
					   cubew_example_simple.c.exe"
	if test "x$MPICC" != "x"; then
	    MPI_CUBEW_FRONTEND_EXAMPLES_SRC=""
	    MPI_CUBEW_FRONTEND_EXAMPLES_EXE=""
	fi
    ])

AS_IF([test "x$WITH_JAVA_READER" = "xyes"],[ 
	JAVA_FRONTEND_EXAMPLES_SRC="jcuber_example.java"
	JAVA_FRONTEND_EXAMPLES_EXE="jcuber_example.java.class"
	JAVA_FRONTEND_EXAMPLES_RUN="jcuber_example"
])


    NOMPI_FRONTEND_APPS_EXE="$NOMPI_CUBE_FRONTEND_EXAMPLES_EXE $NOMPI_CUBEW_FRONTEND_EXAMPLES_EXE"
    MPI_FRONTEND_APPS_EXE="$MPI_CUBE_FRONTEND_EXAMPLES_EXE $MPI_CUBEW_FRONTEND_EXAMPLES_EXE"





    AC_SUBST([NOMPI_CUBE_FRONTEND_EXAMPLES_SRC])
    AC_SUBST([NOMPI_CUBE_FRONTEND_EXAMPLES_EXE])
    AC_SUBST([MPI_CUBE_FRONTEND_EXAMPLES_SRC])
    AC_SUBST([MPI_CUBE_FRONTEND_EXAMPLES_EXE])
    AC_SUBST([NOMPI_CUBEW_FRONTEND_EXAMPLES_SRC])
    AC_SUBST([NOMPI_CUBEW_FRONTEND_EXAMPLES_EXE])
    AC_SUBST([MPI_CUBEW_FRONTEND_EXAMPLES_SRC])
    AC_SUBST([MPI_CUBEW_FRONTEND_EXAMPLES_EXE])

    AC_SUBST([JAVA_FRONTEND_EXAMPLES_SRC])
    AC_SUBST([JAVA_FRONTEND_EXAMPLES_EXE])
    AC_SUBST([JAVA_FRONTEND_EXAMPLES_RUN])


    AC_SUBST([NOMPI_FRONTEND_APPS_EXE])
    AC_SUBST([MPI_FRONTEND_APPS_EXE])





])




AC_DEFUN([AC_SCOREP_BACKEND_EXAMPLES],
[

    MPI_BACKEND_APPS=""
    NOMPI_BACKEND_APPS=""
    JAVA_BACKEND_APPS=""
    
    if test "x$WITH_CPP_LIB" = "xyes"; then
	NOMPI_CUBE_BACKEND_EXAMPLES_SRC=""
	NOMPI_CUBE_BACKEND_EXAMPLES_EXE=""
	if test "x$MPICXX" != "x"; then
	    MPI_CUBE_BACKEND_EXAMPLES_SRC=""
	    MPI_CUBE_BACKEND_EXAMPLES_EXE=""
	fi
    fi
    if test "x$WITH_C_WRITER" = "xyes"; then
	NOMPI_CUBEW_BACKEND_EXAMPLES_SRC=""
	NOMPI_CUBEW_BACKEND_EXAMPLES_EXE=""
	if test "x$MPICC" != "x"; then
	    MPI_CUBEW_BACKEND_EXAMPLES_SRC="cubew_example_simple.mpi.c \
					    cubew_example_complex.mpi.c"
	    MPI_CUBEW_BACKEND_EXAMPLES_EXE="cubew_example_simple.mpi.c.exe \
					    cubew_example_complex.mpi.c.exe"
	fi
    fi
    
    if test "x$WITH_JAVA_READER" = "xyes"; then
	JAVA_BACKEND_EXAMPLES_SRC=""
	JAVA_BACKEND_EXAMPLES_EXE=""
	JAVA_BACKEND_EXAMPLES_RUN=""
    fi

    NOMPI_BACKEND_APPS_EXE="$NOMPI_CUBE_BACKEND_EXAMPLES_EXE $NOMPI_CUBEW_BACKEND_EXAMPLES_EXE"
    MPI_BACKEND_APPS_EXE="$MPI_CUBE_BACKEND_EXAMPLES_EXE $MPI_CUBEW_BACKEND_EXAMPLES_EXE"



    AC_SUBST([NOMPI_CUBE_BACKEND_EXAMPLES_SRC])
    AC_SUBST([NOMPI_CUBE_BACKEND_EXAMPLES_EXE])
    AC_SUBST([MPI_CUBE_BACKEND_EXAMPLES_SRC])
    AC_SUBST([MPI_CUBE_BACKEND_EXAMPLES_EXE])
    AC_SUBST([NOMPI_CUBEW_BACKEND_EXAMPLES_SRC])
    AC_SUBST([NOMPI_CUBEW_BACKEND_EXAMPLES_EXE])
    AC_SUBST([MPI_CUBEW_BACKEND_EXAMPLES_SRC])
    AC_SUBST([MPI_CUBEW_BACKEND_EXAMPLES_EXE])

    AC_SUBST([JAVA_BACKEND_EXAMPLES_SRC])
    AC_SUBST([JAVA_BACKEND_EXAMPLES_EXE])
    AC_SUBST([JAVA_BACKEND_EXAMPLES_RUN])



    AC_SUBST([NOMPI_BACKEND_APPS_EXE])
    AC_SUBST([MPI_BACKEND_APPS_EXE])

])



AC_DEFUN([AC_CUBE_EXAMPLES],
[
    #AC_SCOREP_BACKEND_EXAMPLES
    AC_SCOREP_FRONTEND_EXAMPLES
    AC_SCOREP_BACKEND_EXAMPLES



])




