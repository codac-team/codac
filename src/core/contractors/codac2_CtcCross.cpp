/** 
 *  codac2_CtcCross.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_CtcCross.h"
#include "codac2_arith_sub.h"
#include "codac2_arith_mul.h"
#include "codac2_Interval.h"
#include "codac2_IntervalVector.h"

using namespace std;
using namespace codac2;

void det_contractor(IntervalVector& x, IntervalVector& a, IntervalVector& b, Interval y)
{
  // Forward contraction
  Interval i1 = x[0] - a[0];
  Interval i2 = b[1] - a[1];
  Interval i3 = i1 * i2;
  Interval i4 = x[1] - a[1];
  Interval i5 = b[0] - a[0];
  Interval i6 = i4 * i5;
  Interval i7 = i3 - i6;

  // Backward contraction
  i7 &= y;
  SubOp::bwd(i7, i3, i6);
  MulOp::bwd(i6, i4, i5);
  SubOp::bwd(i5, b[0], a[0]);
  SubOp::bwd(i4, x[1], a[1]);
  MulOp::bwd(i3, i1, i2);
  SubOp::bwd(i2, b[1], a[1]);
  SubOp::bwd(i1, x[0], a[0]);
}

void CtcCross::contract(IntervalVector& x) const
{
  assert_release(x.size() == 2 && "only 2d segments are supported");
  IntervalVector a(_e[0]), b(_e[1]), r(_r); // variables that may be contracted

  IntervalVector x1(x), x2(x), x3(x), x4(x);
  IntervalVector ra = a - r;
  IntervalVector rb = b - r;

  det_contractor(x1, a, b, Interval(-oo, 0));
  det_contractor(x2, ra, rb, Interval(0, oo));
  det_contractor(x3, ra, a, Interval(-oo, 0));
  det_contractor(x4, rb, b, Interval(0, oo));

  x &= (x1 | x2 | x3 | x4);
}

void CtcNoCross::contract(IntervalVector& x) const
{
  assert_release(x.size() == 2 && "only 2d segments are supported");
  IntervalVector a(_e[0]), b(_e[1]), r(_r); // variables that may be contracted

  IntervalVector x1(x), x2(x), x3(x), x4(x);
  IntervalVector ra = a - r;
  IntervalVector rb = b - r;

  det_contractor(x1, a, b, Interval(0, oo));
  det_contractor(x2, ra, rb, Interval(-oo, 0));
  det_contractor(x3, ra, a, Interval(0, oo));
  det_contractor(x4, rb, b, Interval(-oo, 0));

  x &= (x1 & x2 & x3 & x4);
}