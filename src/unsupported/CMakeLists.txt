# ==================================================================
#  codac-unsupported - cmake configuration file
# ==================================================================

  list(APPEND SRC 

    # Thickets
    #${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_exploredAreaTools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_GeoMapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_PNode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_PNode_impl.hpp
    #${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_sweepTest.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_sweepTest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_ThickBoolean.h
    #${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_ThickBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_ThickBox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_ThickBox_impl.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_ThickFunction.h
    #${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_ThickInterval.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_ThickInterval.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_ThickPaving.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_ThickPaving.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_ThickTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thickset/codac_ThickTest.h

    # Separators
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_CtcHull.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_CtcHull.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_CtcTransform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_CtcTransform.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_QInterProjF.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_QInterProjF.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepCtcPairProj.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepCtcPairProj.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepFixPoint.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepFixPoint.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepProj.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepProj.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepTransform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepTransform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepUnionBbox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepUnionBbox.h

    # Pavings
    ${CMAKE_CURRENT_SOURCE_DIR}/paving/codac_PavingVisitor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/paving/codac_PSetNode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/paving/codac_PSetNode_impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/paving/codac_SepPaving.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/paving/codac_SepPaving.h

    # Geometry
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_SepDiskExists.h
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_SepDiskExists.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_SepPolarXY.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_SepPolarXY.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_CtcSegment.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_CtcSegment.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_SepPolygon.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_PdcInPolygon.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_PdcInPolygon.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_SepPolygon.cpp
    )

################################################################################
# Create the target for libcodac-unsupported
################################################################################

  add_library(codac-unsupported ${SRC})
  target_compile_options(codac-unsupported PUBLIC -w)

  # todo: find a clean way to access codac header files?
  set(CODAC_HEADERS_DIR ${CMAKE_CURRENT_BINARY_DIR}/../../include)
  target_include_directories(codac-unsupported PUBLIC ${CODAC_HEADERS_DIR}
                                              ${CMAKE_CURRENT_SOURCE_DIR}/thickset
                                              ${CMAKE_CURRENT_SOURCE_DIR}/separators
                                              ${CMAKE_CURRENT_SOURCE_DIR}/paving
                                              ${CMAKE_CURRENT_SOURCE_DIR}/geometry
                                              )
  target_link_libraries(codac-unsupported PUBLIC Ibex::ibex codac)


################################################################################
# Installation of libcodac-unsupported files
################################################################################

# Getting header files from sources

  foreach(srcfile ${SRC})
    if(srcfile MATCHES "\\.h$" OR srcfile MATCHES "\\.hpp$")
      list(APPEND HDR ${srcfile})
      # Copying header files for other Codac libraries compiled in the same time
      file(COPY ${srcfile} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../include)
    endif()
  endforeach()

# Generating a codac-unsupported.h file

  set(CODAC_UNSUPPORTED_MAIN_HEADER ${CMAKE_CURRENT_BINARY_DIR}/codac-unsupported.h)
  file(WRITE ${CODAC_UNSUPPORTED_MAIN_HEADER} "/* This file is generated by CMake */\n\n")
  file(APPEND ${CODAC_UNSUPPORTED_MAIN_HEADER} "#ifndef __CODAC_UNSUPPORTED_H__\n#define __CODAC_UNSUPPORTED_H__\n\n")
  foreach(header_path ${HDR})
    if(NOT header_path MATCHES "_impl.hpp$")
      get_filename_component(header_name ${header_path} NAME)
      file(APPEND ${CODAC_UNSUPPORTED_MAIN_HEADER} "#include <${header_name}>\n")
    endif()
  endforeach()
  file(APPEND ${CODAC_UNSUPPORTED_MAIN_HEADER} "\n#endif /* __CODAC_UNSUPPORTED_H__ */\n")
  file(COPY ${CODAC_UNSUPPORTED_MAIN_HEADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../include)

# Install files in system directories

  install(TARGETS codac-unsupported DESTINATION ${CMAKE_INSTALL_LIBDIR})
  install(FILES ${HDR} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/codac-unsupported)
  install(FILES ${CODAC_UNSUPPORTED_MAIN_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/codac-unsupported)