# ==================================================================
#  Codac - cmake configuration file
# ==================================================================

  list(APPEND CODAC_CORE_SRC

    ${CMAKE_CURRENT_SOURCE_DIR}/3rd/codac2_eigen.h
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd/codac2_ibex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd/codac2_ibex.h

    ${CMAKE_CURRENT_SOURCE_DIR}/actions/codac2_OctaSym.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/actions/codac2_OctaSym.h

    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_Ctc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcAction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcAction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcCartProd.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcEllipse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcEllipse.h
    #${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcCenteredForm.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcInter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcInverse.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcInverseNotIn.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcLazy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcLazy.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcNot.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcUnion.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcPolar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcPolar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcSegment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcSegment.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcWrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_DirectedCtc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_DirectedCtc.h

    ${CMAKE_CURRENT_SOURCE_DIR}/domains/codac2_BoolInterval.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/codac2_Domain.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_Interval.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_Interval.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_Interval_operations.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_Interval_operations.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_IntervalMatrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_IntervalMatrixBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_IntervalVector.h

    ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac2_ExprBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac2_ExprBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac2_FunctionArgsList.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac2_FunctionBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac2_FunctionBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac2_VarBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_analytic_constants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_analytic_operations.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_analytic_values.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_analytic_variables.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_AnalyticExpr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_AnalyticFunction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_set_operations.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_set_operators.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_set_variables.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_SetExpr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_SetFunction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_set_constants.h

    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_geometry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_geometry.h

    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac2_Figure2D.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac2_Figure2D.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac2_Figure2DInterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac2_OutputFigure2D.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac2_OutputFigure2D.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac2_StyleProperties.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac2_StyleProperties.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/ipe/codac2_Figure2D_IPE.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/ipe/codac2_Figure2D_IPE.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/vibes/codac2_Figure2D_VIBes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/vibes/codac2_Figure2D_VIBes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/vibes/vibes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/vibes/vibes.h

    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_GaussJordan.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_GaussJordan.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_Matrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_MatrixBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_Vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_VectorBase.h

    ${CMAKE_CURRENT_SOURCE_DIR}/paver/codac2_Paver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/paver/codac2_Paver.h

    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_Sep.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepAction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepAction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCartProd.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCartProd.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCtc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCtcBoundary.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCtcBoundary.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCtcPair.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCtcPair.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepEllipse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepEllipse.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepInter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepInter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepInverse.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepNot.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepPolygon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepPolygon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepProj.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepProj.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepUnion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepUnion.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepWrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepWrapper.h

    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_Approx.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_assert.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_Collection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_math.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_object_file_format.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_object_file_format.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_template_tools.h
  )

################################################################################
# Create the target for libcodac-core
################################################################################

  #if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
  #endif()

  add_library(${PROJECT_NAME}-core ${CODAC_CORE_SRC})
  target_include_directories(${PROJECT_NAME}-core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd
    ${CMAKE_CURRENT_SOURCE_DIR}/actions
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors
    ${CMAKE_CURRENT_SOURCE_DIR}/domains
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval
    ${CMAKE_CURRENT_SOURCE_DIR}/functions
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/ipe
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/vibes
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices
    ${CMAKE_CURRENT_SOURCE_DIR}/paver
    ${CMAKE_CURRENT_SOURCE_DIR}/separators
    ${CMAKE_CURRENT_SOURCE_DIR}/tools
  )
  target_link_libraries(${PROJECT_NAME}-core PUBLIC Ibex::ibex)
  
################################################################################
# Installation of libcodac-core files
################################################################################
  
# Getting header files from sources

  foreach(srcfile ${CODAC_CORE_SRC})
    if(srcfile MATCHES "\\.h$" OR srcfile MATCHES "\\.hpp$")
      list(APPEND CODAC_CORE_HDR ${srcfile})
      file(COPY ${srcfile} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../include)
    endif()
  endforeach()

# Generating the file codac-core.h

  set(CODAC_CORE_MAIN_HEADER ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-core.h)
  file(WRITE ${CODAC_CORE_MAIN_HEADER} "/* This file is generated by CMake */\n\n")
  file(APPEND ${CODAC_CORE_MAIN_HEADER} "#ifndef __CODAC_CORE_H__\n#define __CODAC_CORE_H__\n\n")
  foreach(header_path ${CODAC_CORE_HDR})
    get_filename_component(header_name ${header_path} NAME)
    file(APPEND ${CODAC_CORE_MAIN_HEADER} "#include <${header_name}>\n")
  endforeach()
  file(APPEND ${CODAC_CORE_MAIN_HEADER} "\n#endif /* __CODAC_CORE_H__ */\n")
  file(COPY ${CODAC_CORE_MAIN_HEADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../include)

# Install files in system directories

  install(TARGETS ${PROJECT_NAME}-core DESTINATION ${CMAKE_INSTALL_LIBDIR})
  install(FILES ${CODAC_CORE_HDR} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-core)
  install(FILES ${CODAC_CORE_MAIN_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-core)