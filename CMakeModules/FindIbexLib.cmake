# - Try to find IBEX
# Once done this will define
#  IBEX_FOUND - System has ibex
#  IBEX_INCLUDE_DIRS - The ibex include directories
#  IBEX_LIBRARIES - The libraries needed to use ibex
#  IBEX_DEFINITIONS - Compiler switches required for using ibex

find_package(PkgConfig)
pkg_check_modules(PC_IBEXLIB QUIET ibex)
message(STATUS "IBEX_ROOT ${IBEX_ROOT}")

if(IbexLib_USE_STATIC)
  SET(CMAKE_FIND_LIBRARY_SUFFIXES .a)
endif()

set(IBEX_DEFINITIONS ${PC_IBEX_CFLAGS_OTHER})
find_path(IBEX_INCLUDE_DIR ibex.h
          HINTS ${PC_IBEX_INCLUDEDIR} ${PC_IBEX_INCLUDE_DIRS} ${IBEX_ROOT}
          PATH_SUFFIXES include include/ibex )

find_path(FILIB_INCLUDE_DIR ieee/primitive.hpp
          HINTS ${PC_IBEX_INCLUDEDIR} ${PC_IBEX_INCLUDE_DIRS} ${IBEX_ROOT}
          PATH_SUFFIXES include
          )


find_library(IBEX_LIBRARY NAMES ibex
            HINTS ${PC_IBEX_LIBDIR} ${PC_IBEX_LIBRARY_DIRS}  ${IBEX_ROOT}
            PATH_SUFFIXES lib
						)

find_library(FILIB_LIBRARY NAMES prim
            HINTS ${PC_IBEX_LIBDIR} ${PC_IBEX_LIBRARY_DIRS}  ${IBEX_ROOT}
            PATH_SUFFIXES lib
						)

set(IBEX_LIBRARIES ${IBEX_LIBRARY} ${FILIB_LIBRARY})
set(IBEX_INCLUDE_DIRS ${IBEX_INCLUDE_DIR} ${FILIB_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set IBEX_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(IBEX  DEFAULT_MSG
                                  IBEX_LIBRARY IBEX_INCLUDE_DIR)

mark_as_advanced(IBEX_INCLUDE_DIR IBEX_LIBRARY )
