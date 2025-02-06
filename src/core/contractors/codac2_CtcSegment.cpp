/** 
 *  codac2_CtcSegment.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_CtcSegment.h"
#include "codac2_det.h"
#include "codac2_arith_sub.h"

using namespace std;
using namespace codac2;

void CtcSegment::contract(IntervalVector& x) const
{
  assert_release(x.size() == 2 && "only 2d segments are supported");
  IntervalVector a(_a), b(_b); // variables that may be contracted
  x &= a | b; // boxed domain of the segment
  IntervalVector c = a-x, d = b-a;
  DetOp::bwd(0.,d,c);
  SubOp::bwd(c[0],a[0],x[0]);
  SubOp::bwd(c[1],a[1],x[1]);
}