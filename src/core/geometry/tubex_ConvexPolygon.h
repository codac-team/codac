/** 
 *  ConvexPolygon class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CONVEXPOLYGON_H__
#define __TUBEX_CONVEXPOLYGON_H__

#include <vector>
#include "tubex_Polygon.h"
#include "ibex_BoolInterval.h"

namespace tubex
{
  class ConvexPolygon : public Polygon
  {
    public:

      /// \name Definition
      /// @{

        ConvexPolygon();
        ConvexPolygon(const ConvexPolygon& p);
        explicit ConvexPolygon(const ibex::IntervalVector& box);
        ConvexPolygon(const std::vector<Point>& v_thick_pts);
        ConvexPolygon(const std::vector<ibex::Vector>& v_floating_pts, bool convex_and_convention_order = false);

      /// @}
      /// \name Tests
      /// @{

        const ibex::BoolInterval is_subset(const ConvexPolygon& p) const;
        const ibex::BoolInterval encloses(const Point& p) const;

      /// @}
      /// \name Setting values
      /// @{

        const ConvexPolygon& inflate(double rad);
        const ConvexPolygon& simplify(size_t max_edges);
        const ConvexPolygon& rotate(const ibex::Interval& theta, const ibex::IntervalVector& center);

      /// @}
      /// \name Operators
      /// @{

        const ibex::IntervalVector fast_intersection(const ibex::IntervalVector& x) const;

      /// @}
  };
}

#endif