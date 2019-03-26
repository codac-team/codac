/** 
 *  Point class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_POINT_H__
#define __TUBEX_POINT_H__

#include <vector>
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_BoolInterval.h"

namespace tubex
{
  class Point
  {
    public:

      Point(); // undefined point
      Point(const ibex::Interval& x, const ibex::Interval& y);
      const ibex::Interval& x() const;
      const ibex::Interval& y() const;
      const ibex::IntervalVector box() const;
      bool operator==(const Point& p) const;
      bool operator!=(const Point& p) const;
      const Point operator|=(const Point& p);
      bool does_not_exist() const;
      const Point& inflate(double rad);

      friend std::ostream& operator<<(std::ostream& str, const Point& p);

      static const ibex::BoolInterval aligned(const Point& a, const Point& b, const Point& c);
      static const Point center(const std::vector<Point> v_pts);
      static const std::vector<Point> merge_close_points(const std::vector<Point>& v_pts);
      static const std::vector<Point> delete_redundant_points(const std::vector<Point>& v_pts);

    public:

      // Reliable representation of points:
      ibex::Interval m_x;
      ibex::Interval m_y;
  };

  const Point operator|(const Point& p1, const Point& p2);
  const Point operator-(const Point& p1, const Point& p2);
  void push_points(const ibex::IntervalVector& box, std::vector<Point>& v_points);
}

#endif