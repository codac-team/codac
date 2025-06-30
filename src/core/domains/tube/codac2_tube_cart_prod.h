/** 
 *  \file codac2_tube_cart_prod.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_cart_prod.h"
#include "codac2_SlicedTube.h"

namespace codac2
{
  template<typename... X>
  SlicedTube<IntervalVector> tube_cart_prod(const X&... x)
  {
    Index s = 0;
    ((s += x.size()), ...);

    auto tdomain = std::get<0>(std::make_tuple(x...)).tdomain();
    SlicedTube v(tdomain, IntervalVector(s));

    for(auto it = tdomain->begin() ; it != tdomain->end() ; it++)
      v(it)->codomain() = cart_prod(x(it)->codomain()...);

    return v;
  }
}