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
  Point::Point(const Interval& x, const Interval& y)
    : m_x(x), m_y(y)
  {
    if(m_x.is_empty()) m_y.set_empty();
    if(m_y.is_empty()) m_x.set_empty();
  }

  const Interval& Point::x() const
  {
    return m_x;
  }

  const Interval& Point::y() const
  {
    return m_y;
  }
  
  const IntervalVector Point::box() const
  {
    IntervalVector box(2);
    box[0] = m_x; box[1] = m_y;
    return box;
  }

  bool Point::operator==(const Point& p) const
  {
    return m_x == p.m_x && m_y == p.m_y;
  }

  bool Point::operator!=(const Point& p) const
  {
    return m_x != p.m_x || m_y != p.m_y;
  }

  ostream& operator<<(ostream& str, const Point& p)
  {
    str << "(" << p.m_x << "," << p.m_y << ")";
    return str;
  }

  void push_points(const IntervalVector& box, vector<Point>& v_points)
  {
    assert(box.size() == 2);

    Interval xlb = box[1].lb() != NEG_INFINITY ? box[1].lb() : Interval(NEG_INFINITY, box[1].ub());
    Interval xub = box[1].ub() != POS_INFINITY ? box[1].ub() : Interval(box[1].lb(), POS_INFINITY);

    v_points.push_back(Point(box[0].lb(), xlb));
    v_points.push_back(Point(box[0].lb(), xub));
    v_points.push_back(Point(box[0].ub(), xub));
    v_points.push_back(Point(box[0].ub(), xlb));
  }
}