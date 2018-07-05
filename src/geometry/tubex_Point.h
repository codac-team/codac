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

#ifndef __TUBEX_POINT_H__
#define __TUBEX_POINT_H__

#include <vector>
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"

namespace tubex
{
  class Point
  {
    public:

      Point(const ibex::Interval& t, const ibex::Interval& x);
      const ibex::Interval& t() const;
      const ibex::Interval& x() const;
      const ibex::IntervalVector box() const;
      bool operator==(const Point& p) const;
      bool operator!=(const Point& p) const;
      friend std::ostream& operator<<(std::ostream& str, const Point& p);

    public:

      // Reliable representation of points:
      ibex::Interval m_t;
      ibex::Interval m_x;
  };

  void pushPoints(const ibex::IntervalVector& box, std::vector<Point>& v_points);
}

#endif