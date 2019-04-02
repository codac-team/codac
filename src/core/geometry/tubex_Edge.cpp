/** 
 *  Edge class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iostream>
#include <iomanip>
#include "tubex_Edge.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Edge::Edge(const Point& p1, const Point& p2) : m_p1(p1), m_p2(p2)
  {

  }
  
  const Point& Edge::p1() const
  {
    return m_p1;
  }
  
  const Point& Edge::p2() const
  {
    return m_p2;
  }

  const Interval Edge::length() const
  {
    return sqrt(sqr(m_p1.x() - m_p2.x()) + sqr(m_p1.y() - m_p2.y()));
  }

  const BoolInterval Edge::is_horizontal() const
  {
    if(box()[1].is_degenerated())
      return YES;

    else if(m_p1.y().intersects(m_p2.y()))
      return MAYBE;

    else
      return NO;
  }

  const BoolInterval Edge::is_vertical() const
  {
    if(box()[0].is_degenerated())
      return YES;

    else if(m_p1.x().intersects(m_p2.x()))
      return MAYBE;

    else
      return NO;
  }

  const BoolInterval Edge::is_degenerated() const
  {
    if(m_p1.x() == m_p2.x() && m_p1.y() == m_p2.y())
      return YES;

    else if(m_p1.x().intersects(m_p2.x()) && m_p1.y().intersects(m_p2.y()))
      return MAYBE;

    else
      return NO;
  }

  const IntervalVector Edge::operator&(const IntervalVector& x) const
  {
    assert(x.size() == 2);

    IntervalVector inter(2, Interval::EMPTY_SET);

    if(box().is_flat())
      return x & box();

    else
    {
      if(m_p1.box().is_subset(x))
        inter |= m_p1.box();

      if(m_p2.box().is_subset(x))
        inter |= m_p2.box();

      if(m_p1.box().is_subset(x) && m_p2.box().is_subset(x))
        return inter;

      else // interpolation
      {
        vector<Edge> v_box_edges;
        push_edges(x, v_box_edges);
        for(int i = 0 ; i < v_box_edges.size() ; i++)
          inter |= (*this & v_box_edges[i]).box();
        return inter;
      }
    }

    assert(inter.is_empty());
    return inter;
  }

  const IntervalVector Edge::box() const
  {
    return m_p1.box() | m_p2.box();
  }
  
  bool Edge::operator==(const Edge& e) const
  {
    return m_p1 == e.m_p1 && m_p2 == e.m_p2;
  }
  
  bool Edge::operator!=(const Edge& e) const
  {
    return m_p1 != e.m_p1 || m_p2 != e.m_p2;
  }

  bool Edge::does_not_exist() const
  {
    return m_p1.does_not_exist() || m_p2.does_not_exist();
  }
  
  const Point Edge::operator&(const Edge& e) const
  {
    const Point proj = proj_intersection(*this, e);
    return Point(proj.x() & box()[0] & e.box()[0], proj.y() & box()[1] & e.box()[1]);
  }

  const Point Edge::proj_intersection(const Edge& e1, const Edge& e2)
  {
    assert(!e1.does_not_exist() && !e2.does_not_exist());

    Point p;

    if(e1.is_degenerated() == NO && e2.is_degenerated() != NO)
      p = proj_intersection(e2, e1); // permutation

    else if(e1.is_degenerated() != NO && e2.is_degenerated() != NO)
      p = Point(e1.box()[0] & e2.box()[0], e1.box()[1] & e2.box()[1]);

    else if(e2.is_vertical() != NO)
    {
      if(e1.is_degenerated() != NO)
        p = Point(e1.box()[0] & e2.box()[0], e1.box()[1]);

      else if(e1.is_vertical() != NO)
        p = Point(e1.box()[0] & e2.box()[0], Interval::ALL_REALS); // undefined intersection

      else if(e1.is_horizontal() != NO)
        p = Point(e2.box()[0], e1.box()[1]);

      else // oblique e1
      {
        // Line equation x=a*t+b
        Interval a = (e1.p2().y() - e1.p1().y()) / (e1.p2().x() - e1.p1().x()); // slope
        Interval b = e1.p1().y();

        // Intersecting polygon's line and edge's line
        p = Point(e2.box()[0], b + a * (e2.box()[0] - e1.p1().x()));
      }
    }

    else if(e2.is_horizontal() != NO)
    {
      if(e1.is_degenerated() != NO)
        p = Point(e1.box()[0], e1.box()[1] & e2.box()[1]);

      else if(e1.is_horizontal() != NO)
        p = Point(Interval::ALL_REALS, e1.box()[1] & e2.box()[1]); // undefined intersection

      else if(e1.is_vertical() != NO)
        p = Point(e1.box()[0], e2.box()[1]);

      else // oblique e1
      {
        // Line equation x=a*t+b
        Interval a = (e1.p2().y() - e1.p1().y()) / (e1.p2().x() - e1.p1().x()); // slope
        Interval b = e1.p1().y();

        // Intersecting polygon's line and edge's line
        p = Point(e1.p1().x() + ((e2.box()[1] - b) / a), e2.box()[1]);
      }
    }

    else
    {
      Interval x1 = e1.p1().x(), y1 = e1.p1().y();
      Interval x2 = e1.p2().x(), y2 = e1.p2().y();
      Interval x3 = e2.p1().x(), y3 = e2.p1().y();
      Interval x4 = e2.p2().x(), y4 = e2.p2().y();

      p = Point((((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4))),
                (((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4))));
    }

    return p;
  }

  const BoolInterval Edge::contains(const Point& p) const
  {
    if(Point::aligned(p, m_p1, m_p2) == NO)
      return NO;

    // Testing if p is between m_p1 and m_p2
    Interval k_p1p = (m_p2.x() - m_p1.x()) * (m_p2.x() - p.x()) + (m_p2.y() - m_p1.y()) * (m_p2.y() - p.y());
    Interval k_p1p2 = pow(m_p2.x() - m_p1.x(), 2) + pow(m_p2.y() - m_p1.y(), 2);

    return k_p1p.is_subset(k_p1p2 | 0.) ? MAYBE : NO;
  }
  
  ostream& operator<<(ostream& str, const Edge& e)
  {
    str << e.m_p1 << "--" << e.m_p2;
    return str;
  }

  void push_edges(const IntervalVector& box, vector<Edge>& v_edges)
  {
    assert(box.size() == 2);

    if(!box.is_empty())
    {
      Interval xlb = box[1].lb() != NEG_INFINITY ? box[1].lb() : Interval(NEG_INFINITY, box[1].ub());
      Interval xub = box[1].ub() != POS_INFINITY ? box[1].ub() : Interval(box[1].lb(), POS_INFINITY);

      v_edges.push_back(Edge(Point(box[0].lb(), xlb), Point(box[0].ub(), xlb)));
      v_edges.push_back(Edge(Point(box[0].ub(), xlb), Point(box[0].ub(), xub)));
      v_edges.push_back(Edge(Point(box[0].ub(), xub), Point(box[0].lb(), xub)));
      v_edges.push_back(Edge(Point(box[0].lb(), xub), Point(box[0].lb(), xlb)));
    }
  }

  const BoolInterval Edge::parallel(const Edge& e1, const Edge& e2)
  {
    assert(!e1.does_not_exist() && !e2.does_not_exist());

    if(e1.box()[0].is_degenerated() && e2.box()[0].is_degenerated())
      return YES; // vertical lines 

    if(e1.box()[1].is_degenerated() && e2.box()[1].is_degenerated())
      return YES; // horizontal lines 

    // Trying to compute the intersection point
    return (e1 & e2).box().is_unbounded() ? MAYBE : NO;
  }
}