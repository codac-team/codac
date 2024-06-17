/** 
 *  codac2_SepInter.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SepInter.h"

using namespace std;
using namespace codac2;

std::shared_ptr<Sep> SepInter::copy() const
{
  return std::make_shared<SepInter>(*this);
}

BoxPair SepInter::separate(const IntervalVector& x) const
{
  assert(x.size() == this->size());
  auto x_in = IntervalVector::empty(x.size());
  auto x_out = x;

  for(const auto& si : _seps)
  {
    auto x_sep = si->separate(x);
    x_out &= x_sep.outer;
    x_in |= x_sep.inner;
  }
  
  assert((x_in | x_out) == x);
  return { x_in, x_out };
}