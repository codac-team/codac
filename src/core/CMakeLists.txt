# ==================================================================
#  Codac - cmake configuration file
# ==================================================================

  list(APPEND SRC ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac_Function.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac_TFnc.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac_TFnc.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac_TFunction.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac_TFunction.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac_DelayTFunction.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac_DelayTFunction.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/codac_polygon_arithmetic.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/codac_polygon_arithmetic.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/codac_predef_values.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/codac_tube_arithmetic.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/codac_tube_arithmetic_scalar.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/codac_tube_arithmetic_vector.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/codac_traj_arithmetic.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/codac_traj_arithmetic_scalar.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/codac_traj_arithmetic_vector.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/codac_matrix_arithmetic.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/codac_matrix_arithmetic.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac_Figure.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac_Figure.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/vibes/vibes.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/vibes/vibes.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac_VIBesFigTube.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac_VIBesFigTube.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac_ColorMap.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac_ColorMap.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac_VIBesFigTubeVector.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac_VIBesFigTubeVector.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac_colors.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac_colors.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac_VIBesFig.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac_VIBesFig.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac_VIBesFigPaving.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/codac_VIBesFigPaving.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/codac_ConnectedSubset.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/codac_ConnectedSubset.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/codac_Paving.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/codac_Paving.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/codac_Set.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/codac_Set.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/codac_TubePaving.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/codac_TubePaving.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/codac_ConnectedSubset_degree.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/codac_SIVIAPaving.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/codac_SIVIAPaving.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_Polygon.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_Polygon.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_ConvexPolygon.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_ConvexPolygon.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_SepPolarXY.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_SepPolarXY.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_ThickEdge.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_ThickEdge.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_GrahamScan.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_GrahamScan.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_ThickPoint.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_ThickPoint.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_PdcInPolygon.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_PdcInPolygon.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_SepPolygon.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac_SepPolygon.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac_Interval.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac_IntervalVector.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac_IntervalMatrix.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac_BoolInterval.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac_DynamicalItem.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac_DynamicalItem.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac_TubeVector.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac_TubeVector.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac_TubeVector_operators.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac_Tube.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac_Tube.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac_Tube_operators.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac_TubePolynomialSynthesis.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac_TubePolynomialSynthesis.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac_TubeSynthesis.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac_TubeTreeSynthesis.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac_TubeTreeSynthesis.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/slice/codac_Slice.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/slice/codac_Slice.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/slice/codac_Slice_polygon.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/domains/slice/codac_Slice_operators.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/variables/real/codac_Vector.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/variables/real/codac_Matrix.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/variables/trajectory/codac_RandTrajectory.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/variables/trajectory/codac_RandTrajectory.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/variables/trajectory/codac_Trajectory.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/variables/trajectory/codac_Trajectory.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/variables/trajectory/codac_Trajectory_operators.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/variables/trajectory/codac_TrajectoryVector.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/variables/trajectory/codac_TrajectoryVector.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/variables/trajectory/codac_TrajectoryVector_operators.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/exceptions/codac_Exception.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/exceptions/codac_Exception.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/exceptions/codac_DomainsTypeException.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/exceptions/codac_DomainsTypeException.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/exceptions/codac_DomainsSizeException.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/exceptions/codac_DomainsSizeException.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/serialize/codac_serialize_trajectories.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/serialize/codac_serialize_trajectories.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/serialize/codac_serialize_tubes.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/serialize/codac_serialize_tubes.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/serialize/codac_serialize_intervals.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/serialize/codac_serialize_intervals.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/codac_Ctc.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/codac_CtcBox.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/codac_CtcBox.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/codac_CtcCartProd.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/codac_CtcCartProd.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/codac_CtcCompo.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/codac_CtcDist.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/codac_CtcDist.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/codac_CtcFunction.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/codac_CtcFunction.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/codac_CtcPolar.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/codac_CtcPolar.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/codac_CtcUnion.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/codac_CtcSegment.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/codac_CtcSegment.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_DynCtc.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_DynCtc.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_CtcPicard.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_CtcPicard.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_CtcLohner.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_CtcLohner.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_CtcChain.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_CtcChain.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_CtcDelay.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_CtcDelay.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_CtcStatic.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_CtcStatic.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_CtcEval.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_CtcEval.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_CtcDeriv.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_CtcDeriv.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_CtcLinobs.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/codac_CtcLinobs.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac_predef_contractors.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac_predef_contractors.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/codac_Domain.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/codac_Domain.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/codac_Contractor.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/codac_Contractor.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/codac_ContractorNetwork.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/codac_ContractorNetwork_solve.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/codac_ContractorNetwork_visu.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/codac_ContractorNetwork.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/codac_Hashcode.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/codac_Hashcode.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/codac_Variable.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/codac_Variable.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepBox.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepBox.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepFunction.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepFunction.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_QInterProjF.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_QInterProjF.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepCtcPairProj.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepCtcPairProj.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepFixPoint.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepFixPoint.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepProj.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac_SepProj.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac_Tools.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac_Tools.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac_Eigen.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac_Eigen.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/sivia/codac_sivia.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/sivia/codac_sivia.h
                  )


################################################################################
# Create the target for libcodac
################################################################################

  add_library(codac ${SRC})
  target_include_directories(codac PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/functions
                                          ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic
                                          ${CMAKE_CURRENT_SOURCE_DIR}/graphics
                                          ${CMAKE_CURRENT_SOURCE_DIR}/graphics/vibes
                                          ${CMAKE_CURRENT_SOURCE_DIR}/paving
                                          ${CMAKE_CURRENT_SOURCE_DIR}/geometry
                                          ${CMAKE_CURRENT_SOURCE_DIR}/domains
                                          ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval
                                          ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube
                                          ${CMAKE_CURRENT_SOURCE_DIR}/domains/slice
                                          ${CMAKE_CURRENT_SOURCE_DIR}/variables/real
                                          ${CMAKE_CURRENT_SOURCE_DIR}/variables/trajectory
                                          ${CMAKE_CURRENT_SOURCE_DIR}/exceptions
                                          ${CMAKE_CURRENT_SOURCE_DIR}/serialize
                                          ${CMAKE_CURRENT_SOURCE_DIR}/contractors
                                          ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static
                                          ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn
                                          ${CMAKE_CURRENT_SOURCE_DIR}/cn
                                          ${CMAKE_CURRENT_SOURCE_DIR}/tools
                                          ${CMAKE_CURRENT_SOURCE_DIR}/sivia)
  target_link_libraries(codac PUBLIC Ibex::ibex)
  

################################################################################
# Installation of libcodac files
################################################################################
  
# Getting header files from sources

  foreach(srcfile ${SRC})
    if(srcfile MATCHES "\\.h$" OR srcfile MATCHES "\\.hpp$")
      list(APPEND HDR ${srcfile})
      # Copying header files for other Codac libraries compiled in the same time
      file(COPY ${srcfile} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../include)
    endif()
  endforeach()

# Generating a codac.h file

  set(CODAC_MAIN_HEADER ${CMAKE_CURRENT_BINARY_DIR}/codac.h)
  file(WRITE ${CODAC_MAIN_HEADER} "/* This file is generated by CMake */\n\n")
  file(APPEND ${CODAC_MAIN_HEADER} "#ifndef __CODAC_H__\n#define __CODAC_H__\n\n")
  file(APPEND ${CODAC_MAIN_HEADER} "\n#include <codac-rob.h>\n") # todo: clean this organization
  foreach(header_path ${HDR})
    get_filename_component(header_name ${header_path} NAME)
    file(APPEND ${CODAC_MAIN_HEADER} "#include <${header_name}>\n")
  endforeach()
  file(APPEND ${CODAC_MAIN_HEADER} "\n#endif /* __CODAC_H__ */\n")
  file(COPY ${CODAC_MAIN_HEADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../include)

# Install files in system directories

  install(TARGETS codac DESTINATION ${CMAKE_INSTALL_LIBDIR})
  install(FILES ${HDR} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/codac)
  install(FILES ${CODAC_MAIN_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/codac)