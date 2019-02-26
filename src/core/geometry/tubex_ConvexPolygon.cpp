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
    order_points(m_v_vertices);
  }
  
  ConvexPolygon::ConvexPolygon(const std::vector<Point>& v_points) : Polygon(v_points)
  {
    // todo: (test convexity, or make it convex)
    order_points(m_v_vertices);
  }
  
  const IntervalVector ConvexPolygon::operator&(const IntervalVector& x) const
  {
    assert(x.size() == 2);

    if(does_not_exist())
      return IntervalVector(2, Interval::EMPTY_SET);

    IntervalVector reduced_x = x & box();
    IntervalVector inter(2, Interval::EMPTY_SET);

    vector<Edge> v_edges = edges();
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
  
  const ConvexPolygon ConvexPolygon::intersect(const ConvexPolygon& p1, const ConvexPolygon& p2)
  {
    vector<Point> v_pts;

    // Add all corners of p1 that are inside p2
    for(int i = 0 ; i < p1.vertices().size() ; i++)
      if(p2.encloses(p1.vertices()[i]) != NO)
        v_pts.push_back(p1.vertices()[i]);

    // Add all corners of p2 that are inside p1
    for(int i = 0 ; i < p2.vertices().size() ; i++)
      if(p1.encloses(p2.vertices()[i]) != NO)
        v_pts.push_back(p2.vertices()[i]);

    // Add all intersection points
    for(int i = 0 ; i < p1.edges().size() ; i++)
      for(int j = 0 ; j < p2.edges().size() ; j++)
      {
        Point intersection_pt = p1.edges()[i] & p2.edges()[j];

        if(!intersection_pt.does_not_exist())
        {
          if(Edge::parallel(p1.edges()[i], p2.edges()[j]) != NO)
          {
            if(p1.edges()[i].contains(p2.edges()[j].p1()) != NO)
              v_pts.push_back(p2.edges()[j].p1());

            if(p1.edges()[i].contains(p2.edges()[j].p2()) != NO)
              v_pts.push_back(p2.edges()[j].p2());

            if(p2.edges()[j].contains(p1.edges()[i].p1()) != NO)
              v_pts.push_back(p1.edges()[i].p1());

            if(p2.edges()[j].contains(p1.edges()[i].p2()) != NO)
              v_pts.push_back(p1.edges()[i].p2());
          }

          else
            v_pts.push_back(intersection_pt);
        }
      }

    // Merge equivalent points

      vector<Point> merged_points;
      order_points(v_pts);

      int i = 0, n = v_pts.size();
      while(i < n)
      {
        Point p = v_pts[i];

        int j = i + 1;
        while(j < n && v_pts[i].x().intersects(v_pts[j].x()) && v_pts[i].y().intersects(v_pts[j].y()))
        {
          p |= v_pts[j];
          j++;
        }

        merged_points.push_back(p);
        i = j;
      }

    return ConvexPolygon(merged_points);
  }
  
  const ConvexPolygon ConvexPolygon::intersect(const ConvexPolygon& p, const ibex::IntervalVector& x)
  {
    assert(x.size() == 2);
    return intersect(p, ConvexPolygon(x));
  }
}