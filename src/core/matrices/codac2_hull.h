/** 
 *  \file codac2_cart_prod.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_matrices.h"
#include "codac2_Wrapper.h"

namespace codac2
{
  template<typename X1,typename... X>
  inline Wrapper<X1>::Domain hull(const X1& x1, const X&... x)
  {
    if constexpr(sizeof...(x) > 0) 
      return hull(x1) | hull(x...);
    else
      return x1.template cast<Interval>();
  }

  template<typename X>
  inline Wrapper<X>::Domain hull(const std::list<X>& l)
  {
    if(l.empty())
    {
      typename Wrapper<X>::Domain e(1);
      e.set_empty();
      return e;
    }

    auto h = l.front().template cast<Interval>();
    for(const auto& li : l)
      h |= li;
    return h;
  }
}