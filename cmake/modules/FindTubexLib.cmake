find_path(TUBEX_INCLUDE_DIR tubex_Slice.h
          HINTS ${PC_TUBEX_INCLUDEDIR} ${PC_TUBEX_INCLUDE_DIRS}
          PATH_SUFFIXES include include/ibex
          )

#list(APPEND TUBEX_INCLUDE_DIR "${TUBEX_INCLUDE_DIR}/ibex"
#                             "${TUBEX_INCLUDE_DIR}/ibex/3rd")

find_library(TUBEX_LIBRARY NAMES tubex
             HINTS ${PC_TUBEX_LIBDIR} ${PC_TUBEX_LIBRARY_DIRS}
             PATH_SUFFIXES lib)


#include(FindPackageHandleStandardArgs)
#find_package_handle_standard_args(IbexLib DEFAULT_MSG
#                                  TUBEX_LIBRARY TUBEX_INCLUDE_DIR)
#mark_as_advanced(TUBEX_INCLUDE_DIR TUBEX_LIBRARY)