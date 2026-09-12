# ==================================================================
#  Codac - cmake configuration file
# ==================================================================

# Finding Catch2

option(ENABLE_FIND_PACKAGE_CATCH2 "ENABLE_FIND_PACKAGE_CATCH2" ON)
if(ENABLE_FIND_PACKAGE_CATCH2)
  find_package(Catch2 3.0.0 QUIET) # Use Catch2_DIR or CMAKE_PREFIX_PATH to specify a custom Catch2 installation path...
endif()
if(Catch2_FOUND)
  message(STATUS "Found Catch2 version ${Catch2_VERSION} in ${Catch2_DIR}")
else()
  message(STATUS "Attempt to install Catch2 automatically...")
  FetchContent_Declare(Catch2 URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.6.0.zip)
  #FetchContent_Declare(Catch2 URL ${CMAKE_CURRENT_SOURCE_DIR}/../3rd/Catch2-3.6.0.zip) # If needed to be self-contained...
  FetchContent_MakeAvailable(Catch2)
  # See codac_mark_target_system() in the top-level CMakeLists.txt: without
  # this, Catch2's headers inherit this project's warning level. Only the
  # FetchContent path needs it -- a find_package() target is IMPORTED, and
  # CMake already treats those as system.
  codac_mark_target_system(Catch2)
  codac_mark_target_system(Catch2::Catch2WithMain)
endif()
# Adds Catch2::Catch2WithMain


# Test sources
# ==================================================================

list(APPEND SRC_TESTS # listing files without extension

  # ----------------------------------------------------------------
  # 3rd
  # ----------------------------------------------------------------

  core/3rd/codac2_tests_eigen


  # ----------------------------------------------------------------
  # Actions
  # ----------------------------------------------------------------

  core/actions/codac2_tests_OctaSym


  # ----------------------------------------------------------------
  # Contractors
  # ----------------------------------------------------------------

  core/contractors/codac2_tests_CtcAction
  core/contractors/codac2_tests_CtcCartProd
  core/contractors/codac2_tests_CtcCtcBoundary
  core/contractors/codac2_tests_CtcDeriv
  core/contractors/codac2_tests_CtcEval
  core/contractors/codac2_tests_CtcFixpoint
  core/contractors/codac2_tests_CtcInter
  core/contractors/codac2_tests_CtcInverse
  core/contractors/codac2_tests_CtcInverseNotIn
  core/contractors/codac2_tests_CtcLazy
  core/contractors/codac2_tests_CtcLohner
  core/contractors/codac2_tests_CtcPolygon
  core/contractors/codac2_tests_CtcQInter
  core/contractors/codac2_tests_CtcSegment
  core/contractors/codac2_tests_CtcUnion
  core/contractors/codac2_tests_CtcVisible
  core/contractors/codac2_tests_linear_ctc

  ../doc/manual/manual/contractors/geometric/src
  ../doc/manual/manual/contractors/analytic/src
  ../doc/manual/manual/contractors/set/src
  ../doc/manual/manual/contractors/shape/src


  # ----------------------------------------------------------------
  # Domains
  # ----------------------------------------------------------------

  core/domains/codac2_tests_BoolInterval

  core/domains/ellipsoid/codac2_tests_Ellipsoid

  core/domains/interval/codac2_tests_Interval
  core/domains/interval/codac2_tests_Interval_operations
  core/domains/interval/codac2_tests_IntervalMatrix
  core/domains/interval/codac2_tests_IntervalVector

  ../doc/manual/manual/intervals/src

  core/domains/zonotope/codac2_tests_Parallelepiped_eval
  core/domains/zonotope/codac2_tests_Parallelepiped
  core/domains/zonotope/codac2_tests_Zonotope

  core/domains/tube/codac2_tests_TDomain
  core/domains/tube/codac2_tests_Slice
  core/domains/tube/codac2_tests_Slice_polygon
  core/domains/tube/codac2_tests_SlicedTube
  core/domains/tube/codac2_tests_SlicedTube_integral

  ../doc/manual/manual/tubes/src


  # ----------------------------------------------------------------
  # Functions
  # ----------------------------------------------------------------

  core/functions/analytic/codac2_tests_AnalyticFunction

  ../doc/manual/manual/functions/analytic/src
  ../doc/manual/manual/functions/parallelepiped/src


  # ----------------------------------------------------------------
  # Geometry
  # ----------------------------------------------------------------

  core/geometry/codac2_tests_ConvexPolygon
  core/geometry/codac2_tests_geometry
  core/geometry/codac2_tests_Polygon
  core/geometry/codac2_tests_Segment

  ../doc/manual/manual/geometry/src


  # ----------------------------------------------------------------
  # Matrices
  # ----------------------------------------------------------------

  core/matrices/codac2_tests_arithmetic_add
  core/matrices/codac2_tests_arithmetic_div
  core/matrices/codac2_tests_arithmetic_mul
  core/matrices/codac2_tests_arithmetic_sub
  core/matrices/codac2_tests_cart_prod
  core/matrices/codac2_tests_Matrix
  core/matrices/codac2_tests_Vector
  core/matrices/codac2_tests_hull
  core/matrices/codac2_tests_inversion
  core/matrices/codac2_tests_IntFullPivLU
  core/matrices/codac2_tests_GaussJordan

  ../doc/manual/manual/linear/src


  # ----------------------------------------------------------------
  # Operators
  # ----------------------------------------------------------------

  core/operators/codac2_tests_operators


  # ----------------------------------------------------------------
  # Peibos
  # ----------------------------------------------------------------

  core/peibos/codac2_tests_peibos

  ../doc/manual/manual/functions/peibos/src


  # ----------------------------------------------------------------
  # Separators
  # ----------------------------------------------------------------

  core/separators/codac2_tests_SepCartProd
  core/separators/codac2_tests_SepCtcBoundary
  core/separators/codac2_tests_SepInter
  core/separators/codac2_tests_SepInverse
  core/separators/codac2_tests_SepPolygon
  core/separators/codac2_tests_SepProj
  core/separators/codac2_tests_SepQInter
  core/separators/codac2_tests_SepTransform
  core/separators/codac2_tests_SepUnion
  core/separators/codac2_tests_SepVisible


  # ----------------------------------------------------------------
  # Tools
  # ----------------------------------------------------------------

  core/tools/codac2_tests_Approx
  core/tools/codac2_tests_serialization
  core/tools/codac2_tests_transformations
  core/tools/codac2_tests_trunc
  core/tools/ibex/codac2_tests_ibex

  ../doc/manual/manual/tools/src


  # ----------------------------------------------------------------
  # Trajectory
  # ----------------------------------------------------------------

  core/trajectory/codac2_tests_AnalyticTraj
  core/trajectory/codac2_tests_SampledTraj


  # ----------------------------------------------------------------
  # Graphics
  # ----------------------------------------------------------------

  graphics/styles/codac2_tests_Color
)


# Python files required by tests
# ==================================================================

file(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/core/domains/tube/codac2_tests_predefined_tubes.py
  # Add here other Python files that are not tests but need to be exported to Python package
  # ...
  
  DESTINATION
  ${CMAKE_BINARY_DIR}/python/python_package/codac/tests
)


# Libraries
# ==================================================================

set(CODAC_LIBRARIES ${PROJECT_NAME}-core ${PROJECT_NAME}-graphics)

# Sympy
# ==================================================================

option(BUILD_SYMPY_EMBED_TESTS "Build C++ tests that require embedded Python for Sympy" ON)
if(WITH_PYTHON AND DEFINED PYBIND11_FINDPYTHON AND NOT PYBIND11_FINDPYTHON)
  message(STATUS "Disabling Sympy C++ embed tests because PYBIND11_FINDPYTHON=OFF")
  set(BUILD_SYMPY_EMBED_TESTS OFF)
endif()

# BUILD_SYMPY_EMBED_TESTS is an option(), hence a cache entry: the set() above
# only overrides it for this directory, and examples/ would otherwise read the
# cached ON again and build the sympy example where its own test is disabled.
set(BUILD_SYMPY_EMBED_TESTS "${BUILD_SYMPY_EMBED_TESTS}" PARENT_SCOPE)

# Sympy test
if (WITH_PYTHON AND BUILD_SYMPY_EMBED_TESTS)
  list(APPEND SRC_TESTS
    extensions/sympy/codac2_tests_sympy
    ../doc/manual/manual/extensions/sympy/src
  )
endif()


# CAPD
# ==================================================================
if (WITH_CAPD)
  list(APPEND SRC_TESTS
    extensions/capd/codac2_tests_capd
    extensions/capd/codac2_tests_peibos_capd
    ../doc/manual/manual/extensions/capd/src
  )
  set(CODAC_LIBRARIES ${CODAC_LIBRARIES} ${PROJECT_NAME}-capd capd::capd)
endif()

# IBEX test
#if (WITH_IBEX)
  #find_package(IBEX REQUIRED)
  #ibex_init_common() # IBEX should have installed this function
  #message(STATUS "Found IBEX version ${IBEX_VERSION}")
#endif()



# Common Codac include directories
# ==================================================================
#
# IMPORTANT:
# Use the source tree as the canonical header tree.
#
# The headers are now used directly from the source tree.

# CODAC_SOURCE_INCLUDE_DIRS is filled in by the src/*/CMakeLists.txt of every
# module (core, graphics, unsupported, and capd/sympy when enabled), which are
# all configured before this directory. Using it rather than a second copy of
# the same list keeps the tests compiling against exactly the headers the
# libraries were built from; the copy that used to live here covered core only
# and had already drifted from it.
set(CODAC_CORE_SOURCE_INCLUDE_DIRS ${CODAC_SOURCE_INCLUDE_DIRS})


# ${CMAKE_BINARY_DIR}/src holds the generated "codac" umbrella header;
# CODAC_GENERATED_INCLUDE_DIRS, filled in by the src/*/CMakeLists.txt of every
# module, holds the per-module ones (codac-core.h and the rest), each of which
# is written into its own module's binary directory rather than into this one.
set(
  CODAC_HEADERS_DIR
  ${CMAKE_BINARY_DIR}/src
  ${CODAC_GENERATED_INCLUDE_DIRS}
)



# ------------------------------------------------------------------
# Sanitizer runtime preload for Python tests (Debug builds only)
# ------------------------------------------------------------------
#
# In Debug mode, _core.so (the pybind11 module) is compiled with
# -fsanitize=address,undefined and depends on a *shared* sanitizer
# runtime rather than one statically embedded in the .so:
#  - Clang needs an explicit -shared-libsan flag to get that (see
#    python/src/core/CMakeLists.txt) -- its default is to embed the
#    runtime statically into shared libraries.
#  - GCC needs no flag at all: unless -static-libasan/-static-libubsan
#    is passed (which codac never does), GCC already links libasan.so /
#    libubsan.so dynamically by default. Passing Clang's -shared-libsan
#    to GCC is not an option either way -- GCC's driver rejects it
#    outright as an unrecognized flag.
# Either way, since python3 itself has no sanitizer runtime, that shared
# runtime must be preloaded before Python imports codac, otherwise symbols
# like __ubsan_vptr_type_cache/__asan_* stay unresolved -- or, on GCC, the
# module load aborts immediately with "ASan runtime does not come first in
# initial library list" (reproduced locally: a plain dlopen(), from an
# unsanitized host, of a GCC ASan-built .so fails exactly this way without
# LD_PRELOAD, and succeeds once libasan.so is preloaded).

set(PYTHON_TEST_ENV_ARGS "PYTHONPATH=${PYTHON_PACKAGE_DIR}")

# Not gated behind a plain CMAKE_BUILD_TYPE STREQUAL "Debug" check (see the
# matching note in the root CMakeLists.txt): that variable is meaningless
# for multi-configuration generators, which instead pick the actual
# configuration later, at build time. CMAKE_CONFIGURATION_TYPES is set
# precisely on those generators, so probing here also runs whenever it is
# non-empty -- the individual LD_PRELOAD/ASAN_OPTIONS/UBSAN_OPTIONS entries
# appended below are themselves wrapped in a $<CONFIG:Debug> generator
# expression (see how PYTHON_TEST_ENV_ARGS is consumed via the ENVIRONMENT
# test property further down), so they still only take effect for an
# actual Debug-configuration test run.
if(WITH_PYTHON AND (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_CONFIGURATION_TYPES)
   AND CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU" AND CMAKE_SYSTEM_NAME STREQUAL "Linux")

  # This whole block is Linux-specific by construction, not just by
  # "is the compiler Clang or GCC": LD_PRELOAD is a Linux/glibc loader
  # feature with no equivalent environment variable on Windows, and the
  # shared runtime file names it looks for below only exist in that form
  # on Linux -- macOS uses DYLD_INSERT_LIBRARIES and ships its runtimes
  # under different names (e.g. Clang's combined, no-arch-suffix
  # libclang_rt.asan_osx_dynamic.dylib), and Windows uses neither an
  # env-var preload mechanism nor this naming (clang-cl names them
  # clang_rt.asan_dynamic-<arch>.dll; MinGW-w64 GCC ships asan/ubsan as
  # DLLs with their own layout). Real MSVC (cl.exe) is already excluded
  # by the CMAKE_CXX_COMPILER_ID check above, but clang-cl (Windows) and
  # AppleClang (macOS) both also match "Clang" (and MinGW GCC still
  # matches "GNU"), so without this explicit Linux check they would
  # silently fall through to the "no shared runtime found" warning below
  # instead of making clear that the feature just does not apply there.

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

    # Note: `clang --print-runtime-dir` assumes a per-target-runtime-dir
    # layout (lib/<target-triple>/) which some distro packages (e.g. Ubuntu/
    # Debian's llvm.org apt packages) do not use, instead keeping the legacy
    # lib/linux/ layout: the reported directory would not exist there, even
    # though the runtime libraries are actually installed. `-print-file-name`
    # asks the driver to resolve each file the way it would at link time,
    # which works regardless of which layout the distro packaging uses.
    #
    # The runtime file names themselves are not portable either: Clang embeds
    # its own architecture name in them (e.g. libclang_rt.asan-aarch64.so on
    # an AArch64 Raspberry Pi OS, libclang_rt.asan-armhf.so on a 32-bit
    # ARMv7/ARMv8 hard-float userland). Rather than maintaining our own
    # CMAKE_SYSTEM_PROCESSOR -> Clang-runtime-name table (which would drift
    # out of sync as LLVM adds targets), ask the compiler itself:
    # `-dumpmachine` prints its default target triple (understood by both
    # Clang and GCC, and already normalized -- e.g. a 32-bit x86 compiler
    # reports "i386", never "i686"), and its first field is, for essentially
    # every architecture, precisely the suffix Clang's runtime uses.
    #
    # The one documented exception is 32-bit ARM, where the suffix instead
    # follows the float ABI recorded in the triple's environment field
    # (.../gnueabihf -> "armhf" hard-float, vs. plain "arm" otherwise) rather
    # than the architecture field (which is typically "arm"/"armv7"/...) --
    # this is an LLVM naming convention, not something any compiler flag
    # reports directly, so it is the one case still special-cased below.
    execute_process(
      COMMAND ${CMAKE_CXX_COMPILER} -dumpmachine
      OUTPUT_VARIABLE _clang_target_triple
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(_sanitizer_rt_arch "")
    if(_clang_target_triple)
      string(REPLACE "-" ";" _clang_triple_fields "${_clang_target_triple}")
      list(GET _clang_triple_fields 0 _sanitizer_rt_arch)
      if(_sanitizer_rt_arch MATCHES "^armv?[0-9]*l?$")
        if(_clang_target_triple MATCHES "eabihf$")
          set(_sanitizer_rt_arch "armhf")
        else()
          set(_sanitizer_rt_arch "arm")
        endif()
      endif()
    endif()

    # The ASan runtime only, deliberately: built with -fsanitize=address,
    # undefined, Clang puts the UndefinedBehaviorSanitizer inside the ASan
    # runtime, and ships libclang_rt.ubsan_standalone-* for the builds that
    # use UBSan on its own. Preloading both therefore loads two copies of the
    # same initialisation and the same interceptors into one process. Clang 18
    # tolerated it; Clang 21 wedges the interpreter so thoroughly that even
    # SIGKILL does not remove it, which is what made every Python test of the
    # Clang jobs of .github/workflows/unixdebug.yml run into its timeout while
    # the GCC jobs of the same matrix passed them in about two seconds --
    # GCC's libasan and libubsan being genuinely separate libraries, designed
    # to be loaded together. Preloading ASan alone still gives both sets of
    # checks, since one runtime implements them both.
    set(_sanitizer_rt_candidates "")
    if(_sanitizer_rt_arch)
      set(_sanitizer_rt_candidates
        libclang_rt.asan-${_sanitizer_rt_arch}.so
      )
    endif()

  else() # GNU (GCC)

    # Unlike Clang, GCC's driver is built for one target only, so its
    # runtime file names carry no architecture suffix -- `-print-file-name`
    # already resolves to the right library for whatever target this GCC
    # was built for, on every architecture. GCC also splits ASan and UBSan
    # into two separately-named libraries (libasan.so / libubsan.so),
    # rather than Clang's libclang_rt.asan-*/libclang_rt.ubsan_standalone-*
    # naming.
    set(_sanitizer_rt_candidates libasan.so libubsan.so)

  endif()

  set(SANITIZER_RT_LIBS "")
  foreach(_rt_lib_name ${_sanitizer_rt_candidates})
    execute_process(
      COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=${_rt_lib_name}
      OUTPUT_VARIABLE _rt_lib_path
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(EXISTS "${_rt_lib_path}")
      list(APPEND SANITIZER_RT_LIBS "${_rt_lib_path}")
    endif()
  endforeach()

  if(SANITIZER_RT_LIBS)
    list(JOIN SANITIZER_RT_LIBS ":" SANITIZER_RT_LIBS_JOINED)
    list(APPEND PYTHON_TEST_ENV_ARGS "$<$<CONFIG:Debug>:LD_PRELOAD=${SANITIZER_RT_LIBS_JOINED}>")

    # LeakSanitizer (part of ASan) flags many long-lived, process-lifetime
    # allocations made by the Python interpreter and by pybind11 module
    # init (interned strings, type objects, strdup'd docstrings...) as
    # "leaks", since it can't trace Python's own object graph. This is a
    # well-known false-positive source for embedded Python interpreters,
    # not something codac's code controls. UBSan and ASan's other checks
    # (use-after-free, buffer overflow, UB...) remain fully active.
    list(APPEND PYTHON_TEST_ENV_ARGS "$<$<CONFIG:Debug>:ASAN_OPTIONS=detect_leaks=0>")
    list(APPEND PYTHON_TEST_ENV_ARGS "$<$<CONFIG:Debug>:UBSAN_OPTIONS=print_stacktrace=1>")
    message(STATUS "Python tests: LD_PRELOAD=${SANITIZER_RT_LIBS_JOINED}")
  else()
    message(WARNING
      "Debug build with WITH_PYTHON: no shared ASan/UBSan runtime found "
      "for ${CMAKE_CXX_COMPILER}. With Clang, make sure the _core target "
      "is linked with -shared-libsan; with GCC, make sure it is not "
      "linked with -static-libasan/-static-libubsan. Otherwise Python "
      "tests will fail with 'undefined symbol' errors, or abort with "
      "'ASan runtime does not come first in initial library list'.")
  endif()

endif()

# ------------------------------------------------------------------
# Sanitizer runtime discoverability for MSVC (Debug builds only)
# ------------------------------------------------------------------
#
# CODAC_MSVC_ASAN_RUNTIME_DLL is set by the root CMakeLists.txt (empty if
# no matching runtime could be found, in which case /fsanitize=address was
# never even added -- nothing to do here either). Windows has no
# LD_PRELOAD-style env var and no "must load first" requirement: the
# loader just needs to be able to find the runtime DLL via PATH or next
# to the binary that imports it, when that binary is loaded. Python's
# _core.pyd gets its own copy placed next to it (see
# python/src/core/CMakeLists.txt); the plain C++ test executables and the
# python.exe launched below have no such fixed "next to" location, so
# their directory search is extended via PATH instead, the direct
# Windows analogue of prepending to LD_PRELOAD above.
set(CODAC_MSVC_ASAN_PATH_ENV "")
if(MSVC AND CODAC_MSVC_ASAN_RUNTIME_DLL)
  get_filename_component(_msvc_asan_dll_dir "${CODAC_MSVC_ASAN_RUNTIME_DLL}" DIRECTORY)
  # Both ctest's ENVIRONMENT test property and the COMMAND argument list
  # passed to `cmake -E env` below are themselves ';'-separated CMake
  # lists, so the ';' that Windows uses to separate PATH entries has to be
  # escaped as '\;' here -- otherwise CMake would silently split this one
  # "PATH=......" value into several unrelated list entries.
  #
  # NOTE: $ENV{PATH} is captured at CMake *configure* time, i.e. whatever
  # PATH was active when `cmake` was run to configure the project. If
  # ctest is later invoked from a shell with a materially different PATH,
  # this won't reflect that -- a limitation inherent to baking an
  # environment value into generated build files, not specific to this
  # check.
  string(REPLACE ";" "\\;" _msvc_asan_path_escaped "${_msvc_asan_dll_dir};$ENV{PATH}")
  set(CODAC_MSVC_ASAN_PATH_ENV "PATH=${_msvc_asan_path_escaped}")
  # Appended for every configuration rather than wrapped in a $<CONFIG:Debug>
  # generator expression. `cmake -E env` reads its arguments as VAR=value
  # assignments up to the first one that is not, which it then takes for the
  # command to run: an element that a generator expression reduced to nothing
  # leaves an empty argument in the middle of the list, and the command becomes
  # the empty string. CODAC_MSVC_ASAN_RUNTIME_DLL is computed whatever the
  # configuration (see the root CMakeLists.txt), so on MSVC the element was
  # always there and always empty outside Debug -- every Python test of
  # .github/workflows/vcmatrix.yml died on "no such file or directory" while
  # the C++ ones passed. Prepending the sanitizer directory to PATH costs a
  # Release run nothing, which is what the C++ tests below already rely on.
  list(APPEND PYTHON_TEST_ENV_ARGS "${CODAC_MSVC_ASAN_PATH_ENV}")
endif()


# Environment of the C++ tests and examples
# ==================================================================
#
# The same environment is needed by every C++ binary this project runs under
# ctest, the examples of examples/CMakeLists.txt included, so it is built once
# here and exported to the parent scope rather than recomputed per test.
#
# UBSAN_OPTIONS only means something to the GCC/Clang single-configuration
# builds, where CMAKE_BUILD_TYPE holds the answer. The MSVC AddressSanitizer
# runtime is a DLL that has to be discoverable at load time by every binary
# compiled with /fsanitize=address: without it on PATH a Debug test cannot even
# start, Windows failing the process creation with 0xC0000135
# (STATUS_DLL_NOT_FOUND) before a single assertion runs. It is added whatever
# the configuration, because the multi-configuration generators MSVC is driven
# through only choose theirs at build time, and prepending a toolchain
# directory to PATH costs a Release run nothing.

set(CODAC_CPP_TEST_ENV "")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  list(APPEND CODAC_CPP_TEST_ENV "UBSAN_OPTIONS=print_stacktrace=1")
endif()

if(CODAC_MSVC_ASAN_PATH_ENV)
  list(APPEND CODAC_CPP_TEST_ENV "${CODAC_MSVC_ASAN_PATH_ENV}")
endif()

# examples/ is added by the top-level CMakeLists.txt right after this directory
# and runs the same kind of binaries, so both environments are handed over to it.
set(CODAC_CPP_TEST_ENV "${CODAC_CPP_TEST_ENV}" PARENT_SCOPE)
set(PYTHON_TEST_ENV_ARGS "${PYTHON_TEST_ENV_ARGS}" PARENT_SCOPE)

# Build every test
# ==================================================================

foreach(SRC_TEST ${SRC_TESTS})
  string(REPLACE "/" "_" TEST_NAME ${SRC_TEST})
  string(REPLACE "codac2_tests_" "" TEST_NAME ${TEST_NAME})
  set(TEST_NAME codac2_tests_${TEST_NAME})


  # ---------------------------------------------------------------
  # C++ test
  # ---------------------------------------------------------------

  add_executable(
    ${TEST_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_TEST}.cpp
  )


  # Use the same canonical source headers as the libraries, and reach them by
  # the same path: the source directories come first so that a header is
  # opened at src/... Both compile identically, but the path used is the path gcov
  # records, and different paths would split every header into two unrelated
  # entries in the coverage report (see codac_publish_include_dirs() in the
  # top-level CMakeLists.txt).
  #
  # CODAC_HEADERS_DIR is kept, after them, because it holds the generated
  # umbrella headers: codac, codac-core.h, codac-graphics.h and the rest.

  target_include_directories(
    ${TEST_NAME}
    SYSTEM PUBLIC

      ${CODAC_CORE_SOURCE_INCLUDE_DIRS}

      ${CODAC_HEADERS_DIR}
  )


  target_link_libraries(
    ${TEST_NAME}
    PUBLIC

      Ibex::ibex
      ${CODAC_LIBRARIES}

    PRIVATE

      Catch2::Catch2WithMain
  )

  # Propagate the architecture flags (FMA, etc.) to the tests
  if(CODAC_ARCH_CXX_FLAGS)
    target_compile_options(${TEST_NAME} PRIVATE ${CODAC_ARCH_CXX_FLAGS})
  endif()


  # Sympy embedded tests
  if(
    WITH_PYTHON
    AND SRC_TEST MATCHES "extensions/sympy/"
  )

    target_link_libraries(
      ${TEST_NAME}
      PRIVATE

        ${PROJECT_NAME}-sympy
        pybind11::embed
    )

  endif()


  # C++20
  set_target_properties(
    ${TEST_NAME}
    PROPERTIES

      CXX_STANDARD 20
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
  )


  add_dependencies(
    check
    ${TEST_NAME}
  )

  add_test(
    NAME ${TEST_NAME}_cpp
    COMMAND ${TEST_NAME}
  )

  if(CODAC_CPP_TEST_ENV)
    set_tests_properties(${TEST_NAME}_cpp PROPERTIES
      ENVIRONMENT "${CODAC_CPP_TEST_ENV}"
    )
  endif()

  # ---------------------------------------------------------------
  # Python test
  # ---------------------------------------------------------------

  if(WITH_PYTHON AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${SRC_TEST}.py")
    add_test(
      NAME ${TEST_NAME}_py
      COMMAND
        ${CMAKE_COMMAND} -E env
        ${PYTHON_TEST_ENV_ARGS}
        ${PYTHON_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_TEST}.py
    )
  endif()

endforeach()
