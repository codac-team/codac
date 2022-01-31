
set( SRCS
      ${CMAKE_CURRENT_SOURCE_DIR}/separators/pyibex_SepProj.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/separators/pyibex_SepFixPoint.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/separators/pyibex_SepCtcPairProj.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/separators/pyibex_SepTransform.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/separators/pyibex_CtcTransform.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/separators/pyibex_QInterProjF.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/separators/pyibex_SepUnionBbox.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/separators/pyibex_CtcHull.cpp
     
      ${CMAKE_CURRENT_SOURCE_DIR}/paving/pyibex_SepPaving.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/paving/pyibex_PSetNode_impl.cpp
      # ${CMAKE_CURRENT_SOURCE_DIR}/paving/pyibex_pyibex_paving.cpp

            # Polar sources
      ${CMAKE_CURRENT_SOURCE_DIR}/geometry/pyibex_CtcPolar.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/geometry/pyibex_SepPolarXY.cpp
      # Polygon sources
      ${CMAKE_CURRENT_SOURCE_DIR}/geometry/pyibex_CtcSegment.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/geometry/pyibex_PdcInPolygon.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/geometry/pyibex_SepPolygon.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/geometry/pyibex_SepDiskExists.cpp
      # vibes.cpp
    )
    
set( HEADERS
      ${CMAKE_CURRENT_SOURCE_DIR}/separators/pyibex_SepProj.h
      ${CMAKE_CURRENT_SOURCE_DIR}/separators/pyibex_SepFixPoint.h
      ${CMAKE_CURRENT_SOURCE_DIR}/separators/pyibex_SepCtcPairProj.h
      ${CMAKE_CURRENT_SOURCE_DIR}/separators/pyibex_SepUnionBbox.h
      ${CMAKE_CURRENT_SOURCE_DIR}/separators/pyibex_SepTransform.h
      ${CMAKE_CURRENT_SOURCE_DIR}/separators/pyibex_CtcTransform.h
      ${CMAKE_CURRENT_SOURCE_DIR}/separators/pyibex_QInterProjF.h
      ${CMAKE_CURRENT_SOURCE_DIR}/separators/pyibex_CtcHull.h

      ${CMAKE_CURRENT_SOURCE_DIR}/paving/pyibex_PSetNode.h
      ${CMAKE_CURRENT_SOURCE_DIR}/paving/pyibex_SepPaving.h
      ${CMAKE_CURRENT_SOURCE_DIR}/paving/pyibex_PavingVisitor.h
      # ${CMAKE_CURRENT_SOURCE_DIR}/vibes.h
      #Polar headers
      ${CMAKE_CURRENT_SOURCE_DIR}/geometry/pyibex_CtcPolar.h
      ${CMAKE_CURRENT_SOURCE_DIR}/geometry/pyibex_SepPolarXY.h
      #Polygon headers
      ${CMAKE_CURRENT_SOURCE_DIR}/geometry/pyibex_CtcSegment.h
      ${CMAKE_CURRENT_SOURCE_DIR}/geometry/pyibex_PdcInPolygon.h
      ${CMAKE_CURRENT_SOURCE_DIR}/geometry/pyibex_SepPolygon.h
      ${CMAKE_CURRENT_SOURCE_DIR}/geometry/pyibex_SepDiskExists.h
		)

# add_subdirectory(polar)
# FILE(COPY ${HEADERS} DESTINATION ${HEADERS_DIR})

add_library(pyibex_lib ${SRCS} ${HEADERS})
target_include_directories(pyibex_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry
    ${CMAKE_CURRENT_SOURCE_DIR}/paving
    ${CMAKE_CURRENT_SOURCE_DIR}/pyibexlib
)

target_link_libraries(pyibex_lib Ibex::ibex)

# if(NOT MSVC)
  # Make sure C++11/14 are enabled
  # target_compile_options(pyibexlib PUBLIC ${PYBIND11_CPP_STANDARD})
# endif()

INSTALL( TARGETS pyibex_lib DESTINATION lib)
install(FILES ${HEADERS} DESTINATION "include/pyibex_lib")


# PYBIND11_ADD_MODULE(pyibex-geometry SHARED
    # pyIbex_geometry.cpp
#
# )
# target_link_libraries(pyibex-geometry ${LIBS} ibex-geometry)

# add_custom_command(TARGET pyibex-geometry POST_BUILD
	# COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:pyibex-geometry>" "${PYTHON_PACKAGE_DIR}/pyibex-geometry"
# )
