#This script was sloppily frankensteined from a handful of tux4kids'
#other CMakeLists.txt and is not safe for human consumption. 
#Please hack responsibly.

project(Tux4Kids-common)

set(CMAKE_MODULE_PATH "${Tux4Kids-common_SOURCE_DIR}/cmake-modules")

cmake_minimum_required(VERSION 2.6)
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_mixer REQUIRED)

include(FindPkgConfig OPTIONAL)

if(PKG_CONFIG_FOUND)
  include(FindPkgConfig)
  PKG_CHECK_MODULES(RSVG librsvg-2.0)
  if(RSVG_FOUND)
    PKG_CHECK_MODULES(CAIRO cairo)
    if(CAIRO_FOUND)
      set(HAVE_RSVG 1)  # For the config.h file
    endif(CAIRO_FOUND)
  endif(RSVG_FOUND)
endif(PKG_CONFIG_FOUND)

# based on tuxmath's CMakeLists.txt
if (APPLE)
  set(TUX4KIDS-COMMON_MACOSX_BUNDLE_NAME tux4kids-common)
  set(TUX4KIDS-COMMON_DATA_PREFIX
      ${TUX4KIDS-COMMON_MACOSX_BUNDLE_NAME}.app/Contents/Resources)
elseif (UNIX)
  set (TUX4KIDS-COMMON_DATA_PREFIX
      ${CMAKE_INSTALL_PREFIX}/share/tux4kids-common)
endif (APPLE)


add_subdirectory(src)
add_subdirectory(data)

