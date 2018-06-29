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
  Point::Point(const Interval& t, const Interval& x)
    : m_t(t), m_x(x)
  {
    
  }

  const Interval& Point::t() const
  {
    return m_t;
  }

  const Interval& Point::x() const
  {
    return m_x;
  }
  
  const IntervalVector Point::box() const
  {
    IntervalVector box(2);
    box[0] = m_t; box[1] = m_x;
    return box;
  }

  bool Point::operator==(const Point& p) const
  {
    return m_t == p.m_t && m_x == p.m_x;
  }

  bool Point::operator!=(const Point& p) const
  {
    return m_t != p.m_t || m_x != p.m_x;
  }

  ostream& operator<<(ostream& str, const Point& p)
  {
    str << "(" << p.m_t << "," << p.m_x << ")";
    return str;
  }

  void pushPoints(const IntervalVector& box, vector<Point>& v_points)
  {
    Interval xlb = box[1].lb() != NEG_INFINITY ? box[1].lb() : Interval(NEG_INFINITY, box[1].ub());
    Interval xub = box[1].ub() != POS_INFINITY ? box[1].ub() : Interval(box[1].lb(), POS_INFINITY);

    v_points.push_back(Point(box[0].lb(), xlb));
    v_points.push_back(Point(box[0].lb(), xub));
    v_points.push_back(Point(box[0].ub(), xub));
    v_points.push_back(Point(box[0].ub(), xlb));
  }
}