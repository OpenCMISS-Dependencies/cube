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



AC_DEFUN([AC_SCOREP_KDEDETECTION],
[
    AC_CHECK_PROG(ISKDEINIT, kded, "yes", ["no"])
    IS_KDE_DIR=no
    KDE_DIR=
#    if test -d $HOME/.kde/share/applnk; then
	KDE_DIR="$KDE_DIR $HOME/.kde/share/applnk"
	IS_KDE_DIR="yes"
#    fi
#    if test -d $HOME/.local/share/applications; then
	KDE_DIR="$KDE_DIR $HOME/.local/share/applications/Science"
	IS_KDE_DIR="yes"
 #   fi



    if test "x$KDE_FULL_SESSION" = "xtrue"; then
	IS_KDE_DE="yes"
    fi

    IS_KDE=no
    # corrently we conclude that kde is installed by checkig if kdeinit is present
    if test "x$ISKDEINIT"="xyes"; then
	IS_KDE="yes"
    fi
])

AC_DEFUN([AC_SCOREP_GNOMEDETECTION],
[
    AC_CHECK_PROG([ISGNOMEPANEL], [gnome-panel], ["yes"], ["no"])
    GNOME_DIR=
    IS_GNOME_DIR="no"
#    if test -d $HOME/.gnome/apps/; then
	GNOME_DIR="$HOME/.gnome/apps"
	IS_GNOME_DIR="yes"
#    fi

    if test "x$DESKTOP_SESSION" = "xgnome"; then
	IS_GNOME_DE="yes"
    fi

    IS_GNOME=no
    # corrently we conclude that gnome is installed by checkig if gnome-panel is present
    if test "x$ISGNOMEPANEL" = "xyes"; then
	IS_GNOME="yes"
    fi


])


AC_DEFUN([AC_SCOREP_DEBIAN_MENU_DETECTION],
[
    AC_CHECK_PROG([IS_UPDATE_MENU], [update-menus], ["yes"], ["no"])
    DEBIAN_DIR=
    IS_DEBIAN_DIR="no"
#    if test -d /usr/share/menu; then
	DEBIAN_DIR="$HOME/.menu"
	IS_DEBIAN_DIR="yes"
#    fi
    IS_DEBIAN=no
    # corrently we conclude that it is debian if update-menus is present
    if test "x$IS_UPDATE_MENU" = "xyes"; then
	IS_DEBIAN="yes"
    fi
])



AC_DEFUN([AC_SCOREP_DESKTOP_DETECTION],
[
    AC_SCOREP_KDEDETECTION

    DESKTOP_FILES_DIR_LIST=""
    if test "x$IS_KDE" = "xyes"; then
	DESKTOP_FILES_DIR_LIST="$DESKTOP_FILES_DIR_LIST $KDE_DIR "
    fi

    AC_SCOREP_GNOMEDETECTION

    if test "x$IS_GNOME" = "xyes"; then
	DESKTOP_FILES_DIR_LIST="$DESKTOP_FILES_DIR_LIST $GNOME_DIR "
    fi
    #modify Makefile.in to copy .desktop files to proper directories
    AC_SUBST([DESKTOP_FILES_DIR_LIST])

    AC_SCOREP_DEBIAN_MENU_DETECTION

    DEBIAN_FILES_DIR_LIST=
    DEBIAN_AVAILABLE=
    if test "x$IS_DEBIAN" = "xyes"; then
	DEBIAN_FILES_DIR_LIST="$DEBIAN_FILES_DIR_LIST $DEBIAN_DIR "
	DEBIAN_AVAILABLE=debian_available
    fi
    #modify Makefile.in to copy CUBE files to debian menu directories
    AC_SUBST([DEBIAN_FILES_DIR_LIST])
    AC_SUBST([DEBIAN_AVAILABLE])


])


AC_DEFUN([AC_SCOREP_ADJUST_DESKTOP_ENVIRONMENT],
[
    AC_SCOREP_DESKTOP_DETECTION
])

