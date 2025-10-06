/** 
 *  codac2_SepCartPolar.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Verlein Radwan, Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SepCartPolar.h"
#include "codac2_CtcPolar.h"

using namespace codac2;

BoxPair SepCartPolar::separate(const IntervalVector& x) const
{
  assert_release(x.size() == 2);

  CtcPolar ctc_polar;
  IntervalVector x_cart(2), x_pol(x);

  ctc_polar.contract(x_cart[0], x_cart[1], x_pol[0], x_pol[1]);
  auto x_cart_sep = _sep.front()->separate(x_cart);
  BoxPair x_polar_sep { x, x };
  ctc_polar.contract(x_cart_sep.inner[0], x_cart_sep.inner[1], x_polar_sep.inner[0], x_polar_sep.inner[1]);
  ctc_polar.contract(x_cart_sep.outer[0], x_cart_sep.outer[1], x_polar_sep.outer[0], x_polar_sep.outer[1]);

  return x_polar_sep;
}