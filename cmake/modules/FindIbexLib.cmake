find_path(IBEX_INCLUDE_DIR ibex.h
          HINTS ${PC_IBEX_INCLUDEDIR} ${PC_IBEX_INCLUDE_DIRS}
          PATH_SUFFIXES include include/ibex
          )

list(APPEND IBEX_INCLUDE_DIR "${IBEX_INCLUDE_DIR}/ibex"
                             "${IBEX_INCLUDE_DIR}/ibex/3rd")

find_library(IBEX_LIBRARY NAMES ibex
             HINTS ${PC_IBEX_LIBDIR} ${PC_IBEX_LIBRARY_DIRS}
             PATH_SUFFIXES lib)

find_library(FILIB_LIBRARY NAMES prim
             HINTS ${PC_IBEX_LIBDIR} ${PC_IBEX_LIBRARY_DIRS}
             PATH_SUFFIXES lib lib/ibex/3rd/
             )

list(APPEND IBEX_LIBRARY "${FILIB_LIBRARY}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(IbexLib DEFAULT_MSG
                                  IBEX_LIBRARY IBEX_INCLUDE_DIR)
mark_as_advanced(IBEX_INCLUDE_DIR IBEX_LIBRARY)