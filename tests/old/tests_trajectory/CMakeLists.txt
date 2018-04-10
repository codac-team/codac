set(TESTS_TRAJ_SOURCES
  tests_trajectory.cpp
)

add_executable(tubelib_tests_traj ${TESTS_TRAJ_SOURCES})
target_link_libraries(tubelib_tests_traj tubex-lib)
add_test(NAME TestTraj COMMAND tubelib_tests_traj)