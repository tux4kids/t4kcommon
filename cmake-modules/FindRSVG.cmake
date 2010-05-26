# - Locate RSVG and Cairo library
# This module defines
# RSVG_FOUND if false, don't use RSVG
# CAIRO_FOUND if libcairo was found


if (UNIX)
  include(FindPkgConfig)
  pkg_check_modules(RSVG librsvg-2.0)
  if(RSVG_FOUND)
    pkg_check_modules(CAIRO cairo)
    if(CAIRO_FOUND)
      set(HAVE_RSVG 1)  # For the config.h file
    endif(CAIRO_FOUND)
  endif(RSVG_FOUND)
elseif(WIN32)
  
endif(UNIX)

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


