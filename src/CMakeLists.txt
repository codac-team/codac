# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

add_subdirectory(core)

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tubex.pc
              CONTENT "prefix=${CMAKE_INSTALL_PREFIX}
includedir=\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}
libdir=\${prefix}/${CMAKE_INSTALL_LIBDIR}

Name: tubex
Description: ${TUBEX_DESCRIPTION}
Url: ${TUBEX_URL}
Version: ${TUBEX_VERSION}
Requires: ibex
Cflags: -I\${includedir}/tubex 
Libs: -L\${libdir} -ltubex")

# For several libraries:
# Cflags: -I${includedir}/tubex-core -I${includedir}/tubex-solver
# Libs: -L${libdir} -ltubex-core -ltubex-solver 

# todo: specify minimum version of IBEX (in Requires field)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tubex.pc DESTINATION ${CMAKE_INSTALL_PKGCONFIG})