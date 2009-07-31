#This script was sloppily frankensteined from a handful of tux4kids'
#other CMakeLists.txt and is not safe for human consumption. 
#Please hack responsibly.

cmake_minimum_required(VERSION 2.6)

find_library(SDL REQUIRED)
include_directories( ${SDL_INCLUDE_DIR} )


set( T4KCOMMON_SOURCE_DIR . )
set( TUXMATH_DESTINATION_DIR ${CMAKE_SOURCE_DIR}/../../tuxmath/trunk/ )
set( TUXTYPE_DESTINATION_DIR ${CMAKE_SOURCE_DIR}/../../tuxtype/trunk/ )


file(GLOB_RECURSE T4KCOMMON_SOURCES ${T4KCOMMON_SOURCE_DIR}/*.c)
file(GLOB_RECURSE T4KCOMMON_HEADERS ${T4KCOMMON_SOURCE_DIR}/*.h)

add_library( t4kcommon STATIC
	${T4KCOMMON_SOURCES} ${T4KCOMMON_HEADERS} )

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

