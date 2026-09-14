/** 
 *  \file codac2_Scalar.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_Vector.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"

namespace codac2
{
  template<typename T, typename Enable=void>
  struct Scalar
  { };

  template<>
  struct Scalar<Interval> {
    using Type = double;
  };

  template<typename OtherDerived>
  struct Scalar<OtherDerived,
    typename std::enable_if<std::is_base_of_v<Eigen::MatrixBase<OtherDerived>,OtherDerived>>::type>
  {
    using Type = Eigen::Matrix<double,OtherDerived::RowsAtCompileTime,OtherDerived::ColsAtCompileTime>;
    // Automatically sets:
    //   Scalar<IntervalVector>::Type = Vector
    //   Scalar<IntervalMatrix>::Type = Matrix
    //   + related possible expression templates
  };
}