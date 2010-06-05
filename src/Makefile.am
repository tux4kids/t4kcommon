## Makefile.am for t4kcommon/trunk/src
## Process with Automake to create Makefile.in

AM_CFLAGS=-Wall -g -DCOMMON_DATA_PREFIX=\"${COMMON_DATA_PREFIX}\" -DDEBUG -DUSE_T4K_PREFIX=1

if BUILD_MINGW32
  COMMON_DATA_PREFIX=@MINGW32_PACKAGE_DATA_DIR@
else
  COMMON_DATA_PREFIX=${pkgdatadir}
endif

lib_LTLIBRARIES = libt4kcommon.la

libt4kcommon_la_SOURCES = \
	t4k-audio.c	\
	t4k-compiler.h	\
	t4k-globals.h	\
	t4k-loaders.c	\
	t4k-main.c	\
	t4k-menu.c	\
	t4k-pixels.c	\
	t4k-sdl.c

include_HEADERS = t4k-common.h
