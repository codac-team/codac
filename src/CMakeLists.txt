# ==================================================================
#  Codac - cmake configuration file
# ==================================================================


# Compiling sources
# =================

  # Emptied here, then appended to by each module below through
  # codac_publish_include_dirs(). It is a cache entry, so it survives a
  # reconfigure: without this reset it would grow a duplicate of every
  # directory on each run, and would keep the directories of a module that has
  # since been switched off (capd, sympy).
  set(CODAC_SOURCE_INCLUDE_DIRS "" CACHE INTERNAL
      "Directories holding the public headers of the codac modules")

  # Same treatment for the directories of the generated umbrella headers.
  set(CODAC_GENERATED_INCLUDE_DIRS "" CACHE INTERNAL
      "Directories holding the umbrella headers generated for the codac modules")

  # Seeded before the modules are added, because each of them appends its own
  # flags to these through PARENT_SCOPE (see the pkg-config section below).
  # They used to be set after the add_subdirectory() calls, which threw every
  # module's contribution away: the installed codac.pc then named no Codac
  # library and no Codac include directory at all.
  #
  # The two directories the modules do not add themselves: the include root,
  # through which <codac> is reached, and include/eigen3, where the Eigen
  # headers the public headers include are installed. Without them a
  # pkg-config consumer cannot compile a single translation unit -- this is
  # the same pair that codac-core carries in its $<INSTALL_INTERFACE:...>.
  #
  # GAOL's directories are not among them: where they are depends on where GAOL
  # was found, or on its being built with Codac, and the part on the
  # dependencies below adds them as Codac::gaol gives them. The -I\${includedir}/ibex that
  # used to sit here, when Codac reached GAOL through IBEX, named a directory
  # under *Codac's* prefix, which only ever existed when the two happened to
  # share one.
  set(CODAC_PKG_CONFIG_CFLAGS "-I\${includedir} -I\${includedir}/eigen3")
  set(CODAC_PKG_CONFIG_MODULE_LIBS "")

  # The "Requires:" line: a module that depends on an outside library appends
  # its pkg-config name here and lets that library's own .pc answer for its
  # flags. GAOL is not named there: a GAOL built with Codac installs its gaol.pc
  # under lib/codac-3rd, where pkg-config does not look, and a GAOL found by its
  # files has none; its flags and libraries are written below instead, as
  # Codac::gaol gives them. The line starts empty; CAPD joins it from
  # src/extensions/capd/CMakeLists.txt when that module is built.
  set(CODAC_PKG_CONFIG_REQUIRES "")

  add_subdirectory(core)
  add_subdirectory(graphics)
  add_subdirectory(unsupported)

  if(WITH_CAPD)
    add_subdirectory(extensions/capd)
  endif()

  if(WITH_PYTHON)
    add_subdirectory(extensions/sympy)
  endif()
  

# Generating PKG file
# ===================

  set(CODAC_PKG_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc)

  # The modules appended their "-l" flags in dependency order: core first, then
  # the modules that link against it. GNU ld resolves static archives left to
  # right and keeps from each only what is undefined by the time it reads it,
  # so a consumer's link line needs them the other way round -- a module before
  # the module it depends on. Reversing the list is what turns the order the
  # modules were configured in into a usable link order. CODAC_LIBRARIES, in
  # the CMake config file below, gets there instead by exporting real targets
  # and letting CMake order them.
  if(CODAC_PKG_CONFIG_MODULE_LIBS)
    string(STRIP "${CODAC_PKG_CONFIG_MODULE_LIBS}" CODAC_PKG_CONFIG_LIBS)
    string(REPLACE " " ";" CODAC_PKG_CONFIG_LIBS "${CODAC_PKG_CONFIG_LIBS}")
    list(REVERSE CODAC_PKG_CONFIG_LIBS)
    string(REPLACE ";" " " CODAC_PKG_CONFIG_LIBS "${CODAC_PKG_CONFIG_LIBS}")
  else()
    set(CODAC_PKG_CONFIG_LIBS "")
  endif()

  # Codac's own flags, which this build puts in CMAKE_CXX_FLAGS
  # (codac_gaol_portability_flags() in scripts/CMakeModules/codac_gaol.cmake),
  # and the flags GAOL gives through Codac::gaol, CODAC_INTERVAL_CXX_FLAGS, set
  # in the top-level CMakeLists.txt: those of interval arithmetic, and those of
  # the fused multiply-add instructions GAOL was compiled with (-mfma and the
  # like). A CMake consumer gets them through CODAC_CXX_FLAGS, which is what
  # every example uses and what the manual tells users to use; a pkg-config
  # consumer has no equivalent. Compiling Codac's headers without
  # -frounding-math silently gives up the guarantee the whole library rests on,
  # and compiling Eigen's headers under other instruction-set assumptions than
  # the prebuilt archives were built with is an ODR/ABI mismatch.
  foreach(_codac_flags CODAC_PORTABILITY_CXX_FLAGS CODAC_INTERVAL_CXX_FLAGS)
    if(${_codac_flags})
      string(REPLACE ";" " " _codac_pkg_config_flags "${${_codac_flags}}")
      string(APPEND CODAC_PKG_CONFIG_CFLAGS " ${_codac_pkg_config_flags}")
    endif()
  endforeach()

  # GAOL's include directories and libraries, as Codac::gaol gives them
  # (codac_gaol_usage(), in the top-level CMakeLists.txt): where the GAOL found
  # on this machine is, or, for the GAOL codac_gaol_build() built, where it is
  # installed with Codac, under ${prefix}. The directories the compiler searches
  # of its own accord are left out, as CMake leaves them out of its command
  # lines.
  foreach(dir ${CODAC_GAOL_INCLUDE_DIRS})
    if(NOT dir IN_LIST CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES)
      codac_gaol_pkg_config_path(dir)
      string(APPEND CODAC_PKG_CONFIG_CFLAGS " -I${dir}")
    endif()
  endforeach()
  # The libraries are named by their full path, as CMake hands them to the
  # linker on the find_package side, rather than as -L plus -l: there is then no
  # chance of another directory on the search path answering for libgaol first,
  # and nothing to guess about how the file is named on each platform. They come
  # after the Codac archives, which depend on them, and gaol before ultim, which
  # it depends on.
  foreach(item ${CODAC_GAOL_LINK_ITEMS})
    codac_gaol_pkg_config_path(item)
    string(APPEND CODAC_PKG_CONFIG_LIBS " ${item}")
  endforeach()

  # codac-core links PUBLIC against Threads::Threads, and so must anything that
  # links codac-core: without it, a program that reaches the peibos threading
  # code (the 11_peibos example, say) fails with an undefined reference to
  # pthread_create. codac-config.cmake gets there through find_package(Threads);
  # a .pc file has to name what that produces itself.
  #
  # What it produces, and not -pthread on principle: since glibc 2.34 the
  # pthread functions are in libc, FindThreads finds them there, and
  # Threads::Threads then contributes nothing at all -- so a .pc file naming
  # -pthread would be describing a flag the CMake side does not pass. The two
  # variables below are FindThreads' own answer: whether the compiler needs the
  # flag, and what has to go on the link line, either of which may be empty.
  if(THREADS_HAVE_PTHREAD_ARG)
    string(APPEND CODAC_PKG_CONFIG_CFLAGS " -pthread")
  endif()
  if(CMAKE_THREAD_LIBS_INIT)
    string(APPEND CODAC_PKG_CONFIG_LIBS " ${CMAKE_THREAD_LIBS_INIT}")
  endif()

  file(GENERATE OUTPUT ${CODAC_PKG_CONFIG_FILE}
                CONTENT "  prefix=${CMAKE_INSTALL_PREFIX}
    includedir=\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}
    libdir=\${prefix}/${CMAKE_INSTALL_LIBDIR}

    Name: ${PROJECT_NAME}
    Description: ${PROJECT_DESCRIPTION}
    Url: ${PROJECT_HOMEPAGE_URL}
    Version: ${PROJECT_VERSION}
    Requires: ${CODAC_PKG_CONFIG_REQUIRES}
    Cflags: ${CODAC_PKG_CONFIG_CFLAGS}
    Libs: -L\${libdir} ${CODAC_PKG_CONFIG_LIBS}
  ")

  install(FILES ${CODAC_PKG_CONFIG_FILE} DESTINATION ${CMAKE_INSTALL_PKGCONFIG})


# Generating codac-config.cmake file
# ==================================

  # The modules are installed as a CMake export set rather than as a handful of
  # loose library files, and two things follow from that.
  #
  #  - The dependencies between them are exported with them. codac-graphics
  #    declares target_link_libraries(... PUBLIC codac-core), so a consumer
  #    that links Codac::codac-graphics gets Codac::codac-core behind it, in
  #    that order and repeated as often as the linker needs. Handing out plain
  #    library paths could not carry that: CODAC_LIBRARIES listed them in
  #    declaration order, core first, and GNU ld -- which resolves a static
  #    archive against whatever is undefined when it reads it, and never looks
  #    back -- left every reference from codac-graphics into codac-core
  #    unresolved. Every Linux and MinGW job that built an example against an
  #    installed Codac failed on it; macOS and MSVC, whose linkers rescan, did
  #    not.
  #
  #  - The rest of the usage requirements travel too: include directories,
  #    Threads, GAOL, CAPD.
  #
  # What an export set cannot carry are the dependencies this build fetches
  # rather than finds, Eigen and pybind11: they are targets of this project, so
  # no consumer's find_package() could define them. The modules keep those as
  # build-tree-only dependencies (see src/core/CMakeLists.txt), and the install
  # tree reaches Eigen's headers as a plain include directory instead.
  install(EXPORT ${PROJECT_NAME}-targets
          FILE ${PROJECT_NAME}-targets.cmake
          NAMESPACE Codac::
          DESTINATION ${CMAKE_INSTALL_CMAKE})

  # Builds CODAC_LIBRARIES and CODAC_INCLUDE_DIRS in a single place
  function(codac_build_config_vars)
    # Base modules, always included. These name the imported targets that
    # codac-targets.cmake defines, rather than library paths: naming the
    # targets is what lets CMake work the link order out for itself.
    set(libraries
      Codac::${PROJECT_NAME}-core
      Codac::${PROJECT_NAME}-graphics
      Codac::${PROJECT_NAME}-unsupported
      Codac::gaol
      Threads::Threads
    )

    set(include_dirs
      \${CODAC_CORE_INCLUDE_DIR}/../  # header root, for <codac>
      \${CODAC_CORE_INCLUDE_DIR}/../eigen3/
      \${CODAC_CORE_INCLUDE_DIR}
      \${CODAC_GRAPHICS_INCLUDE_DIR}
      \${CODAC_UNSUPPORTED_INCLUDE_DIR}
    )

    # Optional dependencies (CAPD, SymPy). Both lists are local until every
    # module has been added, and only then written to the cache: list(APPEND)
    # on a cache variable leaves a local copy behind and the cache entry as it
    # was, so the optional modules never used to reach the generated file.
    if(WITH_CAPD)
      list(APPEND libraries Codac::${PROJECT_NAME}-capd)
      list(APPEND include_dirs \${CODAC_CAPD_INCLUDE_DIR})
      # CODAC_CAPD_LIBRARY is kept as its own variable too, for any consumer
      # that prefers to opt into CAPD explicitly rather than pull it in via
      # the general CODAC_LIBRARIES. The target brings capd::capd and
      # codac-core along with it.
      set(CODAC_CAPD_LIBRARY Codac::${PROJECT_NAME}-capd CACHE INTERNAL "CAPD module of Codac, with all its dependencies")
    endif()

    if(WITH_PYTHON)
      list(APPEND libraries Codac::${PROJECT_NAME}-sympy)
      list(APPEND include_dirs \${CODAC_SYMPY_INCLUDE_DIR})
      set(CODAC_SYMPY_LIBRARY Codac::${PROJECT_NAME}-sympy CACHE INTERNAL "SymPy module of Codac, with all its dependencies")
    endif()

    set(CODAC_LIBRARIES ${libraries} CACHE INTERNAL "Libraries to link against for CODAC")
    set(CODAC_INCLUDE_DIRS ${include_dirs} CACHE INTERNAL "Include directories for CODAC")

    # Propagate Codac's own flags (codac_gaol_portability_flags()) and the flags
    # GAOL gives (-frounding-math, -mfma, etc.) to external projects. In that
    # order, which is the order this build hands them to the compiler in -- the
    # first through CMAKE_CXX_FLAGS, the second through Codac::gaol, whose
    # options come after a target's own -- and decides which of two conflicting
    # flags wins.
    set(cxx_flags ${CODAC_PORTABILITY_CXX_FLAGS} ${CODAC_INTERVAL_CXX_FLAGS})
    set(CODAC_CXX_FLAGS "${cxx_flags}" CACHE INTERNAL "C++ flags for CODAC")
  endfunction()

  # Build the variables written into codac-config.cmake below
  codac_build_config_vars()

  set(CODAC_CMAKE_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake)

  # The definition of Codac::gaol, written by
  # scripts/CMakeModules/codac_gaol.cmake, which knows where GAOL came from.
  codac_gaol_config_snippet(CODAC_GAOL_CONFIG_SNIPPET)

  # The file is written in three passes: what locates the installed pieces
  # first, then the optional modules, then -- once every dependency the export
  # set names is in scope -- the targets file itself and the variables that
  # describe it. That last pass has to come last: the CAPD and SymPy blocks
  # used to be appended after CODAC_LIBRARIES had already been written out, so
  # the libraries they look for could never appear in it.
  file(WRITE ${CODAC_CMAKE_CONFIG_FILE}
             "# Try to find Codac
  # This file has been generated by CMake

    find_path(CODAC_CORE_INCLUDE_DIR ${PROJECT_NAME}-core.h
              PATH_SUFFIXES include/${PROJECT_NAME}-core)
    find_path(CODAC_GRAPHICS_INCLUDE_DIR ${PROJECT_NAME}-graphics.h
              PATH_SUFFIXES include/${PROJECT_NAME}-graphics)
    find_path(CODAC_UNSUPPORTED_INCLUDE_DIR ${PROJECT_NAME}-unsupported.h
              PATH_SUFFIXES include/${PROJECT_NAME}-unsupported)

    find_library(CODAC_CORE_LIBRARY NAMES ${PROJECT_NAME}-core
                 PATH_SUFFIXES lib)
    find_library(CODAC_GRAPHICS_LIBRARY NAMES ${PROJECT_NAME}-graphics
                 PATH_SUFFIXES lib)
    find_library(CODAC_UNSUPPORTED_LIBRARY NAMES ${PROJECT_NAME}-unsupported
                 PATH_SUFFIXES lib)

${CODAC_GAOL_CONFIG_SNIPPET}
    # Projects written for a Codac that depended on IBEX call ibex_init_common()
    # after find_package(CODAC), as the manual used to tell them to, for the
    # interval arithmetic flags. Those flags now come with CODAC_CXX_FLAGS, and
    # this ibex_init_common() does nothing: it is only there so that such
    # projects still configure. A project that finds IBEX itself, before or
    # after Codac, gets IBEX's own function instead.
    if(NOT COMMAND ibex_init_common)
      function(ibex_init_common)
      endfunction()
    endif()

    # codac-core links PUBLIC against Threads::Threads (see src/core/CMakeLists.txt),
    # e.g. for peibos/threading code -- without it, a downstream target that
    # actually pulls in that code (such as the 11_peibos example) fails to link
    # with an undefined reference to pthread_create. The imported targets
    # included below name Threads::Threads, so it also has to be in scope by
    # the time they are defined.
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
  ")

  if(WITH_PYTHON)

    file(APPEND ${CODAC_CMAKE_CONFIG_FILE} "

  # Optional 3rd party:

    find_path(CODAC_SYMPY_INCLUDE_DIR ${PROJECT_NAME}-sympy.h
              PATH_SUFFIXES include/${PROJECT_NAME}-sympy)
    find_library(CODAC_SYMPY_LIBRARY NAMES ${PROJECT_NAME}-sympy
                 PATH_SUFFIXES lib)
    ")

  endif()

  if(WITH_CAPD)

    file(APPEND ${CODAC_CMAKE_CONFIG_FILE} "

  # Optional 3rd party:

    find_package(CAPD REQUIRED)
    find_path(CODAC_CAPD_INCLUDE_DIR ${PROJECT_NAME}-capd.h
              PATH_SUFFIXES include/${PROJECT_NAME}-capd)
    find_library(CODAC_CAPD_LIBRARY NAMES ${PROJECT_NAME}-capd
                 PATH_SUFFIXES lib)
    ")

  endif()

  file(APPEND ${CODAC_CMAKE_CONFIG_FILE} "

  # The Codac modules, as imported targets:

    include(\${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}-targets.cmake)

    set(CODAC_VERSION ${PROJECT_VERSION})
    set(CODAC_LIBRARIES ${CODAC_LIBRARIES})
    set(CODAC_INCLUDE_DIRS ${CODAC_INCLUDE_DIRS})

    set(CODAC_C_FLAGS \"\")
    set(CODAC_CXX_FLAGS \"${CODAC_CXX_FLAGS}\")
  ")


  install(FILES ${CODAC_CMAKE_CONFIG_FILE} DESTINATION ${CMAKE_INSTALL_CMAKE})


# Generating file for #include <codac>
# ====================================

  set(CODAC_MAIN_HEADER ${CMAKE_CURRENT_BINARY_DIR}/codac)

  set(CODAC_MAIN_HEADER_CONTENT "/* This file is generated by CMake */\n\n")
  string(APPEND CODAC_MAIN_HEADER_CONTENT "#pragma once\n\n")
  foreach(header_path ${CODAC_MAIN_SUBHEADERS})
    get_filename_component(header_name ${header_path} NAME)
    string(APPEND CODAC_MAIN_HEADER_CONTENT "#include <${header_name}>\n")
  endforeach()

  # Compare with the existing file, if any
  if(EXISTS "${CODAC_MAIN_HEADER}")
    file(READ "${CODAC_MAIN_HEADER}" EXISTING_CONTENT)
    string(MD5 EXISTING_HASH "${EXISTING_CONTENT}")
    string(MD5 NEW_HASH "${CODAC_MAIN_HEADER_CONTENT}")
  else()
    set(EXISTING_HASH "")
    set(NEW_HASH "new")
  endif()

  # Only rewrite it when it changed: 24 examples #include <codac>, and an
  # unconditional rewrite moves its timestamp on every cmake run, rebuilding
  # all of them although nothing changed.
  if(NOT EXISTING_HASH STREQUAL NEW_HASH)
    file(WRITE "${CODAC_MAIN_HEADER}" "${CODAC_MAIN_HEADER_CONTENT}")
  endif()
  install(FILES ${CODAC_MAIN_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/)