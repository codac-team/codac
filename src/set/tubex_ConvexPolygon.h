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

#ifndef ConvexPolygon_HEADER
#define ConvexPolygon_HEADER

#include "tubex_Polygon.h"

namespace tubex
{
  class ConvexPolygon : public Polygon
  {
    public:

      ConvexPolygon(const std::vector<ibex::IntervalVector>& v_boxes);
      ConvexPolygon(const std::vector<Point>& v_points);

      friend ConvexPolygon operator&(const ConvexPolygon& p1, const ConvexPolygon& p2);
  };
}

#endif