## Top-level Makefile.am for t4k_common library
## Process with Automake to create Makefile.in

SUBDIRS = data \
	  po	\
	  src

EXTRA_DIST = CMakeLists.txt \
	     config.h.cmake \
             aminclude.am \
             cmake-modules/ConfigureChecks.cmake \
             cmake-modules/ConfigureChecksIntl.cmake \
             cmake-modules/FindRSVG.cmake \
             cmake-modules/FindIconv.cmake \
             cmake-modules/FindSDL_Pango.cmake \
             cmake-modules/T4K_Sugar.cmake \
             cmake-modules/UseDoxygen.cmake

ACLOCAL_AMFLAGS = -I m4

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = t4k_common.pc

## For Doxygen support:
include aminclude.am

# If using AX_PREFIX_CONFIG_H, distclean leaves some files so get rid of them 
# with custom rule:
#distclean-local: distclean-ax-prefix-config-h
#distclean-ax-prefix-config-h:
#	        rm -f _configs.sed t4k_common-config.h
