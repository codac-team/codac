/** 
 *  ConvexPolygon class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CONVEXPOLYGON_H__
#define __TUBEX_CONVEXPOLYGON_H__

#include "tubex_Polygon.h"
#include "ibex_IntervalVector.h"

namespace tubex
{
  class ConvexPolygon : public Polygon
  {
    public:

      ConvexPolygon();
      ConvexPolygon(const ConvexPolygon& p);
      ConvexPolygon(const ibex::IntervalVector& box);
      ConvexPolygon(const std::vector<Point>& v_points, bool convex_points = false);

      const ibex::IntervalVector operator&(const ibex::IntervalVector& x) const;
      static const ConvexPolygon intersect(const ConvexPolygon& p, const ibex::IntervalVector& x);
      static const ConvexPolygon intersect(const ConvexPolygon& p1, const ConvexPolygon& p2);

      void simplify(float n = 6.);
  };
}

#endif