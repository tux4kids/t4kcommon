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
			   t4k_compiler.h	\
			   t4k_globals.h	\
			   t4k_audio.c	\
			   t4k_convert_utf.c	\
			   t4k_linewrap.c	\
			   t4k_loaders.c	\
			   t4k_main.c	\
			   t4k_menu.c	\
			   t4k_pixels.c	\
			   t4k_sdl.c       \
			   t4k_throttle.c	\
			   t4k_replacements.c

include_HEADERS = t4k_common.h \
		  t4k_alphasort.h \
		  t4k_scandir.h

# Test program for library:
bin_PROGRAMS = t4k_test
t4k_test_SOURCES = t4k_test.c
t4k_test_LDADD = libt4k_common.la

EXTRA_DIST = gettext.h \
	     CMakeLists.txt
