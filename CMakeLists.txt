# ==================================================================
#  Codac - cmake configuration file
# ==================================================================

  cmake_minimum_required(VERSION 3.14...3.25)

  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts/CMakeModules/)
  include(version_from_git)
  version_from_git() # Obtains the version number from Git tags

  # To use a specific compiler:
  #set(CMAKE_C_COMPILER "gcc-7")
  #set(CMAKE_CXX_COMPILER "/usr/bin/g++-7")

  # CMP0091 (CMake >=3.15) makes the CMAKE_MSVC_RUNTIME_LIBRARY variable set
  # below actually control MSVC's runtime library selection, instead of
  # CMake's older behavior of baking a hardcoded /MD.../MDd into
  # CMAKE_<LANG>_FLAGS_<CONFIG>. It must be set to NEW before project() --
  # afterwards is too late -- for that variable to have any effect at all.
  if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
  endif()

  project(codac VERSION ${VERSION} LANGUAGES CXX)

  if(NOT VERSION_ID)
    set(PROJECT_VERSION_FULL ${PROJECT_VERSION})
  else()
    set(PROJECT_VERSION_FULL "${PROJECT_VERSION}${VERSION_ID}")
  endif()
  message(STATUS "Full project version is ${PROJECT_VERSION_FULL}")

  set(PROJECT_DESCRIPTION
      "Codac is a library providing tools for constraint programming over reals and trajectories.")
  set(PROJECT_LONG_DESCRIPTION
      "${PROJECT_DESCRIPTION}")
  set(PROJECT_HOMEPAGE_URL "http://codac.io")
  message(STATUS "Configuring build for ${PROJECT_NAME} ${PROJECT_VERSION}")


################################################################################
# Options for directories
################################################################################
  
  # Install directories
  set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "C++ header files (include)")
  set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "object code libraries (lib)")
  set(CMAKE_INSTALL_BINDIR "bin" CACHE PATH "user executables (bin)")
  set(CMAKE_INSTALL_PKGCONFIG "share/pkgconfig" CACHE PATH "pkg files (share/pkgconfig)")
  set(CMAKE_INSTALL_CMAKE "share/codac/cmake" CACHE PATH "cmake files (share/codac/cmake)")


################################################################################
# Compilation configuration
################################################################################
  
  include(FetchContent)

  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
  endif()
  
  if(FAST_RELEASE)
    add_compile_definitions(FAST_RELEASE)
  endif()
  
  if(NOT WIN32)
    string(ASCII 27 Esc)
    set(COLOR_RED "${Esc}[31m")
    set(COLOR_BLUE "${Esc}[36m")
    set(COLOR_RESET "${Esc}[m")
  else()
    set(COLOR_RED "")
    set(COLOR_BLUE "")
    set(COLOR_RESET "")
  endif()

  message(STATUS "${COLOR_BLUE}Configuring ${PROJECT_NAME} in ${CMAKE_BUILD_TYPE} mode.${COLOR_RESET}")

  if(FAST_RELEASE)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      message(STATUS "${COLOR_RED}Option FAST_RELEASE cannot be used in Debug mode.${COLOR_RESET}")
    else()
      message(STATUS "${COLOR_BLUE}Option FAST_RELEASE enabled.${COLOR_RESET}")
    endif()
  else()
    message(STATUS "Note: in Release build mode, the option FAST_RELEASE=ON would allow faster computations if enabled.")
  endif()

  if(MSVC)
    add_compile_options(/W4)
  else()
    add_compile_options(-Wall -Wextra -Wpedantic)
  endif()

  if(MSVC)
    # IBEX (and the gaol interval library it bundles) is only distributed
    # to this project's CI as prebuilt Release binaries (see
    # .github/workflows/vcmatrix.yml and .github/workflows/windebugmatrix.yml):
    # gaol.lib is compiled with the Release CRT (/MD). MSVC's linker refuses
    # to mix object files built against different CRT variants in the same
    # binary -- linking that Release-CRT gaol.lib together with codac's own
    # object files, which without this override would default to the Debug
    # CRT (/MDd) in a Debug build, fails with error LNK2038 ("mismatch
    # detected for 'RuntimeLibrary'/'_ITERATOR_DEBUG_LEVEL'") followed by
    # fatal error LNK1319.
    #
    # Forcing every configuration onto the Release CRT keeps codac's own
    # Debug build link-compatible with that prebuilt gaol.lib without
    # having to rebuild IBEX/gaol from source in Debug ourselves. Release
    # itself already defaulted to /MD, so in practice this only changes
    # what the Debug configuration uses. Debug builds still get no
    # optimization (/Od), full debug symbols (/Zi) and AddressSanitizer
    # (see CODAC_MSVC_ASAN_RUNTIME_DLL below) -- what is traded away is
    # only the *separate* debug-CRT extras that /MDd would otherwise add on
    # top of that (its own heap debugging and iterator-debug-level checks).
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
  endif()

  if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_CONFIGURATION_TYPES)
    # CMAKE_BUILD_TYPE only has meaning for single-configuration generators
    # (Makefiles, Ninja): multi-configuration generators (Visual Studio,
    # Xcode, Ninja Multi-Config -- notably what this project's own CI
    # drives MSVC through, via `-G "Visual Studio 17/18"` in
    # .github/workflows/*matrix.yml) instead select the actual
    # configuration later, at build time (cmake --build . --config
    # Debug), not at configure time; CMAKE_BUILD_TYPE stays whatever it
    # was defaulted to above ("Release") regardless of what configuration
    # is later built. So this outer if() only decides whether it is worth
    # *probing* for Debug-only capabilities at all -- true either when a
    # single-config build was explicitly configured as Debug, or when a
    # multi-config generator defers that choice (CMAKE_CONFIGURATION_TYPES
    # is then non-empty) and so might still end up building a Debug
    # configuration later. The actual compile/link options below are
    # separately wrapped in a $<CONFIG:Debug> generator expression, which
    # -- unlike CMAKE_BUILD_TYPE -- CMake evaluates correctly against
    # whichever configuration is ultimately built, for both single- and
    # multi-config generators alike.
    if(MSVC)
      # cl.exe silently ignores GCC/Clang-style "-f..."/"-g"/"-O0" flags
      # with a D9002 warning rather than failing the build, so without this
      # branch a Debug build under MSVC would quietly end up *unsanitized*
      # instead of erroring -- a behavior gap invisible until someone
      # actually diffs test coverage between platforms.
      #
      # MSVC's AddressSanitizer (/fsanitize=address) has shipped since
      # Visual Studio 16.9; there is no UndefinedBehaviorSanitizer on this
      # toolchain, so Debug builds here get ASan coverage only, not the
      # ASan+UBSan combination GCC/Clang get below. Its runtime is a DLL
      # shipped next to cl.exe, rather than a LD_PRELOAD-style shared
      # object that the dynamic loader pulls in transparently: *every*
      # executable and DLL built with /fsanitize=address needs that
      # runtime DLL to be discoverable at load time (next to the binary,
      # or on PATH) -- per Microsoft's own documentation, this holds even
      # for /MT static-CRT builds, unlike every other CRT component.
      # See tests/CMakeLists.txt and python/src/core/CMakeLists.txt for
      # how CODAC_MSVC_ASAN_RUNTIME_DLL, set below, is used to make that
      # DLL discoverable for the test suite and for the Python bindings.
      #
      # Source: https://learn.microsoft.com/en-us/cpp/sanitizers/asan-runtime
      #
      # Rather than enable /fsanitize=address unconditionally, this whole
      # feature is opt-in on whether the matching runtime DLL can actually
      # be located next to this cl.exe: if it can't (toolset too old,
      # target architecture unsupported by MSVC ASan, or a packaging
      # layout this check doesn't recognize), Debug builds simply stay
      # unsanitized rather than either failing configuration or compiling
      # binaries that are guaranteed to fail to load at run time with a
      # missing-DLL error. This mirrors how the FMA detection below is
      # capability-gated via check_cxx_compiler_flag() rather than an
      # assumed target/toolset list.
      #
      # MSVC ASan runtime binaries are named using Clang's architecture
      # conventions (i386/x86_64/aarch64), not MSVC's own (X86/x64/ARM64) --
      # this is documented by Microsoft itself, not a codac convention.
      # CMAKE_CXX_COMPILER_ARCHITECTURE_ID (a builtin CMake variable, MSVC
      # only) reports the *target* architecture regardless of host, which
      # is what a flag like /fsanitize=address needs to match; MSVC
      # AddressSanitizer only supports x86, x64 and ARM64 (ARM64 as of
      # VS 2022 17.9, as a preview) -- any other value (32-bit ARM, IA64,
      # ARM64EC...) is simply left unmapped below, so the search for a
      # runtime DLL naturally comes up empty and the feature is skipped,
      # exactly as intended.
      set(_msvc_asan_dll_arch "")
      if(CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL "x64")
        set(_msvc_asan_dll_arch "x86_64")
      elseif(CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL "X86")
        set(_msvc_asan_dll_arch "i386")
      elseif(CMAKE_CXX_COMPILER_ARCHITECTURE_ID STREQUAL "ARM64")
        set(_msvc_asan_dll_arch "aarch64")
      endif()

      set(CODAC_MSVC_ASAN_RUNTIME_DLL "")
      if(_msvc_asan_dll_arch)
        get_filename_component(_msvc_bin_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
        # Visual Studio 2022 17.7 Preview 3 unified every CRT configuration
        # (/MT, /MTd, /MD, /MDd) onto a single clang_rt.asan_dynamic-{arch}.dll.
        # Older toolsets instead shipped a separate debug-CRT DLL
        # (clang_rt.asan_dbg_dynamic-{arch}.dll) for /MDd specifically.
        # CMAKE_MSVC_RUNTIME_LIBRARY is forced to the Release CRT (/MD) for
        # every configuration above (Debug included), so in practice only
        # the plain, non-debug DLL is ever needed here now; the /MDd-only
        # name is kept as a fallback purely in case that override above is
        # ever relaxed back to the CMake default. Rather than pin an exact
        # MSVC_VERSION cutoff for the 17.7-era switch -- a compiler-rt
        # packaging detail, not something reported by any compiler flag --
        # just look for whichever of the two names actually exists next to
        # this cl.exe, newest first.
        foreach(_dll_name
          "clang_rt.asan_dynamic-${_msvc_asan_dll_arch}.dll"
          "clang_rt.asan_dbg_dynamic-${_msvc_asan_dll_arch}.dll"
        )
          if(EXISTS "${_msvc_bin_dir}/${_dll_name}")
            set(CODAC_MSVC_ASAN_RUNTIME_DLL "${_msvc_bin_dir}/${_dll_name}")
            break()
          endif()
        endforeach()
      endif()

      if(CODAC_MSVC_ASAN_RUNTIME_DLL)
        add_compile_options($<$<CONFIG:Debug>:/fsanitize=address>)
        # Incremental linking cannot be combined with ASan: the linker drops
        # /INCREMENTAL as soon as an input module carries ASan metadata, and
        # says so once per link (warning LNK4300, 870 times across the five
        # MSVC Debug jobs of .github/workflows/windebugmatrix.yml, VS 2022 and
        # VS 2026 alike). Asking for it off removes the cause rather than the
        # message, and leaves LNK4300 free to report any other option ASan
        # would silently disable. One item per generator expression, for the
        # DIRECTORY-scope reason explained below.
        add_link_options($<$<CONFIG:Debug>:/INCREMENTAL:NO>)
        # Enabling ASan on MSVC also switches on the MSVC STL's own
        # "container annotation" instrumentation for std::string,
        # std::vector and (recent toolsets) std::optional: it poisons
        # their reserved-but-unused capacity so ASan can catch
        # out-of-bounds access within it, and every translation unit
        # linked together records, via #pragma detect_mismatch, whether it
        # was compiled with that instrumentation on or off. gaol.lib
        # (bundled inside the prebuilt, non-ASan IBEX package -- see the
        # CMAKE_MSVC_RUNTIME_LIBRARY comment above for why it is not
        # rebuilt from source here) was compiled without ASan, so its
        # object files carry the instrumentation-off value; without this,
        # codac's own ASan-instrumented Debug object files would carry the
        # opposite value, and MSVC's linker would reject the mix with a
        # second error LNK2038 ("mismatch detected for 'annotate_string' /
        # 'annotate_vector' / 'annotate_optional'"), on top of the CRT one
        # already addressed above. _DISABLE_STRING_ANNOTATION and
        # _DISABLE_VECTOR_ANNOTATION are Microsoft's own documented way to
        # turn this instrumentation back off so codac's object files match
        # gaol.lib's; _DISABLE_OPTIONAL_ANNOTATION is added defensively for
        # the same std::optional check (undocumented as of this writing,
        # but following Microsoft's existing naming convention for the
        # other two) -- worst case it is simply an unused macro if that
        # name turns out to be wrong. Kept as three separate
        # add_compile_definitions() calls, one flag per $<CONFIG:Debug>
        # generator expression, for the same reason as the single-flag
        # add_compile_options() calls further below: this is a
        # DIRECTORY-scope property, which does not evaluate correctly when
        # one such expression packs more than one ';'-separated item.
        add_compile_definitions($<$<CONFIG:Debug>:_DISABLE_STRING_ANNOTATION>)
        add_compile_definitions($<$<CONFIG:Debug>:_DISABLE_VECTOR_ANNOTATION>)
        add_compile_definitions($<$<CONFIG:Debug>:_DISABLE_OPTIONAL_ANNOTATION>)
        message(STATUS "${COLOR_BLUE}MSVC AddressSanitizer will be enabled for Debug-configuration builds (runtime: ${CODAC_MSVC_ASAN_RUNTIME_DLL}).${COLOR_RESET}")
      else()
        message(STATUS "${COLOR_RED}No MSVC AddressSanitizer runtime found next to ${CMAKE_CXX_COMPILER} for architecture '${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}': Debug-configuration builds will not be sanitized.${COLOR_RESET}")
      endif()

      # Cached so python/src/core/CMakeLists.txt and tests/CMakeLists.txt
      # can reuse this result (empty string if not found) without
      # repeating the detection above.
      set(CODAC_MSVC_ASAN_RUNTIME_DLL ${CODAC_MSVC_ASAN_RUNTIME_DLL} CACHE INTERNAL "Path to the MSVC ASan runtime DLL, empty if unavailable")
    else()
      # add_compile_options() (a DIRECTORY-scope property, unlike
      # target_compile_options()) does not evaluate correctly when a single
      # $<CONFIG:Debug> generator expression's content packs more than one
      # flag separated by ';' -- reproduced locally with both Ninja and
      # Unix Makefiles (CMake 3.16 and 4.4.3 alike): the embedded ';' gets
      # split before the genexpr is fully evaluated, corrupting it into
      # literal, unrecognized compiler arguments (e.g. the literal text
      # "$<1:-Wreturn-type" as one broken token). target_compile_options()
      # is not affected, but this is DIRECTORY-scope by design (it must
      # apply project-wide, not to one target), so instead each flag gets
      # its own single-flag generator expression below.
      add_compile_options($<$<CONFIG:Debug>:-Wreturn-type>)
      add_compile_options($<$<CONFIG:Debug>:-fno-omit-frame-pointer>)
      add_compile_options($<$<CONFIG:Debug>:-g>)
      add_compile_options($<$<CONFIG:Debug>:-O0>)

      # -fsanitize=address,undefined is only added once we have actually
      # confirmed that this compiler can *link* a sanitized binary, rather
      # than unconditionally, the way this project used to. On Linux/macOS
      # GCC/Clang, libasan/libubsan are essentially always installed
      # alongside the compiler and this check is a formality; the case it
      # actually guards against is MinGW-w64: the choco `mingw` package used
      # by .github/workflows/windebugmatrix.yml does not ship libasan.a/
      # libubsan.a at all for some GCC versions/architectures, and neither
      # x86 (i686-w64-mingw32) nor x86_64 (x86_64-w64-mingw32) is reliably
      # covered across the versions that package offers -- there is no
      # version/architecture combination worth hardcoding an exception list
      # for. Probing capability directly, exactly like the MSVC ASan
      # runtime-DLL search above and the FMA detection further below, means
      # a Debug configuration simply comes out unsanitized on a toolchain
      # that lacks the runtime, instead of configuring successfully and then
      # failing later at link time with "cannot find -lasan"/"-lubsan" on
      # every single target.
      include(CheckCXXSourceCompiles)
      set(CMAKE_REQUIRED_FLAGS "-fsanitize=address,undefined")
      check_cxx_source_compiles("int main() { return 0; }" CODAC_COMPILER_SUPPORTS_ASAN_UBSAN)
      unset(CMAKE_REQUIRED_FLAGS)

      if(CODAC_COMPILER_SUPPORTS_ASAN_UBSAN)
        add_compile_options($<$<CONFIG:Debug>:-fsanitize=address>)
        add_compile_options($<$<CONFIG:Debug>:-fsanitize=undefined>)
        add_link_options($<$<CONFIG:Debug>:-fsanitize=address,undefined>)
        message(STATUS "${COLOR_BLUE}ASan/UBSan will be enabled for Debug-configuration builds.${COLOR_RESET}")
      else()
        message(STATUS "${COLOR_RED}No ASan/UBSan runtime found for ${CMAKE_CXX_COMPILER}: Debug-configuration builds will not be sanitized.${COLOR_RESET}")
      endif()
    endif()
  endif()
  


  # FMA (fused multiply-add) is only ever activated here through an explicit
  # compiler flag, and only when check_cxx_compiler_flag() confirms it is
  # actually recognized. This matters because "the FMA flag" is not a
  # portable concept:
  #  - x86/x86_64 (GCC/Clang): -mfma enables the FMA3 ISA extension.
  #  - x86/x64 (MSVC): there is no dedicated FMA flag; /arch:AVX2 implies it.
  #  - AArch64 (ARMv8-A, e.g. regular Raspberry Pi OS 64-bit): FMA
  #    (FMADD/FMLA) is part of the base ISA, scalar and NEON alike, so no
  #    flag exists or is needed; -mfma is simply unknown to the compiler
  #    there, the check below fails, and the flag is skipped (the compiler
  #    still emits FMA instructions on its own when it can).
  #  - ARMv7-A / 32-bit ARMv8-A (e.g. a Raspberry Pi 2 in AArch32, or a
  #    Raspberry Pi 3/4 running a 32-bit OS: the silicon is ARMv8-A but the
  #    32-bit userland reports itself as armv7l or armv8l): FMA hardware is
  #    optional on ARMv7-A but mandatory on ARMv8-A even in its 32-bit
  #    execution state, and in both cases it is exposed the same way, via
  #    the NEON-VFPv4 FPU extension, activated by
  #    -mfpu=neon-vfpv4 -mfloat-abi=hard (plus -ffp-contract=fast, since
  #    C++ does not permit fusing a*b+c into FMA by default). -mfma itself
  #    is not a recognized flag on any 32-bit ARM target, so this is
  #    handled in its own branch below rather than falling through to the
  #    generic GCC/Clang case.
  #  - ARMv6 hard-float (e.g. Raspbian Bullseye armv6hf, Raspberry Pi
  #    Zero/1): the FPU predates fused multiply-add support (FMA was only
  #    introduced with VFPv4/ARMv7-A), so there is no equivalent flag to
  #    fall back to either; the check fails and we correctly get plain
  #    (unfused) multiply+add on that target.
  # In all unsupported cases the check below simply leaves the flag out,
  # which is the correct behavior rather than a build failure.
  # CODAC_ARCH_CXX_FLAGS collects whichever of the flags below actually got
  # applied, so that it can be re-exported as part of CODAC_CXX_FLAGS in the
  # generated codac-config.cmake (see src/CMakeLists.txt). Without this,
  # downstream consumers (examples, user projects built via find_package(CODAC))
  # compile Eigen's header-only templates with different alignment/instruction-set
  # assumptions than the ones baked into the prebuilt static libraries they link
  # against -- an ODR/ABI mismatch that Eigen documents as unsafe, and that in
  # practice corrupts the heap (mismatched aligned_malloc/aligned_free paths).
  set(CODAC_ARCH_CXX_FLAGS "")

  include(CheckCXXCompilerFlag)
  if(MSVC)
      check_cxx_compiler_flag("/arch:AVX2" COMPILER_SUPPORTS_FMA)
      if(COMPILER_SUPPORTS_FMA)
        add_compile_options(/arch:AVX2)
        set(CODAC_ARCH_CXX_FLAGS "/arch:AVX2")
      endif()
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^armv[7-9]")
      check_cxx_compiler_flag("-mfpu=neon-vfpv4 -mfloat-abi=hard" COMPILER_SUPPORTS_FMA)
      if(COMPILER_SUPPORTS_FMA)
        add_compile_options(-mfpu=neon-vfpv4 -mfloat-abi=hard -ffp-contract=fast)
        set(CODAC_ARCH_CXX_FLAGS "-mfpu=neon-vfpv4;-mfloat-abi=hard;-ffp-contract=fast")
      endif()
  elseif(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      # -mfma turns AVX on, and AVX asks for a 32-byte aligned stack, which GCC
      # targeting Windows cannot deliver: the x64 calling convention there only
      # guarantees 16 bytes and GCC does not realign on entry, so any spill of a
      # 256-bit register faults. Eigen's own AVX helpers are where this lands --
      # segment_mask_4x64() computes a load mask entirely in registers, has no
      # memory operand of its own, and still died on SIGSEGV in 32 of the 87
      # tests of the MinGW x64 Debug job of .github/workflows/windebugmatrix.yml,
      # every one of them through a partial packet load. The same build in
      # Release survives only because -O3 inlines those helpers and keeps their
      # values in registers, where -Os spills them: a matter of luck, not of
      # correctness. The 32-bit MinGW job is spared because GCC realigns the
      # stack there as a matter of course.
      #
      # Leaving the flag out on this toolchain costs very little. Codac compiles
      # with -ffp-contract=off, so no fused multiply-add is generated from
      # ordinary arithmetic anyway; what is given up is Eigen using 256-bit
      # packets, and only on Windows/GCC. It also protects the users of the
      # MinGW packages this project publishes: CODAC_ARCH_CXX_FLAGS is
      # re-exported through codac-config.cmake, so they would otherwise inherit
      # the flag and the same crash in their own Debug builds.
      check_cxx_compiler_flag("-mfma" COMPILER_SUPPORTS_FMA)
      if(COMPILER_SUPPORTS_FMA)
        message(STATUS "FMA is supported but left disabled: GCC targeting Windows cannot align the stack for the AVX code it enables.")
      endif()
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
      # Probe what -mfma does, not whether the driver accepts it. Clang accepts
      # an ISA flag it cannot honour -- on arm64 it takes -mfma, ignores it, and
      # merely warns -- so check_cxx_compiler_flag() reports success and every
      # translation unit then carries a flag with no effect. That is where the
      # 1138 -Wunused-command-line-argument warnings of the macOS arm64 jobs came
      # from. __FMA__ is defined only when FMA is actually turned on for the
      # target, so compiling against it asks the question we mean to ask, and
      # keeps working for any future ISA flag handled the same way.
      include(CheckCXXSourceCompiles)
      set(CMAKE_REQUIRED_FLAGS "-mfma")
      check_cxx_source_compiles("
        #ifndef __FMA__
        #error -mfma was accepted but had no effect on this target
        #endif
        int main() { return 0; }"
        COMPILER_SUPPORTS_FMA)
      unset(CMAKE_REQUIRED_FLAGS)
      if(COMPILER_SUPPORTS_FMA)
        add_compile_options(-mfma )
        set(CODAC_ARCH_CXX_FLAGS "-mfma")
      endif()
  endif()



  # Temporary attempts to fix errors similar to: 
  # _ number of sections exceeded object file format limit.
  # _ out of memory allocating XXX bytes.
  # _ string table overflow.
  # _ .obj: file too big.
  if(MSVC)
    add_compile_options(/bigobj)
  elseif(MINGW)
    add_compile_options(-Wa,-mbig-obj)
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
      add_compile_options(-g0)
    endif()
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
      add_compile_options(-Os)
    endif()
  endif()

#  # Check that the compiler supports c++20
#  include(CheckCXXCompilerFlag)
#  check_cxx_compiler_flag("-std=c++2a" COMPILER_SUPPORTS_CXX20)
#  
#  if(COMPILER_SUPPORTS_CXX20)
#    add_compile_options(-std=c++2a)
#  else()
#    message(FATAL_ERROR "Codac needs a compiler with C++20 support")
#  endif()

  #if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
  #endif()


################################################################################
# Looking for IBEX
################################################################################

  find_package(IBEX REQUIRED)
  ibex_init_common() # IBEX should have installed this function
  message(STATUS "Found IBEX version ${IBEX_VERSION}")


################################################################################
# Treating dependencies' headers as system headers
################################################################################

  # This project compiles with /W4 or -Wall -Wextra -Wpedantic, and a dependency
  # pulled in with FetchContent inherits that level: its headers are ordinary
  # include directories, not system ones. Eigen and Catch2 alone accounted for
  # about 8300 MSVC warnings per job (C4459, C5054, C4308, C4324) and the 557
  # -Wc2y-extensions Clang reports for Catch2's __COUNTER__, drowning the
  # handful of warnings that actually belong to Codac.
  #
  # FetchContent_Declare(... SYSTEM) does exactly this, but only since CMake
  # 3.25, while this project accepts 3.14. Setting the property by hand is
  # equivalent and has worked for far longer: the compiler then receives
  # -isystem (or /external:I, which CMake already emits for the Visual Studio
  # generator alongside /external:W0).
  function(codac_mark_target_system tgt)
    if(NOT TARGET ${tgt})
      return()
    endif()
    # A property cannot be set on an ALIAS, so resolve it to the target it names
    # (Eigen exports Eigen3::Eigen as an alias of eigen).
    get_target_property(_aliased ${tgt} ALIASED_TARGET)
    if(_aliased)
      set(tgt ${_aliased})
    endif()
    get_target_property(_inc ${tgt} INTERFACE_INCLUDE_DIRECTORIES)
    if(_inc)
      set_target_properties(${tgt} PROPERTIES
                            INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${_inc}")
    endif()
  endfunction()


  # Records the directories in which a module keeps its public headers, in
  # their canonical location under src/. Every consumer (the tests, the
  # examples and the Python bindings) lists CODAC_SOURCE_INCLUDE_DIRS first,
  # so that a header is opened at src/... rather than through a copy in the
  # build tree. Both compile identically, but the path used is the path gcov
  # records: reached both ways, a header ends up as two unrelated entries in
  # the coverage report, each showing only what its own callers exercised,
  # and is reported well below its real coverage.
  #
  # The variable is a cache entry because it is filled in by src/*/CMakeLists.txt
  # and read from tests/, examples/ and python/, which are separate directory
  # scopes; src/CMakeLists.txt empties it before the modules append to it, so
  # that a reconfigure does not append to what the previous one left behind.
  function(codac_publish_include_dirs)
    set(CODAC_SOURCE_INCLUDE_DIRS ${CODAC_SOURCE_INCLUDE_DIRS} ${ARGN}
        CACHE INTERNAL "Directories holding the public headers of the codac modules")
  endfunction()


################################################################################
# Looking for Eigen3
################################################################################

  option(ENABLE_FIND_PACKAGE_EIGEN3 "ENABLE_FIND_PACKAGE_EIGEN3" OFF)
  if(ENABLE_FIND_PACKAGE_EIGEN3)
    find_package(Eigen3 3.4.0 QUIET) # Use Eigen3_DIR or CMAKE_PREFIX_PATH to specify a custom Eigen3 installation path...
  endif()
  if(Eigen3_FOUND)
    message(STATUS "Found Eigen3 version ${Eigen3_VERSION} in ${Eigen3_DIR}")
  else()
    message(STATUS "Attempt to install custom Eigen3 automatically...")
    FetchContent_Declare(Eigen3 URL http://github.com/codac-team/eigen/archive/refs/heads/eigen-5.0.zip)
    #FetchContent_Declare(Eigen3 URL ${CMAKE_CURRENT_SOURCE_DIR}/3rd/eigen-a0dc3994bd5d4e804dab58decb78c56a5af516ba.zip) # If needed to be self-contained...
    FetchContent_MakeAvailable(Eigen3)
    # Only needed for the FetchContent path: a target coming from
    # find_package() is IMPORTED, and CMake already treats those as system.
    codac_mark_target_system(Eigen3::Eigen)
  endif()
  # Adds Eigen3::Eigen

################################################################################
# Looking for Threads
################################################################################

  find_package(Threads REQUIRED)

################################################################################
# Looking for CAPD (if needed)
################################################################################

  option(WITH_CAPD "Using CAPD for accurate integration of ODEs" OFF)

  if(WITH_CAPD)
    find_package(CAPD REQUIRED)
  endif()

################################################################################
# Code coverage
################################################################################

  # WITH_COVERAGE instruments the build and adds two targets: `coverage`, which
  # runs the whole ctest suite and turns the counters gcov leaves behind into a
  # report, and `coverage-report`, which only does the second half. That is what
  # the "Ubuntu 26.04 GCC x86_64 Release coverage" job of
  # .github/workflows/unixdebug.yml now calls, so that a developer measuring
  # coverage by hand and the job reporting it use the same flags, the same gcov
  # and the same filters, and cannot drift apart. Deciding all of that here
  # rather than leaving it to the caller is also what spares everyone the traps
  # below.
  #
  # Coverage answers "what do the tests reach", so it is worth turning
  # BUILD_TESTS and TEST_EXAMPLES on with it -- the examples are integration
  # tests and reach code the unit tests do not.
  #
  # This block has to come before the add_subdirectory() calls below:
  # add_compile_options() is a directory property, and only reaches the targets
  # created after it. Declared any later, it configures cleanly, builds
  # cleanly, and leaves no .gcno file behind at all -- an empty report rather
  # than an error.
  option(WITH_COVERAGE "Instrument the build for code coverage (GCC/Clang)" OFF)

  if(WITH_COVERAGE)

    if(MSVC)
      message(FATAL_ERROR "WITH_COVERAGE relies on gcov, which MSVC does not provide.")
    endif()

    add_compile_options(--coverage)
    add_link_options(--coverage)

    # pybind11 adds -flto to the modules it builds, and GCC cannot combine that
    # with gcov instrumentation: the link then fails on an undefined vtable,
    # the vtable being emitted nowhere once both are on. pybind11 leaves LTO
    # alone as soon as CMAKE_INTERPROCEDURAL_OPTIMIZATION is set, which is what
    # this does -- silently, because a coverage build that does not link is of
    # no use to anyone.
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)

    # gcov itself is toolchain-specific: the notes and counters Clang writes are
    # read by `llvm-cov gcov`, not by GCC's gcov, and pointing gcovr at the
    # wrong one yields an empty report rather than an error. The one shipped
    # next to the compiler in use is therefore named explicitly.
    set(CODAC_GCOV_ARG "")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      # Debian and its derivatives ship the tools of each LLVM release under a
      # versioned name in /usr/bin and unversioned in the release's own prefix,
      # so both spellings and both places are tried, the version being taken
      # from the compiler that will produce the notes.
      get_filename_component(_codac_cxx_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
      string(REGEX MATCH "^[0-9]+" _codac_llvm_major "${CMAKE_CXX_COMPILER_VERSION}")
      find_program(LLVM_COV_EXECUTABLE
                   NAMES llvm-cov llvm-cov-${_codac_llvm_major}
                   HINTS ${_codac_cxx_dir}
                         /usr/lib/llvm-${_codac_llvm_major}/bin)
      if(LLVM_COV_EXECUTABLE)
        set(CODAC_GCOV_ARG --gcov-executable "${LLVM_COV_EXECUTABLE} gcov")
      else()
        message(STATUS "${COLOR_RED}llvm-cov was not found next to ${CMAKE_CXX_COMPILER}: the coverage report would come out empty.${COLOR_RESET}")
      endif()
    endif()

    find_program(GCOVR_EXECUTABLE gcovr)

    if(NOT GCOVR_EXECUTABLE)

      message(STATUS "${COLOR_RED}gcovr was not found: the build is instrumented, but the `coverage` target is unavailable (pip install gcovr).${COLOR_RESET}")

    else()

      # A note on paths, since it decides what the report is worth: gcov
      # records the path each translation unit actually opened a header
      # through, so a header reached both directly and through a copy in
      # the build tree comes out as two unrelated entries, each showing
      # only what its own callers exercised. Every consumer (tests,
      # examples, Python bindings) therefore lists CODAC_SOURCE_INCLUDE_DIRS
      # first -- see codac_publish_include_dirs() above. Without that, a
      # header was reported well below its real coverage, and every header
      # of graphics/, unsupported/ and extensions/ was measured on a
      # build-tree copy that is not the file anyone edits.
      #
      # --gcov-exclude-directories keeps gcovr from even descending into the
      # dependencies FetchContent puts in the build tree, which --exclude alone
      # would only drop from the finished report. It matters beyond noise:
      # Catch2 compiles with -ffile-prefix-map, so the paths recorded in its
      # notes do not resolve from anywhere and gcov fails outright on them; and
      # gcov reports negative hit counts on heavily inlined code, Eigen being
      # where that first shows, which gcovr treats as fatal unless told
      # otherwise. The pattern has no leading ".*/" on purpose: gcovr matches it
      # against a path relative to the working directory, which here starts with
      # "_deps/" and so has nothing for that slash to match.
      set(CODAC_GCOVR_COMMAND
          ${GCOVR_EXECUTABLE}
          --root ${CMAKE_SOURCE_DIR}
          ${CODAC_GCOV_ARG}
          --gcov-exclude-directories ".*_deps.*"
          --gcov-ignore-parse-errors=negative_hits.warn_once_per_file
          --exclude ".*/tests/.*"
          --exclude ".*/examples/.*"
          --exclude ".*/python/.*"
          --print-summary
          --html-details ${CMAKE_BINARY_DIR}/coverage.html
          --xml ${CMAKE_BINARY_DIR}/coverage.xml
          # Where to look for the counters. Without it gcovr falls back on
          # --root, i.e. the source tree, and walks every build directory
          # sitting under it: a second configuration kept next to this one
          # (a Debug tree, an older coverage tree) then has its own .gcda
          # read into this report, mixing counters produced by a different
          # build of different sources. "." rather than an absolute path
          # because the working directory below is already the build tree,
          # and because --gcov-exclude-directories above matches against
          # paths relative to it.
          .)

      # Two targets rather than one, because the counters gcov leaves behind
      # accumulate across runs and are read independently of what produced
      # them: `coverage` is the whole thing, `coverage-report` re-reads whatever
      # the last ctest invocation happened to touch. The second is what to use
      # after running part of the suite by hand -- ctest -R something -- which
      # is the usual way of asking what one test reaches.
      #
      # VERBATIM on both, because the arguments above are regular expressions:
      # without it CMake hands them to the shell unquoted, which expands
      # ".*/tests/.*" as a file pattern -- gcovr then receives "./tests/.." and
      # refuses it.
      add_custom_target(coverage
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMAND ${CODAC_GCOVR_COMMAND}
        COMMAND ${CMAKE_COMMAND} -E echo
                "Coverage report: ${CMAKE_BINARY_DIR}/coverage.html (and coverage.xml, Cobertura)"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running the tests and measuring what they reach"
        VERBATIM)

      add_custom_target(coverage-report
        COMMAND ${CODAC_GCOVR_COMMAND}
        COMMAND ${CMAKE_COMMAND} -E echo
                "Coverage report: ${CMAKE_BINARY_DIR}/coverage.html (and coverage.xml, Cobertura)"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Turning the counters left by the last test run into a report"
        VERBATIM)

      message(STATUS "${COLOR_BLUE}Coverage instrumentation enabled; run `make coverage` to produce the report.${COLOR_RESET}")

    endif()

  endif()


################################################################################
# Compile sources
################################################################################
  
  add_subdirectory(src) # C++ sources
  add_subdirectory(doc) # documentation (Doxygen + Sphinx manual)

  # Python binding:
  option(WITH_PYTHON "Build Python binding" OFF)
  if(WITH_PYTHON)
    set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
    add_subdirectory(python)
  endif()


################################################################################
# Tests
################################################################################
  
  # Note: place this tests block before the add_subdirectory(python),
  # otherwise python tests will not be taken into account. 
  option(BUILD_TESTS "Build test" OFF)
  if(${CMAKE_VERSION} VERSION_LESS "3.14.0")
    # Disable tests due to FetchContent_MakeAvailable which needs CMake >= 3.14.
    set(BUILD_TESTS OFF CACHE BOOL "Build test" FORCE)
  endif()
  if(BUILD_TESTS)
    include(CTest)
    # `check` is `test` plus two things: it prints the output of what failed,
    # and it rebuilds the suite before running it. CMake's own `test` target
    # does neither -- it runs ctest on whatever happens to be in the build tree.
    #
    # There is no DEPENDS here on purpose. In add_custom_target(), DEPENDS names
    # *files*, not targets, and this target used to carry "DEPENDS
    # ${PROJECT_NAME}", which is neither: no file and no target is called
    # "codac", the libraries being codac-core, codac-graphics and so on. make
    # therefore stopped on "No rule to make target 'codac'" after rebuilding
    # everything and never reached ctest, so `make check` could not run the
    # suite at all. What must exist before the suite runs is attached with
    # add_dependencies() instead: every test executable, by
    # tests/CMakeLists.txt; every example, by examples/CMakeLists.txt; and the
    # Python extension modules just below, which the Python half of the suite
    # imports.
    add_custom_target(check
                      COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure $(ARGS)
                      COMMENT "Running the tests")

    foreach(_codac_py_module _core _graphics _unsupported)
      if(TARGET ${_codac_py_module})
        add_dependencies(check ${_codac_py_module})
      endif()
    endforeach()

    add_subdirectory(tests)
  endif()

  option(TEST_EXAMPLES "Testing examples" OFF)
  add_subdirectory(examples) # examples are tested as integration tests


################################################################################
# Archives and packages
################################################################################

  set(CPACK_GENERATOR "TGZ" "ZIP")
  # The DEB generator shells out to dpkg-deb, which only exists on
  # Debian-based systems -- adding it unconditionally would make `cpack`/
  # `make package` fail on Windows, macOS, and non-Debian Linux, even
  # though nothing else in the build depends on it.
  find_program(DPKG_DEB_EXECUTABLE dpkg-deb)
  if(DPKG_DEB_EXECUTABLE)
    list(APPEND CPACK_GENERATOR "DEB")
  endif()
  string(TOLOWER "${CMAKE_PROJECT_NAME}" CPACK_PACKAGE_NAME)
  set(CPACK_PACKAGE_VENDOR "Codac Team")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${CODAC_DESCRIPTION})
  set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Maintainer <simon.rohou@ensta.fr>")
  set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${CODAC_URL})
  # todo: finish deb package

  include(CPack)