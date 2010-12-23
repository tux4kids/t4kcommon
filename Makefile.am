## Top-level Makefile.am for t4k_common library
## Process with Automake to create Makefile.in

SUBDIRS = data \
          linebreak \
	  po	\
	  src

EXTRA_DIST = CMakeLists.txt \
             aminclude.am

ACLOCAL_AMFLAGS = -I m4

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = t4k_common.pc

## For Doxygen support:
include aminclude.am
