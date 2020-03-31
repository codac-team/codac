find_package(PkgConfig REQUIRED)
pkg_search_module(PKG_CAPD REQUIRED  capd capd-gui mpcapd mpcapd-gui )  # this looks for capd.pc file
include_directories(${PKG_CAPD_INCLUDE_DIRS})
message(STATUS "[CAPD INTEGRATION TESTS] PKG_CAPD_INCLUDE_DIRS = ${PKG_CAPD_INCLUDE_DIRS}")
message(STATUS "[CAPD INTEGRATION TESTS] PKG_CAPD_LDFLAGS = ${PKG_CAPD_LDFLAGS}")


add_executable(non_visual test_Capd2Tubex.cpp)
target_link_libraries(non_visual PRIVATE tubex tubex-3rd-capd ${PKG_CAPD_LDFLAGS})
add_dependencies(check non_visual)
add_test(NAME non_visual COMMAND non_visual)

file(COPY parameters DESTINATION ${CMAKE_BINARY_DIR}/tests/3rd/Capd2Tubex)
