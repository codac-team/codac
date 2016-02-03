#--------------------------------------------------------
# The CMakeLists.txt for:                     Tube : test
# Author(s):                                  Simon Rohou
#--------------------------------------------------------

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

### Tube class

  SET(SRC
    ../Tube.cpp
  )

  SET(HEADERS 
    ../Tube.h
  )

  ADD_LIBRARY(ibex-robotics-tube ${SRC})


### Test

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
     ibex-robotics-tube
     ibex
     prim)