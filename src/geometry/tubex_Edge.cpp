/* ============================================================================
 *  tubex-lib - Edge class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#include <iostream>
#include "tubex_Edge.h"
#include "tubex_Exception.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Edge::Edge(const Point& p1, const Point& p2) : m_p1(p1), m_p2(p2)
  {

  }

  const IntervalVector Edge::operator&(const IntervalVector& x) const
  {
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
        pushEdges(x, v_box_edges);
        for(int i = 0 ; i < v_box_edges.size() ; i++)
          inter |= (*this & v_box_edges[i]).box();
        return inter;
      }
    }

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
  
  const Point Edge::operator&(const Edge& e) const
  {
    if(e.box()[0].is_degenerated()) // vertical edge e
    {
      if(box().is_flat()) // vertical or horizontal polygon's line
      {
        return Point(box()[0] & e.box()[0], box()[1] & e.box()[1]);
      }

      else // oblique polygon's line
      {
        // Line equation x=a*t+b
        Interval a = (m_p2.x() - m_p1.x()) / (m_p2.t() - m_p1.t()); // slope
        Interval b = m_p1.x();

        // Intersecting polygon's line and edge's line
        return Point(e.box()[0] & box()[0],
                     e.box()[1] & (b + a * (e.box()[0] - m_p1.t())));
      }
    }

    else if(e.box()[1].is_degenerated()) // horizontal edge e
    {
      if(box().is_flat()) // vertical or horizontal polygon's line
      {
        return Point(box()[0] & e.box()[0], box()[1] & e.box()[1]);
      }

      else // oblique polygon's line
      {
        // Line equation x=a*t+b
        Interval a = (m_p2.x() - m_p1.x()) / (m_p2.t() - m_p1.t()); // slope
        Interval b = m_p1.x();

        // Intersecting polygon's line and edge's line
        return Point(e.box()[0] & (m_p1.t() + ((e.box()[1] - b) / a)),
                     e.box()[1] & box()[1]);
      }
    }

    else
      throw Exception("Edge::operator&", "edge should be vertical or horizontal");
  }
  
  ostream& operator<<(ostream& str, const Edge& e)
  {
    str << e.m_p1 << "--" << e.m_p2;
    return str;
  }

  void pushEdges(const IntervalVector& box, vector<Edge>& v_edges)
  {
    Interval xlb = box[1].lb() != NEG_INFINITY ? box[1].lb() : Interval(NEG_INFINITY, box[1].ub());
    Interval xub = box[1].ub() != POS_INFINITY ? box[1].ub() : Interval(box[1].lb(), POS_INFINITY);

    v_edges.push_back(Edge(Point(box[0].lb(), xlb), Point(box[0].ub(), xlb)));
    v_edges.push_back(Edge(Point(box[0].ub(), xlb), Point(box[0].ub(), xub)));
    v_edges.push_back(Edge(Point(box[0].ub(), xub), Point(box[0].lb(), xub)));
    v_edges.push_back(Edge(Point(box[0].lb(), xub), Point(box[0].lb(), xlb)));
  }
}