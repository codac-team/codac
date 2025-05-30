/** 
 *  \file codac2_Wrapper.h
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

namespace codac2
{
  template<typename T, typename Enable=void>
  struct Wrapper
  { };

  template<>
  struct Wrapper<int> {
    using Domain = Interval;
  };

  template<>
  struct Wrapper<Index> {
    using Domain = Interval;
  };

  template<>
  struct Wrapper<double> {
    using Domain = Interval;
  };

  template<typename T,typename D>
  struct AnalyticType;

  template<typename T,typename D>
  struct Wrapper<AnalyticType<T,D>> {
    using Domain = D;
  };

  template<typename OtherDerived>
  struct Wrapper<OtherDerived,
    typename std::enable_if<std::is_base_of_v<Eigen::MatrixBase<OtherDerived>,OtherDerived>>::type>
  {
    using Domain = Eigen::Matrix<Interval,OtherDerived::RowsAtCompileTime,OtherDerived::ColsAtCompileTime>;
    // Automatically sets:
    //   Wrapper<Vector>::Domain = IntervalVector
    //   Wrapper<IntervalVector>::Domain = IntervalVector
    //   Wrapper<Matrix>::Domain = IntervalMatrix
    //   Wrapper<IntervalMatrix>::Domain = IntervalMatrix
    //   + related possible expression templates
  };
}