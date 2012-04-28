
#Root directory to the source files
set(T4K_SRC_ROOT ${CMAKE_SOURCE_DIR}/src)

#Source files for T4K_Common library
set(T4K_COMMON_SOURCES
    ${T4K_SRC_ROOT}/t4k_audio.c
    ${T4K_SRC_ROOT}/t4k_convert_utf.c
    ${T4K_SRC_ROOT}/t4k_linewrap.c
    ${T4K_SRC_ROOT}/t4k_loaders.c
    ${T4K_SRC_ROOT}/t4k_main.c
    ${T4K_SRC_ROOT}/t4k_menu.c
    ${T4K_SRC_ROOT}/t4k_pixels.c
    ${T4K_SRC_ROOT}/t4k_replacements.c
    ${T4K_SRC_ROOT}/t4k_sdl.c
    ${T4K_SRC_ROOT}/t4k_throttle.c
    )

#Header files for T4K_Common library
set(T4K_COMMON_HEADERS
    ${T4K_SRC_ROOT}/gettext.h
    ${T4K_SRC_ROOT}/t4k_alphasort.h
    ${T4K_SRC_ROOT}/t4k_common.h
    ${T4K_SRC_ROOT}/t4k_compiler.h
    ${T4K_SRC_ROOT}/t4k_globals.h
    ${T4K_SRC_ROOT}/t4k_scandir.h
    )

#Source files the T4K_Common test app
set(T4K_TEST_SOURCES
    ${T4K_SRC_ROOT}/t4k_test.c
    )
