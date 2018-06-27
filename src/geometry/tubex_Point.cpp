/* ============================================================================
 *  tubex-lib - Point class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#include "tubex_Point.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Point::Point(const Interval& t_, const Interval& x_)
  {
    t = t_;
    x = x_;
  }

  bool Point::operator==(const Point& p) const
  {
    return t == p.t && x == p.x;
  }

  bool Point::operator!=(const Point& p) const
  {
    return t != p.t || x != p.x;
  }

  void pushPoints(const IntervalVector& box, vector<Point>& v_points)
  {
    v_points.push_back(Point(box[0].lb(), box[1].lb()));
    v_points.push_back(Point(box[0].lb(), box[1].ub()));
    v_points.push_back(Point(box[0].ub(), box[1].ub()));
    v_points.push_back(Point(box[0].ub(), box[1].lb()));
  }
}