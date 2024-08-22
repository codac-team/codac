/** 
 *  SepPolygon.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <list>
#include "codac2_SepPolygon.h"
#include "codac2_CtcSegment.h"
#include "codac2_geometry.h"

using namespace std;

namespace codac2
{
  SepPolygon::SepPolygon(const std::vector<IntervalVector>& vertices)
    : SepCtcBoundary(

      // Contractor on the boundary
      [vertices]
      {
        CtcUnion<IntervalVector> ctc_segments(2);
        for(size_t i = 0 ; i < vertices.size() ; i++)
        {
          assert_release(vertices[i].size() == 2 && "polygons must be defined by 2d points");
          ctc_segments |= CtcSegment(vertices[i],vertices[(i+1)%vertices.size()]);
        }
        return ctc_segments;
      }(),

      // Tests if a point of a box is inside the polygon
      [vertices](const Vector& x) -> BoolInterval
      {
        assert_release(x.size() == 2);
        return in_polygon(x,vertices);
      }
    )
  { }

  std::shared_ptr<Sep> SepPolygon::copy() const
  {
    return std::make_shared<SepPolygon>(*this);
  }

  BoxPair SepPolygon::separate(const IntervalVector& x) const
  {
    return SepCtcBoundary::separate(x);
  }
}