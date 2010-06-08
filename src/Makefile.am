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
	t4k_audio.c	\
	t4k_compiler.h	\
	t4k_globals.h	\
	t4k_loaders.c	\
	t4k_main.c	\
	t4k_menu.c	\
	t4k_pixels.c	\
	t4k_sdl.c

include_HEADERS = t4k_common.h
