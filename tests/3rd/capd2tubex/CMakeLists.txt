# ==================================================================
#  tubex-lib / tests - cmake configuration file
# ==================================================================

set(TESTS_NAME tubex-tests-3rd-capd)

list(APPEND SRC ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/tests_capd2tubex.cpp
                )

add_executable(${TESTS_NAME} ${SRC})
target_include_directories(${TESTS_NAME} SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../catch)
target_link_libraries(${TESTS_NAME} tubex)
add_dependencies(check ${TESTS_NAME})
add_test(NAME ${TESTS_NAME} COMMAND ${TESTS_NAME})