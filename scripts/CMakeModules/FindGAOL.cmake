# ==================================================================
#  Codac - cmake module looking for an installed GAOL
# ==================================================================
#
# Looks for GAOL, the interval arithmetic library Codac is built upon
# (Frederic Goualard, https://frederic.goualard.net,
# https://github.com/goualard-f/GAOL), and for mathlib, the IBM Accurate
# Portable Mathematical Library GAOL computes its elementary functions with,
# which installs itself as libultim.
#
# Where to look, when GAOL is not installed in a directory CMake searches by
# default, is given by two cache entries -- or by CMAKE_PREFIX_PATH:
#
#   GAOL_DIR     prefix holding include/gaol/gaol.h and lib/<gaol library>
#   MATHLIB_DIR  prefix holding include/MathLib.h and lib/<ultim library>;
#                GAOL_DIR is searched for mathlib as well, the two being
#                usually installed together
#
# What is found is written to the cache entries GAOL_INCDIR, GAOL_LIB,
# MATHLIB_INCDIR and MATHLIB_LIB, which can also be set by hand to bypass the
# search, and summarised in:
#
#   GAOL_FOUND
#   GAOL_VERSION       when gaol/gaol_configuration.h states it
#   GAOL_INCLUDE_DIRS
#   GAOL_LIBRARIES     gaol, then ultim, in link order
#
# No target is defined here. codac_gaol_find() (scripts/CMakeModules/codac_gaol.cmake)
# makes Codac::gaol out of these variables, with the flags of interval
# arithmetic it determines itself, for a GAOL that has neither a CMake package
# nor a gaol.pc, which it looks for first.
#
# Origin
# ------
# This is the search IBEX runs before building a GAOL of its own, taken out of
# IBEX (https://github.com/ibex-team/ibex-lib, GNU LGPL v3) so that Codac can
# find GAOL without IBEX:
#
#  - codac_find_header_custom() and codac_find_library_custom() are the
#    functions find_header_custom() and find_library_custom() of IBEX's
#    cmake.utils/IbexUtils.cmake, written by Cyril Bouvier for the CMake build
#    of IBEX. They are renamed with a codac_ prefix so as not to clash with
#    IBEX's own definitions in a project that loads both, and differ in
#    two respects: they stay silent when find_package(GAOL QUIET) asks for it,
#    and a header found on the default search path is returned as it is,
#    rather than wrapped in $<BUILD_INTERFACE:...> -- that wrapping is how IBEX
#    keeps such a directory out of the export files it generates, and Codac
#    writes its configuration file by other means (see src/CMakeLists.txt).
#
#  - The four calls below, the GAOL_DIR and MATHLIB_DIR hints and the names of
#    the result variables are those of IBEX's
#    interval_lib_wrapper/gaol/CMakeLists.txt, by Cyril Bouvier as well.
#
#  - Reading the version out of gaol/gaol_configuration.h comes from
#    interval_lib_wrapper/gaol/FindGaol.cmake of IBEX (Cyril Bouvier). It
#    reads the GAOL_MAJOR_VERSION, GAOL_MINOR_VERSION and GAOL_MICRO_VERSION
#    macros here, which both the autotools and the CMake builds of GAOL write.


################################################################################
# Functions from IBEX (see above)
################################################################################

  function(codac_find_header_custom prefix hdrname)
    set(opt "")
    set(oneArgs "")
    set(multiArgs PATHS)

    cmake_parse_arguments(FHC "${opt}" "${oneArgs}" "${multiArgs}" ${ARGN})

    if(FHC_UNPARSED_ARGUMENTS)
      message(FATAL_ERROR "Unknown keywords given to codac_find_header_custom(): \"${FHC_UNPARSED_ARGUMENTS}\"")
    endif()

    set(MSG "Looking for ${hdrname}")
    if(NOT GAOL_FIND_QUIETLY)
      message(STATUS "${MSG}")
    endif()

    # First look only in PATHS if given
    if(FHC_PATHS)
      find_path(${prefix}_INCDIR ${hdrname} PATHS ${FHC_PATHS}
                DOC "Set to exact include directory to bypass internal test"
                PATH_SUFFIXES include NO_DEFAULT_PATH)
    endif()

    if(NOT ${prefix}_INCDIR)
      # Now look with system and cmake paths
      find_path(${prefix}_INCDIR ${hdrname}
                DOC "Set to exact include directory to bypass internal test"
                PATH_SUFFIXES include)
    endif()

    if(NOT GAOL_FIND_QUIETLY)
      if(${prefix}_INCDIR)
        message(STATUS "${MSG} -- found at ${${prefix}_INCDIR}")
      else()
        message(STATUS "${MSG} -- not found")
      endif()
    endif()

    set(${prefix}_INCDIR ${${prefix}_INCDIR} PARENT_SCOPE)
    mark_as_advanced(${prefix}_INCDIR)
  endfunction()


  function(codac_find_library_custom prefix libname)
    set(opt "")
    set(oneArgs "")
    set(multiArgs PATHS)

    cmake_parse_arguments(FLC "${opt}" "${oneArgs}" "${multiArgs}" ${ARGN})

    if(FLC_UNPARSED_ARGUMENTS)
      message(FATAL_ERROR "Unknown keywords given to codac_find_library_custom(): \"${FLC_UNPARSED_ARGUMENTS}\"")
    endif()

    set(MSG "Looking for ${libname}")
    if(NOT GAOL_FIND_QUIETLY)
      message(STATUS "${MSG}")
    endif()

    # First look only in PATHS if given
    if(FLC_PATHS)
      find_library(${prefix}_LIB ${libname} PATHS ${FLC_PATHS}
                   DOC "Set to exact lib directory to bypass internal test"
                   PATH_SUFFIXES lib NO_DEFAULT_PATH)
    endif()

    if(NOT ${prefix}_LIB)
      # Now look with system and cmake paths
      find_library(${prefix}_LIB ${libname}
                   DOC "Set to exact lib directory to bypass internal test"
                   PATH_SUFFIXES lib)
    endif()

    if(NOT GAOL_FIND_QUIETLY)
      if(${prefix}_LIB)
        message(STATUS "${MSG} -- found at ${${prefix}_LIB}")
      else()
        message(STATUS "${MSG} -- not found")
      endif()
    endif()

    set(${prefix}_LIB ${${prefix}_LIB} PARENT_SCOPE)
    mark_as_advanced(${prefix}_LIB)
  endfunction()


################################################################################
# Options
################################################################################

  set(MATHLIB_DIR "" CACHE PATH "Path to the Mathlib/ultim lib and include directories")
  set(GAOL_DIR "" CACHE PATH "Path to the Gaol lib and include directories")


################################################################################
# Looking for Mathlib/libultim
################################################################################

  # Looking for MathLib.h, result is written in MATHLIB_INCDIR
  codac_find_header_custom(MATHLIB "MathLib.h" PATHS "${MATHLIB_DIR}" "${GAOL_DIR}")
  # Looking for ultim library, result is written in MATHLIB_LIB
  codac_find_library_custom(MATHLIB "ultim" PATHS "${MATHLIB_DIR}" "${GAOL_DIR}")


################################################################################
# Looking for Gaol
################################################################################

  # Looking for gaol/gaol.h, result is written in GAOL_INCDIR
  codac_find_header_custom(GAOL "gaol/gaol.h" PATHS "${GAOL_DIR}")
  # Looking for gaol library, result is written in GAOL_LIB
  codac_find_library_custom(GAOL "gaol" PATHS "${GAOL_DIR}")

  set(GAOL_VERSION "")
  if(GAOL_INCDIR AND EXISTS "${GAOL_INCDIR}/gaol/gaol_configuration.h")
    file(STRINGS "${GAOL_INCDIR}/gaol/gaol_configuration.h" _gaol_version_lines
         REGEX "^#define GAOL_(MAJOR|MINOR|MICRO)_VERSION[ \t]+[0-9]+")
    set(_gaol_version_parts "")
    foreach(_gaol_part MAJOR MINOR MICRO)
      string(REGEX MATCH "GAOL_${_gaol_part}_VERSION[ \t]+([0-9]+)" _gaol_match "${_gaol_version_lines}")
      if(_gaol_match)
        list(APPEND _gaol_version_parts ${CMAKE_MATCH_1})
      endif()
    endforeach()
    list(LENGTH _gaol_version_parts _gaol_version_length)
    if(_gaol_version_length EQUAL 3)
      string(REPLACE ";" "." GAOL_VERSION "${_gaol_version_parts}")
    endif()
  endif()


################################################################################
# Result
################################################################################

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(GAOL
                                    REQUIRED_VARS GAOL_LIB GAOL_INCDIR MATHLIB_LIB MATHLIB_INCDIR
                                    VERSION_VAR GAOL_VERSION)

  if(GAOL_FOUND)
    set(GAOL_INCLUDE_DIRS ${GAOL_INCDIR} ${MATHLIB_INCDIR})
    list(REMOVE_DUPLICATES GAOL_INCLUDE_DIRS)
    set(GAOL_LIBRARIES ${GAOL_LIB} ${MATHLIB_LIB})
  endif()
