/** 
 *  \file codac2_IntervalVector.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_matrices.h"
#include "codac2_Vector.h"

namespace codac2
{
  using IntervalVector = Eigen::Matrix<Interval,-1,1>;

  inline IntervalVector to_IntervalVector(const Interval& x)
  {
    IntervalVector a(1);
    a[0] = x;
    return a;
  }

  inline IntervalVector to_IntervalVector(const IntervalVector& x)
  {
    return x;
  }

  inline IntervalVector to_IntervalVector(const Vector& x)
  {
    return x.template cast<Interval>();
  }

  template<typename... X>
    requires ((std::is_same_v<int,X> || std::is_same_v<double,X> || std::is_same_v<Interval,X>
     || std::is_same_v<Vector,X> || std::is_same_v<IntervalVector,X>) && ...)
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

  inline std::ostream& operator<<(std::ostream& os, const IntervalVector& x)
  {
    if(x.is_empty())
      return os << "[ empty vector ]";

    else
    {
      os << x.format(codac_vector_fmt());
      return os;
    }
  }
}