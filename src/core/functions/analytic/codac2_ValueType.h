/** 
 *  \file codac2_ValueType.h
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
  struct ValueType
  { };

  template<typename T>
    requires (std::is_arithmetic_v<T>)
  struct ValueType<T> {
    using Type = ScalarType;
  };
  
  template<>
  struct ValueType<Interval> {
    using Type = ScalarType;
  };
  
  template<>
  struct ValueType<ScalarVar> {
    using Type = ScalarType;
  };

  template<>
  struct ValueType<Vector> {
    using Type = VectorType;
  };

  template<>
  struct ValueType<IntervalVector> {
    using Type = VectorType;
  };

  template<>
  struct ValueType<VectorVar> {
    using Type = VectorType;
  };

  template<>
  struct ValueType<Matrix> {
    using Type = MatrixType;
  };

  template<>
  struct ValueType<IntervalMatrix> {
    using Type = MatrixType;
  };

  template<>
  struct ValueType<MatrixVar> {
    using Type = MatrixType;
  };

  template<typename T>
    requires (T::RowsAtCompileTime!=1 && T::ColsAtCompileTime==1)
  struct ValueType<T> {
    using Type = VectorType;
  };

  template<typename T>
    requires (T::RowsAtCompileTime!=1 && T::ColsAtCompileTime!=1)
  struct ValueType<T> {
    using Type = MatrixType;
  };
  
  template<typename T>
  struct ValueType<AnalyticExprWrapper<T>> {
    using Type = T;
  };
}