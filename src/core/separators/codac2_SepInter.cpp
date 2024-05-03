/** 
 *  SepInter class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
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
  auto x_in = IntervalVector::empty(x.size());
  auto x_out = x;

  for(const auto& si : _seps)
  {
    auto x_sep = si->separate(x);
    x_out &= x_sep.out;
    x_in |= x_sep.in;
  }
  
  assert((x_in | x_out) == x);
  return { x_in, x_out };
}