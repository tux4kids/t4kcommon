#This script was sloppily frankensteined from a handful of tux4kids'
#other CMakeLists.txt and is not safe for human consumption. 
#Please hack responsibly.

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

add_subdirectory(src)

