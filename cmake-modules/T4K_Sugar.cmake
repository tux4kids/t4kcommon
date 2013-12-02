# Some syntactic sugar to make scripts more readable
#
# Brendan Luchen, 2010

# Set a variable only if it has not already been set
# Example: gentle_set(CMAKE_BUILD_TYPE DEBUG)
macro(t4k_gentle_set var val)
  if (NOT DEFINED ${var})
    set(${var} ${val})
  else (NOT DEFINED ${var})
    message(STATUS "${var} was previously set to ${${var}}")
  endif (NOT DEFINED ${var})
endmacro(t4k_gentle_set)

macro(t4k_to_unix_path newpath winpath)
  string(REPLACE "\\" "/" ${newpath} ${winpath})
endmacro(t4k_to_unix_path)

macro(t4k_include_directory package)
  if (${package}_FOUND)
    include_directories(${${package}_INCLUDE_DIR})
  endif(${package}_FOUND)
endmacro(t4k_include_directory)

# Propagate a CMake variable to the C preprocessor
# Example: include_definition(HAVE_ICONV)
function(t4k_include_definition name)
  if (${name})
    add_definitions(-D${name}=1)
  else (${name})
    add_definitions(-D${name}=0)
  endif(${name})
endfunction(t4k_include_definition)
