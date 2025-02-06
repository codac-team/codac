/** 
 *  codac2_CtcDist.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <tuple>
#include "codac2_CtcDist.h"
#include "codac2_sqr.h"

using namespace std;
using namespace codac2;

void CtcDist::contract(IntervalVector& x) const
{
  assert_release(x.size() == 5 && "dist constraint is 5d: <ax,ay,bx,bx,d>");
  contract(x[0], x[1], x[2], x[3], x[4]);
}

void CtcDist::contract(Interval& ax, Interval& ay, Interval& bx, Interval& by, Interval& d) const
{
  // Forward
  Interval i1 = -bx;
  Interval i2 = ax + i1;
  Interval i3 = sqr(i2);
  Interval i4 = -by;
  Interval i5 = ay + i4;
  Interval i6 = sqr(i5);
  Interval i7 = i3 + i6;
  d &= sqrt(i7);

  // Backward
  i7 &= sqr(d);
  i3 &= i7 - i6;
  i6 &= i7 - i3;
  SqrOp::bwd(i6, i5);
  ay &= i5 - i4;
  i4 &= i5 - ay;
  by &= -i4;
  SqrOp::bwd(i3, i2);
  ax &= i2 - i1;
  i1 &= i2 - ax;
  bx &= -i1;
}