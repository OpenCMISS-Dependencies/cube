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


# scorep--backend
## Options passed to aclocal to specify local macro directories. Although
## ACLOCAL_AMFLAGS should be replaced by AC_CONFIG_MACRO_DIRS, aclocal-1.13.4
## doesn't honor the paths provided to AC_CONFIG_MACRO_DIRS. See
## https://debbugs.gnu.org/cgi/bugreport.cgi?bug=21352
ACLOCAL_AMFLAGS = -I ../build-config/m4 -I ../vendor/common/build-config/m4

## The SUBDIRS variable holds a list of subdirectories automake (recursivly)
## must build.
#SUBDIRS =

include ../build-includes/common.am


AM_CPPFLAGS += -DBACKEND_BUILD_NOMPI $(BACKEND_LIBZ_HEADERS)
AM_LDFLAGS += $(BACKEND_LIBZ_LD)

bindir = @bindir@@backend_suffix@
libdir = @libdir@@backend_suffix@

if CROSS_BUILD

AM_CPPFLAGS += -DCROSS_BUILD $(BACKEND_LIBZ_HEADERS)
AM_LDFLAGS += $(BACKEND_LIBZ_LD)

include ../build-includes/backend-only.am
include ../build-includes/front-and-backend.am


else

AM_CPPFLAGS += -DNOCROSS_BUILD $(FRONTEND_LIBZ_HEADERS) $(BACKEND_LIBZ_HEADERS)
AM_LDFLAGS += $(FRONTEND_LIBZ_LD) $(BACKEND_LIBZ_LD)
include ../build-includes/backend-only.am
include ../build-includes/front-and-backend.am
include ../build-includes/frontend-only.am


TESTS+= $(CUBE_FRONTEND_SELF_TESTS)
#if BACKEND_TEST_RUNS
#TESTS+=$(CUBE_BACKEND_SELF_TESTS)
#$(CUBE_C_WRITER_TESTS) $(CUBE_PARALLEL_WRITER_TESTS)
#$(CUBE_CPPLIB_TESTS) $(CUBE_TOOLS_TESTS) $(CUBE_GUI_TESTS)


#endif

endif


if PLATFORM_MIC
AM_TARGET=/mic
else
AM_TARGET=
endif



#CONFIG_HEADER_BACKEND = ../src/scorep_config_tool_backend.h
#CONFIG_HEADER_FRONTEND = ../src/scorep_config_tool_frontend.h

DISTCLEANFILES += $(BUILT_SOURCES)  \
		$(builddir)/config.summary \
                $(builddir)/@PACKAGE@.summary

#if !BACKEND_TEST_RUNS
#BUILT_SOURCES += cube_tests
#
##cube_tests:
#	rm -f cube_tests
#	echo "# some cube tests" > cube_tests
#	for i in "$(CUBE_SELF_TESTS)"; do echo "$$i" >> cube_tests; done
#endif


## Convenience variable, used in common.am
BUILD_DIR=build-backend




#EXTRA_LOCAL_CLEAN_TARGETS = clean-local-cube-config-tool

clean-local:
	rm -f cube_tests lex.yy.c scanner.h yacc.c yacc.h

.PHONY: $(PHONY_TARGETS) $(EXTRA_LOCAL_CLEAN_TARGETS)

distclean-local:




if CROSS_BUILD
install-exec-local:
	$(INSTALL) ../bin/cube-config-backend $(DESTDIR)$(exec_prefix)/bin/
install-data-local: create-backend-examples-dir  install-backend-examples
else
install-exec-local:
	$(INSTALL) ../bin/cube-config-backend $(DESTDIR)$(exec_prefix)/bin/
	$(INSTALL) ../bin/cube-config-frontend $(DESTDIR)$(exec_prefix)/bin/
install-data-local:  create-backend-examples-dir install-backend-examples create-frontend-examples-dir install-frontend-examples
endif
	@echo "Install example building controlling Makefile..."
	@$(INSTALL_DATA) $(abs_builddir)/../examples/Makefile $(DESTDIR)$(datadir)/doc/@PACKAGE@/example
	@echo "-------------------Copy examples execution scripts ---------------"
# 	$(INSTALL_DATA) exec.@BATCHSYS@  $(DESTDIR)$(datadir)/@PACKAGE@/doc/example/backend



create-backend-examples-dir:
	@$(MKDIR_P) $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/backend


install-backend-examples: $(INST_BACKEND_EXAMPLES)
	@echo "== Install backend examples"; \
	for example in $(INST_BACKEND_EXAMPLES); do \
	echo  "   Install backend_example $$example"; \
	$(INSTALL_DATA) $$example  $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/backend; \
	done
	@echo "   Install a Makefile.backend for building examples using installed cube"
	@$(INSTALL_DATA) $(abs_builddir)/../examples/Makefile.backend $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/backend


if !CROSS_BUILD
create-frontend-examples-dir:
	@$(MKDIR_P) $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/gui/plugin-simple/
	@$(MKDIR_P) $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/gui/plugin-demo/
	@$(MKDIR_P) $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/gui/plugin-colormap/
	@$(MKDIR_P) $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/gui/plugin-context-free/


install-frontend-examples: $(INST_FRONTEND_EXAMPLES) $(INST_FRONTEND_PLUGIN_EXAMPLES)
	@echo "== Install frontend examples"; \
	for example in $(INST_FRONTEND_EXAMPLES) dummy; do \
            if test $$example != dummy; then \
                echo  "   Install example $$example"; \
                $(INSTALL_DATA) $$example $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/; \
            fi; \
	done
	@echo "== Install GUI plugin examples"; \
	for example in $(EXAMPLE_PLUGIN_SIMPLE) dummy; do \
            if test $$example != dummy; then \
                echo  "   Install example $$example"; \
                $(INSTALL_DATA) $$example $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/gui/plugin-simple/; \
            fi; \
	done
	@for example in $(EXAMPLE_PLUGIN_DEMO) dummy; do \
            if test $$example != dummy; then \
                echo  "   Install example $$example"; \
                $(INSTALL_DATA) $$example $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/gui/plugin-demo; \
            fi; \
	done
	@for example in $(EXAMPLE_PLUGIN_COLOR) dummy; do \
            if test $$example != dummy; then \
                echo  "   Install example $$example"; \
                $(INSTALL_DATA) $$example $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/gui/plugin-colormap; \
            fi; \
	done
	@for example in $(EXAMPLE_PLUGIN_CONTEXTFREE) dummy; do \
            if test $$example != dummy; then \
                echo  "   Install example $$example"; \
                $(INSTALL_DATA) $$example $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/gui/plugin-context-free; \
            fi; \
	done
	@echo "   Install a Makefile for building frontend examples using installed cube"
	@$(INSTALL_DATA) $(abs_builddir)/../examples/Makefile.frontend $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/
	@echo "   Install exampls of a .cubex files to try it out with tools"
	@$(INSTALL_DATA) $(srcdir)/../examples/cube/*.cubex $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/
endif



uninstall-local:
	rm -rf $(DESTDIR)$(datadir)/@PACKAGE@
	rm -f $(DESTDIR)$(exec_prefix)/bin/cube-config-backend
	rm -f $(DESTDIR)$(exec_prefix)/bin/cube-config-frontend


#######################################################################################
# If the plugin doesn't load, the most likely reason is an undefined reference.
# To check this, set CHECK_PLUGIN to the plugin file and call "make check-plugin"
# in the build-backend directory
#######################################################################################
check-plugin:
	@if test -n "$(CHECK_PLUGIN)"; then \
        $(CXXLD) $(CHECK_PLUGIN) \
        .libs/libcube4gui.so .libs/libcube4.so \
        $(SUBLIBS) $(QT_LIBS) \
        $(FRONTEND_LIBZ_LD) \
        $(DBUS_LDFLAGS) \
        $(cube_LDFLAGS) \
        $(SUBLIBS) \
        $(QT_LIBS) \
        $(cube_OBJECTS) && echo "$(CHECK_PLUGIN) passed test for undefined references"; \
        else \
          echo "export CHECK_PLUGIN=PLUGINPATH to check the given plugin file for undefined references";\
	fi
