SET(SRC
  main.cpp
)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

ADD_EXECUTABLE(testTube ${SRC})

TARGET_LINK_LIBRARIES(testTube
    ${CMAKE_THREAD_LIBS_INIT}
    ibex-robotics-graphics
    ibex-robotics-tube
    ${LIBS}
    )

ADD_TEST(testTube testTube)
