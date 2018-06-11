/* ============================================================================
 *  tubex-lib - Point structure
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef Point_HEADER
#define Point_HEADER

#include <vector>
#include "ibex_IntervalVector.h"

namespace tubex
{
  struct Point
  {
    double x;
    double y;

    bool operator==(const Point& p) const;
    bool operator!=(const Point& p) const;
  };

  Point point(double x, double y);
  void pushPoints(std::vector<Point>& v_points, const ibex::IntervalVector& box);
}

#endif