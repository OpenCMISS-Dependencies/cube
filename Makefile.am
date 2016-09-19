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


## The SUBDIRS variable holds a list of subdirectories automake (recursivly)
## must build.
SUBDIRS = $(subdirs) $(subdirs_custom)

## Options passed to aclocal to specify local macro directories. Although
## ACLOCAL_AMFLAGS should be replaced by AC_CONFIG_MACRO_DIRS, aclocal-1.13.4
## doesn't honor the paths provided to AC_CONFIG_MACRO_DIRS. See
## https://debbugs.gnu.org/cgi/bugreport.cgi?bug=21352
ACLOCAL_AMFLAGS = -I build-config/m4 -I vendor/common/build-config/m4

PHONY_TARGETS =





if PLATFORM_MIC
AM_TARGET=/mic
else
AM_TARGET=
endif



include vendor/common/build-config/Makefile.inc.am
include vendor/windows/Makefile.win32.inc.am
include build-config/platforms/platforms.am
include vendor/common/build-config/platforms.am




## To clean up configuration summary files
DISTCLEANFILES += \
    $(builddir)/config.summary \
    $(builddir)/@PACKAGE@.summary


## Still, sometimes there are files that must be distributed, but which are not
## covered in the automatic rules. These files should be listed in the
## EXTRA_DIST variable. You can mention files from subdirectories in EXTRA_DIST.
##
## You can also mention a directory in EXTRA_DIST. In this case the entire
## directory will be recursively copied into the distribution. Please note that
## this will also copy everything in the directory, including CVS/RCS version
## control files. We recommend against using this feature.
EXTRA_DIST += $(srcdir)/src/config.h.in \
	      $(srcdir)/src/cube-version.h.in \
	      $(srcdir)/src/cubelib-config-custom.h \
              $(srcdir)/build-config/VERSION \
              $(srcdir)/build-config/REVISION \
              $(srcdir)/build-config/REVISION_COMMON \
	      $(srcdir)/vendor/common/build-config/generate-library-version.sh \
	      $(srcdir)/vendor/common/build-config/generate-package-version.sh \
	      $(srcdir)/share/icons/Cube.* \
	      $(srcdir)/share/icons/cubelogo.* \
	      $(srcdir)/share/icons/cubeicon.* \
	      $(srcdir)/share/cubelib/* \
	      $(srcdir)/doc/Cube*.pdf \
	      $(srcdir)/CUBE_LATEST_RELEASE



if ! CROSS_BUILD
EXTRA_DIST += $(srcdir)/build-frontend/configure.ac \
              $(srcdir)/build-frontend/aclocal.m4   \
              $(srcdir)/build-frontend/Makefile.am  \
              $(srcdir)/build-frontend/Makefile.in  \
              $(srcdir)/build-frontend/configure    \
              $(srcdir)/src/config-frontend.h.in
endif

#.PHONY: doxygen-project-number update-revision clean-doxygen-user doxygen-user doxygen-user-pdf beautify beautify-clean

#install-data-local: doxygen-user-install
CLEANFILES += \
    src/tools/tools/0002.Config/cubelib_config_usage.h 

#uninstall-local: doxygen-user-uninstall

#dist-hook: prohibit-svn-files-in-dist

#dist-hook: doxygen-user-generate

#clean-local: doxygen-user-clean

CLEANFILES +=
DISTCLEANFILES +=\
		$(builddir)/config.summary \
                $(builddir)/@PACKAGE@.summary \
		include/cubelib/cube_error_decls.gen.h

uninstall-local:
	rm -rf $(DESTDIR)$(includedir)/cube
	rm -rf $(DESTDIR)$(includedir)/cubew
	rm -rf $(DESTDIR)$(includedir)/cubegui
	rm -rf $(DESTDIR)$(datadir)/@PACKAGE_NAME@
	rm -rf $(DESTDIR)$(datadir)/@PACKAGE@
	rm -rf $(DESTDIR)$(datadir)/icons/@PACKAGE@
	rm -rf $(DESTDIR)$(datadir)/@PACKAGE@
	rm -rf $(DESTDIR)$(datadir)/doc/@PACKAGE@
	rm -rf $(DESTDIR)$(datadir)/modulefiles/@PACKAGE_NAME@
	rm -f  $(DESTDIR)$(exec_prefix)/bin/cube-config

install-exec-local:
	$(INSTALL) bin/cube-config $(DESTDIR)$(exec_prefix)/bin/

install-data-local:
	@echo "Install version information "
	$(MKDIR_P) $(DESTDIR)$(datadir)/@PACKAGE@$(AM_TARGET)
	@$(INSTALL_DATA) src/cube-version.h  $(DESTDIR)$(includedir)/cube/cube-version.h
	@$(INSTALL_DATA) src/cube-version.h  $(DESTDIR)$(includedir)/cubew/cube-version.h
	@$(INSTALL_DATA) @PACKAGE@.summary $(DESTDIR)$(datadir)/@PACKAGE@$(AM_TARGET)/@PACKAGE@.summary
	@echo "=============================================================="
	$(MKDIR_P) $(DESTDIR)$(datadir)/doc/cube/
	$(MKDIR_P) $(DESTDIR)$(datadir)/icons/cube
	@$(INSTALL_DATA) $(srcdir)/share/icons/Cube.* $(DESTDIR)$(datadir)/icons/cube
	@$(INSTALL_DATA) $(srcdir)/share/icons/cubelogo.* $(DESTDIR)$(datadir)/icons/cube
	@$(INSTALL_DATA) $(srcdir)/share/icons/cubeicon.* $(DESTDIR)$(datadir)/icons/cube
	$(MKDIR_P) $(DESTDIR)$(datadir)/doc
	@$(INSTALL_DATA) $(srcdir)/doc/Cube*.pdf $(DESTDIR)$(datadir)/doc/@PACKAGE@
	@echo "=============================================================="



if SVN_CONTROLLED

update-revision-file:
	@svnversion $(srcdir) > "$(srcdir)/build-config/REVISION"

else !SVN_CONTROLLED

update-revision-file:

endif !SVN_CONTROLLED




















available: shell_available desktop_available

shell_available:
if !INSTALL_MODULE_FILE
if !ANOTHER_CUBE
	@echo "=============================================================="
if CROSS_BUILD
	@echo "Cube provides c-writer library for frontend and for backend as well. "
	@echo "Frontend library is installed in @prefix@/lib"
	@echo "Backend library is installed in @prefix@/lib/backend"
endif
	@for shell_file in @EXPORT_SHELL_FILES@; do \
	echo "Adopt file $$shell_file"; \
	echo "  Extend PATH, LD_LIBRARY_PATH and MAN ..." ;\
	echo "" >> $$shell_file ; \
	echo '@EXPORT_EXTEND_PATH@' >> $$shell_file ;\
	echo '@EXPORT_EXTEND_LD_LIBRARY_PATH@' >> $$shell_file; \
	echo '@EXPORT_EXTEND_MAN@' >> $$shell_file; \
	echo "  Set variable CUBE_DIR to @prefix@" ;\
	echo "" >> $$shell_file ; \
	echo '@EXPORT_SET_CUBE_DIR@' >> $$shell_file; \
	done
	@for shell_file in @SETENV_SHELL_FILES@; do \
	echo "Adopt file $$shell_file" ;\
	echo "  Extend PATH, LD_LIBRARY_PATH and MAN ..." ;\
	echo "" >> $$shell_file ; \
	echo '@SETENV_EXTEND_PATH@' >> $$shell_file ;\
	echo '@SETENV_EXTEND_LD_LIBRARY_PATH@' >> $$shell_file; \
	echo '@SETENV_EXTEND_MAN@' >> $$shell_file; \
	echo "  Set variable CUBE_DIR to @prefix@" ;\
	echo '@SETENV_SET_CUBE_DIR@' >> $$shell_file; \
	done
	@echo "=============================================================="
else
	@echo "=============================================================="
	@echo "Because of the presence of another CUBE installation, variables PATH and CUBE_DIR are not set. Please change them manually to point to @prefix@ if needed..."
	@echo "=============================================================="
endif
else
if !ANOTHER_CUBE
	@for shell_file in @EXPORT_SHELL_FILES@; do \
	echo "Adopt file $$shell_file";\
	echo "Set the variable MODULEPATH..." ;\
	touch $$shell_file ;\
	echo "" >> $$shell_file ;\
	echo '@EXPORT_EXTEND_MODULEPATH@' >> $$shell_file; \
	done
	@for shell_file in @SETENV_SHELL_FILES@; do \
	echo "Adopt file $$shell_file" ;\
	echo "Set the variable MODULEPATH..." ;\
	touch $$shell_file ;\
	echo "" >> $$shell_file ;\
	echo '@SETENV_EXTEND_MODULEPATH@' >> $$shell_file; \
	done
	@echo "=============================================================="
else
	@echo "=============================================================="
	@echo 'Because of the presence of another CUBE installation, variable MODULEPATH is not set. Pleas adopt it manually.'
	@echo "=============================================================="
endif
else
	@echo "=============================================================="
	@echo 'Package "modules" is not installed. Do not install modulefiles for @PACKAGE_NAME@'
	@echo "=============================================================="
endif


desktop_available: @DEBIAN_AVAILABLE@
	@for cube_desktop_file_dir in @DESKTOP_FILES_DIR_LIST@; do \
	echo "Copy file CUBE.desktop into $$cube_desktop_file_dir"; \
	$(MKDIR_P) $$cube_desktop_file_dir/;\
	cp CUBE.desktop $$cube_desktop_file_dir/ ;\
	done

debian_available:
	@for cube_debian_file_dir in @DEBIAN_FILES_DIR_LIST@; do \
	echo "Copy file cube.debian to $$cube_debian_file_dir"; \
	$(MKDIR_P) $$cube_debian_file_dir/;\
	cp cube.debian $$cube_debian_file_dir/cube ;\
	done
	@echo -n "_Update menus..."
	@update-menus
	@echo "done"



release: release-create-dirs  release-create-docs

release-create-dirs:
	mkdir -p _release/scalasca/releases/@PACKAGE_NAME@/@PACKAGE_VERSION@/dist
	mkdir -p _release/scalasca/releases/@PACKAGE_NAME@/@PACKAGE_VERSION@/docs/manual
	mkdir -p _release/scalasca/releases/@PACKAGE_NAME@/@PACKAGE_VERSION@/docs/install
	mkdir -p _release/scalasca/releases/@PACKAGE_NAME@/@PACKAGE_VERSION@/docs/derivedmetrics
	mkdir -p _release/scalasca/releases/@PACKAGE_NAME@/@PACKAGE_VERSION@/docs/tools

release-create-docs:doxygen-gui doxygen-installation doxygen-derived-metrics doxygen-examples
	cp $(srcdir)/doc/*.pdf _release/scalasca/releases/@PACKAGE_NAME@/@PACKAGE_VERSION@/docs
	cp @PACKAGE_NAME@-@PACKAGE_VERSION@/doc/gui/html/* _release/scalasca/releases/@PACKAGE_NAME@/@PACKAGE_VERSION@/docs/manual
	cp @PACKAGE_NAME@-@PACKAGE_VERSION@/doc/installation/html/* _release/scalasca/releases/@PACKAGE_NAME@/@PACKAGE_VERSION@/docs/install
	cp $(srcdir)/INSTALL _release/scalasca/releases/@PACKAGE_NAME@/@PACKAGE_VERSION@/docs/INSTALL.txt
	cp @PACKAGE_NAME@-@PACKAGE_VERSION@/doc/derived-metrics/html/* _release/scalasca/releases/@PACKAGE_NAME@/@PACKAGE_VERSION@/docs/derivedmetrics
	cp @PACKAGE_NAME@-@PACKAGE_VERSION@/doc/examples/html/* _release/scalasca/releases/@PACKAGE_NAME@/@PACKAGE_VERSION@/docs/tools

release-create-dist: dist
	cp @PACKAGE_NAME@-@PACKAGE_VERSION@.tar.gz _release/scalasca/releases/@PACKAGE_NAME@/@PACKAGE_VERSION@/dist

release-copy-releaseinfo:
	cp $(srcdir)/CUBE_LATEST_RELEASE _release/scalasca/releases/@PACKAGE_NAME@/



##--- Include Makefile.am snippets ------------------------------------------

include doc/Makefile.top.inc.am



.PHONY: $(PHONY_TARGETS)
