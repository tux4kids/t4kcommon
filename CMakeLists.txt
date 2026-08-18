#This script was sloppily frankensteined from a handful of tux4kids'
#other CMakeLists.txt and is not safe for human consumption. 
#Please hack responsibly.

cmake_minimum_required(VERSION 3.10)

set(PACKAGE_NAME "Tux4Kids_common")
set(PACKAGE_VERSION 1.0.0)
set(T4K_COMMON_PACKAGE_STRING "${PACKAGE_NAME}-${PACKAGE_VERSION}" )
set(LIB_NAME "t4k_common")
set(VERSION ${PACKAGE_VERSION})
project(${PACKAGE_NAME})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")
include(T4K_Sugar)

set(T4K_DEVELOPER_WARNINGS OFF CACHE STRING "Change or override to enable warnings")
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(T4K_DEVELOPER_WARNINGS ON)
endif()

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_mixer REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)

# needed for SVG loading and caching
find_package(PNG)
find_package(RSVG)
set(HAVE_RSVG RSVG_FOUND)
set(HAVE_LIBPNG PNG_FOUND)
find_package(SDL3_net CONFIG)
set(HAVE_LIBSDL_NET SDL3_net_FOUND)

# Text-to-speech backend - one of espeak, espeak-ng, or speech-dispatcher
# is required (matches configure.ac). Prefer espeak-ng.
find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(ESPEAK_NG espeak-ng)
endif()
if(ESPEAK_NG_FOUND)
    set(WITH_ESPEAK 1)
    set(WITH_ESPEAK_NG 1)
    set(TTS_LIBRARIES ${ESPEAK_NG_LIBRARIES})
    set(TTS_INCLUDE_DIRS ${ESPEAK_NG_INCLUDE_DIRS})
else()
    find_library(ESPEAK_LIBRARY NAMES espeak)
    find_path(ESPEAK_INCLUDE_DIR speak_lib.h PATH_SUFFIXES espeak)
    if(ESPEAK_LIBRARY AND ESPEAK_INCLUDE_DIR)
        set(WITH_ESPEAK 1)
        set(TTS_LIBRARIES ${ESPEAK_LIBRARY})
        set(TTS_INCLUDE_DIRS ${ESPEAK_INCLUDE_DIR})
    elseif(PKG_CONFIG_FOUND)
        pkg_check_modules(SPEECHD speech-dispatcher)
        if(SPEECHD_FOUND)
            set(WITH_ESPEAK 0)
            set(TTS_LIBRARIES ${SPEECHD_LIBRARIES})
            set(TTS_INCLUDE_DIRS ${SPEECHD_INCLUDE_DIRS})
        endif()
    endif()
endif()
if(NOT DEFINED WITH_ESPEAK)
    message(FATAL_ERROR "No text-to-speech library found (need espeak, espeak-ng, or speech-dispatcher)")
endif()



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
# We should make these variables reflect what libs we actually found - DSB
set(DEP_PKGS "sdl3 >= 3.0.0 sdl3-image sdl3-ttf librsvg-2.0 libxml-2.0")
set(DEP_LIBS "-lSDL3_mixer -lSDL3_net")
set(DEP_LIBS_PRIVATE "-lpthread -lvorbisfile -lvorbis -lm -logg -lfreetype")
