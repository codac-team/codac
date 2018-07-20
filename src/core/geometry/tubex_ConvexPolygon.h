/* ============================================================================
 *  tubex-lib - ConvexPolygon class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

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
      ConvexPolygon(const ibex::IntervalVector& box);
      ConvexPolygon(const std::vector<Point>& v_points);

      const ibex::IntervalVector operator&(const ibex::IntervalVector& x) const;
  };
}

#endif