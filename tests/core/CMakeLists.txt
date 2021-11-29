# ==================================================================
#  codac / tests - cmake configuration file
# ==================================================================

set(TESTS_NAME codac-tests-core)

list(APPEND SRC_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_predefined_tubes.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_predefined_tubes.h
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_arithmetic.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_cn.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_ctc_delay.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_ctc_deriv.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_ctc_chain.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_ctc_eval.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_ctc_picard.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_ctc_lohner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_ctc_static.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_definition.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_functions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_integration.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_operators.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_geometry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_polygons.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_serialization.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_slices_structure.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_trajectory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_values.cpp
        )

add_executable(${TESTS_NAME} ${SRC_TESTS})
# todo: find a clean way to access codac header files?
set(CODAC_HEADERS_DIR ${CMAKE_CURRENT_BINARY_DIR}/../../include)
target_include_directories(${TESTS_NAME} SYSTEM PUBLIC ${CODAC_HEADERS_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../catch)
target_link_libraries(${TESTS_NAME} PUBLIC Ibex::ibex codac)
add_dependencies(check ${TESTS_NAME})
add_test(NAME ${TESTS_NAME} COMMAND ${TESTS_NAME})