/** 
 *  ThickEdge class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iostream>
#include <iomanip>
#include "codac_ThickEdge.h"

using namespace std;
using namespace ibex;

namespace codac
{
  // Definition

  ThickEdge::ThickEdge(const ThickPoint& p1, const ThickPoint& p2)
  {
    m_pts[0] = p1;
    m_pts[1] = p2;

    if(m_pts[1].does_not_exist()) m_pts[0] = ThickPoint();
    if(m_pts[0].does_not_exist()) m_pts[1] = ThickPoint();
  }

  ThickEdge::ThickEdge(const Vector& p1, const Vector& p2)
    : ThickEdge(ThickPoint(p1), ThickPoint(p2))
  {
    
  }

  const ThickEdge& ThickEdge::operator=(const ThickEdge& e)
  {
    m_pts[0] = e.m_pts[0];
    m_pts[1] = e.m_pts[1];
    return *this;
  }


  // Accessing values

  const ThickPoint& ThickEdge::p1() const
  {
    return m_pts[0];
  }

  const ThickPoint& ThickEdge::p2() const
  {
    return m_pts[1];
  }

  const Interval ThickEdge::length() const
  {
    return sqrt(sqr(m_pts[0][0]-m_pts[1][0]) + sqr(m_pts[0][1]-m_pts[1][1]));
  }

  const IntervalVector ThickEdge::box() const
  {
    return m_pts[0].box() | m_pts[1].box();
  }


  // Tests

  const BoolInterval ThickEdge::is_horizontal() const
  {
    if(box()[1].is_degenerated())
      return YES;

    else if(m_pts[0][1].intersects(m_pts[1][1]))
      return MAYBE;

    else
      return NO;
  }

  const BoolInterval ThickEdge::is_vertical() const
  {
    if(box()[0].is_degenerated())
      return YES;

    else if(m_pts[0][0].intersects(m_pts[1][0]))
      return MAYBE;

    else
      return NO;
  }

  const BoolInterval ThickEdge::is_degenerated() const
  {
    if(m_pts[0][0] == m_pts[1][0] && m_pts[0][1] == m_pts[1][1])
      return YES;

    else if(m_pts[0][0].intersects(m_pts[1][0]) && m_pts[0][1].intersects(m_pts[1][1]))
      return MAYBE;

    else
      return NO;
  }

  bool ThickEdge::does_not_exist() const
  {
    return m_pts[0].does_not_exist() || m_pts[1].does_not_exist();
  }

  const BoolInterval ThickEdge::contains(const ThickPoint& p) const
  {
    if(ThickPoint::aligned(p, m_pts[0], m_pts[1]) == NO)
      return NO;

    // Testing if p is between p1 and p2
    const Interval k_p1p = (m_pts[1][0] - m_pts[0][0]) * (m_pts[1][0] - p[0]) + (m_pts[1][1] - m_pts[0][1]) * (m_pts[1][1] - p[1]);
    const Interval k_p1p2 = pow(m_pts[1][0] - m_pts[0][0], 2) + pow(m_pts[1][1] - m_pts[0][1], 2);

    return k_p1p.is_subset(k_p1p2 | 0.) ? MAYBE : NO;
  }

  bool ThickEdge::operator==(const ThickEdge& e) const
  {
    return m_pts[0] == e.m_pts[0] && m_pts[1] == e.m_pts[1];
  }

  bool ThickEdge::operator!=(const ThickEdge& e) const
  {
    return m_pts[0] != e.m_pts[0] || m_pts[1] != e.m_pts[1];
  }


  // Operators

  const IntervalVector ThickEdge::operator&(const IntervalVector& x) const
  {
    assert(x.size() == 2);
    assert(!x.is_empty());

    IntervalVector inter(2, Interval::EMPTY_SET);

    if(box().is_flat())
      return x & box();

    else
    {
      if(m_pts[0].box().is_subset(x))
        inter |= m_pts[0].box();

      if(m_pts[1].box().is_subset(x))
        inter |= m_pts[1].box();

      if(m_pts[0].box().is_subset(x) && m_pts[1].box().is_subset(x))
        return inter;

      else // interpolation
      {
        vector<ThickEdge> v_box_edges;
        ThickEdge::push(x, v_box_edges);
        for(size_t i = 0 ; i < v_box_edges.size() ; i++)
          inter |= (*this & v_box_edges[i]).box();
        return inter;
      }
    }

    assert(inter.is_empty());
    return inter;
  }

  const ThickPoint ThickEdge::operator&(const ThickEdge& e) const
  {
    const ThickPoint proj = proj_intersection(*this, e);
    return ThickPoint(proj[0] & box()[0] & e.box()[0], proj[1] & box()[1] & e.box()[1]);
  }

  const ThickPoint ThickEdge::proj_intersection(const ThickEdge& e1, const ThickEdge& e2)
  {
    assert(!e1.does_not_exist() && !e2.does_not_exist());

    ThickPoint p;

    if(e1.is_degenerated() == NO && e2.is_degenerated() != NO)
      p = proj_intersection(e2, e1); // permutation

    else if(e1.is_degenerated() != NO && e2.is_degenerated() != NO)
      p = ThickPoint(e1.box()[0] & e2.box()[0], e1.box()[1] & e2.box()[1]);

    else if(e2.is_vertical() != NO)
    {
      if(e1.is_degenerated() != NO)
        p = ThickPoint(e1.box()[0] & e2.box()[0], e1.box()[1]);

      else if(e1.is_vertical() != NO)
        p = ThickPoint(e1.box()[0] & e2.box()[0], Interval::ALL_REALS); // undefined intersection

      else if(e1.is_horizontal() != NO)
        p = ThickPoint(e2.box()[0], e1.box()[1]);

      else // oblique e1
      {
        // Line equation x=a*t+b
        const Interval a = (e1.p2()[1] - e1.p1()[1]) / (e1.p2()[0] - e1.p1()[0]); // slope
        const Interval b = e1.p1()[1];

        // Intersecting polygon's line and edge's line
        p = ThickPoint(e2.box()[0], b + a * (e2.box()[0] - e1.p1()[0]));
      }
    }

    else if(e2.is_horizontal() != NO)
    {
      if(e1.is_degenerated() != NO)
        p = ThickPoint(e1.box()[0], e1.box()[1] & e2.box()[1]);

      else if(e1.is_horizontal() != NO)
        p = ThickPoint(Interval::ALL_REALS, e1.box()[1] & e2.box()[1]); // undefined intersection

      else if(e1.is_vertical() != NO)
        p = ThickPoint(e1.box()[0], e2.box()[1]);

      else // oblique e1
      {
        // Line equation x=a*t+b
        const Interval a = (e1.p2()[1] - e1.p1()[1]) / (e1.p2()[0] - e1.p1()[0]); // slope
        const Interval b = e1.p1()[1];

        // Intersecting polygon's line and edge's line
        p = ThickPoint(e1.p1()[0] + ((e2.box()[1] - b) / a), e2.box()[1]);
      }
    }

    else
    {
      const Interval x1 = e1.p1()[0], y1 = e1.p1()[1];
      const Interval x2 = e1.p2()[0], y2 = e1.p2()[1];
      const Interval x3 = e2.p1()[0], y3 = e2.p1()[1];
      const Interval x4 = e2.p2()[0], y4 = e2.p2()[1];

      const Interval a = (x1*y2-y1*x2);
      const Interval b = (x3*y4-y3*x4);
      const Interval c = ((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));

      p = ThickPoint((a*(x3-x4)-(x1-x2)*b)/c, (a*(y3-y4)-(y1-y2)*b)/c);
    }

    return p;
  }


  // String

  ostream& operator<<(ostream& str, const ThickEdge& e)
  {
    str << e.m_pts[0] << "--" << e.m_pts[1];
    return str;
  }


  // Static methods

  const BoolInterval ThickEdge::parallel(const ThickEdge& e1, const ThickEdge& e2)
  {
    assert(!e1.does_not_exist() && !e2.does_not_exist());

    if(e1.is_vertical() == YES && e2.is_vertical() == YES)
      return YES; // vertical lines 

    if(e1.is_horizontal() == YES && e2.is_horizontal() == YES)
      return YES; // horizontal lines 

    // Trying to compute the intersection point
    return proj_intersection(e1, e2).is_unbounded() ? MAYBE : NO;
  }

  void ThickEdge::push(const IntervalVector& box, vector<ThickEdge>& v_edges)
  {
    assert(box.size() == 2);
    assert(!box.is_empty());

    const Interval xlb = box[1].lb() != NEG_INFINITY ? box[1].lb() : Interval(NEG_INFINITY, box[1].ub());
    const Interval xub = box[1].ub() != POS_INFINITY ? box[1].ub() : Interval(box[1].lb(), POS_INFINITY);

    v_edges.push_back(ThickEdge(ThickPoint(box[0].lb(), xlb), ThickPoint(box[0].ub(), xlb)));
    v_edges.push_back(ThickEdge(ThickPoint(box[0].ub(), xlb), ThickPoint(box[0].ub(), xub)));
    v_edges.push_back(ThickEdge(ThickPoint(box[0].ub(), xub), ThickPoint(box[0].lb(), xub)));
    v_edges.push_back(ThickEdge(ThickPoint(box[0].lb(), xub), ThickPoint(box[0].lb(), xlb)));
  }
}