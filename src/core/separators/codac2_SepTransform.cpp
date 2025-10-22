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
  IntervalVector y = _f_inv.eval(x);
  auto ys = _sep.front()->separate(y);
  BoxPair bp = { x & _f.eval(ys.inner), x & _f.eval(ys.outer) };
  assert((bp.inner | bp.outer) == x);
  return bp;
}