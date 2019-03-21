/** 
 *  ConvexPolygon class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iostream>
#include <iomanip>
#include "tubex_ConvexPolygon.h"
#include "tubex_GrahamScan.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  ConvexPolygon::ConvexPolygon() : Polygon()
  {

  }

  ConvexPolygon::ConvexPolygon(const IntervalVector& box) : Polygon(box)
  {
    assert(box.size() == 2);
    m_v_vertices = GrahamScan::convex_hull(m_v_vertices);
  }
  
  ConvexPolygon::ConvexPolygon(const std::vector<Point>& v_points) : Polygon(v_points)
  {
    vector<Point> v_pts;
    for(int i = 0 ; i < m_v_vertices.size() ; i++) // uncertain points are cut
    {
      /*if(m_v_vertices[i].x().is_degenerated() || m_v_vertices[i].y().is_degenerated())
      {
        v_pts.push_back(Point(m_v_vertices[i].x().lb(), m_v_vertices[i].y().lb()));
        v_pts.push_back(Point(m_v_vertices[i].x().ub(), m_v_vertices[i].y().lb()));
        v_pts.push_back(Point(m_v_vertices[i].x().ub(), m_v_vertices[i].y().ub()));
        v_pts.push_back(Point(m_v_vertices[i].x().lb(), m_v_vertices[i].y().ub()));
      }

      else*/
        v_pts.push_back(m_v_vertices[i]);
    }

    v_pts = Point::delete_redundant_points(v_pts);
    m_v_vertices = GrahamScan::convex_hull(v_pts);
  }
  
  const IntervalVector ConvexPolygon::operator&(const IntervalVector& x) const
  {
    // todo: keep this?

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
          // If edges are possibly parallel:
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

      vector<Point> merged_points = Point::merge_close_points(v_pts);
      merged_points = GrahamScan::convex_hull(merged_points);
      //merged_points = Point::delete_redundant_points(merged_points); // todo: keep this?

      // todo: two convex computations are performed: do only one

    return ConvexPolygon(merged_points);
  }
  
  const ConvexPolygon ConvexPolygon::intersect(const ConvexPolygon& p, const ibex::IntervalVector& x)
  {
    assert(x.size() == 2);
    return ConvexPolygon(intersect(p, ConvexPolygon(x)).vertices());
  }

  void ConvexPolygon::simplify()
  {
    vector<Point> v_pts = m_v_vertices;
    Point c = Point::center(v_pts);

    ConvexPolygon p(v_pts);
    ConvexPolygon pf(box());

    double da = M_PI/6.;
    for(double a = da ; a < M_PI ; a+=da)
    {
      ConvexPolygon p1 = p;
      p1.rotate(a, c);

      IntervalVector hull = p1.box();

      ConvexPolygon p2(hull);
      p2.rotate(-a, c);

      pf = ConvexPolygon::intersect(pf, p2);
    }

    m_v_vertices = GrahamScan::convex_hull(pf.vertices());
  }
}