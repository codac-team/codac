/** 
 *  SepUnion class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_SepUnion.h"

using namespace std;
using namespace codac2;

std::shared_ptr<Sep> SepUnion::copy() const
{
  return std::make_shared<SepUnion>(*this);
}

BoxPair SepUnion::separate(const IntervalVector& x) const
{
  auto x_in = x;
  auto x_out = IntervalVector::empty(x.size());

  for(const auto& si : _seps)
  {
    auto x_sep = si->separate(x);
    x_out |= x_sep.out;
    x_in &= x_sep.in;
  }

  assert((x_in | x_out) == x);
  return { x_in, x_out };
}