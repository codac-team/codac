# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

  list(APPEND SRC ${CMAKE_CURRENT_SOURCE_DIR}/functions/tubex_TFnc.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/functions/tubex_TFnc.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/functions/tubex_TFunction.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/functions/tubex_TFunction.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/functions/tubex_DelayTFunction.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/functions/tubex_DelayTFunction.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/tubex_polygon_arithmetic.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/tubex_polygon_arithmetic.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/tubex_predef_values.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/tubex_tube_arithmetic.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/tubex_tube_arithmetic_scalar.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/tubex_tube_arithmetic_vector.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/tubex_traj_arithmetic.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/tubex_traj_arithmetic_scalar.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic/tubex_traj_arithmetic_vector.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/tubex_Figure.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/tubex_Figure.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/vibes/vibes.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/vibes/vibes.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/tubex_VIBesFigTube.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/tubex_VIBesFigTube.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/tubex_ColorMap.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/tubex_ColorMap.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/tubex_VIBesFigTubeVector.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/tubex_VIBesFigTubeVector.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/tubex_colors.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/tubex_colors.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/tubex_VIBesFig.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/tubex_VIBesFig.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/tubex_VIBesFigPaving.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/graphics/tubex_VIBesFigPaving.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/tubex_ConnectedSubset.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/tubex_ConnectedSubset.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/tubex_Paving.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/tubex_Paving.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/tubex_Set.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/tubex_Set.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/tubex_TubePaving.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/tubex_TubePaving.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/tubex_ConnectedSubset_degree.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/tubex_SIVIAPaving.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/paving/tubex_SIVIAPaving.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/tubex_Polygon.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/tubex_Polygon.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/tubex_ConvexPolygon.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/tubex_ConvexPolygon.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/tubex_Edge.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/tubex_Edge.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/tubex_GrahamScan.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/tubex_GrahamScan.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/tubex_Point.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/geometry/tubex_Point.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/tubex_DynamicalItem.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/tubex_DynamicalItem.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/tube/tubex_TubeVector.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/tube/tubex_TubeVector.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/tube/tubex_TubeVector_operators.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/tube/tubex_Tube.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/tube/tubex_Tube.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/tube/tubex_Tube_operators.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/tube/tubex_TubeTreeSynthesis.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/tube/tubex_TubeTreeSynthesis.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/slice/tubex_Slice.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/slice/tubex_Slice.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/slice/tubex_Slice_polygon.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/slice/tubex_Slice_operators.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/trajectory/tubex_RandTrajectory.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/trajectory/tubex_RandTrajectory.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/trajectory/tubex_Trajectory.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/trajectory/tubex_Trajectory.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/trajectory/tubex_Trajectory_operators.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/trajectory/tubex_TrajectoryVector.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/trajectory/tubex_TrajectoryVector.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/trajectory/tubex_TrajectoryVector_operators.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/exceptions/tubex_Exception.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/exceptions/tubex_Exception.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/serialize/tubex_serialize_trajectories.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/serialize/tubex_serialize_trajectories.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/serialize/tubex_serialize_tubes.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/serialize/tubex_serialize_tubes.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/serialize/tubex_serialize_intervals.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/serialize/tubex_serialize_intervals.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/tubex_CtcDist.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/tubex_CtcDist.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/tubex_CtcFunction.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static/tubex_CtcFunction.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/tubex_DynCtc.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/tubex_DynCtc.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/tubex_CtcPicard.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/tubex_CtcPicard.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/tubex_CtcDelay.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/tubex_CtcDelay.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/tubex_CtcStatic.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/tubex_CtcStatic.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/tubex_CtcEval.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/tubex_CtcEval.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/tubex_CtcDeriv.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/tubex_CtcDeriv.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/tubex_CtcLinobs.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn/tubex_CtcLinobs.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/tubex_predef_contractors.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/contractors/tubex_predef_contractors.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/tubex_Domain.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/tubex_Domain.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/tubex_Contractor.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/tubex_Contractor.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/tubex_ContractorNetwork.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/tubex_ContractorNetwork_solve.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/tubex_ContractorNetwork_visu.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/cn/tubex_ContractorNetwork.h
                  ${CMAKE_CURRENT_SOURCE_DIR}/tools/tubex_Tools.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/tools/tubex_Tools.h
                  )


################################################################################
# Create the target for libtubex
################################################################################

  add_library(tubex ${SRC})
  target_include_directories(tubex PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/functions
                                          ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic
                                          ${CMAKE_CURRENT_SOURCE_DIR}/graphics
                                          ${CMAKE_CURRENT_SOURCE_DIR}/graphics/vibes
                                          ${CMAKE_CURRENT_SOURCE_DIR}/paving
                                          ${CMAKE_CURRENT_SOURCE_DIR}/geometry
                                          ${CMAKE_CURRENT_SOURCE_DIR}/dynamics
                                          ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/tube
                                          ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/slice
                                          ${CMAKE_CURRENT_SOURCE_DIR}/dynamics/trajectory
                                          ${CMAKE_CURRENT_SOURCE_DIR}/exceptions
                                          ${CMAKE_CURRENT_SOURCE_DIR}/serialize
                                          ${CMAKE_CURRENT_SOURCE_DIR}/contractors
                                          ${CMAKE_CURRENT_SOURCE_DIR}/contractors/static
                                          ${CMAKE_CURRENT_SOURCE_DIR}/contractors/dyn
                                          ${CMAKE_CURRENT_SOURCE_DIR}/cn
                                          ${CMAKE_CURRENT_SOURCE_DIR}/tools)
  target_link_libraries(tubex PUBLIC Ibex::ibex)


################################################################################
# Installation of libtubex files
################################################################################
  
# Getting header files from sources

  foreach(srcfile ${SRC})
    if(srcfile MATCHES "\\.h$" OR srcfile MATCHES "\\.hpp$")
      list(APPEND HDR ${srcfile})
      # Copying header files for other Tubex libraries compiled in the same time
      file(COPY ${srcfile} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../include)
    endif()
  endforeach()

# Generating a tubex.h file

  set(TUBEX_MAIN_HEADER ${CMAKE_CURRENT_BINARY_DIR}/tubex.h)
  file(WRITE ${TUBEX_MAIN_HEADER} "/* This file is generated by CMake */\n\n")
  file(APPEND ${TUBEX_MAIN_HEADER} "#ifndef __TUBEX_H__\n#define __TUBEX_H__\n\n")
    # Simplifying access to frequently used items of IBEX 
      file(APPEND ${TUBEX_MAIN_HEADER} "#include <ibex_Interval.h>\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "#include <ibex_IntervalVector.h>\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "#include <ibex_Vector.h>\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "#include <ibex_Matrix.h>\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "#include <ibex_IntervalMatrix.h>\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "#include <ibex_BoolInterval.h>\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "#include <ibex_Ctc.h>\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "#include <ibex_Function.h>\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "namespace tubex\n{\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "  using ibex::Interval;\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "  using ibex::IntervalVector;\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "  using ibex::Vector;\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "  using ibex::Matrix;\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "  using ibex::IntervalMatrix;\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "  using ibex::BoolInterval;\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "  using ibex::Ctc;\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "  using ibex::Function;\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "}\n\n")
      file(APPEND ${TUBEX_MAIN_HEADER} "#include <tubex-rob.h>\n") # todo: clean this organization
      file(APPEND ${TUBEX_MAIN_HEADER} "#include <tubex-pyibex.h>\n") # todo: clean this organization
  foreach(header_path ${HDR})
    get_filename_component(header_name ${header_path} NAME)
    file(APPEND ${TUBEX_MAIN_HEADER} "#include <${header_name}>\n")
  endforeach()
  file(APPEND ${TUBEX_MAIN_HEADER} "\n#endif /* __TUBEX_H__ */\n")
  file(COPY ${TUBEX_MAIN_HEADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../include)

# Install files in system directories

  install(TARGETS tubex DESTINATION ${CMAKE_INSTALL_LIBDIR})
  install(FILES ${HDR} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tubex)
  install(FILES ${TUBEX_MAIN_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tubex)