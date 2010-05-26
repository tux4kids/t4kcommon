#This script was sloppily frankensteined from a handful of tux4kids'
#other CMakeLists.txt and is not safe for human consumption. 
#Please hack responsibly.

cmake_minimum_required(VERSION 2.6)

find_library(SDL REQUIRED)
find_package(SDL_Pango)

include_directories( ${SDL_INCLUDE_DIR} )

set( T4KCOMMON_SOURCE_DIR . )
set( TUXMATH_DESTINATION_DIR 
	${CMAKE_SOURCE_DIR}/../tuxmath/
	CACHE STRING "Working copy of TuxMath"
)
set( TUXTYPE_DESTINATION_DIR 
	${CMAKE_SOURCE_DIR}/../tuxtype/ 
	CACHE STRING "Working copy of TuxType"
)


file(GLOB_RECURSE T4KCOMMON_SOURCES ${T4KCOMMON_SOURCE_DIR}/*.c)
file(GLOB_RECURSE T4KCOMMON_HEADERS ${T4KCOMMON_SOURCE_DIR}/*.h)

add_library( ${LIB_NAME} SHARED
	${T4KCOMMON_SOURCES} ${T4KCOMMON_HEADERS} )


set_target_properties (
  ${LIB_NAME}
  PROPERTIES COMPILE_DEFINITIONS
  COMMON_DATA_PREFIX="${TUX4KIDS-COMMON_DATA_PREFIX}" 
  PROPERTIES COMPILE_FLAGS
  ${_rsvg_def} ${_pango_def} ${_rsvg_cflags} ${_cairo_cflags}"
  )

install( TARGETS ${LIB_NAME}
	ARCHIVE DESTINATION lib 
	LIBRARY DESTINATION lib )
install( TARGETS ${LIB_NAME}
	DESTINATION ${TUXMATH_DESTINATION_DIR}/${LIB_NAME} )
install( TARGETS ${LIB_NAME}
	DESTINATION ${TUXTYPE_DESTINATION_DIR}/${LIB_NAME} )

install(FILES ${T4KCOMMON_SOURCE_DIR}/t4k-common.test.h 
	RENAME t4k-common.h
	DESTINATION include/${LIB_NAME})
install(FILES ${T4KCOMMON_SOURCE_DIR}/t4k-common.test.h 
	RENAME t4k-common.h
	DESTINATION ${TUXMATH_DESTINATION_DIR}/${LIB_NAME} )
install(FILES ${T4KCOMMON_SOURCE_DIR}/t4k-common.test.h 
	RENAME t4k-common.h
	DESTINATION ${TUXTYPE_DESTINATION_DIR}/${LIB_NAME} )

