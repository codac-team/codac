/** 
 *  CtcPolygon.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <list>
#include "codac2_CtcPolygon.h"
#include "codac2_CtcUnion.h"
#include "codac2_CtcSegment.h"
#include "codac2_geometry.h"

using namespace std;

namespace codac2
{
  CtcPolygon::CtcPolygon(const Polygon& p)
    : CtcCtcBoundary(

      // Contractor on the boundary
      [p]
      {
        CtcUnion<IntervalVector> ctc_segments(2);
        for(const auto& edge_i : p.edges())
          ctc_segments |= CtcSegment(edge_i[0],edge_i[1]);
        return ctc_segments;
      }(),

      // Tests if a point of a box is inside the polygon
      [p](const Vector& x) -> BoolInterval
      {
        assert_release(x.size() == 2);
        return p.contains(x);
      }
    )
  { }

  void CtcPolygon::contract(IntervalVector& x) const
  {
    CtcCtcBoundary::contract(x);
  }
}