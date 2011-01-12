## Top-level Makefile.am for t4k_common library
## Process with Automake to create Makefile.in

SUBDIRS = data \
	  po	\
	  src

EXTRA_DIST = CMakeLists.txt \
             aminclude.am \
             cmake-modules/ConfigureChecksIntl.cmake \
             cmake-modules/FindRSVG.cmake \
             cmake-modules/FindSDL_Pango.cmake \
             cmake-modules/T4K_Sugar.cmake \
             cmake-modules/UseDoxygen.cmake

ACLOCAL_AMFLAGS = -I m4

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = t4k_common.pc

## For Doxygen support:
include aminclude.am

# If using AX_PREFIX_CONFIG_H, distclean leaves some files so get rid of them (HACK)
distclean-hdr:
	-rm -f config.h stamp-h1 _configs.sed t4k_common-config.h	
