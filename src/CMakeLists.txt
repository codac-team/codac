# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

add_subdirectory(core)
add_subdirectory(robotics)
add_subdirectory(3rd) # 3rd party (todo: make a clean dependency)

set(TUBEX_PKG_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/tubex.pc)

set(TUBEX_PKG_CONFIG_CFLAGS "-I\${includedir}/ibex -I\${includedir}/tubex -I\${includedir}/tubex-rob -I\${includedir}/tubex-pyibex")
set(TUBEX_PKG_CONFIG_LIBS "-L\${libdir} -ltubex -ltubex-rob -ltubex-pyibex")

if(WITH_CAPD)
  set(TUBEX_PKG_CONFIG_CFLAGS "${TUBEX_PKG_CONFIG_CFLAGS} -I\${includedir}/tubex-ode")
  set(TUBEX_PKG_CONFIG_LIBS "${TUBEX_PKG_CONFIG_LIBS} -ltubex-ode")
endif()

set(TUBEX_PKG_CONFIG_LIBS "${TUBEX_PKG_CONFIG_LIBS} -ltubex") # Seems to be needed

file(GENERATE OUTPUT ${TUBEX_PKG_CONFIG_FILE}
              CONTENT "prefix=${CMAKE_INSTALL_PREFIX}
includedir=\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}
libdir=\${prefix}/${CMAKE_INSTALL_LIBDIR}

Name: tubex
Description: ${PROJECT_DESCRIPTION}
Url: ${PROJECT_HOMEPAGE_URL}
Version: ${PROJECT_VERSION}
Requires: ibex
Cflags: ${TUBEX_PKG_CONFIG_CFLAGS}
Libs: ${TUBEX_PKG_CONFIG_LIBS}")

# todo: specify minimum version of IBEX (in Requires field)
# todo: update in case of optional 3rd party (ex: CAPD)

install(FILES ${TUBEX_PKG_CONFIG_FILE} DESTINATION ${CMAKE_INSTALL_PKGCONFIG})
install(FILES ${PROJECT_SOURCE_DIR}/tubex-config.cmake DESTINATION ${CMAKE_INSTALL_CMAKE})