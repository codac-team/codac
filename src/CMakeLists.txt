# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

add_subdirectory(core)
add_subdirectory(robotics)
add_subdirectory(3rd) # 3rd party (todo: make a clean dependency)

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tubex.pc
              CONTENT "prefix=${CMAKE_INSTALL_PREFIX}
includedir=\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}
libdir=\${prefix}/${CMAKE_INSTALL_LIBDIR}

Name: tubex
Description: ${TUBEX_DESCRIPTION}
Url: ${TUBEX_URL}
Version: ${TUBEX_VERSION}
Requires: ibex
Cflags: -I\${includedir}/tubex -I\${includedir}/tubex-rob -I\${includedir}/tubex-3rd-pyibex
Libs: -L\${libdir} -ltubex -ltubex-rob -ltubex-3rd-pyibex -ltubex")

# todo: specify minimum version of IBEX (in Requires field)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tubex.pc DESTINATION ${CMAKE_INSTALL_PKGCONFIG})