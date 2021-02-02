/** 
 *  CtcDist class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_CtcDist.h"

using namespace std;
using namespace ibex;

namespace codac
{
  CtcDist::CtcDist()
    : Ctc(5)
  {
    
  }

  void CtcDist::contract(IntervalVector& x)
  {
    assert(x.size() == 5);
    contract(x[0], x[1], x[2], x[3], x[4]);
  }

  void CtcDist::contract(IntervalVector& a, IntervalVector& b, Interval& d)
  {
    assert(a.size() == 2 && b.size() == 2);
    contract(a[0], a[1], b[0], b[1], d);
  }

  void CtcDist::contract(Interval& ax, Interval& ay, Interval& bx, Interval& by, Interval& d)
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
    bwd_sqr(i6, i5);
    ay &= i5 - i4;
    i4 &= i5 - ay;
    by &= -i4;
    bwd_sqr(i3, i2);
    ax &= i2 - i1;
    i1 &= i2 - ax;
    bx &= -i1;
  }
}