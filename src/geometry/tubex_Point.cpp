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
    : t(t_), x(x_)
  {
    
  }
  
  const IntervalVector Point::box() const
  {
    IntervalVector box(2);
    box[0] = t; box[1] = x;
    return box;
  }

  bool Point::operator==(const Point& p) const
  {
    return t == p.t && x == p.x;
  }

  bool Point::operator!=(const Point& p) const
  {
    return t != p.t || x != p.x;
  }

  ostream& operator<<(ostream& str, const Point& p)
  {
    str << "(" << p.t << "," << p.x << ")";
    return str;
  }

  void pushPoints(const IntervalVector& box, vector<Point>& v_points)
  {
    Interval xlb = box[1].lb() != NEG_INFINITY ? box[1].lb() : Interval::ALL_REALS;
    Interval xub = box[1].ub() != POS_INFINITY ? box[1].ub() : Interval::ALL_REALS;

    v_points.push_back(Point(box[0].lb(), xlb));
    v_points.push_back(Point(box[0].lb(), xub));
    v_points.push_back(Point(box[0].ub(), xub));
    v_points.push_back(Point(box[0].ub(), xlb));
  }
}