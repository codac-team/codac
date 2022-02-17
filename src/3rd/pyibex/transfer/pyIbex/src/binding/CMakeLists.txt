set( SRCS 
    pyibex.cpp
    pyibex_Interval.cpp
    pyibex_IntervalVector.cpp
    pyibex_Function.cpp
    pyibex_Ctc.cpp
    pyibex_Separators.cpp
    pyibex_Bsc.cpp
    pyibex_IntervalMatrix.cpp
)

include_directories( ${CMAKE_SOURCE_DIR}/src/pyibex_lib/paving)
include_directories( ${CMAKE_SOURCE_DIR}/src/pyibex_lib/separators)
include_directories( ${CMAKE_SOURCE_DIR}/src/pyibex_lib/geometry)

#####################################################################################
#### CORE MODULE
#####################################################################################

  pybind11_add_module(pyibex SHARED ${SRCS})
  target_link_libraries(pyibex PRIVATE Ibex::ibex pyibex_lib )

  add_custom_command(TARGET pyibex POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:pyibex>" "${PYTHON_PACKAGE_DIR}/pyibex"
  )

#####################################################################################
#### GEOMETRY MODULE
#####################################################################################


PYBIND11_ADD_MODULE(geometry SHARED
    pyibex_geometry.cpp
)
target_link_libraries(geometry PRIVATE pyibex_lib Ibex::ibex)
add_custom_command(TARGET geometry POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:geometry>" "${PYTHON_PACKAGE_DIR}/pyibex/geometry"
)


#####################################################################################
#### PAVING MODULE
#####################################################################################

PYBIND11_ADD_MODULE(paving SHARED pyibex_paving.cpp)
target_link_libraries(paving PRIVATE pyibex_lib Ibex::ibex)

add_custom_command(TARGET paving POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:paving>" "${PYTHON_PACKAGE_DIR}/pyibex"
)
