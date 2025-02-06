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

namespace codac2
{
  template<typename T>
  struct ValueType
  { };

  template<>
  struct ValueType<int> {
    using Type = ScalarType;
  };

  template<>
  struct ValueType<Index> {
    using Type = ScalarType;
  };

  template<>
  struct ValueType<double> {
    using Type = ScalarType;
  };

  template<>
  struct ValueType<Interval> {
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
  struct ValueType<Matrix> {
    using Type = MatrixType;
  };

  template<>
  struct ValueType<IntervalMatrix> {
    using Type = MatrixType;
  };
}