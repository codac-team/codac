# ==================================================================
#  Codac - cmake module looking for an installed GAOL v5 with pkg-config
# ==================================================================
#
# Looks for GAOL v5 (https://github.com/Jordan08/GAOL), the interval arithmetic
# library Codac is built upon, by its gaol.pc: the second way of
# codac_gaol_find() (scripts/CMakeModules/codac_gaol.cmake), for a GAOL v5
# installed without its CMake package, which codac_gaol_find() looks for first.
# The three builds of GAOL v5 (autotools, meson and CMake) install a gaol.pc
# whose Cflags carry the flags of interval arithmetic and of the instructions
# libgaol was compiled with, and whose Libs carry libgaol, into which the
# elementary functions of CORE-MATH are compiled. That GAOL is used with those
# flags, as its gaol.pc gives them.
#
# Where to look, when gaol.pc is not in a directory pkg-config searches by
# default: PKG_CONFIG_PATH, or CMAKE_PREFIX_PATH, which FindPkgConfig adds to
# it.
#
# A gaol.pc older than CODAC_GAOL_MIN_VERSION (codac_gaol.cmake: GAOL 4, which
# Codac would have to call with the guards it no longer has, GAOL 5.0.0, whose
# power Codac no longer reverses rightly, or GAOL 5.0.1, whose headers GCC 9.3
# and 10.2 for 64-bit ARM cannot compile) is not used, and neither is a
# gaol.pc whose Cflags lack -frounding-math with a compiler that takes it, as
# the one the meson build of Frederic Goualard's GAOL 4 installs: it does not
# describe the flags GAOL needs. A message says why. Not with Visual C++, whose
# libraries pkg-config does not name.
#
# Defines:
#
#   GAOL_FOUND
#   GAOL_VERSION                the version gaol.pc states
#   GAOL_PC_PREFIX              the prefix of gaol.pc
#   PkgConfig::CODAC_GAOL_PC    the imported target pkg_check_modules() makes
#                               of gaol.pc: include directories, flags (its
#                               INTERFACE_COMPILE_OPTIONS), library files and
#                               link options
#
# codac_gaol_find() makes Codac::gaol link PkgConfig::CODAC_GAOL_PC.

set(GAOL_VERSION "")
set(GAOL_PC_PREFIX "")

if(NOT MSVC)
  find_package(PkgConfig QUIET)
endif()

if(PKG_CONFIG_FOUND)

  pkg_check_modules(CODAC_GAOL_PC QUIET IMPORTED_TARGET "gaol>=${CODAC_GAOL_MIN_VERSION}")

  if(CODAC_GAOL_PC_FOUND)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(-frounding-math COMPILER_SUPPORTS_FROUNDING_MATH)
    if(COMPILER_SUPPORTS_FROUNDING_MATH AND NOT "-frounding-math" IN_LIST CODAC_GAOL_PC_CFLAGS_OTHER)
      message(STATUS "Found gaol.pc ${CODAC_GAOL_PC_VERSION} in ${CODAC_GAOL_PC_PREFIX}, whose Cflags lack "
                     "-frounding-math: not used")
    else()
      set(GAOL_VERSION "${CODAC_GAOL_PC_VERSION}")
      set(GAOL_PC_PREFIX "${CODAC_GAOL_PC_PREFIX}")
    endif()

  else()
    # Any gaol.pc, to say why it is not used
    pkg_check_modules(CODAC_GAOL_PC_ANY QUIET gaol)
    if(CODAC_GAOL_PC_ANY_FOUND)
      message(STATUS "Found gaol.pc ${CODAC_GAOL_PC_ANY_VERSION} in ${CODAC_GAOL_PC_ANY_PREFIX}, "
                     "older than ${CODAC_GAOL_MIN_VERSION}: not used")
    endif()
  endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GAOL
                                  REQUIRED_VARS GAOL_PC_PREFIX GAOL_VERSION
                                  VERSION_VAR GAOL_VERSION)
