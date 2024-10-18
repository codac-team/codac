/** 
 *  codac2_geometry.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_geometry.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  BoolInterval orientation(const IntervalVector& p1, const IntervalVector& p2, const IntervalVector& p3)
  {
    assert_release(p1.size() == 2 && p2.size() == 2 && p3.size() == 2);

    Interval o = (p2[1]-p1[1])*(p3[0]-p2[0])-(p2[0]-p1[0])*(p3[1]-p2[1]); 

    if(o.is_empty())
      return BoolInterval::EMPTY;
    if(o.contains(0.)) // possibly collinear points
      return BoolInterval::UNKNOWN;
    else if(o.lb() > 0.) // clockwise orientation
      return BoolInterval::TRUE;
    else // anticlockwise orientation
      return BoolInterval::FALSE;
  }
}