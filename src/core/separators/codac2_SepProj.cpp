/** 
 *  \file
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <list>
#include "codac2_SepProj.h"

using namespace std;

namespace codac2
{
  BoxPair SepProj::separate(const IntervalVector& x) const
  {
    // Default value for eps, for removing virtuality
    return separate(x, 0.1);
  }

  BoxPair SepProj::separate(const IntervalVector& x, double eps) const
  {
    auto x_in = x;
    auto x_out = x; x_out.set_empty();

    list<pair<IntervalVector,IntervalVector>> l;
    l.push_back({ x, _y });

    while(!l.empty())
    {
      auto x0 = l.front().first;
      auto y0 = l.front().second;
      l.pop_front();

      // ? x0 &= x_in;
      auto x_sep = _sep.front().separate(cart_prod(x0,y0));
      auto x_boundary = x_sep.in & x_sep.out;

      if(x_boundary.is_empty() || x_boundary.is_flat() || x_boundary.max_diam() < eps
        || y0.is_empty() || y0.max_diam() < eps)
      {
        x_out |= x_sep.out;
      }

      else if(y0.max_diam() > eps)
      {
        auto y_bisected = y0.bisect(y0.max_diam_index());
        l.push_back({ x0, y_bisected.first });
        l.push_back({ x0, y_bisected.second });
      }

    }

    return { x_in, x_out };
  }

}