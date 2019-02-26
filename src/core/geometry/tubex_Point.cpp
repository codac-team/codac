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

#include <iomanip>
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

  bool Point::does_not_exist() const
  {
    return m_x.is_empty() || m_y.is_empty();
  }
  
  const Point Point::operator|=(const Point& p)
  {
    return Point(m_x | p.m_x, m_y | p.m_y);
  }

  ostream& operator<<(ostream& str, const Point& p)
  {
    str << "(";
    
    if(p.m_x.is_degenerated()) str << p.m_x.lb();
    else str << p.m_x;

    str << ",";

    if(p.m_y.is_degenerated()) str << p.m_y.lb();
    else str << p.m_y;

    str << ")";
    return str;
  }

  const BoolInterval Point::aligned(const Point& a, const Point& b, const Point& c)
  {
    Interval cross_product = (b.x()-a.x()) * (c.y()-a.y()) - (b.y() - a.y()) * (c.x() - a.x());
    return (cross_product == Interval(0.)) ? YES : (cross_product.contains(0.) ? MAYBE : NO);
  }

  void push_points(const IntervalVector& box, vector<Point>& v_points)
  {
    assert(box.size() == 2);
    if(!box.is_empty())
    {
      Interval xlb = box[0].lb() != NEG_INFINITY ? box[0].lb() : Interval(NEG_INFINITY, box[0].ub());
      Interval xub = box[0].ub() != POS_INFINITY ? box[0].ub() : Interval(box[0].lb(), POS_INFINITY);

      Interval ylb = box[1].lb() != NEG_INFINITY ? box[1].lb() : Interval(NEG_INFINITY, box[1].ub());
      Interval yub = box[1].ub() != POS_INFINITY ? box[1].ub() : Interval(box[1].lb(), POS_INFINITY);

      v_points.push_back(Point(xlb, ylb));
      v_points.push_back(Point(xlb, yub));
      v_points.push_back(Point(xub, yub));
      v_points.push_back(Point(xub, ylb));
    }
  }
}