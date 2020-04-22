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

Name: ${PROJECT_NAME}
Description: ${PROJECT_DESCRIPTION}
Url: ${PROJECT_HOMEPAGE_URL}
Version: ${PROJECT_VERSION}
Requires: ibex
Cflags: -I\${includedir}/tubex -I\${includedir}/tubex-rob -I\${includedir}/tubex-pyibex
Libs: -L\${libdir} -ltubex -ltubex-rob -ltubex-pyibex -ltubex")

# todo: specify minimum version of IBEX (in Requires field)
# todo: update in case of optional 3rd party (ex: CAPD)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tubex.pc DESTINATION ${CMAKE_INSTALL_PKGCONFIG})
install(FILES ${PROJECT_SOURCE_DIR}/tubex-config.cmake DESTINATION ${CMAKE_INSTALL_CMAKE})