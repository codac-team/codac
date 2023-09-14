/** 
 *  \file
 *  
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <cassert>
#include "codac2_cart_prod.h"

using namespace std;

namespace codac2
{
  IntervalVector cart_prod_dyn(const Interval& x1, const Interval& x2)
  {
    return IntervalVector({x1,x2});
  }

  IntervalVector_<2> cart_prod_static(const Interval& x1, const Interval& x2)
  {
    return IntervalVector_<2>({x1,x2});
  }

  IntervalVector cart_prod_dyn(const IntervalVector& x1, const Interval& x2)
  {
    IntervalVector x(x1.size()+1);
    x << x1,x2;
    return x;
  }

  IntervalVector cart_prod_dyn(const Interval& x1, const IntervalVector& x2)
  {
    IntervalVector x(x2.size()+1);
    x << x1,x2;
    return x;
  }

  IntervalVector cart_prod_dyn(const IntervalVector& x1, const IntervalVector& x2)
  {
    IntervalVector x(x1.size()+x2.size());
    x << x1,x2;
    return x;
  }

} // namespace codac