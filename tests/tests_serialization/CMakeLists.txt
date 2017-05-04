# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================


set(TESTS_SERIALIZ_SOURCES
  tests_serialization.cpp
)

add_executable(tubelib_tests_serialization ${TESTS_SERIALIZ_SOURCES})
target_link_libraries(tubelib_tests_serialization tubex-core)
add_test(NAME TestSerializ COMMAND tubelib_tests_serialization)