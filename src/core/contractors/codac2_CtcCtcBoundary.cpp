/** 
 *  codac2_CtcCtcBoundary.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_CtcCtcBoundary.h"

using namespace std;
using namespace codac2;

void CtcCtcBoundary::contract(IntervalVector& x) const
{
  assert_release(x.size() == this->size());
  
  Index attempt_nb = 5;
  IntervalVector prev_x(x);
  _ctc_boundary.front().contract(x);

  for(const auto& b : prev_x.diff(x,false))
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
      case BoolInterval::UNKNOWN:
        x |= b;
        break;
      case BoolInterval::FALSE:
        break;
      case BoolInterval::EMPTY:
        assert(false && "BoolInterval::EMPTY should not happen");
        break;
    }
  }
}