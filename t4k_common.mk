# This file is part of mingw-cross-env.
# See doc/index.html for further information.

# T4K_Common
PKG             := t4k_common
$(PKG)_IGNORE   :=
$(PKG)_VERSION  := 0.0.0
$(PKG)_CHECKSUM := 07b29aededd83b3c386533ff6a2797fe3fcf3374
$(PKG)_SUBDIR   := T4K_Common-$($(PKG)_VERSION)
$(PKG)_FILE     := T4K_Common-$($(PKG)_VERSION).tar.gz
$(PKG)_WEBSITE  := http://tux4kids.alioth.debian.org/

# FIXME find out url that can be specified just with package name and version
$(PKG)_URL      := https://alioth.debian.org/frs/download.php/3409/t4k_common-0.0.0.tar.gz
 
$(PKG)_DEPS     := gcc sdl sdl_mixer sdl_image sdl_net sdl_pango sdl_ttf librsvg libxml2

# FIXME find out how to update package
#define $(PKG)_UPDATE
#    $(call SOURCEFORGE_FILES,http://sourceforge.net/projects/sdlpango/files/SDL_Pango/) | \
#    $(SED) -n 's,.*SDL_Pango-\([0-9][^>]*\)\.tar.*,\1,p' | \
#    tail -1
#endef

define $(PKG)_BUILD
    cd '$(1)' && ./configure \
        --host='$(TARGET)' \
        --disable-shared \
        --prefix='$(PREFIX)/$(TARGET)' \
    $(MAKE) -C '$(1)' -j '$(JOBS)' install bin_PROGRAMS= sbin_PROGRAMS= noinst_PROGRAMS=
endef
