/* ============================================================================
 *  tubex-lib - ConvexPolygon class
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
#include <iomanip>
#include "tubex_ConvexPolygon.h"
#include "tubex_VibesFigure.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  ConvexPolygon::ConvexPolygon(const IntervalVector& box) : Polygon(box)
  {
    assert(box.size() == 2);
  }
  
  ConvexPolygon::ConvexPolygon(const std::vector<Point>& v_points) : Polygon(v_points)
  {
    // todo: (test convexity, or make it convex)
  }
  
  const IntervalVector ConvexPolygon::operator&(const IntervalVector& x) const
  {
    assert(x.size() == 2);
    // todo: the following could be easily optimized

    if(does_not_exist())
      return IntervalVector(2, Interval::EMPTY_SET);

    IntervalVector reduced_x = x & box();
    IntervalVector inter(2, Interval::EMPTY_SET);

    vector<Edge> v_edges = get_edges();
    for(int i = 0 ; i < v_edges.size() ; i++)
      inter |= v_edges[i] & reduced_x;

    vector<Point> v_x_vertices;
    push_points(reduced_x, v_x_vertices);
    for(int i = 0 ; i < v_x_vertices.size() ; i++)
    {
      if(this->encloses(v_x_vertices[i]) != NO)
        inter |= v_x_vertices[i].box();
    }

    return inter;
  }
  
  const ConvexPolygon ConvexPolygon::intersect(const ConvexPolygon& p, const ibex::IntervalVector& x)
  {
    assert(x.size() == 2);

    // todo: the following could be easily optimized

    IntervalVector reduced_x = x & p.box();
    
    if(x.is_empty() || p.does_not_exist() || reduced_x.is_empty())
      return ConvexPolygon(IntervalVector(2, Interval::EMPTY_SET));

    vector<Point> v_points;
    vector<Edge> p_edges = p.get_edges();

    Point a(reduced_x[0].lb(), reduced_x[1].ub());
    Point b(reduced_x[0].ub(), reduced_x[1].ub());
    Point c(reduced_x[0].ub(), reduced_x[1].lb());
    Point d(reduced_x[0].lb(), reduced_x[1].lb());

    int j = 0, k = 0;
    int n = p_edges.size();

    if(p.encloses(a) != NO)
      v_points.push_back(a);

    // A-B
    Edge ab(a, b);
    for(int i = j ; i-j < n ; i++)
    {
      if(Edge::parallel(p_edges[i%n], ab) == NO)
      {
        Point e = p_edges[i%n] & ab;
        if(!e.does_not_exist() && p.encloses(e) != NO)
        {
          v_points.push_back(e);
          k = i;
        }
      }
    }
    j = k;

    if(p.encloses(b) != NO)
      v_points.push_back(b);

    // B-C
    Edge bc(b, c);
    for(int i = j ; i < n+j ; i++)
    {
      if(Edge::parallel(p_edges[i%n], bc) == NO)
      {
        Point e = p_edges[i%n] & bc;
        if(!e.does_not_exist() && p.encloses(e) != NO)
        {
          v_points.push_back(e);
          k = i%n;
        }
      }
    }
    j = k;

    if(p.encloses(c) != NO)
      v_points.push_back(c);

    // C-D
    Edge cd(c, d);
    for(int i = j ; i < n+j ; i++)
    {
      if(Edge::parallel(p_edges[i%n], cd) == NO)
      {
        Point e = p_edges[i%n] & cd;
        if(!e.does_not_exist() && p.encloses(e) != NO)
        {
          v_points.push_back(e);
          k = i%n;
        }
      }
    }
    j = k;

    if(p.encloses(d) != NO)
      v_points.push_back(d);

    // D-A
    Edge da(d, a);
    for(int i = j ; i < n+j ; i++)
    {
      if(Edge::parallel(p_edges[i%n], da) == NO)
      {
        Point e = p_edges[i%n] & da;
        if(!e.does_not_exist() && p.encloses(e) != NO)
        {
          v_points.push_back(e);
          k = i%n;
        }
      }
    }

    if(v_points.size() <= 2)
    {
    vibes::beginDrawing();
      cout << endl << "DEBUG" << endl << endl;
      cout << setprecision(40) << "   x=" << x << ", p=" << p << endl;
      VibesFigure fig("poly");
      fig.set_properties(100, 100, 400, 400);
      fig.draw_polygon(p);
      fig.draw_box(x);
      fig.axis_limits(x | p.box());
      fig.show();

      cout << "detail des points: (nb: " << v_points.size() << ")" << endl;
      for(int i = 0 ; i < v_points.size() ; i++)
        cout << "  " << v_points[i] << endl;
    }

    assert(!v_points.empty());
    return ConvexPolygon(v_points);
  }
}