#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([Tux4Kids-Common],[1.0.0],[tuxmath-devel@lists.sourceforge.net],[t4k_common])
AC_CONFIG_SRCDIR([src/t4k_audio.c])
AC_CONFIG_MACRO_DIR([m4])

AC_PREREQ([2.63])
AC_CANONICAL_HOST
AC_CANONICAL_BUILD
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE(foreign)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])

NAME_VERSION=$PACKAGE-$VERSION

AC_DEFINE_UNQUOTED([PROGRAM_NAME], "$PACKAGE", [Tux4Kids-Common])

AC_SUBST(NAME_VERSION)

# Needed for gettext:
AC_GNU_SOURCE
AM_GNU_GETTEXT_VERSION([0.18.1])
AM_GNU_GETTEXT([external])


# ----------------------------------------------------------------------
# Checks for programs.
# ----------------------------------------------------------------------

AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_AWK
AC_PROG_CPP
AC_PROG_MKDIR_P
AC_PROG_YACC
AC_PROG_LIBTOOL


# -----------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------


dnl ---------------------------------------------------------------------------
dnl In the following checks we attempt to use pkg-config if available, falling
dnl back to standard autoconf macro as needed:
dnl ---------------------------------------------------------------------------

dnl Create output variables to be used in t4k_common.pc.in:
dnl NOTE - the libs in DEP_LIBS_PRIVATE are all needed, even if all optional
dnl libraries are deselected. The one lib in the list that is not needed by the
dnl DEP_LIBS is -lpthread, which is needed by TuxMath itself.  Perhaps we should
dnl check for it with the pkg-config/autoconf macros?

DEP_PKGS=""
DEP_LIBS=""
DEP_LIBS_PRIVATE="-mwindows -lmikmod -lsmpeg -lstdc++ -lmingw32 -lSDLmain -liconv -luser32 -lgdi32 -lwinmm -ldxguid -lSDL -lpthread -lvorbisfile -lvorbis -lm -logg -lfreetype"


dnl Check for SDL
PKG_CHECK_MODULES([SDL],
	[sdl >= 1.2.0],
	[],
	[SDL_VERSION=1.2.0
	 AM_PATH_SDL($SDL_VERSION, :,
		AC_MSG_ERROR([SDL version $SDL_VERSION not found! http://www.libsdl.org]))])

CFLAGS="$CFLAGS $SDL_CFLAGS" 
LIBS="$LIBS $SDL_LIBS"
DEP_PKGS="$DEP_PKGS sdl >= 1.2.0"



dnl Check for SDL_image: ------------------------------------------------------

PKG_CHECK_MODULES([SDL_IMAGE],
	[SDL_image],
	[],
	[AC_MSG_NOTICE([SDL_image not located by pkg-config, checking with AC CHECK LIB:])
	 AC_CHECK_LIB([SDL_image],
		[IMG_Load],
		[],
		[AC_MSG_ERROR([SDL_image not found! http://www.libsdl.org/projects/SDL_image])])])

AC_DEFINE([HAVE_LIBSDL_IMAGE],[1],[Define to 1 if you have the 'SDL_image' library])
CFLAGS="$CFLAGS $SDL_IMAGE"
LIBS="$LIBS $SDL_IMAGE_LIBS"
DEP_PKGS="$DEP_PKGS SDL_image"

dnl Check for SDL_mixer: --------------------------------------------------------

PKG_CHECK_MODULES([SDL_MIXER],
	[SDL_mixer],
	[],
	[AC_MSG_NOTICE([SDL_mixer not located by pkg-config, checking with AC CHECK LIB:])
	 AC_CHECK_LIB([SDL_mixer],
		[Mix_OpenAudio],
		[],
		[AC_MSG_ERROR([SDL_mixer not found! http://www.libsdl.org/projects/SDL_mixer])])])

AC_DEFINE([HAVE_LIBSDL_MIXER],[1],[Define to 1 if you have the 'SDL_mixer' library])
CFLAGS="$CFLAGS $SDL_MIXER_CFLAGS"
LIBS="$LIBS $SDL_MIXER_LIBS"
DEP_LIBS="$DEP_LIBS -lSDL_mixer"


dnl Check for SDL_Pango: --------------------------------------------------------

dnl SDL_Pango is enabled by default.
dnl If SDL_Pango disabled at configure time, or if we can't find it,
dnl we look for SDL_ttf:

AC_ARG_WITH([sdlpango],
	[AS_HELP_STRING([--without-sdlpango],
	[Do not use SDL_Pango even if available])],
	[],
	[with_sdlpango=yes])


if test "x$with_sdlpango" != xno; then

PKG_CHECK_MODULES([SDL_PANGO],
	[SDL_Pango],
	[],
	[AC_MSG_NOTICE([SDL_Pango not located by pkg-config, checking with AC CHECK LIB:])
	 AC_CHECK_LIB([SDL_Pango],
		[SDLPango_Init],
		[],
		[AC_MSG_FAILURE([SDL_Pango test failed (--without-sdlpango to disable)])])])

AC_DEFINE([HAVE_LIBSDL_PANGO],[1],[Define to 1 if you have the `SDL_Pango` library])
CFLAGS="$CFLAGS $SDL_PANGO_CFLAGS"
LIBS="$LIBS $SDL_PANGO_LIBS"
DEP_PKGS="$DEP_PKGS SDL_Pango"

fi



dnl We only use SDL_ttf if SDL_Pango disabled: --------------------------------------

if [test "x$with_sdlpango" = xno]; then
dnl Add freetype link "by hand" until mingw-cross-env takes care
dnl of this automatically
LIBS="-lfreetype $LIBS"

PKG_CHECK_MODULES([SDL_ttf],
	[SDL_ttf],
	[],
	[AC_MSG_NOTICE([SDL_ttf not located by pkg-config, checking with AC CHECK LIB:])
	 AC_CHECK_LIB([SDL_ttf],
		[TTF_Init],
		[],
		[AC_MSG_ERROR([SDL_ttf not found! http://www.libsdl.org/projects/SDL_ttf])])])

AC_DEFINE([HAVE_LIBSDL_TTF],[1],[Define to 1 if you have the `SDL_ttf` library])
CFLAGS="$CFLAGS $SDL_TTF_CFLAGS"
LIBS="$LIBS $SDL_TTF_LIBS"
DEP_LIBS="$DEP_LIBS -lSDL_ttf"
fi



dnl Check for SDL_net: --------------------------------------------------------

dnl SDL_net is enabled by default.

AC_ARG_WITH([sdlnet],
	[AS_HELP_STRING([--without-sdlnet],
	[Do not use SDL_net even if available])],
	[],
	[with_sdlnet=yes]
)

if test "x$with_sdlnet" != xno; then
dnl We will need this for socket programming on Win32
AC_CHECK_LIB([ws2_32],
             [WSAStartup],
	     []  ,
	     [AC_MSG_NOTICE([Windows socket library ws2_32 not found - (only needed on win32)])])


PKG_CHECK_MODULES([SDL_NET],
	[SDL_net],
	[],
	[AC_MSG_NOTICE([SDL_net not located by pkg-config, checking with AC CHECK LIB:])
	 AC_CHECK_LIB([SDL_net],
		[SDLNet_Init],
		[],
		[AC_MSG_FAILURE([SDL_net test failed (--without-sdlnet to disable)])])])

AC_DEFINE([HAVE_LIBSDL_NET],[1],[Define to 1 if you have the 'SDL_net' library])
CFLAGS="$CFLAGS $SDL_NET_CFLAGS"
LIBS="$LIBS $SDL_NET_LIBS"
DEP_LIBS="$DEP_LIBS -lSDL_net"
fi



dnl Check for libRSVG and cairo. SVG support is enabled by default ----------------------

AC_ARG_WITH([rsvg],
	[AS_HELP_STRING([--without-rsvg],[do not use libRSVG even if available])],
	[with_rsvg=no],
	[with_rsvg=yes])

if test "x$with_rsvg" = xyes; then
PKG_CHECK_MODULES([RSVG],
	[librsvg-2.0 >= 2.18],
	[],
	[AC_MSG_FAILURE([libRSVG test failed (--without-rsvg to disable svg support)])])

CFLAGS="$CFLAGS $RSVG_CFLAGS"
LIBS="$LIBS $RSVG_LIBS"
DEP_PKGS="$DEP_PKGS librsvg-2.0"
fi

if test "x$with_rsvg" = xyes; then
PKG_CHECK_MODULES([CAIRO],
	[cairo >= 1.4.10],
	[],
	[AC_MSG_FAILURE([cairo test failed (--without-rsvg to disable svg support)])])

CFLAGS="$CFLAGS $CAIRO_CFLAGS"
LIBS="$LIBS $CAIRO_LIBS"
AC_DEFINE([HAVE_RSVG],[1],[Define to 1 if you have the `libRSVG` library])
fi



if test "x$with_rsvg" = xyes; then
PKG_CHECK_MODULES([LIBPNG],
	[libpng >= 1.2.37],
	[],
	[AC_MSG_FAILURE([libpng test failed (--without-rsvg to disable svg support)])])
CFLAGS="$CFLAGS $LIBPNG_CFLAGS"
LIBS="$LIBS $LIBPNG_LIBS"
AC_DEFINE([HAVE_LIBPNG],[1],[Define to 1 if you have the `libpng` library])
fi



dnl Check for libxml2: --------------------------------------------------------

PKG_CHECK_MODULES([XML2],
	[libxml-2.0],
	[],
	[AC_MSG_NOTICE([libxml2 not located by pkg-config, checking with AC CHECK LIB:])
	 AC_CHECK_LIB([xml2],
		[xmlParseFile],
		[],
		[AC_MSG_ERROR([libxml-2.0 not found: http://www.xmlsoft.org])])])

AC_DEFINE([HAVE_LIBXML2],[1],[Define to 1 if you have the `libxml-2.0` library])
CFLAGS="$CFLAGS $XML2_CFLAGS"
LIBS="$LIBS $XML2_LIBS"
DEP_PKGS="$DEP_PKGS libxml-2.0"



dnl Check for math functions - needed for SDL_extras: --------------------------------------------

AC_CHECK_LIB([m],
	[csin],
	[],
	[AC_MSG_ERROR([Math library not found - functions in <math.h> may not be available.])])


dnl One of espeak, espeak-ng, or speech-dispatcher is required
AC_ARG_WITH([espeak],[AS_HELP_STRING([--without-espeak], [Do not use espeak even if available])])
AC_ARG_WITH([espeak-ng],[AS_HELP_STRING([--without-espeak-ng], [Do not use espeak-ng even if available])])
AC_ARG_WITH([speech-dispatcher],[AS_HELP_STRING([--without-speech-dispatcher], [Do not use speech-dispatcher even if available])])

AS_IF([test "x$with_espeak" != "xno"],
	[AC_CHECK_LIB([espeak],
		[espeak_Cancel],
		[ESPEAK_TTS_CFLAGS="-I/usr/include/espeak/"
		 ESPEAK_TTS_LIBS="-lespeak"
		 have_espeak=yes],
		[have_espeak=no])])

AS_IF([test "x$with_espeak_ng" != "xno"],
	[PKG_CHECK_MODULES([ESPEAK_NG_TTS],
		 [espeak-ng],
		 [have_espeak_ng=yes],
		 [have_espeak_ng=no])])

AS_IF([test "x$with_speech_dispatcher" != "xno"],
	[PKG_CHECK_MODULES([SPEECHD_TTS],
		 [speech-dispatcher],
		 [have_speechd=yes],
		 [have_speechd=no])])

AS_IF([test "x$have_espeak" = "xyes"], [
       TTS_CFLAGS="$ESPEAK_TTS_CFLAGS" TTS_LIBS="$ESPEAK_TTS_LIBS"
       AC_DEFINE([WITH_ESPEAK],[1],[Compile with espeak])],
      [test "x$have_espeak_ng" = "xyes"], [
       TTS_CFLAGS="$ESPEAK_NG_TTS_CFLAGS" TTS_LIBS="$ESPEAK_NG_TTS_LIBS"
       AC_DEFINE([WITH_ESPEAK],[1],[Compile with espeak])
       AC_DEFINE([WITH_ESPEAK_NG],[1],[Use espeak-ng fork])],
      [test "x$have_speechd" = "xyes"], [
       TTS_CFLAGS="$SPEECHD_TTS_CFLAGS" TTS_LIBS="$SPEECHD_TTS_LIBS"
       AC_DEFINE([WITH_ESPEAK],[0],[Compile with espeak])],
      AC_MSG_ERROR([No text-to-speech library found]))

CFLAGS="$CFLAGS $TTS_CFLAGS"
LIBS="$LIBS $TTS_LIBS"

dnl Output substitutions needed for t4k_common.pc.in:
dnl (these are used by the subsequent build process of a program using t4k_common,
dnl not by the build process of t4k_common itself).

AC_SUBST(DEP_PKGS)
AC_SUBST(DEP_LIBS)
AC_SUBST(DEP_LIBS_PRIVATE)


# --------------------------------------------------------------------------------------
# Checks for header files.
# --------------------------------------------------------------------------------------

AC_FUNC_ALLOCA
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([argz.h error.h errno.h fcntl.h float.h iconv.h inttypes.h langinfo.h libgen.h libintl.h limits.h locale.h malloc.h math.h stddef.h stdint.h stdio_ext.h stdlib.h string.h strings.h sys/param.h unistd.h wchar.h])


# --------------------------------------------------------------------------------------------
# Checks for typedefs, structures, and compiler characteristics.
# --------------------------------------------------------------------------------------------

AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_C_RESTRICT
AC_TYPE_MODE_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
AC_C_VOLATILE
AC_CHECK_TYPES([ptrdiff_t])



# -------------------------------------------------------------------------------------------
# Checks for library functions.
# -------------------------------------------------------------------------------------------


AC_FUNC_CLOSEDIR_VOID
AC_FUNC_ERROR_AT_LINE
#AC_FUNC_MALLOC
AC_FUNC_MMAP
#AC_FUNC_REALLOC
AC_FUNC_STAT
AC_FUNC_STRCOLL
AC_FUNC_STRTOD
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([__argz_count __argz_next __argz_stringify atexit basename bcopy floor getcwd localeconv localtime_r memmove mempcpy memset mkdir munmap nl_langinfo scandir alphasort setlocale sqrt stpcpy strcasecmp strchr strcspn strdup strncasecmp strndup strrchr strstr strtoul])


dnl FIXME - get rid of remaining win32 conditional code

# Check platform - see if WinMain needed:
AC_MSG_CHECKING([for native Win32])
case "$host" in
  *-*-mingw*)
    native_win32=yes
    ;;
  *)
    native_win32=no
    ;;
esac
AC_MSG_RESULT([$native_win32])

AM_CONDITIONAL(BUILD_MINGW32, test $native_win32 = yes)

# Set the 
MINGW32_PACKAGE_DATA_DIR="data"
AC_SUBST(MINGW32_PACKAGE_DATA_DIR)


if test $native_win32 = yes; then
   AC_DEFINE([BUILD_MINGW32], 1,[Native MinGW32 build])
   CFLAGS="$CFLAGS -D__GW32__"
   LIBS="$LIBS -luuid -lole32 -lwsock32 -mwindows"
   CPPFLAGS="$CPPFLAGS -idirafter $prefix/include/glibc"
   AC_PATH_PROG(WINDRES, "$target_alias-windres", no, [$PATH])
fi

dnl ----------------------------------------------------------

dnl Needed for Doxygen:
AC_SUBST(T4K_DEVELOPER_WARNINGS, ["NO"])
AC_SUBST(DOXYFILE_SOURCE_DIR, [$srcdir"/src"])
AC_SUBST(DOXYFILE_OUTPUT_DIR, [doxygen])

DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(ON)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN(t4k_common, doxygen.cfg)

AC_CONFIG_FILES([Makefile
                 data/Makefile
                 data/images/Makefile
                 data/images/menu/Makefile
                 po/Makefile.in
                 src/Makefile
                 src/linebreak/Makefile
		 Doxyfile
                 t4k_common.pc])
AC_OUTPUT
