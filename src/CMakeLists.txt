# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

add_subdirectory(core)
add_subdirectory(robotics)
add_subdirectory(3rd) # 3rd party (todo: make a clean dependency)


set(TUBEX_PKG_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/tubex.pc)
file(WRITE ${TUBEX_PKG_CONFIG_FILE} "prefix=${CMAKE_INSTALL_PREFIX}
includedir=\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}
libdir=\${prefix}/${CMAKE_INSTALL_LIBDIR}

Name: tubex
Description: ${TUBEX_DESCRIPTION}
Url: ${TUBEX_URL}
Version: ${TUBEX_VERSION}
Requires: ibex\n")
if(WITH_CAPD)
    file(APPEND ${TUBEX_PKG_CONFIG_FILE} "Cflags: -I\${includedir}/ibex -I\${includedir}/tubex -I\${includedir}/tubex-rob -I\${includedir}/tubex-pyibex -I\${includedir}/tubex-capd\n")
    file(APPEND  ${TUBEX_PKG_CONFIG_FILE} "Libs: -L\${libdir} -ltubex -ltubex-rob -ltubex-pyibex -ltubex-capd -ltubex")
else()
    file(APPEND ${TUBEX_PKG_CONFIG_FILE} "Cflags: -I\${includedir}/ibex -I\${includedir}/tubex -I\${includedir}/tubex-rob -I\${includedir}/tubex-pyibex\n")
    file(APPEND  ${TUBEX_PKG_CONFIG_FILE} "Libs: -L\${libdir} -ltubex -ltubex-rob -ltubex-pyibex -ltubex")
endif()
# todo: specify minimum version of IBEX (in Requires field)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tubex.pc DESTINATION ${CMAKE_INSTALL_PKGCONFIG})