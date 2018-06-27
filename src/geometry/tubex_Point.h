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
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"

namespace tubex
{
  class Point
  {
    public:

      Point(const ibex::Interval& t, const ibex::Interval& x);
      const ibex::IntervalVector box() const;
      bool operator==(const Point& p) const;
      bool operator!=(const Point& p) const;

    public:

      // Reliable representation of points:
      ibex::Interval t;
      ibex::Interval x;
  };

  void pushPoints(const ibex::IntervalVector& box, std::vector<Point>& v_points);
}

#endif