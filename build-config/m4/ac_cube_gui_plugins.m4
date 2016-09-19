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
# Simple check for shipped plugins

AC_DEFUN([AC_CUBE_SUPPORTED_PLUGINS],
[
## plugin example
AC_MSG_CHECKING([ plugin "Launch"])
AC_CUBE_PLUGIN_LAUNCH

## plugin graphwidgetcommon
AC_MSG_CHECKING([ plugin "graphwidgetcommon"])
AC_CUBE_PLUGIN_GRAPHWIDGETCOMMON
AM_CONDITIONAL([WITH_GUI_PLUGIN_GRAPHWIDGET_COMMON], [test x$WITH_GUI_PLUGIN_GRAPHWIDGETCOMMON == "xyes"])


## plugin barplot
AC_MSG_CHECKING([ plugin "Barplot"])
AC_CUBE_PLUGIN_BARPLOT
AM_CONDITIONAL([WITH_GUI_PLUGIN_BARPLOT], [test x$WITH_GUI_PLUGIN_BARPLOT == "xyes"])

## plugin heatmap
AC_MSG_CHECKING([ plugin "Heatmap"])
AC_CUBE_PLUGIN_HEATMAP
AM_CONDITIONAL([WITH_GUI_PLUGIN_HEATMAP], [test x$WITH_GUI_PLUGIN_HEATMAP == "xyes"])

## plugin source
AC_MSG_CHECKING([ plugin "SourceEditor"])
AC_CUBE_PLUGIN_SOURCE_EDITOR
AM_CONDITIONAL([WITH_GUI_PLUGIN_SOURCE_EDITOR], [test x$WITH_GUI_PLUGIN_SOURCE_EDITOR == "xyes"])

## plugin statistics
AC_MSG_CHECKING([ plugin "Statistics"])
AC_CUBE_PLUGIN_STATISTICS
AM_CONDITIONAL([WITH_GUI_PLUGIN_STATISTICS], [test x$WITH_GUI_PLUGIN_STATISTICS == "xyes"])

## plugin vampir
AC_CUBE_PLUGIN_VAMPIR
AM_CONDITIONAL([WITH_GUI_PLUGIN_VAMPIR], [test x$WITH_GUI_PLUGIN_VAMPIR == "xyes"])

## plugin paraver
AC_MSG_CHECKING([ plugin "Paraver"])
AC_CUBE_PLUGIN_PARAVER
AM_CONDITIONAL([WITH_GUI_PLUGIN_PARAVER], [test x$WITH_GUI_PLUGIN_PARAVER == "xyes"])

## plugin paraver
AC_MSG_CHECKING([ plugin "AdvancedColorMaps"])
AC_CUBE_PLUGIN_ADVANCEDCM
AM_CONDITIONAL([WITH_GUI_PLUGIN_ADVANCEDCM], [test x$WITH_GUI_PLUGIN_ADVANCEDCM == "xyes"])
])




AC_DEFUN([AC_CUBE_PLUGIN_LAUNCH],
[
WITH_GUI_PLUGIN_LAUNCH=yes
AC_ARG_WITH(plugin-launch, [AS_HELP_STRING([--with-plugin-launch | --without-plugin-launch], 
          [ Enables (default) or disables build and installation of a plugin "Launch"])], [], [])

AS_IF( [ test x$with_plugin_launch = "xno" || test x$without_plugin_launch = "xyes" ], [
WITH_GUI_PLUGIN_LAUNCH="no";
AC_MSG_RESULT([ deactivate.])
], [ 
AC_MSG_RESULT([ activate.])
])


])


AC_DEFUN([AC_CUBE_PLUGIN_BARPLOT],
[
WITH_GUI_PLUGIN_BARPLOT=yes
AC_ARG_WITH(plugin-barplot, [AS_HELP_STRING([--with-plugin-barplot| --without-plugin-barplot ], 
          [ Enables (default) or disables build and installation of a plugin "Barplot"])], [], [])

AS_IF( [ test x$with_plugin_barplot = "xno" || test x$without_plugin_barplot = "xyes"], [
WITH_GUI_PLUGIN_BARPLOT="no";
AC_MSG_RESULT([ deactivate.])
], [ 
AC_MSG_RESULT([ activate.])
])
])

AC_DEFUN([AC_CUBE_PLUGIN_HEATMAP],
[
WITH_GUI_PLUGIN_HEATMAP=yes
AC_ARG_WITH(plugin-heatmap, [AS_HELP_STRING([--with-plugin-heatmap | --without-plugin-heatmap], 
          [ Enables (default) or disables build and installation of a plugin "Heatmap"])], [], [])

AS_IF( [ test x$with_plugin_heatmap = "xno" || test x$without_plugin_heatmap = "xyes"], [
WITH_GUI_PLUGIN_HEATMAP="no";
AC_MSG_RESULT([ deactivate.])
], [ 
AC_MSG_RESULT([ activate.])
])
])


AC_DEFUN([AC_CUBE_PLUGIN_STATISTICS],
[
WITH_GUI_PLUGIN_STATISTICS=yes
AC_ARG_WITH(plugin-statistics, [AS_HELP_STRING([--with-plugin-statistics | --without-plugin-statistics], 
          [ Enables (default) or disables build and installation of a plugin "Statistics"])], [], [])

AS_IF( [ test x$with_plugin_statistics = "xno" || test x$without_plugin_statistics = "xyes"], [
WITH_GUI_PLUGIN_STATISTICS="no";
AC_MSG_RESULT([ deactivate.])
], [ 
AC_MSG_RESULT([ activate.])
])
])


AC_DEFUN([AC_CUBE_PLUGIN_SOURCE_EDITOR],
[
WITH_GUI_PLUGIN_SOURCE_EDITOR=yes
AC_ARG_WITH(plugin-source, [AS_HELP_STRING([--with-plugin-source | --without-plugin-source], 
          [ Enables (default) or disables build and installation of a plugin "Source Code Viewer"])], [], [])

AS_IF( [ test x$with_plugin_source = "xno" || test x$without_plugin_source = "xyes"], [
WITH_GUI_PLUGIN_SOURCE_EDITOR="no";
AC_MSG_RESULT([ deactivate.])
], [ 
AC_MSG_RESULT([ activate.])
])
])


AC_DEFUN([AC_CUBE_PLUGIN_VAMPIR],
[
WITH_GUI_PLUGIN_VAMPIR=yes
AC_ARG_WITH(plugin-vampir, [AS_HELP_STRING([--with-plugin-vampir | --without-plugin-vampir], 
          [ Enables (default) or disables build and installation of a plugin "Vampir connection"])], [], [])

AS_IF( [ test x$with_plugin_vampir = "xno" || test x$without_plugin_vampir = "xyes"], [
WITH_GUI_PLUGIN_VAMPIR="no";
AC_MSG_CHECKING([ plugin "Vampir"])
AC_MSG_RESULT([ deactivate.])
], [ 
  WITH_GUI_PLUGIN_VAMPIR="yes"

  AC_DBUS_AVAILABLE

AC_MSG_CHECKING([ plugin "Vampir"])
AS_IF( [ test x$DBUS_AVAILABLE = "xno"], [
WITH_GUI_PLUGIN_VAMPIR="no"
AC_MSG_RESULT([ deactivate.])
],[
AC_MSG_RESULT([ activate.])
])

])
])

AC_DEFUN([AC_CUBE_PLUGIN_PARAVER],
[
WITH_GUI_PLUGIN_PARAVER=yes
AC_ARG_WITH(plugin-paraver, [AS_HELP_STRING([--with-plugin-paraver | --without-plugin-paraver], 
          [ Enables (default) or disables build and installation of a plugin "Paraver connection"])], [], [])

AS_IF( [ test x$with_plugin_paraver = "xno" || test x$without_plugin_paraver = "xyes"], [
WITH_GUI_PLUGIN_PARAVER="no";
AC_MSG_RESULT([ deactivate.])
], [ 
AC_MSG_RESULT([ activate.])
])
])



AC_DEFUN([AC_CUBE_PLUGIN_GRAPHWIDGETCOMMON],
[
WITH_GUI_PLUGIN_GRAPHWIDGETCOMMON=yes
AC_MSG_RESULT([ activate.]) 
#AC_ARG_WITH(plugin-graphwidgetcommon, [AS_HELP_STRING([--with-plugin-graphwidgetcommon | --without-plugin-graphwidgetcommon],  
          #[ Enables (default) or disables build and installation of a plugin "GraphwidgetCommon"])], [], []) 


#AS_IF( [ test x$with_plugin_graphwidgetcommon = "xno" ], [
#WITH_GUI_PLUGIN_GRAPHWIDGETCOMMON="no";
#AC_MSG_RESULT([ deactivate.])
#], [ 
#AC_MSG_RESULT([ activate.])
#])
AC_DEFUN([AC_CUBE_PLUGIN_ADVANCEDCM],
[
WITH_GUI_PLUGIN_ADVANCEDCM=yes
AC_ARG_WITH(plugin-advancedcolormaps, [AS_HELP_STRING([--with-plugin-advancedcolormaps | --without-plugin-advancedcolormaps], 
          [ Enables (default) or disables build and installation of a plugin "Advanced Color Maps"])], [], [])

AS_IF( [ test x$with_plugin_advancedcolormaps = "xno" || test x$without_plugin_advancedcolormaps = "xyes"], [
WITH_GUI_PLUGIN_ADVANCEDCM="no";
AC_MSG_RESULT([ deactivate.])
], [ 
AC_MSG_RESULT([ activate.])
])
])


])
