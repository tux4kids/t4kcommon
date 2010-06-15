#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([Tux4Kids-Common],[0.0.1],[tuxmath-devel@lists.sourceforge.net],[t4k_common])
AC_CONFIG_SRCDIR([src/t4k-audio.c])
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
AM_GNU_GETTEXT_VERSION([0.17])
AM_GNU_GETTEXT([external])
#AM_GNU_GETTEXT
#AM_GNU_GETTEXT_INTL_SUBDIR


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

# Check for SDL 
SDL_VERSION=1.2.0

AM_PATH_SDL($SDL_VERSION, :,
         AC_MSG_ERROR([SDL version $SDL_VERSION not found! http://www.libsdl.org]) 
) 

CFLAGS="$CFLAGS $SDL_CFLAGS" 


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

if test $native_win32 = yes; then
  LIBS="$LIBS -lmingw32 -lSDLmain -lSDL"
else
  LIBS="$LIBS $SDL_LIBS"
fi



dnl Check for SDL_image: 
AC_CHECK_LIB([SDL_image], 
             [IMG_Load],
             ,
             [AC_MSG_ERROR([SDL_image not found! http://www.libsdl.org/projects/SDL_image])])

dnl Check for SDL_mixer:. 
AC_CHECK_LIB([SDL_mixer], 
             [Mix_OpenAudio], 
             ,
             [AC_MSG_ERROR([SDL_mixer not found! http://www.libsdl.org/projects/SDL_mixer])])

dnl SDL_Pango is enabled by default.
dnl If SDL_Pango disabled at configure time, or if we can't find it,
dnl we look for SDL_ttf:

AC_ARG_WITH([sdlpango],
            [AS_HELP_STRING([--without-sdlpango],[don't use SDL_Pango even if available])],
            [with_sdlpango=no],
            [with_sdlpango=yes])

if test "x$with_sdlpango" != xno; then
AC_CHECK_LIB([SDL_Pango],
             [SDLPango_Init],
             ,
             [with_sdlpango=no;
              AC_MSG_FAILURE([SDL_Pango test failed (--without-sdlpango to disable)])])
fi

if [test "x$with_sdlpango" = xno]; then
AC_CHECK_LIB([SDL_ttf],
             [TTF_Init],
             ,
             [AC_MSG_ERROR([SDL_ttf not found! http://www.libsdl.org/projects/SDL_ttf])])
fi

dnl Check for libRSVG and cairo. SVG support is enabled by default

AC_ARG_WITH([rsvg],
            [AS_HELP_STRING([--without-rsvg],[don't use libRSVG even if available])],
            [with_rsvg=no],
            [with_rsvg=yes])

if test "x$with_rsvg" = xyes; then
  RSVG_CFLAGS=""
  RSVG_LIBS=""
  PKG_CHECK_MODULES([RSVG],
                    [librsvg-2.0 >= 2.18],
                    ,
                    [AC_MSG_FAILURE([libRSVG test failed (--without-rsvg to disable svg support)])])
  CFLAGS="$CFLAGS $RSVG_CFLAGS"
  LIBS="$LIBS $RSVG_LIBS"
fi

if test "x$with_rsvg" = xyes; then
  CAIRO_CFLAGS=""
  CAIRO_LIBS=""
  PKG_CHECK_MODULES([CAIRO],
                    [cairo >= 1.4.10],
                    ,
                    [AC_MSG_FAILURE([cairo test failed (--without-rsvg to disable svg support)])])
  CFLAGS="$CFLAGS $CAIRO_CFLAGS"
  LIBS="$LIBS $CAIRO_LIBS"
  AC_DEFINE([HAVE_RSVG],[1],[Define to 1 if you have the `libRSVG` library])
fi



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
AC_CHECK_FUNCS([__argz_count __argz_next __argz_stringify atexit basename bcopy floor getcwd localeconv localtime_r memmove mempcpy memset mkdir munmap nl_langinfo scandir setlocale sqrt stpcpy strcasecmp strchr strcspn strdup strncasecmp strndup strrchr strstr strtoul])



AC_CONFIG_FILES([Makefile
                 data/Makefile
                 data/images/Makefile
                 data/images/menu/Makefile
                 po/Makefile.in
                 src/Makefile])
AC_OUTPUT
