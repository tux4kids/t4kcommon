# - Run Doxygen
#
# Adds a doxygen target that runs doxygen to generate the html
# and optionally the LaTeX API documentation.
# The doxygen target is added to the doc target as dependency.
# i.e.: the API documentation is built with:
#  make doc
#
# USAGE: GLOBAL INSTALL
#
# Install it with:
#  cmake ./ && sudo make install
# Add the following to the CMakeLists.txt of your project:
#  include(UseDoxygen OPTIONAL)
# Optionally copy Doxyfile.in in the directory of CMakeLists.txt and edit it.
#
# USAGE: INCLUDE IN PROJECT
#
#  set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
#  include(UseDoxygen)
# Add the Doxyfile.in and UseDoxygen.cmake files to the projects source directory.
#
#
# Variables you may define are:
#  DOXYFILE_SOURCE_DIR - Path where the Doxygen input files are. Defaults to the current binary dir.
#  DOXYFILE_OUTPUT_DIR - Path where the Doxygen output is stored. Defaults to "doc".
#
#  DOXYFILE_LATEX_DIR - Directory relative to DOXYFILE_OUTPUT_DIR where
#  the Doxygen LaTeX output is stored. Defaults to "latex".
#
#  DOXYFILE_HTML_DIR - Directory relative to DOXYFILE_OUTPUT_DIR where
#  the Doxygen html output is stored. Defaults to "html".
#

#
#  Copyright (c) 2009 Tobias Rautenkranz <tobias@rautenkranz.ch>
#  With additions 2010 by Brendan Luchen <bml4633@rit.edu>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

macro(usedoxygen_set_default name value)
	if(NOT DEFINED "${name}")
		set("${name}" "${value}")
	endif()
endmacro()

find_package(Doxygen)
find_package(LATEX)

if(DOXYGEN_FOUND)
	find_path(DOXYFILE_DIR "Doxyfile.in"
			PATHS "${CMAKE_SOURCE_DIR}" "${CMAKE_ROOT}/Modules/")
	set(DOXYFILE_IN "${DOXYFILE_DIR}/Doxyfile.in")
	set(DOXYFILE "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(Doxyfile.in DEFAULT_MSG DOXYFILE_IN)
endif()

if(DOXYGEN_FOUND AND DOXYFILE_IN)

	usedoxygen_set_default(DOXYFILE_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doc")
	usedoxygen_set_default(DOXYFILE_HTML_DIR "html")
	usedoxygen_set_default(DOXYFILE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

	set_property(DIRECTORY APPEND PROPERTY
			ADDITIONAL_MAKE_CLEAN_FILES "${DOXYFILE_OUTPUT_DIR}")

	set(DOXYFILE_LATEX "NO")
	set(DOXYFILE_PDFLATEX "NO")
	set(DOXYFILE_DOT "NO")

	if(LATEX_COMPILER AND MAKEINDEX_COMPILER)
		set(DOXYFILE_LATEX "YES")
		usedoxygen_set_default(DOXYFILE_LATEX_DIR "latex")

		set_property(DIRECTORY APPEND PROPERTY
				ADDITIONAL_MAKE_CLEAN_FILES
				"${DOXYFILE_OUTPUT_DIR}/${DOXYFILE_LATEX_DIR}")

		if(PDFLATEX_COMPILER)
			set(DOXYFILE_PDFLATEX "YES")
		endif()
		if(DOXYGEN_DOT_EXECUTABLE)
			set(DOXYFILE_DOT "YES")
		endif()

		add_custom_command(TARGET doxygen
			POST_BUILD
			COMMAND ${CMAKE_MAKE_PROGRAM}
			WORKING_DIRECTORY "${DOXYFILE_OUTPUT_DIR}/${DOXYFILE_LATEX_DIR}")
	endif()


	configure_file(${DOXYFILE_IN} ${DOXYFILE} ESCAPE_QUOTES IMMEDIATE @ONLY)

	add_custom_target(doxygen 
		COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --switch=${CMAKE_COLOR_MAKEFILE} --red --bold "Writing documentation to ${DOXYFILE_OUTPUT_DIR}..."
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
		WORKING_DIRECTORY ${DOXYFILE_SOURCE_DIR} )

	get_target_property(DOC_TARGET doc TYPE)
	if(NOT DOC_TARGET)
		add_custom_target(doc)
	endif()
		
	add_dependencies(doc doxygen)
endif()
