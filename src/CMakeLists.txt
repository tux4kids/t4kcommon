#This script was sloppily frankensteined from a handful of tux4kids'
#other CMakeLists.txt and is not safe for human consumption. 
#Please hack responsibly.

# TODO - add cmake build support for t4k_test program - DSB

cmake_minimum_required(VERSION 2.6)

include(ConfigureChecks)
configure_file(${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_BINARY_DIR}/config.h)
include(T4K_Sugar)

#Cmake module contains the source files for this library
include(${CMAKE_SOURCE_DIR}/cmake-modules/Files.cmake)

find_package(LibXml2 REQUIRED)

#Look here for headers
include_directories(
    ${SDL_INCLUDE_DIR}
    ${SDLIMAGE_INCLUDE_DIR}
    ${SDLMIXER_INCLUDE_DIR}
    ${LIBXML2_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/gettext
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src
    ${CMAKE_BINARY_DIR}/gettext
    ${CMAKE_BINARY_DIR}/src/linebreak
    ${CMAKE_SOURCE_DIR}/src/linebreak
    )

#Look here for optional headers
t4k_include_directory(SDLPANGO)
t4k_include_directory(SDLTTF)
t4k_include_directory(PNG)
t4k_include_directory(RSVG)

#include_directories( ${T4KCOMMON_INCLUDE_DIRS} )

set( T4K_COMMON_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
if(T4K_DEVELOPER_WARNINGS)
    list(APPEND CMAKE_C_FLAGS -Wall) 
endif()

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
    ${LIBXML2_LIBRARIES}
    ${LINEBREAK_BINARY_DIR}/liblinebreak.a
    )

t4k_include_definition(HAVE_LIBSDL_PANGO)
t4k_include_definition(HAVE_LIBPNG)
set_target_properties (${LIB_NAME} PROPERTIES 
    COMPILE_FLAGS "${_rsvg_def} ${_pango_def} ${_rsvg_cflags} ${_cairo_cflags}"
    COMPILE_DEFINITIONS
    "COMMON_DATA_PREFIX=\"${DATA_PREFIX}\";USE_T4K_PREFIX=1;PACKAGE_STRING=\"${PACKAGE_NAME}\""
    LINK_INTERFACE_LIBRARIES ""
    )

add_dependencies(${LIB_NAME} linebreak)

install( TARGETS ${LIB_NAME}
    DESTINATION lib ) 
install(FILES ${T4K_COMMON_SOURCE_DIR}/t4k_common.h
    ${T4K_COMMON_SOURCE_DIR}/t4k_alphasort.h
    ${T4K_COMMON_SOURCE_DIR}/t4k_scandir.h
    DESTINATION include)

include(UseDoxygen OPTIONAL)

#process subdirectories
add_subdirectory(linebreak)

#Build t4k_test app
set(T4K_TESTAPP t4k_test)
add_executable(${T4K_TESTAPP} ${T4K_TEST_SOURCES})
TARGET_LINK_LIBRARIES(${T4K_TESTAPP}
    ${LIB_NAME}
    ${SDL_LIBRARY}
    ${SDLMIX_LIBRARY}
    ${SDLIMAGE_LIBRARY}
    ${SDLNET_LIBRARY}
    ${SDLPANGO_LIBRARY}
    ${SDLTTF_LIBRARY}
    ${LIBXML2_LIBRARIES}
    ${RSVG_LIBRARIES}
    ${LINEBREAK_BINARY_DIR}/liblinebreak.a
    )
