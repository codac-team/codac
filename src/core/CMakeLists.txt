# ==================================================================
#  Codac - cmake configuration file
# ==================================================================

  list(APPEND CODAC_CORE_SRC

    ${CMAKE_CURRENT_SOURCE_DIR}/actions/codac2_OctaSym.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/actions/codac2_OctaSym.h
    ${CMAKE_CURRENT_SOURCE_DIR}/actions/codac2_OctaSym_operator.h

    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_Ctc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcAction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcAction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcCartProd.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcConstell.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcConstell.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcCross.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcCross.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcCtcBoundary.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcCtcBoundary.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcDeriv.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcDeriv.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcDist.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcDist.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcEllipse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcEllipse.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcEmpty.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcEmpty.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcEval.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcFixpoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcFixpoint.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcIdentity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcIdentity.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcInnerOuter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcInter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcInverse.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcInverseNotIn.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcLazy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcLazy.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcLohner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcLohner.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcNot.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcUnion.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcVisible.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcVisible.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcPointCloud.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcPointCloud.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcPolar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcPolar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcPolygon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcPolygon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcProj.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcProj.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcQInter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcQInter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcSegment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcSegment.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_CtcWrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_linear_ctc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors/codac2_linear_ctc.h

    ${CMAKE_CURRENT_SOURCE_DIR}/domains/codac2_BoolInterval.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/codac2_Domain.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/codac2_Scalar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/codac2_Wrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/ellipsoid/codac2_Ellipsoid.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/ellipsoid/codac2_Ellipsoid_checks.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/ellipsoid/codac2_Ellipsoid_mapping.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/ellipsoid/codac2_Ellipsoid.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/ellipsoid/codac2_Ellipsoid_utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/ellipsoid/codac2_Ellipsoid_utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_Interval.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_Interval_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_Interval_operations.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_Interval_operations_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_IntervalMatrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_IntervalRow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/codac2_IntervalVector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/paving/codac2_Paving.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/paving/codac2_Paving.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/paving/codac2_PavingNode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/paving/codac2_Subpaving.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/zonotope/codac2_Parallelepiped.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/zonotope/codac2_Parallelepiped.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/zonotope/codac2_Zonotope.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/zonotope/codac2_Zonotope.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_Slice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_SliceBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_SliceBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_SlicedTube.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_SlicedTube_integral_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_SlicedTube_operations.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_SlicedTube_operations.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_SlicedTubeBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_TDomain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_TDomain.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_TimePropag.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_TSlice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_TSlice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_TubeBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_Tube_operator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube/codac2_tube_cart_prod.h

    ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac2_ExprBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac2_ExprBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac2_FunctionArgsList.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac2_FunctionBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/codac2_VarBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_analytic_componentwise.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_analytic_componentwise.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_analytic_constants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_analytic_constants_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_analytic_flat_input_layout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_analytic_flat_input_layout.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_analytic_variables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_analytic_variables.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_AnalyticExpr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_AnalyticExprWrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_AnalyticFunction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_AnalyticType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic/codac2_ExprType.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_set_operations.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_set_operators.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_set_variables.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_SetExpr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_SetFunction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set/codac2_set_constants.h

    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_ConvexPolygon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_ConvexPolygon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_geometry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_geometry.h
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_Polygon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_Polygon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_Segment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry/codac2_Segment.h
    
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_abs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_acos.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_arith_add.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_arith_div.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_arith_mul.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_arith_mul.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_arith_sub.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_asin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_atan.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_atan2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_ceil.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_chi.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_component.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_cos.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_cosh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_cross_prod.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_det.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_exp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_extend.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_flatten.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_floor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_log.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_mat.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_max.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_min.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_mod.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_operators.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_pow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_sign.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_sin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_sinh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_sqr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_sqrt.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_subvector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_tan.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_tanh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_transpose.h
    ${CMAKE_CURRENT_SOURCE_DIR}/operators/codac2_vec.h

    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_Base.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_include.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_IntervalMatrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_IntervalMatrixBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_IntervalVector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_Matrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_MatrixBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_Vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons/codac2_Matrix_addons_VectorBase.h

    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_Base.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_include.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_IntervalMatrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_IntervalMatrixBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_IntervalVector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_Matrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_MatrixBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_Vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons/codac2_MatrixBase_addons_VectorBase.h
    
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_cart_prod.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_GaussJordan.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_GaussJordan.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_hull.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_inversion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_inversion.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_matrices.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_Matrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_Row.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_Vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_IntvFullPivLU.h
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/codac2_IntvFullPivLU.cpp


    ${CMAKE_CURRENT_SOURCE_DIR}/paver/codac2_pave.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/paver/codac2_pave.h

    ${CMAKE_CURRENT_SOURCE_DIR}/peibos/codac2_peibos_tools.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/peibos/codac2_peibos_tools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/peibos/codac2_peibos.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/peibos/codac2_peibos.h

    ${CMAKE_CURRENT_SOURCE_DIR}/proj/codac2_ProjBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/proj/codac2_ProjBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/proj/codac2_qinter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/proj/codac2_qinter.h

    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_Sep.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepAction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepAction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCartPolar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCartPolar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCartProd.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCartProd.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepChi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepChi.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepCross.h
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
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepPolarCart.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepPolarCart.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepPolygon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepPolygon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepProj.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepProj.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepQInter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepQInter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepTransform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepTransform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepUnion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepUnion.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepVisible.h
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepWrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/separators/codac2_SepWrapper.h

    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_Approx.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_assert.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_Collection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_fixpoint.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_Index.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_math.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_object_file_format.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_object_file_format.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_RobotSimulator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_RobotSimulator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_serialization.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_template_tools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_threading.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_threading.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_transformations.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_transformations.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_trunc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_trunc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/codac2_TypeInfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/ibex/codac2_ibex.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/ibex/codac2_ibex_impl.h

    ${CMAKE_CURRENT_SOURCE_DIR}/trajectory/codac2_AnalyticTraj.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trajectory/codac2_SampledTraj.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trajectory/codac2_SampledTraj_operations.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/trajectory/codac2_SampledTraj_operations.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trajectory/codac2_traj_cart_prod.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trajectory/codac2_Traj_operator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trajectory/codac2_TrajBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trajectory/codac2_TrajBase_impl.h
  )

################################################################################
# Create the target for libcodac-core
################################################################################

  add_library(${PROJECT_NAME}-core ${CODAC_CORE_SRC})

  set_target_properties(${PROJECT_NAME}-core PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
  )

  # The directories holding the public core headers, in their canonical
  # location under src/core. Published as a cache variable so that the tests,
  # the examples and the Python bindings all reach the headers by the same
  # path as codac-core itself, instead of each keeping its own copy of this
  # list (which had already drifted).
  #
  # The path a translation unit used is the path gcov records, so a header
  # reached both through src/ and through a copy in the build tree ends up as
  # two unrelated entries in the coverage report, each showing only the part
  # its own callers exercised, and is reported well below its real coverage.
  # Consumers therefore list these directories first; the build tree stays on
  # the path for the generated umbrella headers (codac-core.h and the rest).
  set(CODAC_CORE_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/actions
    ${CMAKE_CURRENT_SOURCE_DIR}/contractors
    ${CMAKE_CURRENT_SOURCE_DIR}/domains
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/ellipsoid
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/interval/eigen
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/paving
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/zonotope
    ${CMAKE_CURRENT_SOURCE_DIR}/domains/tube
    ${CMAKE_CURRENT_SOURCE_DIR}/functions
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/analytic
    ${CMAKE_CURRENT_SOURCE_DIR}/functions/set
    ${CMAKE_CURRENT_SOURCE_DIR}/geometry
    #${CMAKE_CURRENT_SOURCE_DIR}/graphics
    #${CMAKE_CURRENT_SOURCE_DIR}/graphics/ipe
    #${CMAKE_CURRENT_SOURCE_DIR}/graphics/vibes
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/Matrix_addons
    ${CMAKE_CURRENT_SOURCE_DIR}/matrices/eigen/MatrixBase_addons
    ${CMAKE_CURRENT_SOURCE_DIR}/operators
    ${CMAKE_CURRENT_SOURCE_DIR}/paver
    ${CMAKE_CURRENT_SOURCE_DIR}/peibos
    ${CMAKE_CURRENT_SOURCE_DIR}/proj
    ${CMAKE_CURRENT_SOURCE_DIR}/separators
    ${CMAKE_CURRENT_SOURCE_DIR}/tools
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/ibex
    ${CMAKE_CURRENT_SOURCE_DIR}/trajectory
    CACHE INTERNAL "Directories holding the public headers of codac-core")

  # The source directories describe the build tree only: they are gone once
  # Codac is installed, and install(EXPORT) below refuses to export a target
  # whose interface names a path inside the source tree. The install tree
  # answers with the directories the headers were copied into -- include/
  # itself, through which the <codac> umbrella header is reached, and
  # include/eigen3, where Eigen's headers land.
  #
  # Eigen is a build-tree dependency for the same reason: it is fetched by this
  # build rather than found, so Eigen3::Eigen is a target of ours that no
  # consumer's find_package() could produce. Downstream it is reached as the
  # plain include directory above, which is all a header-only library needs.
  target_include_directories(${PROJECT_NAME}-core PUBLIC
    "$<BUILD_INTERFACE:${CODAC_CORE_INCLUDE_DIRS}>"
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-core>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/eigen3>
  )
  codac_publish_include_dirs(${CODAC_CORE_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME}-core PUBLIC Ibex::ibex $<BUILD_INTERFACE:Eigen3::Eigen> Threads::Threads)
  

################################################################################
# For the generation of the PKG file
################################################################################
  
  set(CODAC_PKG_CONFIG_CFLAGS "${CODAC_PKG_CONFIG_CFLAGS} -I\${includedir}/${PROJECT_NAME}-core" PARENT_SCOPE)
  set(CODAC_PKG_CONFIG_MODULE_LIBS "${CODAC_PKG_CONFIG_MODULE_LIBS} -l${PROJECT_NAME}-core" PARENT_SCOPE)


################################################################################
# Installation / build include tree
################################################################################

  foreach(srcfile ${CODAC_CORE_SRC})
    if(srcfile MATCHES "\\.h$" OR srcfile MATCHES "\\.hpp$")
      list(APPEND CODAC_CORE_HDR ${srcfile})
    endif()
  endforeach()


# Generating the file codac-core.h
# ================================

  set(CODAC_CORE_MAIN_HEADER ${CMAKE_CURRENT_BINARY_DIR}/codac-core.h)
  codac_publish_generated_include_dirs(${CMAKE_CURRENT_BINARY_DIR})
  set(CODAC_MAIN_SUBHEADERS ${CODAC_MAIN_SUBHEADERS} "codac-core.h" PARENT_SCOPE)
  
  # Generate the content of the umbrella header
  set(CODAC_CORE_HEADER_CONTENT "/* This file is generated by CMake */\n\n#pragma once\n\n")
  foreach(header_path ${CODAC_CORE_HDR})
    get_filename_component(header_name ${header_path} NAME)
    if((NOT header_name MATCHES "^.*_addons.*$") AND (NOT header_name MATCHES "^.*_impl.*$"))
      string(APPEND CODAC_CORE_HEADER_CONTENT "#include <${header_name}>\n")
    endif()
  endforeach()
  
  # Compare with the existing file, if any
  if(EXISTS "${CODAC_CORE_MAIN_HEADER}")
    file(READ "${CODAC_CORE_MAIN_HEADER}" EXISTING_CONTENT)
    string(MD5 EXISTING_HASH "${EXISTING_CONTENT}")
    string(MD5 NEW_HASH "${CODAC_CORE_HEADER_CONTENT}")
  else()
    set(EXISTING_HASH "")
    set(NEW_HASH "new")
  endif()
  
  # Only rewrite it when it changed
  if(NOT EXISTING_HASH STREQUAL NEW_HASH)
    file(WRITE "${CODAC_CORE_MAIN_HEADER}" "${CODAC_CORE_HEADER_CONTENT}")
  endif()


# Install files in system directories
# ====================================

  install(TARGETS ${PROJECT_NAME}-core
          EXPORT ${PROJECT_NAME}-targets
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
  install(FILES ${CODAC_CORE_HDR} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-core)
  install(FILES ${CODAC_CORE_MAIN_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-core)
