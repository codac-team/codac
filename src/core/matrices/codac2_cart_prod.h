/** 
 *  \file codac2_cart_prod.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_IntervalVector.h"
#include "codac2_Matrix.h"
#include "codac2_matrices.h"

namespace codac2
{
  inline IntervalVector to_IntervalVector(const Interval& x)
  {
    return { x };
  }

  template<typename OtherDerived>
    requires (std::is_same_v<typename OtherDerived::Scalar,Interval> && OtherDerived::ColsAtCompileTime==1)
  inline IntervalVector to_IntervalVector(const Eigen::MatrixBase<OtherDerived>& x)
  {
    return x;
  }

  template<typename OtherDerived>
    requires (std::is_same_v<typename OtherDerived::Scalar,double> && OtherDerived::ColsAtCompileTime==1)
  inline IntervalVector to_IntervalVector(const Eigen::MatrixBase<OtherDerived>& x)
  {
    return x.template cast<Interval>();
  }

  // Flatten some matrix into an interval vector
  template<typename T,int R,int C>
    requires (!Eigen::IsVectorOrRow<R,C>)
  inline IntervalVector to_IntervalVector(const Eigen::Matrix<T,R,C>& x)
  {
    return x.reshaped();
  }

  inline IntervalVector cart_prod()
  {
    return IntervalVector(0);
  }

  template<typename... X>
    requires ((is_interval_based_v<X>) || ...)
      && ((!is_ctc_v<X>) && ...) && ((!is_sep_v<X>) && ...)
  inline IntervalVector cart_prod(const X&... x)
  {
    std::vector<IntervalVector> v_x;
    ((v_x.push_back(to_IntervalVector(x))), ...);

    Index n = 0;
    for(const auto& xi : v_x)
      n += xi.size();
    IntervalVector x_(n);

    Index i = 0;
    for(const auto& xi : v_x)
    {
      x_.put(i, xi);
      i += xi.size();
    }
    return x_;
  }

  template<typename... X>
    requires ((!is_interval_based_v<X>) && ...)
      && ((!is_ctc_v<X>) && ...) && ((!is_sep_v<X>) && ...)
  inline Vector cart_prod(const X&... x)
  {
    return cart_prod(to_IntervalVector(x)...).mid();
  }
}
