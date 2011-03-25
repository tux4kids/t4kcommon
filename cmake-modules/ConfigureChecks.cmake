include(CheckIncludeFile)
include(CheckSymbolExists)

check_symbol_exists(scandir dirent.h HAVE_SCANDIR)
check_symbol_exists(alphasort dirent.h HAVE_ALPHASORT) 
check_include_file (error.h HAVE_ERROR_H)
check_include_file (search.h HAVE_TSEARCH)
check_include_file (stdint.h HAVE_STDINT_H)
