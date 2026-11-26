# ==================================================================
#  Codac - cmake module for GAOL, the interval arithmetic library
# ==================================================================
#
# codac2::Interval derives from gaol::interval (see
# src/core/domains/interval/codac2_Interval.h). GAOL is written by Frederic
# Goualard (https://frederic.goualard.net, https://github.com/goualard-f/GAOL)
# and distributed under the GNU LGPL v2.
#
# Codac is built on GAOL v5, the continuation of GAOL by Jordan Ninin,
# https://github.com/Jordan08/GAOL: a CMake build, the fixes Codac used to work
# around (Codac now calls GAOL without guards, see codac2_Interval.h), the
# elementary functions of CORE-MATH, correctly rounded, compiled into libgaol in
# the place of mathlib (libultim), and tests of its bounds. Fixes of GAOL go into
# that repository rather than into Codac.
#
# GAOL's interval operations are inline: the code including its headers, Codac's
# and that of Codac's users, has to be compiled with the flags of interval
# arithmetic (-frounding-math and its companions: without them, the compiler is
# free to evaluate a floating-point expression at compile time in the rounding
# mode to nearest, or to contract it, and a bound computed that way no longer
# encloses anything), and with the flags of the instructions libgaol was compiled
# with (-mfma). Which flags is GAOL's to say, and codac_gaol_find() takes them
# from the GAOL it finds or builds, in this order:
#
#  1. the CMake package of GAOL v5, find_package(gaol ${CODAC_GAOL_MIN_VERSION}
#     CONFIG): gaol::gaol carries the include directory and the flags (PUBLIC);
#  2. pkg-config, for a GAOL v5 installed without its CMake package (by its
#     autotools or meson build): the Cflags of its gaol.pc carry the flags, its
#     Libs libgaol (FindGAOL.cmake);
#  3. when neither is found, or with ENABLE_FIND_PACKAGE_GAOL OFF, GAOL is
#     downloaded from the master branch of https://github.com/Jordan08/GAOL and
#     built as a part of Codac (FetchContent), its version checked to be
#     CODAC_GAOL_MIN_VERSION or later, and installed with Codac
#     (codac_gaol_fetch()).
#
# A GAOL older than CODAC_GAOL_MIN_VERSION found in 1 or 2 is passed over, with
# a message saying so, and GAOL v5 is downloaded in 3: GAOL 4, which still needs
# the guards, GAOL 5.0.0, whose power Codac no longer reverses rightly, and GAOL
# 5.0.1, whose headers GCC 9.3 and 10.2 for 64-bit ARM cannot compile (see
# CODAC_GAOL_MIN_VERSION below).
#
# All three end in Codac::gaol, the imported target the Codac libraries link
# PUBLIC, and which codac-config.cmake defines again for their users
# (codac_gaol_config_snippet()).
#
# With the developer option WITH_IBEX (top-level CMakeLists.txt), none of the
# above: Codac is built as it was before GAOL v5, on IBEX and the GAOL 4 that
# IBEX bundles, with the guards. Codac::gaol then names Ibex::ibex
# (codac_gaol_from_ibex()).
#
# Origin
# ------
# What follows comes from IBEX (https://github.com/ibex-team/ibex-lib, GNU
# LGPL v3):
#
#  - codac_gaol_portability_flags() is the part of ibex_init_common()
#    (cmake.utils/ibex-config-utils.cmake) that sets the floating-point and
#    portability flags which are not those of interval arithmetic, in the version
#    of the IBEX fork maintained by Fabrice Le Bars,
#    https://github.com/lebarsfa/ibex-lib, tag ibex-2.8.9.20260819, which Codac
#    was built against until now. ibex_init_common() was written by Cyril
#    Bouvier for the CMake build of IBEX; the IEEE 754 flags it sets were added
#    to it by Fabrice Le Bars.
#
#  - codac_gaol_config_snippet() does, for Codac, what
#    create_target_import_and_export() of cmake.utils/IbexUtils.cmake (Cyril
#    Bouvier) does for IBEX: the lines that define the imported target of GAOL
#    again for a consumer of the installed library.
#
# What differs from IBEX
# ----------------------
# IBEX extracts GAOL and mathlib from archives kept in its own repository, and
# builds them as a part of itself (add_subdirectory()), installing them in
# include/ibex/3rd and lib/ibex/3rd. Here GAOL is found on the machine, or
# downloaded from its repository and built as a part of Codac by FetchContent,
# with the CMake build of GAOL v5, which supports it, and installed in the
# standard directories (include/gaol, lib, lib/cmake/gaol), where a later build
# of Codac, or of another project, finds it with find_package(gaol). The other
# differences are explained where they occur.


################################################################################
# CODAC_GAOL_MIN_VERSION
################################################################################
#
# The oldest GAOL Codac is built on without guards, asked of the GAOL found
# (find_package(), gaol.pc), of the GAOL downloaded, and again by
# codac-config.cmake and codac.pc for the users of the installed Codac.
#
# 5.0.2, for two reasons:
#  - not 5.0.0: PowOp::bwd() (src/core/operators/codac2_pow.h) reverses x1^x2
#    with pow_rel() and pow_exponent_rel() of GAOL alone, which GAOL 5.0.1 made
#    the reverses of gaol::pow, the power Codac computes, negative bases with an
#    integer exponent included (https://github.com/Jordan08/GAOL/issues/400). In
#    GAOL 5.0.0 they reversed the pow of IEEE 1788, which has no negative base,
#    and gaol::pow left out the negative bases of any exponent but a degenerate
#    integer: with it, PowOp::bwd() would empty x1 for y = x1^3 and x1 < 0,
#    losing solutions, and pow([-2,2],[0.5,3]) would be [0,8], without
#    (-1)^3 = -1.
#  - not 5.0.1: GCC 9.3 and 10.2 for 64-bit ARM (10.2 is the compiler of Debian
#    11 Bullseye, which the CI of Codac builds on for arm64) stop with an
#    internal compiler error in output_constant_pool_2 on the NEON intervals of
#    GAOL 5.0.0 and 5.0.1, whose constructor interval(l, r) set the two bounds
#    with vcombine_f64() (GCC bugs 99037 and 100441, fixed in GCC 9.4 and 10.3).
#    GAOL 5.0.2 sets them with vsetq_lane_f64()
#    (https://github.com/Jordan08/GAOL/pull/404). That constructor is in the
#    headers of GAOL, which the sources of Codac and those of its users include:
#    a GAOL 5.0.1 with its NEON intervals would stop such a compiler on Codac
#    too, even with libgaol compiled by another compiler.
# The package of GAOL is versioned SameMajorVersion: find_package(gaol 5.0.2)
# takes any GAOL 5 from 5.0.2 on.
set(CODAC_GAOL_MIN_VERSION 5.0.2)


################################################################################
# codac_gaol_portability_flags(<outvar>)
################################################################################
#
# Returns in <outvar> the flags of ibex_init_common() (see the top of this file)
# that are not those of interval arithmetic, and that Codac keeps whatever GAOL
# says: the top-level CMakeLists.txt puts them in CMAKE_CXX_FLAGS, where
# ibex_init_common() put them, after GAOL is brought in, so that GAOL is not
# compiled with them. They are returned as a list, one flag per element,
# because Codac also hands them over to its consumers, through CODAC_CXX_FLAGS
# and codac.pc; for the same reason, the Visual Studio "/D NAME" pairs of IBEX
# are spelt "/DNAME", which target_compile_options() cannot mistake for two
# duplicated "/D" and merge. The rest of ibex_init_common() is left out: the
# installation directories, the build type, the C++ standard and the uninstall
# target are Codac's own decisions, and the Debug-only -Wall -DDEBUG (/D DEBUG)
# have no use here, Codac choosing its own warnings and none of its code reading
# DEBUG.
#
# The two flags IBEX added for filib, which is not GAOL, are kept all the same,
# so that Codac goes on being compiled with the flags it had; so are those of
# Visual C++, until its jobs of the continuous integration tell whether Codac's
# own code still needs them with GAOL v5. With WITH_IBEX, ibex_init_common() is
# called itself and this function is not used.
function(codac_gaol_portability_flags outvar)

  include(CheckCXXCompilerFlag)
  set(flags "")

  if(MSVC)
    list(APPEND flags /D_CRT_SECURE_NO_WARNINGS /D_CRT_NONSTDC_NO_WARNINGS /Zc:__cplusplus /Zc:strictStrings-)
  elseif(APPLE)
    # Due to warnings on macOS with filib
    check_cxx_compiler_flag("-Wno-undefined-var-template" COMPILER_SUPPORTS_WNO_UNDEFINED_VAR_TEMPLATE)
    if(COMPILER_SUPPORTS_WNO_UNDEFINED_VAR_TEMPLATE)
      list(APPEND flags -Wno-undefined-var-template)
    endif()
  endif()
  # Claim IEEE 754 double compatibility, for filib
  list(APPEND flags -D__STDC_IEC_559__=1)

  set(${outvar} ${flags} PARENT_SCOPE)
endfunction()


################################################################################
# codac_gaol_fetch()
################################################################################
#
# Downloads GAOL from the master branch of https://github.com/Jordan08/GAOL and
# adds it to this build (FetchContent), as Eigen and Catch2 are: the target gaol,
# its alias gaol::gaol, and the installation of GAOL with Codac, in the standard
# directories (include/gaol, lib/libgaol.a or gaol.lib, lib/cmake/gaol and
# lib/pkgconfig/gaol.pc), since the Codac libraries are of no use without it.
# Installed there, it is found by find_package(gaol) at a later configuration,
# of Codac or of another project. Being the standard directories, they are
# also those of a GAOL installed in the same prefix by other means, a GAOL 4
# among them, which the installation of Codac overwrites.
#
# Built as a part of Codac rather than as a project of its own (Codac used to
# configure, build and install GAOL apart while it was configured): the CMake
# build of GAOL v5 is made for it (the alias gaol::gaol, -fPIC by default, an
# optimization when no build type is given, its tests and other targets only
# when GAOL is the project built, its installation with the project that brings
# it in). GAOL is then compiled with the compilers, the toolchain, the build
# type and the runtime library of Visual C++ of Codac, so that they always
# match: Visual C++ refuses to link objects compiled for its Debug runtime
# (/MDd) with others compiled for the Release one (/MD) -- error LNK2038,
# "mismatch detected for 'RuntimeLibrary'" -- which is why the top-level
# CMakeLists.txt had to force the Release runtime on Codac while GAOL was built
# apart, in Release.
#
# Not compiled with Codac's warnings and definitions, though (/W4, -Wall -Wextra
# -Wpedantic, /bigobj, FAST_RELEASE...): the top-level CMakeLists.txt adds them
# with add_compile_options() and add_compile_definitions(), whose directory
# properties a subdirectory takes from its parent when it is added. They are
# emptied while GAOL is added, and given back right after: GAOL is compiled with
# the flags it chooses, and with CMAKE_CXX_FLAGS as it is given to this build
# (codac_gaol_portability_flags() is added to it afterwards).
#
# From the head of the master branch, so that the fixes pushed to GAOL reach
# Codac without a change here. Cloned with Git, the sources are brought up to
# date with the branch at each configuration of Codac (the update step of
# FetchContent), which needs network access: without it, configure with
# -DFETCHCONTENT_UPDATES_DISCONNECTED_GAOL=ON to build the sources downloaded
# before. Without Git, the archive GitHub makes of the branch is downloaded
# instead, only once per build directory: a new commit reaches such a build once
# _deps/gaol-* is deleted from it. Whichever it is, the version of the GAOL
# obtained is checked: below CODAC_GAOL_MIN_VERSION, the configuration stops.
#
# Sets CODAC_GAOL_BINARY_DIR in the caller's scope: the build directory of GAOL,
# where its gaol.pc is (codac_gaol_pkg_config()).
function(codac_gaol_fetch)

  include(FetchContent)
  find_package(Git QUIET)
  if(GIT_FOUND)
    # The files as they are in the repository, whatever core.autocrlf says (true
    # on the Windows runners of GitHub Actions)
    FetchContent_Declare(gaol
      GIT_REPOSITORY https://github.com/Jordan08/GAOL.git
      GIT_TAG master
      GIT_SHALLOW TRUE
      GIT_CONFIG core.autocrlf=false)
  else()
    FetchContent_Declare(gaol URL https://github.com/Jordan08/GAOL/archive/refs/heads/master.zip)
  endif()

  message(STATUS "GAOL: downloading the master branch of https://github.com/Jordan08/GAOL, built with Codac")

  get_property(_codac_compile_options DIRECTORY PROPERTY COMPILE_OPTIONS)
  get_property(_codac_compile_definitions DIRECTORY PROPERTY COMPILE_DEFINITIONS)
  set_property(DIRECTORY PROPERTY COMPILE_OPTIONS "")
  set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS "")
  FetchContent_MakeAvailable(gaol)
  set_property(DIRECTORY PROPERTY COMPILE_OPTIONS "${_codac_compile_options}")
  set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS "${_codac_compile_definitions}")

  # The version of the GAOL downloaded, which its CMake build gives the library
  # (VERSION, from its VERSION.txt). master is GAOL v5; a branch that went back
  # below CODAC_GAOL_MIN_VERSION, or sources of an older GAOL left in _deps by an
  # older build of Codac and not updated (offline), would give Codac an interval
  # arithmetic that needs the guards it no longer has (GAOL 4), a power whose
  # reverse is not the one PowOp::bwd() needs (GAOL 5.0.0), or headers GCC 9.3
  # and 10.2 for 64-bit ARM cannot compile (GAOL 5.0.1).
  set(_version "")
  if(TARGET gaol::gaol)
    get_target_property(_target gaol::gaol ALIASED_TARGET)
    if(_target)
      get_target_property(_version ${_target} VERSION)
    endif()
  endif()
  if(NOT _version OR _version VERSION_LESS CODAC_GAOL_MIN_VERSION)
    message(FATAL_ERROR "The GAOL downloaded from the master branch of https://github.com/Jordan08/GAOL is "
                        "version ${_version}, older than ${CODAC_GAOL_MIN_VERSION}, the oldest one Codac is "
                        "built on without guards (or it defines no target gaol::gaol). Configure Codac again "
                        "with network access, after "
                        "deleting ${FETCHCONTENT_BASE_DIR}/gaol-*, or with -DWITH_IBEX=ON to build on IBEX.")
  endif()
  message(STATUS "GAOL ${_version} downloaded, built with Codac and installed with it")

  set(GAOL_VERSION "${_version}" PARENT_SCOPE)
  set(CODAC_GAOL_BINARY_DIR "${gaol_BINARY_DIR}" PARENT_SCOPE)
endfunction()


################################################################################
# codac_gaol_find()
################################################################################
#
# Finds GAOL v5 in the order given at the top of this file, or downloads it,
# and defines Codac::gaol, the imported target through which the Codac
# libraries, and the targets linking them, get GAOL's include directories,
# flags and library. Sets in the caller's scope:
#
#   CODAC_GAOL_FROM           "package", "pkg-config" or "fetched"
#   CODAC_GAOL_BINARY_DIR     the build directory of the GAOL fetched
#   CODAC_INTERVAL_CXX_FLAGS  the flags Codac::gaol gives (codac_gaol_flags())
#   GAOL_VERSION
#
# An INTERFACE library rather than GAOL's own target, whatever provided GAOL:
# CODAC_LIBRARIES names it, and codac-config.cmake defines it again
# (codac_gaol_config_snippet()), which it could not do under the name gaol::gaol
# without clashing with a find_package(gaol) of the consumer.
function(codac_gaol_find)

  set(_from "")
  set(_version "")
  set(_binary_dir "")

  if(ENABLE_FIND_PACKAGE_GAOL)

    # 1. The CMake package of GAOL (gaol_DIR, or CMAKE_PREFIX_PATH), version
    # CODAC_GAOL_MIN_VERSION asked. The version is checked by the package's
    # gaolConfigVersion.cmake (SameMajorVersion) before the package is loaded: a
    # GAOL 4, 5.0.0 or 5.0.1 is refused without defining its targets. Loaded,
    # the gaol::gaol of such a GAOL would clash with the alias gaol::gaol of the
    # GAOL v5 downloaded next, and CMake would stop ("add_library cannot create
    # ALIAS target "gaol::gaol" because another target with the same name
    # already exists").
    find_package(gaol ${CODAC_GAOL_MIN_VERSION} CONFIG QUIET)
    if(gaol_FOUND)
      set(_from package)
      set(_version "${gaol_VERSION}")
      message(STATUS "Found GAOL ${gaol_VERSION}, CMake package in ${gaol_DIR}")
    elseif(gaol_CONSIDERED_VERSIONS)
      message(STATUS "Found GAOL ${gaol_CONSIDERED_VERSIONS} (CMake package ${gaol_CONSIDERED_CONFIGS}), "
                     "older than ${CODAC_GAOL_MIN_VERSION}: not used")
    endif()

    # 2. pkg-config (PKG_CONFIG_PATH, or CMAKE_PREFIX_PATH), see FindGAOL.cmake
    if(NOT _from)
      find_package(GAOL MODULE QUIET)
      if(GAOL_FOUND)
        set(_from pkg-config)
        set(_version "${GAOL_VERSION}")
        message(STATUS "Found GAOL ${GAOL_VERSION}, gaol.pc in ${GAOL_PC_PREFIX}, used with the flags of its Cflags")
      endif()
    endif()
  endif()

  # 3. Downloaded and built with Codac
  if(NOT _from)
    codac_gaol_fetch()
    set(_from fetched)
    set(_version "${GAOL_VERSION}")
    set(_binary_dir "${CODAC_GAOL_BINARY_DIR}")
  endif()

  add_library(Codac::gaol INTERFACE IMPORTED)
  if(_from STREQUAL "pkg-config")
    set_target_properties(Codac::gaol PROPERTIES INTERFACE_LINK_LIBRARIES PkgConfig::CODAC_GAOL_PC)
  else()
    set_target_properties(Codac::gaol PROPERTIES INTERFACE_LINK_LIBRARIES gaol::gaol)
  endif()

  codac_gaol_flags(Codac::gaol _flags)

  set(CODAC_GAOL_FROM "${_from}" PARENT_SCOPE)
  set(CODAC_GAOL_BINARY_DIR "${_binary_dir}" PARENT_SCOPE)
  set(CODAC_INTERVAL_CXX_FLAGS ${_flags} PARENT_SCOPE)
  set(GAOL_VERSION "${_version}" PARENT_SCOPE)
endfunction()


################################################################################
# codac_gaol_from_ibex(<CMAKE_CXX_FLAGS before ibex_init_common()>)
################################################################################
#
# With WITH_IBEX: Codac::gaol for the GAOL 4 of IBEX, after the top-level
# CMakeLists.txt has run find_package(IBEX) and ibex_init_common() as Codac did
# before GAOL v5 (ibex_init_common() sets variables in the scope it is called
# from, and cannot be called from a function here). Codac::gaol names Ibex::ibex,
# which brings GAOL, mathlib and their flags (Ibex::gaol carries -frounding-math).
#
# Sets CODAC_GAOL_FROM to "ibex" in the caller's scope, and
# CODAC_INTERVAL_CXX_FLAGS to what ibex_init_common() appended to
# CMAKE_CXX_FLAGS: the IEEE 754 flags of the IBEX of Fabrice Le Bars, for
# instance, which this build compiles every target with, and which the users of
# the installed Codac get through CODAC_CXX_FLAGS. They used to call
# ibex_init_common() themselves after find_package(CODAC), as the examples did;
# the examples no longer do, whatever IBEX or GAOL Codac is built on.
function(codac_gaol_from_ibex cxx_flags_before)

  set(_added "")
  string(LENGTH "${cxx_flags_before}" _length)
  string(LENGTH "${CMAKE_CXX_FLAGS}" _length_after)
  if(_length_after GREATER _length)
    string(SUBSTRING "${CMAKE_CXX_FLAGS}" 0 ${_length} _start)
    if(_start STREQUAL cxx_flags_before)
      string(SUBSTRING "${CMAKE_CXX_FLAGS}" ${_length} -1 _added)
      separate_arguments(_added UNIX_COMMAND "${_added}")
    endif()
  endif()

  add_library(Codac::gaol INTERFACE IMPORTED)
  set_target_properties(Codac::gaol PROPERTIES INTERFACE_LINK_LIBRARIES Ibex::ibex)

  set(CODAC_GAOL_FROM ibex PARENT_SCOPE)
  set(CODAC_INTERVAL_CXX_FLAGS ${_added} PARENT_SCOPE)
endfunction()


################################################################################
# codac_gaol_flags(<target> <outvar>)
################################################################################
#
# Returns in <outvar> the compilation flags <target> gives the targets linking
# it, walked out of it and of the targets it links: its options and its
# definitions (as -D, or /D with Visual C++). This is for what cannot link
# Codac::gaol: CODAC_CXX_FLAGS in codac-config.cmake. Of the generator
# expressions, $<$<COMPILE_LANGUAGE:CXX>:...> is unwrapped (GAOL gives
# -faligned-new=8 to C++ only, on 32-bit Windows), the others left out, and so
# is what $<LINK_ONLY:...> names, which gives no flag.
function(codac_gaol_flags target outvar)

  set(flags "")
  set(queue ${target})
  set(seen "")

  while(queue)
    list(GET queue 0 item)
    list(REMOVE_AT queue 0)
    if(item IN_LIST seen OR NOT TARGET "${item}")
      continue()
    endif()
    list(APPEND seen "${item}")

    get_target_property(_value "${item}" INTERFACE_COMPILE_OPTIONS)
    if(_value)
      list(APPEND flags ${_value})
    endif()
    get_target_property(_value "${item}" INTERFACE_COMPILE_DEFINITIONS)
    if(_value)
      foreach(_definition ${_value})
        if(MSVC)
          list(APPEND flags "/D${_definition}")
        else()
          list(APPEND flags "-D${_definition}")
        endif()
      endforeach()
    endif()
    get_target_property(_value "${item}" INTERFACE_LINK_LIBRARIES)
    if(_value)
      list(APPEND queue ${_value})
    endif()
  endwhile()

  set(_flags "")
  foreach(flag ${flags})
    if(flag MATCHES "^\\$<\\$<COMPILE_LANGUAGE:CXX>:(.*)>$")
      list(APPEND _flags "${CMAKE_MATCH_1}")
    elseif(NOT flag MATCHES "\\$<")
      list(APPEND _flags "${flag}")
    endif()
  endforeach()
  if(_flags)
    list(REMOVE_DUPLICATES _flags)
  endif()

  set(${outvar} ${_flags} PARENT_SCOPE)
endfunction()


################################################################################
# codac_gaol_pkg_config(<cflags_var> <requires_var> <libs_var>)
################################################################################
#
# Returns what codac.pc says of GAOL (src/CMakeLists.txt), which cannot link a
# target:
#
#  - for the GAOL of IBEX (WITH_IBEX): "Requires: ibex" and
#    -I${includedir}/ibex, as codac.pc said before GAOL v5;
#  - for a GAOL v5 found by its CMake package or its gaol.pc: "Requires: gaol
#    >= CODAC_GAOL_MIN_VERSION", whose gaol.pc gives the flags of interval
#    arithmetic and libgaol;
#  - for the GAOL v5 downloaded and installed with Codac: the Cflags and Libs of
#    the gaol.pc its build wrote, copied into those of codac.pc. GAOL installs
#    its gaol.pc in lib/pkgconfig, and codac.pc goes to share/pkgconfig: a
#    "Requires: gaol" would fail for a user whose PKG_CONFIG_PATH names only the
#    latter, with a prefix pkg-config does not search by default. The Cflags
#    and Libs of gaol.pc name ${includedir} and ${libdir}, which codac.pc
#    defines too, as the same directories: GAOL is installed under the prefix of
#    Codac, with the same CMAKE_INSTALL_INCLUDEDIR and CMAKE_INSTALL_LIBDIR.
#    With Visual C++, GAOL writes no gaol.pc, and nothing is copied.
function(codac_gaol_pkg_config cflags_var requires_var libs_var)

  set(_cflags "")
  set(_requires "")
  set(_libs "")

  if(CODAC_GAOL_FROM STREQUAL "ibex")
    set(_cflags "-I\${includedir}/ibex")
    set(_requires "ibex")
  elseif(CODAC_GAOL_FROM STREQUAL "fetched")
    if(EXISTS "${CODAC_GAOL_BINARY_DIR}/gaol.pc")
      file(STRINGS "${CODAC_GAOL_BINARY_DIR}/gaol.pc" _lines REGEX "^(Cflags|Libs):")
      foreach(_line ${_lines})
        if(_line MATCHES "^Cflags:(.*)$")
          string(STRIP "${CMAKE_MATCH_1}" _cflags)
        elseif(_line MATCHES "^Libs:(.*)$")
          string(STRIP "${CMAKE_MATCH_1}" _libs)
        endif()
      endforeach()
      # Without the -L${libdir} that the Libs of codac.pc start with already
      string(REGEX REPLACE "^-L\\$\\{libdir\\}[ ]*" "" _libs "${_libs}")
    endif()
  else()
    set(_requires "gaol >= ${CODAC_GAOL_MIN_VERSION}")
  endif()

  set(${cflags_var} "${_cflags}" PARENT_SCOPE)
  set(${requires_var} "${_requires}" PARENT_SCOPE)
  set(${libs_var} "${_libs}" PARENT_SCOPE)
endfunction()


################################################################################
# codac_gaol_config_snippet(<outvar>)
################################################################################
#
# Returns the lines of codac-config.cmake that define Codac::gaol for a consumer
# of the installed Codac, which CODAC_LIBRARIES names -- the counterpart of the
# ibex-config-gaol.cmake and ibex-config-ultim.cmake files that
# create_target_import_and_export() writes for IBEX.
#
#  - The GAOL v5 downloaded and installed with Codac is found again by
#    find_package(gaol CODAC_GAOL_MIN_VERSION CONFIG) under the prefix Codac is
#    installed under, by a path relative to the configuration file, since the
#    prefix may be moved, and only there: Codac's inline interval operations are
#    compiled against the GAOL of the program that includes them, which has to
#    be the one libgaol was compiled from, and a package search path of the
#    consumer could lead to another.
#  - A GAOL v5 found by its CMake package, where it was found, before the usual
#    search, version CODAC_GAOL_MIN_VERSION asked as when Codac was built.
#  - A GAOL v5 found by pkg-config is named by what its gaol.pc gave: the
#    include directories, the flags, the library files and the link options.
#  - The GAOL of IBEX (WITH_IBEX), by find_package(IBEX), as codac-config.cmake
#    did before GAOL v5, where IBEX was found.
function(codac_gaol_config_snippet outvar)

  if(CODAC_GAOL_FROM STREQUAL "ibex")
    set(_find "find_package(IBEX REQUIRED HINTS \"${IBEX_DIR}\")")
    set(_properties "INTERFACE_LINK_LIBRARIES Ibex::ibex")
  elseif(CODAC_GAOL_FROM STREQUAL "fetched")
    file(RELATIVE_PATH _to_prefix "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_CMAKE}" "${CMAKE_INSTALL_PREFIX}")
    set(_find "get_filename_component(_codac_prefix \"\${CMAKE_CURRENT_LIST_DIR}/${_to_prefix}\" ABSOLUTE)
    find_package(gaol ${CODAC_GAOL_MIN_VERSION} CONFIG REQUIRED NO_DEFAULT_PATH
                 PATHS \"\${_codac_prefix}/${CMAKE_INSTALL_LIBDIR}/cmake/gaol\")")
    set(_properties "INTERFACE_LINK_LIBRARIES gaol::gaol")
  elseif(CODAC_GAOL_FROM STREQUAL "package")
    set(_find "find_package(gaol ${CODAC_GAOL_MIN_VERSION} CONFIG REQUIRED HINTS \"${gaol_DIR}\")")
    set(_properties "INTERFACE_LINK_LIBRARIES gaol::gaol")
  else()
    set(_find "")
    set(_properties "")
    foreach(_property INTERFACE_INCLUDE_DIRECTORIES INTERFACE_COMPILE_OPTIONS
                      INTERFACE_LINK_LIBRARIES INTERFACE_LINK_OPTIONS)
      get_target_property(_value PkgConfig::CODAC_GAOL_PC ${_property})
      if(_value)
        if(_properties)
          string(APPEND _properties "\n                            ")
        endif()
        string(APPEND _properties "${_property} \"${_value}\"")
      endif()
    endforeach()
  endif()

  set(${outvar} "
    # GAOL, the interval arithmetic library Codac is built upon, with the flags
    # of interval arithmetic, as Codac::gaol, which CODAC_LIBRARIES names
    ${_find}
    if(NOT TARGET Codac::gaol)
      add_library(Codac::gaol INTERFACE IMPORTED)
      set_target_properties(Codac::gaol PROPERTIES
                            ${_properties})
    endif()
" PARENT_SCOPE)
endfunction()
