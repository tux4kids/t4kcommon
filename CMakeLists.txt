#This script was sloppily frankensteined from a handful of tux4kids'
#other CMakeLists.txt and is not safe for human consumption. 
#Please hack responsibly.

cmake_minimum_required(VERSION 2.6)

set(PACKAGE_NAME Tux4Kids_common)
set(LIB_NAME t4k_common)
project(${PACKAGE_NAME})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")
include(T4K_Sugar)

set(SDL_BUILDING_LIBRARY 1) #indicate we don't need to link SDLmain

set(T4K_DEVELOPER_WARNINGS ON CACHE STRING "Change or override to disable warnings")
if(CMAKE_BUILD_TYPE MATCHES Release)
  set(T4K_DEVELOPER_WARNINGS OFF)
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

find_package(RSVG)
message("SDL_Pango:  ${SDLPANGO_FOUND}")


# based on tuxmath's CMakeLists.txt
if (APPLE)
  set(T4K_COMMON_MACOSX_BUNDLE_NAME ${PACKAGE_NAME})
  set(T4K_COMMON_DATA_PREFIX
      ${T4K_COMMON_MACOSX_BUNDLE_NAME}.app/Contents/Resources)
elseif(UNIX)
  set (T4K_COMMON_DATA_PREFIX
      ${CMAKE_INSTALL_PREFIX}/share/${LIB_NAME})
elseif(WIN32)
  t4k_to_unix_path(T4K_COMMON_DATA_PREFIX "\$ENV{APPDATA}/${PACKAGE_NAME}")
else()
  message(SEND_ERROR "What OS are you on?")
endif()


#process subdirectories
add_subdirectory(src)
add_subdirectory(data)

include(CPack)
