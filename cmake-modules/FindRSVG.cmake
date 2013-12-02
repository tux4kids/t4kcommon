# - Locate RSVG and Cairo library
# This module defines
# RSVG_FOUND if false, don't use RSVG
# CAIRO_FOUND if libcairo was found
# RSVG_LIBRARIES
# RSVG_CFLAGS
# CAIRO_CFLAGS



set(RSVG_FOUND false)

# first try using pkg-config
if (UNIX)
  include(FindPkgConfig)
  pkg_check_modules(RSVG librsvg-2.0)
  if(RSVG_FOUND)
    pkg_check_modules(CAIRO cairo)
    if(CAIRO_FOUND)
      set(HAVE_RSVG 1)  # For the config.h file
    endif(CAIRO_FOUND)
  endif(RSVG_FOUND)
endif(UNIX)

if (NOT RSVG_FOUND)
  find_path(RSVG_INCLUDE_DIR rsvg.h
    /usr/include
    /usr/include/librsvg-2
    /usr/include/librsvg-2/librsvg
    /usr/local/include
    /usr/local/include/librsvg-2
    /usr/local/include/librsvg-2/librsvg
  )
endif(NOT RSVG_FOUND)


# getting rid of semicolons
set(_rsvg_cflags "")
foreach(f ${RSVG_CFLAGS})
  set(_rsvg_cflags "${_rsvg_cflags} ${f}")
endforeach(f)
set(RSVG_CFLAGS ${_rsvg_cflags})

set(_cairo_cflags "")
foreach(f ${CAIRO_CFLAGS})
  set(_cairo_cflags "${_cairo_cflags} ${f}")
endforeach(f)
set(CAIRO_CFLAGS ${_cairo_cflags})

set(_rsvg_def "")
if(HAVE_RSVG)
  set(_rsvg_def "-DHAVE_RSVG=1")
endif(HAVE_RSVG)

set(_pango_def "")
if (SDLPANGO_FOUND)
  set(_pango_def "-DHAVE_LIBSDL_PANGO=1")
endif (SDLPANGO_FOUND)


