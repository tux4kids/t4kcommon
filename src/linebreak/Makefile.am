## Makefile.am for tuxmath/linebreak:
## Process with Automake to create Makefile.in

noinst_LTLIBRARIES = liblinebreak.la
# This bundled gnulib snippet calls unistr.h functions whose prototypes are
# gated behind GNULIB_UNISTR_* macros that gnulib-tool would normally define;
# this build never wires those up, so the prototypes are invisible even
# though the functions are compiled in below. Older GCC only warned about
# the resulting implicit declarations; newer GCC defaults to an error.
liblinebreak_la_CFLAGS = -Wno-implicit-function-declaration
liblinebreak_la_SOURCES = linebreak.c \
  unistr/u16-mbtouc-aux.c \
  unistr/u16-mbtouc.c \
  unistr/u16-mbtouc-unsafe-aux.c \
  unistr/u16-mbtouc-unsafe.c \
  unistr/u8-mbtouc-aux.c \
  unistr/u8-mbtouc.c \
  unistr/u8-mbtouc-unsafe-aux.c \
  unistr/u8-mbtouc-unsafe.c \
  unistr/u8-uctomb-aux.c \
  unistr/u8-uctomb.c \
  uniwidth/width.c

EXTRA_DIST = lbrkprop.h \
  linebreak.h \
  localcharset.h \
  streq.h \
  unistr.h \
  unitypes.h \
  uniwidth.h \
  xsize.h \
  uniwidth/cjk.h \
  CMakeLists.txt \
  linebreak_config.h.cmake

