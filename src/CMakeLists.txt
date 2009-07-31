#This script was sloppily frankensteined from a handful of tux4kids'
#other CMakeLists.txt and is not safe for human consumption. 
#Please hack responsibly.

cmake_minimum_required(VERSION 2.6)

find_library(SDL REQUIRED)
find_package(SDL_Pango)

include_directories( ${SDL_INCLUDE_DIR} )

set( T4KCOMMON_SOURCE_DIR . )
set( TUXMATH_DESTINATION_DIR ${CMAKE_SOURCE_DIR}/../../tuxmath/trunk/ )
set( TUXTYPE_DESTINATION_DIR ${CMAKE_SOURCE_DIR}/../../tuxtype/trunk/ )


file(GLOB_RECURSE T4KCOMMON_SOURCES ${T4KCOMMON_SOURCE_DIR}/*.c)
file(GLOB_RECURSE T4KCOMMON_HEADERS ${T4KCOMMON_SOURCE_DIR}/*.h)

add_library( t4kcommon STATIC
	${T4KCOMMON_SOURCES} ${T4KCOMMON_HEADERS} )

# getting rid of semicolons
set(_rsvg_cflags "")
foreach(f ${RSVG_CFLAGS})
  set(_rsvg_cflags "${_rsvg_cflags} ${f}")
endforeach(f)

set(_cairo_cflags "")
foreach(f ${CAIRO_CFLAGS})
  set(_cairo_cflags "${_cairo_cflags} ${f}")
endforeach(f)

set(_rsvg_def "")
if(HAVE_RSVG)
  set(_rsvg_def "-DHAVE_RSVG=1")
endif(HAVE_RSVG)

set(_pango_def "")
if (SDLPANGO_FOUND)
  set(_pango_def "-DHAVE_LIBSDL_PANGO=1")
endif (SDLPANGO_FOUND)


set_target_properties (
  t4kcommon
  PROPERTIES COMPILE_FLAGS
  "${_rsvg_def} ${_pango_def} ${_rsvg_cflags} ${_cairo_cflags}"
  )


INSTALL( TARGETS t4kcommon
	ARCHIVE DESTINATION lib )
INSTALL( TARGETS t4kcommon
	DESTINATION ${TUXMATH_DESTINATION_DIR}/t4kcommon )
INSTALL( TARGETS t4kcommon
	DESTINATION ${TUXTYPE_DESTINATION_DIR}/t4kcommon )

INSTALL(FILES ${T4KCOMMON_SOURCE_DIR}/tux4kids-common.h 
	DESTINATION include/t4kcommon)
INSTALL(FILES ${T4KCOMMON_SOURCE_DIR}/tux4kids-common.h 
	DESTINATION ${TUXMATH_DESTINATION_DIR}/t4kcommon )
INSTALL(FILES ${T4KCOMMON_SOURCE_DIR}/tux4kids-common.h 
	DESTINATION ${TUXTYPE_DESTINATION_DIR}/t4kcommon )

