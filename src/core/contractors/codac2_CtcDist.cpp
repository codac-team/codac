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

CtcDist::CtcDist()
  : Ctc<CtcDist,IntervalVector>(5)
{ }

void CtcDist::contract(IntervalVector& x) const
{
  assert_release(x.size() == 5 && "dist constraint is 5d: <a1,a2,b1,b2,d>");
  contract(x[0], x[1], x[2], x[3], x[4]);
}

void CtcDist::contract(Interval& a1, Interval& a2, Interval& b1, Interval& b2, Interval& d) const
{
  // Forward
  Interval i1 = -b1;
  Interval i2 = a1 + i1;
  Interval i3 = sqr(i2);
  Interval i4 = -b2;
  Interval i5 = a2 + i4;
  Interval i6 = sqr(i5);
  Interval i7 = i3 + i6;
  d &= sqrt(i7);

  // Backward
  i7 &= sqr(d);
  i3 &= i7 - i6;
  i6 &= i7 - i3;
  SqrOp::bwd(i6, i5);
  a2 &= i5 - i4;
  i4 &= i5 - a2;
  b2 &= -i4;
  SqrOp::bwd(i3, i2);
  a1 &= i2 - i1;
  i1 &= i2 - a1;
  b1 &= -i1;
}