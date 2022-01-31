set( SRCS
      # Polar sources
      pyibex_CtcPolar.cpp
      pyibex_SepPolarXY.cpp
      # Polygon sources
      pyibex_CtcSegment.cpp
      pyibex_PdcInPolygon.cpp
      pyibex_SepPolygon.cpp
      pyibex_SepDiskExists.cpp
  )

set( HEADERS
      #Polar headers
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_CtcPolar.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_SepPolarXY.h
      #Polygon headers
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_CtcSegment.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_PdcInPolygon.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_SepPolygon.h
      ${CMAKE_CURRENT_SOURCE_DIR}/pyibex_SepDiskExists.h
  )


add_library(ibex-geometry ${SRCS} ${HEADERS})
target_link_libraries(ibex-geometry Ibex::ibex)

PYBIND11_ADD_MODULE(geometry SHARED
    pyibex_geometry.cpp
)
target_link_libraries(geometry PRIVATE Ibex::ibex ibex-geometry)
add_custom_command(TARGET geometry POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:geometry>" "${PYTHON_PACKAGE_DIR}/pyibex/geometry"
)
