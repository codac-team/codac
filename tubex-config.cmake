## Try to find Tubex
## todo: generate this file with CMake

find_path(TUBEX_INCLUDE_DIR tubex.h
          PATH_SUFFIXES include/tubex)
find_path(TUBEX_ROB_INCLUDE_DIR tubex-rob.h
          PATH_SUFFIXES include/tubex-rob)
find_path(TUBEX_PYIBEX_INCLUDE_DIR tubex-pyibex.h
          PATH_SUFFIXES include/tubex-pyibex)

find_library(TUBEX_LIBRARY NAMES tubex
             PATH_SUFFIXES lib)
find_library(TUBEX_ROB_LIBRARY NAMES tubex-rob
             PATH_SUFFIXES lib)
find_library(TUBEX_PYIBEX_LIBRARY NAMES tubex-pyibex
             PATH_SUFFIXES lib)

set(TUBEX_VERSION 2.5.0)
set(TUBEX_LIBRARIES ${TUBEX_LIBRARY} ${TUBEX_ROB_LIBRARY} ${TUBEX_PYIBEX_LIBRARY})
set(TUBEX_INCLUDE_DIRS ${TUBEX_INCLUDE_DIR} ${TUBEX_ROB_INCLUDE_DIR} ${TUBEX_PYIBEX_INCLUDE_DIR})

# Optional parts: CAPD
find_path(TUBEX_CAPD_INCLUDE_DIR tubex-capd.h
          PATH_SUFFIXES include/tubex-capd)
set(TUBEX_INCLUDE_DIRS ${TUBEX_INCLUDE_DIRS} ${TUBEX_CAPD_INCLUDE_DIR})

find_library(TUBEX_CAPD_LIBRARY NAMES tubex-capd
             PATH_SUFFIXES lib)
set(TUBEX_LIBRARIES ${TUBEX_LIBRARIES} ${TUBEX_CAPD_LIBRARY})