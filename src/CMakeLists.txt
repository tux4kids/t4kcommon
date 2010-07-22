#This script was sloppily frankensteined from a handful of tux4kids'
#other CMakeLists.txt and is not safe for human consumption. 
#Please hack responsibly.

cmake_minimum_required(VERSION 2.6)
include(T4K_Sugar)

#Look here for SDL headers
include_directories( ${SDL_INCLUDE_DIR} )

set( T4K_COMMON_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

#Find source files
file(GLOB_RECURSE T4K_COMMON_SOURCES ${T4K_COMMON_SOURCE_DIR}/*.c)
file(GLOB_RECURSE T4K_COMMON_HEADERS ${T4K_COMMON_SOURCE_DIR}/*.h)

#Declare the library as a SO 
add_library( ${LIB_NAME} SHARED
	${T4K_COMMON_SOURCES} ${T4K_COMMON_HEADERS} )

#Avoid linking to both libraries
if(SDLPANGO_FOUND)
    set(SDLTTF_LIBRARY "")
else()
    set(SDLPANGO_LIBRARY "")
endif()

#Depend on these libraries. For Pango or TTF, whichever is found will be used
target_link_libraries(${LIB_NAME}
	${SDL_LIBRARY}
	${SDLMIXER_LIBRARY}
	${SDLIMAGE_LIBRARY}
	${SDLPANGO_LIBRARY}
	${SDLTTF_LIBRARY}
)

t4k_include_definition(HAVE_LIBSDL_PANGO)
set_target_properties (${LIB_NAME} PROPERTIES 
	COMPILE_FLAGS "${_rsvg_def} ${_pango_def} ${_rsvg_cflags} ${_cairo_cflags}"
	COMPILE_DEFINITIONS
	"COMMON_DATA_PREFIX=\"${T4K_COMMON_DATA_PREFIX}\";USE_T4K_PREFIX=1;PACKAGE_STRING=\"${PACKAGE_STRING}\""
	LINK_INTERFACE_LIBRARIES ""
)

install( TARGETS ${LIB_NAME}
	DESTINATION lib )

install(FILES ${T4K_COMMON_SOURCE_DIR}/t4k_common.h
	DESTINATION include)

include(UseDoxygen OPTIONAL)

