include(CheckIncludeFile)
include(CheckSymbolExists)

check_symbol_exists(scandir dirent.h T4K_COMMON_HAVE_SCANDIR)
check_symbol_exists(alphasort dirent.h T4K_COMMON_HAVE_ALPHASORT) 
check_include_file (error.h T4K_COMMON_HAVE_ERROR_H)
check_include_file (search.h T4K_COMMON_HAVE_TSEARCH)
