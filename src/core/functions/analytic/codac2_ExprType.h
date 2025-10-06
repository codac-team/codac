/** 
 *  \file codac2_ExprType.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_Vector.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_AnalyticType.h"
#include "codac2_analytic_variables.h"

namespace codac2
{
  template<typename T>
  struct ExprType
  { };

  template<typename T>
    requires (std::is_arithmetic_v<T>)
  struct ExprType<T> {
    using Type = ScalarType;
  };
  
  template<>
  struct ExprType<Interval> {
    using Type = ScalarType;
  };
  
  template<>
  struct ExprType<ScalarVar> {
    using Type = ScalarType;
  };

  template<>
  struct ExprType<Vector> {
    using Type = VectorType;
  };

  template<>
  struct ExprType<IntervalVector> {
    using Type = VectorType;
  };

  template<>
  struct ExprType<VectorVar> {
    using Type = VectorType;
  };

  template<>
  struct ExprType<Matrix> {
    using Type = MatrixType;
  };

  template<>
  struct ExprType<IntervalMatrix> {
    using Type = MatrixType;
  };

  template<>
  struct ExprType<MatrixVar> {
    using Type = MatrixType;
  };

  template<typename T>
    requires (T::RowsAtCompileTime!=1 && T::ColsAtCompileTime==1)
  struct ExprType<T> {
    using Type = VectorType;
  };

  template<typename T>
    requires (T::RowsAtCompileTime!=1 && T::ColsAtCompileTime!=1)
  struct ExprType<T> {
    using Type = MatrixType;
  };
  
  template<typename T>
  struct ExprType<AnalyticExprWrapper<T>> {
    using Type = T;
  };

  template<typename T>
  class SlicedTube;
  
  template<typename T>
  struct ExprType<SlicedTube<T>> {
    using Type = typename ExprType<T>::Type;
  };
}