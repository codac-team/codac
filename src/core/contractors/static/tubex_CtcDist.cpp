/** 
 *  CtcDist class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_CtcDist.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcDist::CtcDist()
    : CtcFunction(
        Function("a[2]", "b[2]", "d",
                 "d - sqrt((a[0]-b[0])^2+(a[1]-b[1])^2)"))
  {
    
  }

  void CtcDist::contract(IntervalVector& a, IntervalVector& b, Interval& d)
  {
    assert(a.size() == 2 && b.size() == 2);

    IntervalVector box(5);
    box.put(0, a);
    box.put(2, b);
    box[4] = d;

    CtcFwdBwd::contract(box);

    a &= box.subvector(0,1);
    b &= box.subvector(2,3);
    d &= box[4];
  }
}