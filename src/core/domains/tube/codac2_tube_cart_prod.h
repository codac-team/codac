/** 
 *  \file codac2_tube_cart_prod.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <tuple>
#include "codac2_cart_prod.h"
#include "codac2_SlicedTube.h"

namespace codac2
{
  /**
   * \brief Computes the Cartesian product of several sliced tubes
   *
   * This function builds a vector-valued sliced tube whose temporal domain is
   * shared with the input tubes. For each temporal slice, the codomain of the
   * resulting tube is the Cartesian product of the codomains of the input tubes
   * over the same temporal slice.
   *
   * The dimension of the resulting codomain is equal to the sum of the
   * dimensions of all input tubes.
   *
   * \note All input tubes must share the same temporal partition.
   *
   * \tparam X types of the input sliced tubes
   * \param x input sliced tubes
   * \return a sliced tube enclosing the Cartesian product of the input tubes
   */
  template<typename... X>
  SlicedTube<IntervalVector> tube_cart_prod(const X&... x)
  {
    static_assert(sizeof...(X) > 0, "tube_cart_prod requires at least one tube");
    Index s = 0;
    ((s += x.size()), ...);

    const auto& x0 = std::get<0>(std::forward_as_tuple(x...));
    auto tdomain = x0.tdomain();
    assert_release(((x.tdomain() == tdomain) && ...));
    SlicedTube v(tdomain, IntervalVector(s));

    for(auto it = tdomain->begin() ; it != tdomain->end() ; it++)
      v.slice(it)->codomain() = cart_prod(x.slice(it)->codomain()...);

    return v;
  }
}