/** 
 *  codac2_SepUnion.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SepUnion.h"

using namespace std;
using namespace codac2;

BoxPair SepUnion::separate(const IntervalVector& x) const
{
  assert_release(x.size() == this->size());
  
  auto x_in = x;
  auto x_out = IntervalVector::empty(x.size());

  for(const auto& si : _seps)
  {
    auto x_sep = si->separate(x_in);
    x_out |= x_sep.outer;
    x_in &= x_sep.inner;
  }

  assert((x_in | x_out) == x);
  return { x_in, x_out };
}