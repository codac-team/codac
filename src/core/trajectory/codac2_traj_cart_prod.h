/** 
 *  \file codac2_traj_cart_prod.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_cart_prod.h"
#include "codac2_Vector.h"
#include "codac2_SampledTraj.h"

namespace codac2
{
  template<typename... X>
    requires ((std::is_same_v<SampledTraj<double>,X> || std::is_same_v<SampledTraj<Vector>,X>) && ...)
  inline SampledTraj<Vector> traj_cart_prod(const X&... x)
  {
    auto&& x0 = std::get<0>(std::forward_as_tuple(x...));
    assert_release((SampledTraj<Vector>::same_sampling(x0, x) && ...));
    SampledTraj<Vector> y;
    for(auto it = x0.begin() ; it != x0.end() ; it++)
      y.set(cart_prod(x.at(it->first)...), it->first);
    return y;
  }
}