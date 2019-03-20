find_path(IBEXGEO_INCLUDE_DIR ibex_CtcPolar.h
          HINTS ${PC_IBEXGEO_INCLUDEDIR} ${PC_IBEXGEO_INCLUDE_DIRS} ${IBEXGEO_ROOT}/make
          PATH_SUFFIXES include
          )

find_library(IBEXGEO_LIBRARY NAMES ibex-geometry
             HINTS ${PC_IBEXGEO_LIBDIR} ${PC_IBEXGEO_LIBRARY_DIRS} ${IBEXGEO_ROOT}/make/src
             PATH_SUFFIXES lib)

message(IBEXGEO_ROOT ${IBEXGEO_ROOT})
message(IBEXGEO_INCLUDE_DIR ${IBEXGEO_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(IbexGeometry DEFAULT_MSG
                                  IBEXGEO_LIBRARY IBEXGEO_INCLUDE_DIR)
mark_as_advanced(IBEXGEO_INCLUDE_DIR IBEXGEO_LIBRARY)