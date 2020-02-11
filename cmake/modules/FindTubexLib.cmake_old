find_path(TUBEX_INCLUDE_DIR tubex.h
          HINTS ${PC_TUBEX_INCLUDEDIR} ${PC_TUBEX_INCLUDE_DIRS} ${TUBEX_ROOT}/make
          PATH_SUFFIXES include
          )

find_library(TUBEX_CORE_LIBRARY NAMES tubex-core
             HINTS ${PC_TUBEX_LIBDIR} ${PC_TUBEX_LIBRARY_DIRS}  ${TUBEX_ROOT}/make/src/core/
             PATH_SUFFIXES lib)

find_library(TUBEX_ROBOTICS_LIBRARY NAMES tubex-robotics
             HINTS ${PC_TUBEX_LIBDIR} ${PC_TUBEX_LIBRARY_DIRS}  ${TUBEX_ROOT}/make/src/robotics/
             PATH_SUFFIXES lib)

find_library(TUBEX_SOLVER_LIBRARY NAMES tubex-solver
             HINTS ${PC_TUBEX_LIBDIR} ${PC_TUBEX_LIBRARY_DIRS}  ${TUBEX_ROOT}/make/src/solver/
             PATH_SUFFIXES lib)

list(APPEND TUBEX_LIBRARY
     ${TUBEX_CORE_LIBRARY} ${TUBEX_ROBOTICS_LIBRARY} ${TUBEX_SOLVER_LIBRARY})

include(FindPackageHandleStandardArgs)
## handle the QUIETLY and REQUIRED arguments and set TUBEX_FOUND to TRUE if all listed variables are TRUE
find_package_handle_standard_args(TubexLib DEFAULT_MSG
                                  TUBEX_LIBRARY TUBEX_INCLUDE_DIR)

mark_as_advanced(TUBEX_INCLUDE_DIR TUBEX_LIBRARY)