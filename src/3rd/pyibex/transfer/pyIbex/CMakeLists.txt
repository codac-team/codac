cmake_minimum_required(VERSION 3.0.2)

project (PYIBEX VERSION 1.9.2 LANGUAGES CXX)
set (PYIBEX_DESCRIPTION "A set of python modules for solving nonlinear problems using Interval Arithmetic tools." )
set (PYIBEX_URL "http://benensta.github.io/pyIbex/")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/)

################################################################################
# Read Pproject version from git tag
################################################################################
include(version_from_git)
version_from_git()

################################################################################
# build type
################################################################################
IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE "Release")
	#SET(CMAKE_BUILD_TYPE "DEBUG")
	#SET(CMAKE_BUILD_TYPE "RELWITHDEBINFO")
	#SET(CMAKE_BUILD_TYPE "MINSIZEREL")
ENDIF()


################################################################################
# option declaration
################################################################################
option(BUILD_TESTS "Build tests" ON)

################################################################################
# Find IbexLib using cmake
################################################################################
find_package (IBEX REQUIRED)
message (STATUS "Found Ibex version ${IBEX_VERSION}")
message(STATUS "ibex includes ${IBEX_INCLUDES}")
message(STATUS Ibex::ibex)
ibex_init_common() # Ibex should have installed this function
# set(LIBS Ibex::ibex)


################################################################################
# Add pybind11 
################################################################################
add_subdirectory(pybind11)

################################################################################
# Add pybind11 to link static lib into .so
################################################################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")


################################################################################
# Set win32 flags to generate and use static lib
################################################################################
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
  foreach(CompilerFlag ${CompilerFlags})
    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
  endforeach()
endif()


add_subdirectory(src)

#########################################################################
### Enable Tests 
#########################################################################
if (BUILD_TESTS)
	ENABLE_TESTING()
	include( CTest )
	add_subdirectory(pyibex/tests)
endif(BUILD_TESTS)

#########################################################################
### Uninstall ###
#########################################################################
# configure_file("${CMAKE_HOME_DIRECTORY}/CMakeModules/cmake_uninstall.cmake.in" "cmake_uninstall.cmake" IMMEDIATE @ONLY)
# add_custom_target(uninstall "${CMAKE_COMMAND}" -P "cmake_uninstall.cmake")

#########################################################################
### Package configuration (Not working yet) ###
#########################################################################
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "B. Desrochers")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Python wrapper for Ibex Library")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
#set(CPACK_PACKAGE_INSTALL_DIRECTORY "pyIbex")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Benoit Desrochers <ben.ensta@gmail.com>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS)
SET(CPACK_GENERATOR "DEB")
set (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
INCLUDE(CPack)
