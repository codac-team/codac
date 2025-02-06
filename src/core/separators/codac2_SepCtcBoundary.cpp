/** 
 *  codac2_SepCtcBoundary.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SepCtcBoundary.h"

using namespace std;
using namespace codac2;

BoxPair SepCtcBoundary::separate(const IntervalVector& x) const
{
  assert_release(x.size() == this->size());
  
  Index attempt_nb = 5;
  IntervalVector x_boundary(x);
  _ctc_boundary.front().contract(x_boundary);

  IntervalVector x_in(x_boundary), x_out(x_boundary);

  for(const auto& b : x.diff(x_boundary,false))
  {
    if(b.is_empty())
      continue;

    Vector m = b.mid(); // first try: midpoint of the box
    BoolInterval d;
    Index k = 0;

    do
    {
      d = _inside_test(m);
      if(d == BoolInterval::UNKNOWN)
        m = b.rand(); // now, trying a random vector in the box
    } while(d == BoolInterval::UNKNOWN && k++ < attempt_nb);

    switch(d)
    {
      case BoolInterval::TRUE:
        x_out |= b;
        break;
      case BoolInterval::FALSE:
        x_in |= b;
        break;
      case BoolInterval::UNKNOWN:
        x_out |= b; x_in |= b;
        break;
      case BoolInterval::EMPTY:
        assert(false && "BoolInterval::EMPTY should not happen");
        break;
    }
  }
  
  assert((x_in | x_out) == x);
  return { x_in, x_out };
}