# ==================================================================
#  tube-lib - cmake configuration file
# ==================================================================


# Sources

  set(SRC
    main.cpp
  )

  find_package(OpenMP)
  if (OPENMP_FOUND)
      set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  endif()

  add_executable(testTube ${SRC})

  target_link_libraries(testTube
      ${CMAKE_THREAD_LIBS_INIT}
      tube-core
      ${LIBS}
      )

  add_test(testTube testTube)