#This script was sloppily frankensteined from a handful of tux4kids'
#other CMakeLists.txt and is not safe for human consumption. 
#Please hack responsibly.

cmake_minimum_required(VERSION 2.6)

set(PACKAGE_NAME Tux4Kids-common)
set(LIB_NAME t4kcommon)
project(${PACKAGE_NAME})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_mixer REQUIRED)

find_package(RSVG)
#include(FindPkgConfig)
#pkg_check_modules(rsvg librsvg-2.0)
#if(rsvg_found)
#  pkg_check_modules(cairo cairo)
#  if(cairo_found)
#    set(have_rsvg 1)  # for the config.h file
#  endif(cairo_found)
#endif(rsvg_found)

message("RSVG: ${RSVG_CFLAGS}")
message("CAIRO: ${CAIRO_CFLAGS}")


# based on tuxmath's CMakeLists.txt
if (APPLE)
  set(T4KCOMMON_MACOSX_BUNDLE_NAME ${PACKAGE_NAME})
  set(T4KCOMMON_DATA_PREFIX
      ${T4KCOMMON_MACOSX_BUNDLE_NAME}.app/Contents/Resources)
elseif (UNIX)
  set (T4KCOMMON_DATA_PREFIX
      ${CMAKE_INSTALL_PREFIX}/share/${LIB_NAME})
endif (APPLE)


#process subdirectories
add_subdirectory(src)
add_subdirectory(data)

