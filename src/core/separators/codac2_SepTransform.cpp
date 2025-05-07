/** 
 *  codac2_SepTransform.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SepTransform.h"
#include "codac2_CtcInverse.h"

using namespace std;
using namespace codac2;

BoxPair SepTransform::separate(const IntervalVector& x) const
{
  assert(x.size() == this->size());

  IntervalVector y = _f.eval(x);
  auto ys = _sep.front()->separate(y);
  
  IntervalVector x_in = x & _f_inv.eval(ys.inner);
  _ctc_inv.contract_(ys.inner, x_in);

  IntervalVector x_out = x & _f_inv.eval(ys.outer);
  _ctc_inv.contract_(ys.outer, x_out);

  return { x_in, x_out };
}