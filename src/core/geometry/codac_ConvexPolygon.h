/** 
 *  ConvexPolygon class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_CONVEXPOLYGON_H__
#define __CODAC_CONVEXPOLYGON_H__

#include <vector>
#include "codac_Polygon.h"
#include "codac_BoolInterval.h"

namespace codac
{
  class ConvexPolygon : public Polygon
  {
    public:

      /// \name Definition
      /// @{

        ConvexPolygon();
        ConvexPolygon(const ConvexPolygon& p);
        explicit ConvexPolygon(const IntervalVector& box);
        ConvexPolygon(const std::vector<ThickPoint>& v_thick_pts);
        ConvexPolygon(const std::vector<Vector>& v_floating_pts, bool convex_and_convention_order = false);

      /// @}
      /// \name Tests
      /// @{

        const BoolInterval is_subset(const ConvexPolygon& p) const;
        const BoolInterval encloses(const ThickPoint& p) const;

      /// @}
      /// \name Setting values
      /// @{

        const ConvexPolygon& inflate(double rad);
        const ConvexPolygon& simplify(size_t max_edges);
        const ConvexPolygon& rotate(const Interval& theta, const IntervalVector& center);

      /// @}
      /// \name Operators
      /// @{

        const IntervalVector fast_intersection(const IntervalVector& x) const;

      /// @}
  };
}

#endif