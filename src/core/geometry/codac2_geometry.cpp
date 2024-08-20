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

  BoolInterval segments_intersect(const array<IntervalVector,2>& a, const array<IntervalVector,2>& b)
  {
    assert_release(a[0].size() == 2 && a[1].size() == 2 && b[0].size() == 2 && b[1].size() == 2);

    BoolInterval o1 = orientation(a[0], a[1], b[0]);
    BoolInterval o2 = orientation(a[0], a[1], b[1]);
    BoolInterval o3 = orientation(b[0], b[1], a[0]);
    BoolInterval o4 = orientation(b[0], b[1], a[1]);

    if(o1 == BoolInterval::EMPTY || o2 == BoolInterval::EMPTY || 
        o3 == BoolInterval::EMPTY || o4 == BoolInterval::EMPTY)
      return BoolInterval::EMPTY;

    else if(o1 == BoolInterval::UNKNOWN || o2 == BoolInterval::UNKNOWN || 
        o3 == BoolInterval::UNKNOWN || o4 == BoolInterval::UNKNOWN)
      return BoolInterval::UNKNOWN;

    else if(o1 != o2 && o3 != o4)
      return BoolInterval::TRUE;

    else
      return BoolInterval::FALSE;
  }

  BoolInterval in_polygon(const IntervalVector& p, const vector<IntervalVector>& polygon)
  {
    assert_release(p.size() == 2);

    size_t i = 0;
    array<IntervalVector,2> transect { IntervalVector({next_float(-oo),p[1]}), p };

    for(size_t k = 0 ; k < polygon.size() ; k++)
    {
      assert_release(polygon[k].size() == 2);

      array<IntervalVector,2> edge_k { polygon[k], polygon[(k+1)%polygon.size()] };
      switch(segments_intersect(transect,edge_k))
      {
        case BoolInterval::TRUE:
          i++;
          break;

        case BoolInterval::FALSE:
          // no intersection
          break;

        case BoolInterval::EMPTY:
          assert(false && "BoolInterval::EMPTY should not happen");
          break;

        case BoolInterval::UNKNOWN: // case of colinear edges
        {
          if((transect[0] | transect[1]).intersects(edge_k[0] | edge_k[1]))
            return BoolInterval::UNKNOWN;
          
          else
          {
            // no intersection
          }
        }
      }
    }

    return (i%2 == 0) ? BoolInterval::FALSE : BoolInterval::TRUE;
  }
}