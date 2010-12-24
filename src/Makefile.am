## Makefile.am for t4kcommon/trunk/src
## Process with Automake to create Makefile.in
SUBDIRS = linebreak

AM_CFLAGS=-Wall -g -DCOMMON_DATA_PREFIX=\"${COMMON_DATA_PREFIX}\" -DDEBUG -DUSE_T4K_PREFIX=1 

if BUILD_MINGW32
  COMMON_DATA_PREFIX=@MINGW32_PACKAGE_DATA_DIR@
else
  COMMON_DATA_PREFIX=${pkgdatadir}
endif

libt4k_common_la_LIBADD = linebreak/liblinebreak.la

lib_LTLIBRARIES = libt4k_common.la

libt4k_common_la_SOURCES = \
	t4k_audio.c	\
	t4k_compiler.h	\
	t4k_globals.h	\
        t4k_linewrap.c	\
	t4k_loaders.c	\
	t4k_main.c	\
	t4k_menu.c	\
	t4k_pixels.c	\
	t4k_sdl.c       \
	t4k_throttle.c

include_HEADERS = t4k_common.h

EXTRA_DIST = gettext.h
