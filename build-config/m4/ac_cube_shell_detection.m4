## -*- mode: autoconf -*-
##*************************************************************************##
##  CUBE        http://www.score-p.org/                                    ##
##  SCALASCA    http://www.scalasca.org/                                   ##
##*************************************************************************##
##  Copyright (c) 1998-2015                                                ##
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


AC_DEFUN([AC_SCOREP_SHELL_DETECTION],
[


#    OWN_SHELL=`echo $SHELL| sed -e "s,.*/,,"`
    AC_CHECK_PROG(BASH_SHELL, [bash], [bash], [])
    AC_CHECK_PROG(SH_SHELL, sh, sh, [])
    AC_CHECK_PROG(CSH_SHELL, csh, csh, [])
    AC_CHECK_PROG(TCSH_SHELL, tcsh, tcsh, [])
    AC_CHECK_PROG(KSH_SHELL, ksh, ksh, [])
    AC_CHECK_PROG(ASH_SHELL, ash, ash, [])
    AC_CHECK_PROG(ZSH_SHELL, zsh, zsh, [])


    EXPORT_SHELL_FILES=""
    SETENV_SHELL_FILES=""

    AC_SCOREP_SET_SHELL_SELECTION([$SH_SHELL])

    if test "x$SHELL_SET_ENV" == "xexport"; then
	_check_for_presence=`echo $EXPORT_SHELL_FILES | grep "$SHELL_FILE"`
	if test "x$_check_for_presence" = "x"; then
	    EXPORT_SHELL_FILES="$EXPORT_SHELL_FILES $SHELL_FILE"
	fi
    fi
    if test "x$SHELL_SET_ENV" == "xsetenv"; then
	_check_for_presence=`echo $SETENV_SHELL_FILES | grep "$SHELL_FILE"`
	if test "x$_check_for_presence" = "x"; then
	    SETENV_SHELL_FILES="$SETENV_SHELL_FILE $SHELL_FILE"
	fi
    fi



    AC_SCOREP_SET_SHELL_SELECTION([$BASH_SHELL])
    if test "x$SHELL_SET_ENV" == "xexport"; then
	_check_for_presence=`echo $EXPORT_SHELL_FILES | grep "$SHELL_FILE"`
	if test "x$_check_for_presence" = "x"; then
	    EXPORT_SHELL_FILES="$EXPORT_SHELL_FILES $SHELL_FILE"
	fi
    fi
    if test "x$SHELL_SET_ENV" == "xsetenv"; then
	_check_for_presence=`echo $SETENV_SHELL_FILES | grep "$SHELL_FILE"`
	if test "x$_check_for_presence" = "x"; then
	    SETENV_SHELL_FILES="$SETENV_SHELL_FILE $SHELL_FILE"
	fi
    fi

    AC_SCOREP_SET_SHELL_SELECTION([$CSH_SHELL])
    if test "x$SHELL_SET_ENV" == "xexport"; then
	_check_for_presence=`echo $EXPORT_SHELL_FILES | grep "$SHELL_FILE"`
	if test "x$_check_for_presence" = "x"; then
	    EXPORT_SHELL_FILES="$EXPORT_SHELL_FILES  $SHELL_FILE"
	fi
    fi
    if test "x$SHELL_SET_ENV" == "xsetenv"; then
	_check_for_presence=`echo $SETENV_SHELL_FILES | grep "$SHELL_FILE"`
	if test "x$_check_for_presence" = "x"; then
	    SETENV_SHELL_FILES="$SETENV_SHELL_FILE $SHELL_FILE"
	fi
    fi

    AC_SCOREP_SET_SHELL_SELECTION([$TCSH_SHELL])
    if test "x$SHELL_SET_ENV" == "xexport"; then
	_check_for_presence=`echo $EXPORT_SHELL_FILES | grep "$SHELL_FILE"`
	if test "x$_check_for_presence" = "x"; then
	    EXPORT_SHELL_FILES="$EXPORT_SHELL_FILES  $SHELL_FILE"
	fi
    fi
    if test "x$SHELL_SET_ENV" == "xsetenv"; then
	_check_for_presence=`echo $SETENV_SHELL_FILES | grep "$SHELL_FILE"`
	if test "x$_check_for_presence" = "x"; then
	    SETENV_SHELL_FILES="$SETENV_SHELL_FILE $SHELL_FILE"
	fi
    fi

    AC_SCOREP_SET_SHELL_SELECTION([$KSH_SHELL])
    if test "x$SHELL_SET_ENV" == "xexport"; then
	_check_for_presence=`echo $EXPORT_SHELL_FILES | grep "$SHELL_FILE"`
	if test "x$_check_for_presence" = "x"; then
	    EXPORT_SHELL_FILES="$EXPORT_SHELL_FILES $SHELL_FILE"
	fi
    fi
    if test "x$SHELL_SET_ENV" == "xsetenv"; then
	_check_for_presence=`echo $SETENV_SHELL_FILES | grep "$SHELL_FILE"`
	if test "x$_check_for_presence" = "x"; then
	    SETENV_SHELL_FILES="$SETENV_SHELL_FILE $SHELL_FILE"
	fi
    fi

    AC_SCOREP_SET_SHELL_SELECTION([$ASH_SHELL])
    if test "x$SHELL_SET_ENV" == "xexport"; then
	_check_for_presence=`echo $EXPORT_SHELL_FILES | grep "$SHELL_FILE"`
	if test "x$_check_for_presence" = "x"; then
	    EXPORT_SHELL_FILES="$EXPORT_SHELL_FILES $SHELL_FILE"
	fi
    fi
    if test "x$SHELL_SET_ENV" == "xsetenv"; then
	_check_for_presence=`echo $SETENV_SHELL_FILES | grep "$SHELL_FILE"`
	if test "x$_check_for_presence" = "x"; then
	    SETENV_SHELL_FILES="$SETENV_SHELL_FILE $SHELL_FILE"
	fi
    fi

    AC_SCOREP_SET_SHELL_SELECTION([$ZSH_SHELL])
    if test "x$SHELL_SET_ENV" == "xexport"; then
	_check_for_presence=`echo $EXPORT_SHELL_FILES | grep "$SHELL_FILE"`
	if test "x$_check_for_presence" = "x"; then
	    EXPORT_SHELL_FILES="$EXPORT_SHELL_FILES $SHELL_FILE"
	fi
    fi
    if test "x$SHELL_SET_ENV" == "xsetenv"; then
	_check_for_presence=`echo $SETENV_SHELL_FILES | grep "$SHELL_FILE"`
	if test "x$_check_for_presence" = "x"; then
	    SETENV_SHELL_FILES="$SETENV_SHELL_FILE $SHELL_FILE"
	fi
    fi

    AC_SUBST([EXPORT_SHELL_FILES])
    AC_SUBST([SETENV_SHELL_FILES])


])

#    * Shell de Stephen Bourne
#         o Bourne shell (sh)
#         o Bourne-Again shell (bash)
#    * Csh : C shell
#          o tcsh (Tenex C shell, version moderne du csh)
#    * Shell de David Korn (en) : Korn shell (ksh)
#    * Shell de Kenneth Almquis (en), utilisé lorsqu'il est nécessaire d'avoir un shell prenant peu de place sur le disque, clone de la variante SVR4 du Bourne shell;
#          o Ash (Almquist SHell)
#          o Dash shell (Debian Almquist SHell)
#    * Z Shell (zsh), reprenant les fonctions les plus pratiques de bash, ksh et tcsh.


# taken from http://en.wikipedia.org/wiki/Unix_shell
AC_DEFUN([AC_SCOREP_SET_SHELL_SELECTION],
[
    SHELL_FILE=""
    SHELL_SET_ENV=""
    case "$1" in
    "sh")
	SHELL_FILE="~/.profile"
	SHELL_SET_ENV="export"
	;;
    "bash")
	SHELL_FILE="~/.bashrc"
	SHELL_SET_ENV="export"
	;;
    "csh")
	SHELL_FILE="~/.cshrc"
	SHELL_SET_ENV="setenv"	
	;;
    "tcsh")
	SHELL_FILE="~/.tcshrc"
	SHELL_SET_ENV="setenv"	
	;;
    "ksh")
	SHELL_FILE="~/.profile"
	SHELL_SET_ENV="export"
	;;
    "ash")
	SHELL_FILE="~/.profile"
	SHELL_SET_ENV="export"	
	;;
    "dash")
	SHELL_FILE="~/.profile"
	SHELL_SET_ENV="export"	
	;;
    "zsh")
	SHELL_FILE="~/.zshrc"
	SHELL_SET_ENV="export"	
	;;
    esac 
])


# taken from http://en.wikipedia.org/wiki/Unix_shell
AC_DEFUN([AC_SCOREP_ADJUST_ENVIRONMENT],
[
   AC_SCOREP_SHELL_DETECTION

    EXPORT_SET_CUBE_DIR="export CUBE_DIR=$prefix"
    EXPORT_EXTEND_PATH="export PATH=\$\$PATH:$prefix/bin"
    EXPORT_EXTEND_LD_LIBRARY_PATH="export LD_LIBRARY_PATH=\$\$LD_LIBRARY_LIB:$prefix/lib:$prefix/lib/backend"
    EXPORT_EXTEND_MAN="export MAN=\$\$MAN:$prefix/man"
    EXPORT_EXTEND_MODULEPATH="export MODULEPATH=\$\$MODULEPATH:$datadir/modulefiles"

    SETENV_SET_CUBE_DIR="setenv CUBE_DIR $prefix"
    SETENV_EXTEND_PATH="setenv PATH \$\$PATH:$prefix/bin"
    SETENV_EXTEND_LD_LIBRARY_PATH="setenv LD_LIBRARY_PATH \$\$LD_LIBRARY_LIB:$prefix/lib:$prefix/lib/backend"
    SETENV_EXTEND_MAN="setenv MAN \$\$MAN:$prefix/man"
    SETENV_EXTEND_MODULEPATH="setenv MODULEPATH \$\$MODULEPATH:$datadir/modulefiles"


    AC_SUBST([EXPORT_SET_CUBE_DIR])
    AC_SUBST([EXPORT_EXTEND_PATH])
    AC_SUBST([EXPORT_EXTEND_LD_LIBRARY_PATH])
    AC_SUBST([EXPORT_EXTEND_MAN])
    AC_SUBST([EXPORT_EXTEND_MODULEPATH])

    AC_SUBST([SETENV_SET_CUBE_DIR])
    AC_SUBST([SETENV_EXTEND_PATH])
    AC_SUBST([SETENV_EXTEND_LD_LIBRARY_PATH])
    AC_SUBST([SETENV_EXTEND_MAN])
    AC_SUBST([SETENV_EXTEND_MODULEPATH])

])
