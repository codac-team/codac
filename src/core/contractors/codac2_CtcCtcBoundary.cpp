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
  
  srand(time(NULL));
  Index attempt_nb = 5;
  IntervalVector prev_x(x), larger_x(x);
  _ctc_boundary.front()->contract(x);

  // We restrict the "neighbourhood" of x in which we will take
  // random vectors to be tested (_inside_test). Otherwise, if x
  // is very large or unbounded prior to the contraction, then the
  // candidate points will be large also and the inside test may 
  // result in some unfortunate overflow.
  if(!x.is_empty())
    larger_x = IntervalVector(x).inflate(x.rad());

  for(const auto& b : prev_x.diff(x,false))
  {
    if(b.is_empty())
      continue;

    Vector m = (larger_x & b).mid(); // first try: midpoint of the box
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