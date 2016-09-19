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





# scorep--frontend

## Options passed to aclocal to specify local macro directories. Although
## ACLOCAL_AMFLAGS should be replaced by AC_CONFIG_MACRO_DIRS, aclocal-1.13.4
## doesn't honor the paths provided to AC_CONFIG_MACRO_DIRS. See
## https://debbugs.gnu.org/cgi/bugreport.cgi?bug=21352
ACLOCAL_AMFLAGS = -I ../build-config/m4 -I ../vendor/common/build-config/m4

include ../build-includes/common.am
#include ../build-includes/mpi.targets.am

AM_CPPFLAGS += -DFRONTEND_BUILD -DCROSS_BUILD $(FRONTEND_LIBZ_HEADERS)
AM_LDFLAGS += $(FRONTEND_LIBZ_LD)


if PLATFORM_MIC
AM_TARGET=/mic
else
AM_TARGET=
endif


include ../build-includes/frontend-only.am
include ../build-includes/front-and-backend.am



TESTS+= $(CUBE_FRONTEND_SELF_TESTS)

#$(CUBE_CPPLIB_TESTS) $(CUBE_TOOLS_TEST) $(CUBE_GUI_TESTS)


## Convenience variable, used in common.am
BUILD_DIR=build-frontend



DISTCLEANFILES += $(BUILT_SOURCES)\
		$(builddir)/config.summary \
                $(builddir)/@PACKAGE@.summary

.PHONY: $(PHONY_TARGETS)


## Here we write only backend values.
create-config-tool-headers:
	(cd ../build-backend; $(MAKE) create-config-tool-headers)


install-data-local: create-frontend-examples-dir install-frontend-examples
install-exec-local:
	$(INSTALL) ../bin/cube-config-frontend $(DESTDIR)$(exec_prefix)/bin

uninstall-local:
	rm -f $(DESTDIR)$(exec_prefix)/bin/cube-config-frontend


create-frontend-examples-dir:
	@$(MKDIR_P) $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/gui/plugin-example/


install-frontend-examples: $(INST_FRONTEND_EXAMPLES) $(INST_FRONTEND_PLUGIN_EXAMPLES)
	@echo "== Install frontend examples"; \
	for example in $(INST_FRONTEND_EXAMPLES) dummy; do \
            if test $$example != dummy; then \
                echo  "   Install example $$example"; \
                $(INSTALL_DATA) $$example $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/; \
           fi; \
	done
	@echo "== Install GUI plugin examples"; \
	for example in $(INST_FRONTEND_PLUGIN_EXAMPLES) dummy; do \
            if test $$example != dummy; then \
                echo  "   Install example $$example"; \
                $(INSTALL_DATA) $$example $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/gui/plugin-example/; \
           fi; \
	done
	@echo "   Install a Makefile for building frontend examples using installed cube"
	@$(INSTALL_DATA) $(abs_builddir)/../examples/Makefile.frontend $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/
	@echo "   Install exampls of a .cubex files to try it out with tools"
	@$(INSTALL_DATA) $(srcdir)/../examples/cube/*.cubex $(DESTDIR)$(datadir)/doc/@PACKAGE@/example/
