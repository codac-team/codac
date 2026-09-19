# ==================================================================
#  Codac - cmake module for GAOL, the interval arithmetic library
# ==================================================================
#
# codac2::Interval derives from gaol::interval (see
# src/core/domains/interval/codac2_Interval.h). GAOL is written by Frederic
# Goualard (https://frederic.goualard.net, https://github.com/goualard-f/GAOL)
# and distributed under the GNU LGPL v2. It computes its elementary functions
# with mathlib, the IBM Accurate Portable Mathematical Library (libultim), which
# Frederic Goualard distributes along with it, under the GNU GPL v2 or later.
#
# Codac builds the GAOL of Jordan Ninin's fork, https://github.com/Jordan08/GAOL,
# which adds to GAOL a CMake build, the fixes Codac depends on (those of the
# patch IBEX applies to GAOL, and those Visual C++, MinGW and ARM processors
# need) and tests of its bounds. Fixes of GAOL go into the fork rather than into
# Codac. The fork's CMake build downloads mathlib from Frederic Goualard's site,
# and refuses the compilers that do not compute GAOL's intervals right (see its
# README.md).
#
# GAOL's interval operations are inline: the code including its headers, Codac's
# and that of Codac's users, has to be compiled with the flags of interval
# arithmetic, and linked with GAOL and mathlib. Which flags and which libraries
# is GAOL's to say, and codac_gaol_find() takes them from the GAOL it finds or
# builds, in this order:
#
#  1. the CMake package of GAOL, which the CMake build of the fork installs:
#     find_package(gaol CONFIG) gives gaol::gaol, which carries the include
#     directory, the flags (PUBLIC), __GAOL_PUBLIC__= for Visual C++, and
#     mathlib (gaol::ultim);
#  2. pkg-config: the gaol.pc the autotools and meson builds of the fork install
#     carries the flags in its Cflags, and GAOL and mathlib in its Libs;
#  3. the files themselves (FindGAOL.cmake), for a GAOL installed with neither,
#     whose flags Codac then determines itself (codac_gaol_interval_flags());
#  4. when none is found, or with ENABLE_FIND_PACKAGE_GAOL OFF, codac_gaol_build()
#     builds the fork with its CMake build and installs it with its installer,
#     and find_package(gaol CONFIG) takes the package it installed, as in 1.
#
# All four end in Codac::gaol, the imported target the Codac libraries link
# PUBLIC, and which codac-config.cmake defines again for their users
# (codac_gaol_config_snippet()).
#
# Origin
# ------
# What follows comes from IBEX (https://github.com/ibex-team/ibex-lib, GNU
# LGPL v3):
#
#  - codac_gaol_portability_flags() and codac_gaol_interval_flags() are, divided
#    in two, the part of ibex_init_common() (cmake.utils/ibex-config-utils.cmake)
#    that sets the floating-point flags, in the version of the IBEX fork
#    maintained by Fabrice Le Bars, https://github.com/lebarsfa/ibex-lib, tag
#    ibex-2.8.9.20260819, which Codac was built against until now.
#    ibex_init_common() was written by Cyril Bouvier for the CMake build of IBEX;
#    the flags it sets -- -frounding-math and the others that IEEE 754 double
#    support depends on, and their Visual Studio counterparts -- were added to it
#    by Fabrice Le Bars. The condition under which -ffloat-store is added is the
#    one of the CMake build of the fork of GAOL (its CMakeLists.txt).
#
#  - codac_gaol_build() is the "not found, install it" branch of
#    interval_lib_wrapper/gaol/CMakeLists.txt, written by Cyril Bouvier: build
#    mathlib and GAOL, and install the result next to the library it serves, as
#    IBEX does in include/ibex/3rd and lib/ibex/3rd. The CMake build of GAOL and
#    mathlib it runs is the fork's, which says where it comes from: from IBEX
#    (Cyril Bouvier, Gilles Chabert), with the portability fixes for Visual
#    C++, MinGW and ARM of the forks of GAOL and mathlib by Fabrice Le Bars.
#
#  - codac_gaol_config_snippet() does, for Codac, what
#    create_target_import_and_export() of cmake.utils/IbexUtils.cmake (Cyril
#    Bouvier) does for IBEX: the lines that define the imported target of GAOL
#    again for a consumer of the installed library.
#
# What differs from IBEX
# ----------------------
# IBEX extracts GAOL and mathlib from archives kept in its own repository, and
# builds them as a part of itself (add_subdirectory()). Here GAOL is downloaded
# from the fork, at the head of its master branch, and built as a project of its
# own when Codac is configured, for the reasons given at codac_gaol_build(); the
# fork's build downloads mathlib, checked against the SHA256 of its archive. The
# other differences are explained where they occur.


# Where a GAOL built by codac_gaol_build() is installed, under the installation
# prefix. IBEX keeps the libraries it builds for itself in include/ibex/3rd and
# lib/ibex/3rd, out of the way of a GAOL installed separately; include/codac
# cannot play that part here, being the name of Codac's umbrella header. GAOL's
# CMake package goes in lib/codac-3rd/cmake/gaol, where the search of
# find_package() does not look: only codac-config.cmake finds it there.
set(CODAC_INSTALL_INCLUDEDIR_3RD "${CMAKE_INSTALL_INCLUDEDIR}/codac-3rd")
set(CODAC_INSTALL_LIBDIR_3RD "${CMAKE_INSTALL_LIBDIR}/codac-3rd")

# Where codac_gaol_build() downloads, builds and installs GAOL in the build tree
set(CODAC_GAOL_WORK_DIR "${CMAKE_BINARY_DIR}/_deps/gaol")


################################################################################
# codac_gaol_portability_flags(<outvar>)
################################################################################
#
# Returns in <outvar> the flags of ibex_init_common() (see the top of this file)
# that are not those of interval arithmetic, and that Codac keeps whatever GAOL
# says: the top-level CMakeLists.txt puts them in CMAKE_CXX_FLAGS, where
# ibex_init_common() put them. They are returned as a list, one flag per
# element, because Codac also hands them over to its consumers, through
# CODAC_CXX_FLAGS and codac.pc; for the same reason, the Visual Studio "/D NAME"
# pairs of IBEX are spelt "/DNAME", which target_compile_options() cannot
# mistake for two duplicated "/D" and merge. The rest of ibex_init_common() is
# left out: the installation directories, the build type, the C++ standard and
# the uninstall target are Codac's own decisions, and the Debug-only -Wall
# -DDEBUG (/D DEBUG) have no use here, Codac choosing its own warnings and none
# of its code reading DEBUG.
#
# The two flags IBEX added for filib, which is not GAOL, are kept all the same,
# so that Codac goes on being compiled with the flags it had.
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
# codac_gaol_interval_flags(<outvar>)
################################################################################
#
# Returns in <outvar> the compilation flags interval arithmetic depends on, for
# a GAOL found by its files alone, without a CMake package or a gaol.pc to say
# them (see codac_gaol_find()). Without -frounding-math and its companions, the
# compiler is free to evaluate a floating-point expression at compile time in
# the default rounding mode, or to contract it, and a bound computed that way no
# longer encloses anything.
#
# From ibex_init_common() (see the top of this file), which appends the same
# flags to CMAKE_C_FLAGS and CMAKE_CXX_FLAGS, but for -ffloat-store, added only
# where the builds of GAOL add it.
function(codac_gaol_interval_flags outvar)

  include(CheckCXXCompilerFlag)
  include(CheckCXXSourceCompiles)
  set(flags "")

  if(MSVC)
    list(APPEND flags /fp:strict)
  else()
    # Each flag is kept only where the compiler takes it. The check results are
    # cached under the names ibex_init_common() gives them, e.g.
    # COMPILER_SUPPORTS_FROUNDING_MATH for -frounding-math.
    foreach(flag -frounding-math -fno-fast-math -ffp-contract=off
                 -ffp-mode=full -fp-model=strict -fp:strict -mpc64)
      string(MAKE_C_IDENTIFIER "${flag}" _flag_id)
      string(TOUPPER "COMPILER_SUPPORTS${_flag_id}" _flag_var)
      check_cxx_compiler_flag("${flag}" ${_flag_var})
      if(${_flag_var})
        list(APPEND flags ${flag})
      endif()
    endforeach()

    # Not from ibex_init_common(), but Codac's own: on a 32-bit x86 processor,
    # doubles are computed in SSE2 rather than on the x87 FPU. Visual Studio,
    # in the branch above, computes them in SSE2 already.
    #
    # Computed on the x87, GAOL's bounds and mathlib's results are only right
    # while the precision of the x87 is set to 53 bits, and nothing keeps it
    # so: mathlib's Init_Lib() sets it where mathlib has a version for 32-bit x86
    # (see cmake/mathlib/mathlib_configuration.h.in in the fork of GAOL), but
    # GAOL, initialised right after, restores the default floating-point
    # environment (gaol::init()), whose precision is 64 bits on Linux and
    # with MinGW. Built for an i686 computing on the x87 (Clang 21 with
    # -mcpu=i686), GAOL returned [1.99975, 1.99975] for exp([1,1]), and the
    # bounds of exp, sin and cos missed the exact value for 4000, 3302 and 3913
    # of 4000 random arguments; built with the two flags below, the same
    # program gave all of its 48000 bounds bit for bit as on x86_64. In SSE2,
    # the precision is not a setting. SSE2 asks nothing more of the processor
    # than GAOL's own builds do: they compile GAOL for SSE2 on these systems.
    check_cxx_source_compiles("
      #if !defined(__i386__)
      #error not a 32-bit x86 target
      #endif
      int main() { return 0; }"
      CODAC_TARGET_IS_X86_32)
    if(CODAC_TARGET_IS_X86_32)
      check_cxx_compiler_flag("-msse2 -mfpmath=sse" COMPILER_SUPPORTS_MSSE2_MFPMATH_SSE)
      if(COMPILER_SUPPORTS_MSSE2_MFPMATH_SSE)
        list(APPEND flags -msse2 -mfpmath=sse)
      endif()
    endif()

    # -ffloat-store only where doubles are still computed in extended precision
    # with the flags above (FLT_EVAL_METHOD not 0: a 32-bit x86 target computing
    # on the x87), whose 80-bit registers keep more digits than a double: the
    # condition under which the CMake, autotools and meson builds of the fork of
    # GAOL add it. ibex_init_common() added it wherever the compiler took it.
    # Where doubles are computed in double precision (SSE2, ARM and the other
    # processors), it brings nothing to the bounds, and makes GCC store every
    # floating-point variable in memory rather than in a register: with it, on
    # x86_64 with GCC 9.4, x + y took 9.1 ns rather than 3.0, and sqrt(x) 41 ns
    # rather than 8.2.
    string(REPLACE ";" " " CMAKE_REQUIRED_FLAGS "${flags}")
    check_cxx_source_compiles("
      #include <cfloat>
      #if defined(FLT_EVAL_METHOD) && FLT_EVAL_METHOD == 0
      #error doubles are computed in double precision
      #endif
      int main() { return 0; }"
      CODAC_DOUBLES_IN_EXTENDED_PRECISION)
    unset(CMAKE_REQUIRED_FLAGS)
    if(CODAC_DOUBLES_IN_EXTENDED_PRECISION)
      check_cxx_compiler_flag(-ffloat-store COMPILER_SUPPORTS_FFLOAT_STORE)
      if(COMPILER_SUPPORTS_FFLOAT_STORE)
        list(APPEND flags -ffloat-store)
      endif()
    endif()

    # Not from ibex_init_common() either: a warning when the compiler takes
    # -frounding-math but says it does not honour the rounding direction on the
    # target, as Clang does for 32-bit ARM processors ("overriding currently
    # unsupported rounding mode on this target"). It then optimises the
    # negations by which GAOL rounds downward with the rounding direction set
    # upward, and no flag or change to GAOL can prevent it. Built by Clang 21 for
    # 32-bit ARM, 4556 of 16000 random products, squares and cubes computed by
    # GAOL did not enclose their exact value; built by GCC 15, none. The builds
    # of the fork of GAOL refuse such a compiler, and so do its headers; the
    # warning remains for a GAOL found without its CMake package or its gaol.pc.
    if(COMPILER_SUPPORTS_FROUNDING_MATH)
      set(CMAKE_REQUIRED_FLAGS "-frounding-math")
      check_cxx_source_compiles("int main() { return 0; }" CODAC_COMPILER_HONOURS_ROUNDING_MATH
                                FAIL_REGEX "unsupported rounding mode")
      unset(CMAKE_REQUIRED_FLAGS)
      if(NOT CODAC_COMPILER_HONOURS_ROUNDING_MATH)
        message(WARNING "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} does not honour the rounding "
                        "direction on this target (-frounding-math): the intervals computed by Codac may not "
                        "enclose the values they should. Use a compiler that does, such as GCC.")
      endif()
    endif()
  endif()

  set(${outvar} ${flags} PARENT_SCOPE)
endfunction()


################################################################################
# codac_gaol_step(<description> <log file> <command>...)
################################################################################
#
# Runs one step of codac_gaol_build(), its output written to <log file>, and
# stops the configuration with that output when the step fails, as
# LOG_OUTPUT_ON_FAILURE does for an ExternalProject.
function(codac_gaol_step description log)
  execute_process(COMMAND ${ARGN}
                  OUTPUT_FILE "${log}" ERROR_FILE "${log}"
                  RESULT_VARIABLE result)
  if(NOT result EQUAL 0)
    file(READ "${log}" output)
    message(FATAL_ERROR "GAOL: ${description} failed (${result}). Its output, in ${log}:\n${output}")
  endif()
endfunction()


################################################################################
# codac_gaol_build()
################################################################################
#
# Downloads GAOL, builds it and mathlib in Release with the CMake build of the
# fork, and installs them in the build tree with its installer, all while Codac
# is configured; sets CODAC_GAOL_INSTALL_TREE, where they are installed, in the
# caller's scope, for codac_gaol_find() to find the CMake package of GAOL there.
# GAOL and mathlib are also installed with Codac, by the same installer, in
# CODAC_INSTALL_INCLUDEDIR_3RD and CODAC_INSTALL_LIBDIR_3RD, since the Codac
# libraries are of no use without them.
#
# While Codac is configured, and not while it is built, as with an
# ExternalProject: the flags and the libraries of GAOL are read from the CMake
# package it installs, which has to exist for find_package() to read it.
#
# A project of its own, as in IBEX's build of GAOL with its autotools, rather
# than the FetchContent that Eigen and Catch2 are brought in with, which would
# build GAOL as a part of this project. Kept apart, GAOL is compiled with the
# flags it chooses, and not with Codac's warnings, and it is always built in
# Release, whatever the configuration of Codac, which is what the MSVC runtime
# choice of the top-level CMakeLists.txt counts on. CMAKE_CXX_FLAGS and
# CMAKE_C_FLAGS are handed over as they are when this is called, before Codac
# adds its own flags to them.
#
# GAOL comes from the head of the master branch of the fork (version 4.3.1 of
# GAOL), so that the fixes pushed to the fork reach Codac without a change here.
# Cloned with Git, the sources are brought up to date with the branch at each
# configuration of Codac, which needs network access (without it, the sources
# already downloaded are built, with a warning), and only what a new commit
# changes is compiled again. Without Git, the archive GitHub makes of the
# branch is downloaded instead, only once per build directory: a new commit
# reaches such a build once _deps/gaol is deleted from it. The fork's CMake
# build downloads mathlib 2.1.1 from Frederic Goualard's site, checks its
# checksum, and builds and installs it along with GAOL.
function(codac_gaol_build)

  set(_work "${CODAC_GAOL_WORK_DIR}")
  set(_source "${_work}/src")
  set(_binary "${_work}/build")
  set(_install "${_work}/install")
  file(MAKE_DIRECTORY "${_work}")

  message(STATUS "GAOL: downloading the fork of GAOL, building it and installing it in ${_work}")

  # The sources
  find_package(Git QUIET)
  if(GIT_FOUND)
    if(NOT EXISTS "${_source}/.git")
      file(REMOVE_RECURSE "${_source}")
      # The files as they are in the repository, whatever core.autocrlf says
      # (true on the Windows runners of GitHub Actions).
      codac_gaol_step("cloning https://github.com/Jordan08/GAOL.git" "${_work}/download.log"
        "${GIT_EXECUTABLE}" clone --depth 1 --branch master --config core.autocrlf=false
        https://github.com/Jordan08/GAOL.git "${_source}")
    else()
      execute_process(COMMAND "${GIT_EXECUTABLE}" -C "${_source}" fetch --depth 1 origin master
                      OUTPUT_FILE "${_work}/update.log" ERROR_FILE "${_work}/update.log"
                      RESULT_VARIABLE _fetch_result)
      if(_fetch_result EQUAL 0)
        codac_gaol_step("updating the sources with the master branch" "${_work}/update.log"
          "${GIT_EXECUTABLE}" -C "${_source}" reset --hard FETCH_HEAD)
      else()
        message(WARNING "GAOL could not be brought up to date with the master branch of "
                        "https://github.com/Jordan08/GAOL.git (see ${_work}/update.log): "
                        "the sources downloaded before are built.")
      endif()
    endif()
  elseif(NOT EXISTS "${_source}/CMakeLists.txt")
    file(DOWNLOAD https://github.com/Jordan08/GAOL/archive/refs/heads/master.zip "${_work}/GAOL-master.zip"
         STATUS _download_status LOG _download_log)
    list(GET _download_status 0 _download_code)
    if(NOT _download_code EQUAL 0)
      message(FATAL_ERROR "GAOL: downloading https://github.com/Jordan08/GAOL/archive/refs/heads/master.zip "
                          "failed (${_download_status}):\n${_download_log}")
    endif()
    file(REMOVE_RECURSE "${_work}/GAOL-master" "${_source}")
    codac_gaol_step("extracting GAOL-master.zip" "${_work}/download.log"
      "${CMAKE_COMMAND}" -E chdir "${_work}" "${CMAKE_COMMAND}" -E tar xf GAOL-master.zip)
    file(RENAME "${_work}/GAOL-master" "${_source}")
  endif()

  # The configuration of GAOL, written as an initial cache (-C) rather than on
  # the command line, where the ";" of a list such as CMAKE_OSX_ARCHITECTURES
  # would split an argument in two. FORCE, so that a value changed since the
  # last configuration of Codac replaces the one in GAOL's cache.
  #
  # Every value below has to come out the same at each configuration of Codac
  # that changes nothing: GAOL is configured again as soon as one of them
  # differs, and everything in Codac that includes an interval may then be
  # recompiled. This is why the top-level CMakeLists.txt enables C in project()
  # rather than leaving it to a dependency, which changed CMAKE_C_COMPILER
  # between the first configuration and the second.
  set(_cache "")
  macro(codac_gaol_cache_entry name type value)
    string(APPEND _cache "set(${name} [==[${value}]==] CACHE ${type} \"\" FORCE)\n")
  endmacro()

  codac_gaol_cache_entry(CMAKE_BUILD_TYPE STRING Release)
  # Codac's Python modules link these archives into shared libraries.
  codac_gaol_cache_entry(CMAKE_POSITION_INDEPENDENT_CODE BOOL ON)
  # Where Codac is installed, which the gaol.pc GAOL installs with Codac names;
  # the installation in the build tree gives its own prefix.
  codac_gaol_cache_entry(CMAKE_INSTALL_PREFIX PATH "${CMAKE_INSTALL_PREFIX}")
  codac_gaol_cache_entry(CMAKE_INSTALL_INCLUDEDIR PATH "${CODAC_INSTALL_INCLUDEDIR_3RD}")
  codac_gaol_cache_entry(CMAKE_INSTALL_LIBDIR PATH "${CODAC_INSTALL_LIBDIR_3RD}")
  # The fork's continuous integration runs its tests
  codac_gaol_cache_entry(GAOL_BUILD_TESTS BOOL OFF)
  # The mathlib the fork downloads and builds, never one installed on this
  # machine: the installation of Codac counts on it
  codac_gaol_cache_entry(GAOL_FIND_MATHLIB BOOL OFF)
  # Without the fused multiply-add instructions of the processor, whose flags
  # would reach every target linking GAOL: a library for any processor of the
  # architecture
  codac_gaol_cache_entry(GAOL_FMA BOOL OFF)
  codac_gaol_cache_entry(CMAKE_CXX_FLAGS STRING "${CMAKE_CXX_FLAGS}")
  codac_gaol_cache_entry(CMAKE_C_FLAGS STRING "${CMAKE_C_FLAGS}")

  # The generator is handed over below, with its platform (-A) and toolset (-T),
  # but nothing else is: the compilers, the flags and the target have to be
  # given explicitly, or GAOL would be built for another machine than the one
  # Codac is built for. The Visual Studio and Xcode generators take their
  # compilers from the toolset and ignore these.
  if(NOT CMAKE_GENERATOR MATCHES "Visual Studio|Xcode")
    codac_gaol_cache_entry(CMAKE_CXX_COMPILER FILEPATH "${CMAKE_CXX_COMPILER}")
    codac_gaol_cache_entry(CMAKE_C_COMPILER FILEPATH "${CMAKE_C_COMPILER}")
    codac_gaol_cache_entry(CMAKE_MAKE_PROGRAM FILEPATH "${CMAKE_MAKE_PROGRAM}")
  endif()
  foreach(_var CMAKE_TOOLCHAIN_FILE CMAKE_MSVC_RUNTIME_LIBRARY CMAKE_GENERATOR_INSTANCE
               CMAKE_OSX_DEPLOYMENT_TARGET CMAKE_OSX_SYSROOT CMAKE_OSX_ARCHITECTURES)
    # Quoted: a list, as CMAKE_OSX_ARCHITECTURES can be, would give if() as many
    # arguments as elements
    if(NOT "${${_var}}" STREQUAL "")
      codac_gaol_cache_entry(${_var} STRING "${${_var}}")
    endif()
  endforeach()
  # Only when this build is itself a cross-compilation (as the macOS jobs of
  # .github/workflows are, by setting CMAKE_SYSTEM_NAME explicitly): passing
  # the host's own name would make GAOL's build believe it cross-compiles.
  if(CMAKE_CROSSCOMPILING)
    codac_gaol_cache_entry(CMAKE_SYSTEM_NAME STRING "${CMAKE_SYSTEM_NAME}")
    if(CMAKE_SYSTEM_PROCESSOR)
      codac_gaol_cache_entry(CMAKE_SYSTEM_PROCESSOR STRING "${CMAKE_SYSTEM_PROCESSOR}")
    endif()
  endif()
  file(WRITE "${_work}/initial-cache.cmake" "${_cache}")

  set(_generator -G "${CMAKE_GENERATOR}")
  if(CMAKE_GENERATOR_PLATFORM)
    list(APPEND _generator -A "${CMAKE_GENERATOR_PLATFORM}")
  endif()
  if(CMAKE_GENERATOR_TOOLSET)
    list(APPEND _generator -T "${CMAKE_GENERATOR_TOOLSET}")
  endif()

  codac_gaol_step("configuring" "${_work}/configure.log"
    "${CMAKE_COMMAND}" ${_generator} -C "${_work}/initial-cache.cmake" -S "${_source}" -B "${_binary}")
  # Release whatever the configuration of Codac: --config is what a
  # multi-configuration generator reads, and what the others ignore. In
  # parallel as CMAKE_BUILD_PARALLEL_LEVEL says, when it is set.
  codac_gaol_step("building" "${_work}/build.log"
    "${CMAKE_COMMAND}" --build "${_binary}" --config Release)
  # The installer of GAOL's build (cmake_install.cmake, which cmake --install
  # runs from CMake 3.15 on), with the prefix of the build tree
  codac_gaol_step("installing" "${_work}/install.log"
    "${CMAKE_COMMAND}" "-DCMAKE_INSTALL_PREFIX=${_install}" -DCMAKE_INSTALL_CONFIG_NAME=Release
    -P "${_binary}/cmake_install.cmake")

  # And with Codac, by the same installer, with the prefix Codac is installed
  # under, as the installation of Codac gives it (cmake --install --prefix,
  # CPack), and into DESTDIR when it is set, which it inherits.
  install(CODE "
    # GAOL and mathlib, installed by the installer of their CMake build (see
    # codac_gaol_build() in scripts/CMakeModules/codac_gaol.cmake)
    execute_process(COMMAND \"${CMAKE_COMMAND}\" \"-DCMAKE_INSTALL_PREFIX=\${CMAKE_INSTALL_PREFIX}\"
                            -DCMAKE_INSTALL_CONFIG_NAME=Release -P \"${_binary}/cmake_install.cmake\"
                    RESULT_VARIABLE _codac_gaol_install_result)
    if(NOT _codac_gaol_install_result EQUAL 0)
      message(FATAL_ERROR \"The installation of GAOL failed (\${_codac_gaol_install_result})\")
    endif()
  ")

  set(CODAC_GAOL_INSTALL_TREE "${_install}" PARENT_SCOPE)
endfunction()


################################################################################
# codac_gaol_find()
################################################################################
#
# Finds GAOL in the order given at the top of this file, or builds it, and
# defines Codac::gaol, the imported target through which the Codac libraries,
# and the targets linking them, get GAOL's include directories, flags and
# libraries. Sets in the caller's scope:
#
#   CODAC_GAOL_FROM          "package", "pkg-config" or "files"
#   CODAC_GAOL_BUILT_HERE    TRUE when codac_gaol_build() built it
#   CODAC_GAOL_INSTALL_TREE  where codac_gaol_build() installed it
#   GAOL_VERSION
#
# An INTERFACE library rather than GAOL's own target, whatever provided GAOL:
# CODAC_LIBRARIES names it, and codac-config.cmake defines it again
# (codac_gaol_config_snippet()), which it could not do under the name gaol::gaol
# without clashing with a find_package(gaol) of the consumer.
function(codac_gaol_find)

  include(CheckCXXCompilerFlag)
  set(_from "")
  set(_built_here FALSE)
  set(_version "")

  if(ENABLE_FIND_PACKAGE_GAOL)

    # 1. The CMake package of GAOL (gaol_DIR, or CMAKE_PREFIX_PATH). A gaol_DIR
    # left by an earlier configuration of this build, naming the GAOL
    # codac_gaol_build() installed in it, is not a GAOL of this machine: it is
    # forgotten, and that GAOL is built again, up to date, if none is found.
    string(FIND "${gaol_DIR}" "${CODAC_GAOL_WORK_DIR}/" _in_work_dir)
    if(_in_work_dir EQUAL 0)
      unset(gaol_DIR CACHE)
    endif()
    find_package(gaol CONFIG QUIET)
    if(gaol_FOUND)
      set(_from package)
      set(_version "${gaol_VERSION}")
      message(STATUS "Found GAOL ${gaol_VERSION}, CMake package in ${gaol_DIR}")
    endif()

    # 2. pkg-config (PKG_CONFIG_PATH, or CMAKE_PREFIX_PATH). Not with Visual C++,
    # whose libraries pkg-config does not name. A gaol.pc whose Cflags lack
    # -frounding-math, as the one the meson build of Frederic Goualard's GAOL
    # installs, does not describe the flags GAOL needs, and is not used.
    if(NOT _from AND NOT MSVC)
      find_package(PkgConfig QUIET)
      if(PKG_CONFIG_FOUND)
        pkg_check_modules(CODAC_GAOL_PC QUIET IMPORTED_TARGET gaol)
        if(CODAC_GAOL_PC_FOUND)
          check_cxx_compiler_flag(-frounding-math COMPILER_SUPPORTS_FROUNDING_MATH)
          if(COMPILER_SUPPORTS_FROUNDING_MATH AND NOT "-frounding-math" IN_LIST CODAC_GAOL_PC_CFLAGS_OTHER)
            message(STATUS "Found gaol.pc in ${CODAC_GAOL_PC_PREFIX}, whose Cflags lack -frounding-math: not used")
          else()
            set(_from pkg-config)
            set(_version "${CODAC_GAOL_PC_VERSION}")
            message(STATUS "Found GAOL ${CODAC_GAOL_PC_VERSION}, gaol.pc in ${CODAC_GAOL_PC_PREFIX}")
          endif()
        endif()
      endif()
    endif()

    # 3. The files (GAOL_DIR and MATHLIB_DIR, or CMAKE_PREFIX_PATH)
    if(NOT _from)
      find_package(GAOL MODULE QUIET)
      if(GAOL_FOUND)
        set(_from files)
        set(_version "${GAOL_VERSION}")
        message(STATUS "Found GAOL ${GAOL_VERSION} in ${GAOL_INCDIR}, without a CMake package or a gaol.pc: "
                       "compiled with the flags of interval arithmetic Codac determines")
      endif()
    endif()
  endif()

  # 4. Built, and found as in 1
  if(NOT _from)
    codac_gaol_build()
    unset(gaol_DIR CACHE)
    find_package(gaol CONFIG REQUIRED NO_DEFAULT_PATH
                 PATHS "${CODAC_GAOL_INSTALL_TREE}/${CODAC_INSTALL_LIBDIR_3RD}/cmake/gaol")
    set(_from package)
    set(_built_here TRUE)
    set(_version "${gaol_VERSION}")
    message(STATUS "GAOL ${gaol_VERSION} built and installed, CMake package in ${gaol_DIR}")
  endif()

  add_library(Codac::gaol INTERFACE IMPORTED)
  if(_from STREQUAL "package")
    set_target_properties(Codac::gaol PROPERTIES INTERFACE_LINK_LIBRARIES gaol::gaol)
  elseif(_from STREQUAL "pkg-config")
    set_target_properties(Codac::gaol PROPERTIES INTERFACE_LINK_LIBRARIES PkgConfig::CODAC_GAOL_PC)
  else()
    codac_gaol_interval_flags(_interval_flags)
    set_target_properties(Codac::gaol PROPERTIES
                          INTERFACE_INCLUDE_DIRECTORIES "${GAOL_INCLUDE_DIRS}"
                          INTERFACE_COMPILE_OPTIONS "${_interval_flags}"
                          INTERFACE_LINK_LIBRARIES "${GAOL_LIBRARIES}")
    # For Visual C++, GAOL declares its classes and functions
    # __declspec(dllimport), as for a DLL, unless __GAOL_PUBLIC__ is defined
    # (gaol/gaol_config.h). None of GAOL's builds -- its autotools and meson
    # builds, and the CMake build of the fork -- defines
    # _COMPILING__GAOL_PUBLIC__, with which GAOL would export a DLL: the GAOL
    # Visual C++ links is a static library, which every file including its
    # headers has to be told. gaol::gaol, of the CMake package, tells it the
    # same way.
    if(MSVC)
      set_target_properties(Codac::gaol PROPERTIES INTERFACE_COMPILE_DEFINITIONS "__GAOL_PUBLIC__=")
    endif()
  endif()

  set(CODAC_GAOL_FROM "${_from}" PARENT_SCOPE)
  set(CODAC_GAOL_BUILT_HERE ${_built_here} PARENT_SCOPE)
  set(CODAC_GAOL_INSTALL_TREE "${CODAC_GAOL_INSTALL_TREE}" PARENT_SCOPE)
  set(GAOL_VERSION "${_version}" PARENT_SCOPE)
endfunction()


################################################################################
# codac_gaol_usage(<include_dirs_var> <flags_var> <link_var>)
################################################################################
#
# Returns what Codac::gaol gives the targets linking it, walked out of it and of
# the targets it links: its include directories, its compilation flags (with
# its definitions, as -D or /D) and what goes on the link line (library files,
# and flags). This is for what cannot link Codac::gaol: CODAC_CXX_FLAGS, codac.pc,
# and codac-config.cmake when GAOL has no CMake package. Generator expressions
# are left out, but for $<LINK_ONLY:...>, which is unwrapped: GAOL's targets
# have no other.
function(codac_gaol_usage include_dirs_var flags_var link_var)

  set(include_dirs "")
  set(flags "")
  set(link "")
  set(queue Codac::gaol)
  set(seen "")

  while(queue)
    list(GET queue 0 item)
    list(REMOVE_AT queue 0)
    if(item MATCHES "^\\$<LINK_ONLY:(.*)>$")
      set(item "${CMAKE_MATCH_1}")
    endif()
    if(item IN_LIST seen OR item MATCHES "\\$<")
      continue()
    endif()
    list(APPEND seen "${item}")

    if(NOT TARGET "${item}")
      # A library file, a flag, or the name of a library of the system
      if(item MATCHES "^-" OR IS_ABSOLUTE "${item}")
        list(APPEND link "${item}")
      else()
        list(APPEND link "-l${item}")
      endif()
      continue()
    endif()

    get_target_property(_value "${item}" INTERFACE_INCLUDE_DIRECTORIES)
    if(_value)
      list(APPEND include_dirs ${_value})
    endif()
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

    # An imported archive names its file either outright or per configuration.
    # An INTERFACE library has no file at all, and before CMake 3.19 merely
    # asking one for IMPORTED_LOCATION is a fatal error rather than an empty
    # answer ("INTERFACE_LIBRARY targets may only have whitelisted
    # properties") -- which is what Debian Bullseye, on CMake 3.18.4, reported.
    # TYPE is whitelisted, so it can be asked first, as can the INTERFACE_
    # properties read above and below.
    get_target_property(_type "${item}" TYPE)
    if(NOT _type STREQUAL "INTERFACE_LIBRARY")
      get_target_property(_location "${item}" IMPORTED_LOCATION)
      if(NOT _location)
        get_target_property(_configurations "${item}" IMPORTED_CONFIGURATIONS)
        if(_configurations)
          list(GET _configurations 0 _configuration)
          get_target_property(_location "${item}" IMPORTED_LOCATION_${_configuration})
        endif()
      endif()
      if(_location)
        list(APPEND link "${_location}")
      endif()
    endif()

    get_target_property(_value "${item}" INTERFACE_LINK_LIBRARIES)
    if(_value)
      list(APPEND queue ${_value})
    endif()
    get_target_property(_value "${item}" INTERFACE_LINK_OPTIONS)
    if(_value)
      list(APPEND link ${_value})
    endif()
  endwhile()

  foreach(_list include_dirs flags link)
    list(FILTER ${_list} EXCLUDE REGEX "\\$<")
  endforeach()
  if(include_dirs)
    list(REMOVE_DUPLICATES include_dirs)
  endif()
  if(flags)
    list(REMOVE_DUPLICATES flags)
  endif()

  set(${include_dirs_var} ${include_dirs} PARENT_SCOPE)
  set(${flags_var} ${flags} PARENT_SCOPE)
  set(${link_var} ${link} PARENT_SCOPE)
endfunction()


################################################################################
# codac_gaol_pkg_config_path(<var>)
################################################################################
#
# When <var> is a path of the GAOL codac_gaol_build() installed in the build
# tree, turns it into the path GAOL is installed at with Codac, under the
# ${prefix} of codac.pc.
function(codac_gaol_pkg_config_path var)
  if(CODAC_GAOL_BUILT_HERE)
    string(FIND "${${var}}" "${CODAC_GAOL_INSTALL_TREE}/" _position)
    if(_position EQUAL 0)
      string(LENGTH "${CODAC_GAOL_INSTALL_TREE}/" _length)
      string(SUBSTRING "${${var}}" ${_length} -1 _rest)
      set(${var} "\${prefix}/${_rest}" PARENT_SCOPE)
    endif()
  endif()
endfunction()


################################################################################
# codac_gaol_config_snippet(<outvar>)
################################################################################
#
# Returns the lines of codac-config.cmake that define Codac::gaol for a consumer
# of the installed Codac, which CODAC_LIBRARIES names -- the
# counterpart of the ibex-config-gaol.cmake and ibex-config-ultim.cmake files
# that create_target_import_and_export() writes for IBEX.
#
#  - A GAOL with a CMake package is found again by find_package(gaol CONFIG):
#    the GAOL Codac built under the prefix Codac is installed under, by a path
#    relative to the configuration file, since the prefix may be moved; a GAOL
#    found on this machine where it was found, before the usual search.
#  - A GAOL found by pkg-config or by its files is named by the paths it was
#    found at, with the flags it was compiled with.
function(codac_gaol_config_snippet outvar)

  if(CODAC_GAOL_FROM STREQUAL "package")
    if(CODAC_GAOL_BUILT_HERE)
      file(RELATIVE_PATH _to_prefix "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_CMAKE}" "${CMAKE_INSTALL_PREFIX}")
      set(_find "get_filename_component(_codac_prefix \"\${CMAKE_CURRENT_LIST_DIR}/${_to_prefix}\" ABSOLUTE)
    find_package(gaol CONFIG REQUIRED NO_DEFAULT_PATH
                 PATHS \"\${_codac_prefix}/${CODAC_INSTALL_LIBDIR_3RD}/cmake/gaol\")")
    else()
      set(_find "find_package(gaol CONFIG REQUIRED HINTS \"${gaol_DIR}\")")
    endif()
    set(_properties "INTERFACE_LINK_LIBRARIES gaol::gaol")
  else()
    codac_gaol_usage(_include_dirs _flags _link)
    set(_find "")
    set(_properties "INTERFACE_INCLUDE_DIRECTORIES \"${_include_dirs}\"
                            INTERFACE_COMPILE_OPTIONS \"${_flags}\"
                            INTERFACE_LINK_LIBRARIES \"${_link}\"")
  endif()

  set(${outvar} "
    # GAOL, the interval arithmetic library Codac is built upon, with mathlib
    # (libultim) and the flags of interval arithmetic, as Codac::gaol, which
    # CODAC_LIBRARIES names.
    ${_find}
    if(NOT TARGET Codac::gaol)
      add_library(Codac::gaol INTERFACE IMPORTED)
      set_target_properties(Codac::gaol PROPERTIES
                            ${_properties})
    endif()
" PARENT_SCOPE)
endfunction()
