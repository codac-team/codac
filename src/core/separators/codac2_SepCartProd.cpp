/** 
 *  codac2_SepCartProd.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Quentin Brateau, Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SepCartProd.h"

using namespace std;
using namespace codac2;

BoxPair SepCartProd::separate(const IntervalVector& x) const
{
  assert_release(x.size() == this->size());
  auto x_in = x, x_out = x;

  Index i = 0;
  for(const auto& si : _seps)
  {
    IntervalVector xi = x.subvector(i,i+si->size()-1);
    auto xi_sep = si->separate(xi);
    x_in.put(i,xi_sep.inner);
    x_out.put(i,xi_sep.outer);
    i += si->size();
  }

  assert(i == x.size());
  assert((x_in | x_out) == x);

  if(x_out.is_empty())
    return { IntervalVector(size()), IntervalVector::empty(size()) };
  if(x_in.is_empty())
    return { IntervalVector::empty(size()), IntervalVector(size()) };
  return { x_in, x_out };
}