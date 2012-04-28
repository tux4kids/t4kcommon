#This script was sloppily frankensteined from a handful of tux4kids'
#other CMakeLists.txt and is not safe for human consumption. 
#Please hack responsibly.

cmake_minimum_required(VERSION 2.6)

set(PACKAGE_NAME "Tux4Kids_common")
set(PACKAGE_VERSION 1.0.0)
set(T4K_COMMON_PACKAGE_STRING "${PACKAGE_NAME}-${PACKAGE_VERSION}" )
set(LIB_NAME "t4k_common")
set(VERSION ${PACKAGE_VERSION})
project(${PACKAGE_NAME})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")
include(T4K_Sugar)

set(SDL_BUILDING_LIBRARY 1) #indicate we don't need to (and shouldn't) link SDLmain

set(T4K_DEVELOPER_WARNINGS OFF CACHE STRING "Change or override to enable warnings")
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(T4K_DEVELOPER_WARNINGS ON)
endif()

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(SDL_Pango)
if (SDLPANGO_FOUND)
    set(HAVE_LIBSDL_PANGO 1)
else (SDLPANGO_FOUND)
    find_package(SDL_ttf REQUIRED) # we prefer Pango but use SDL_ttf as a backup
endif (SDLPANGO_FOUND)

# needed for SVG loading and caching
find_package(PNG)
find_package(RSVG)
set(HAVE_RSVG RSVG_FOUND)
set(HAVE_LIBPNG PNG_FOUND)
find_package(SDL_net)
set(HAVE_LIBSDL_NET SDL_net_FOUND)

message("SDL_Pango:  ${SDLPANGO_FOUND}")



# based on tuxmath's CMakeLists.txt
if (APPLE)
    set(MACOSX_BUNDLE_NAME ${PACKAGE_NAME})
    set(DATA_PREFIX
	${T4K_COMMON_MACOSX_BUNDLE_NAME}.app/Contents/Resources)
elseif(UNIX)
    set (DATA_PREFIX
	${CMAKE_INSTALL_PREFIX}/share/${LIB_NAME})
elseif(WIN32)
    t4k_to_unix_path(DATA_PREFIX "\$ENV{APPDATA}/${PACKAGE_NAME}")
else()
    message(SEND_ERROR "What OS are you on?")
endif()

set(LINEBREAK_BINARY_DIR ${CMAKE_BINARY_DIR}/src/linebreak)
set(LINEBREAK_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/linebreak)

#process subdirectories
add_subdirectory(src)
add_subdirectory(data)

include(CPack)

# FIXME - below are "dumb" assignments to the variables
# required for t4k_common.pc.in to generate a working t4k_common.pc
# We should make these variables reflect what libs we actually found,
# particularly SDL_Pango vs. SDL_ttf - DSB
set(DEP_PKGS "sdl >= 1.2.0 SDL_image SDL_Pango librsvg-2.0 libxml-2.0") 
set(DEP_LIBS "-lSDL_mixer -lSDL_net") 
set(DEP_LIBS_PRIVATE "-mwindows -lmikmod -lsmpeg -lstdc++ -lmingw32 -lSDLmain -liconv -luser32 -lgdi32 -lwinmm -ldxguid -lSDL -lpthread -lvorbisfile -lvorbis -lm -logg -lfreetype") 
