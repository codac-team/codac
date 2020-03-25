/** 
 *  ConvexPolygon class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iostream>
#include <iomanip>
#include "tubex_ConvexPolygon.h"
#include "tubex_GrahamScan.h"
#include "tubex_VIBesFig.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  ConvexPolygon::ConvexPolygon() : Polygon()
  {

  }

  ConvexPolygon::ConvexPolygon(const ConvexPolygon& p) : Polygon(p.vertices())
  {

  }

  ConvexPolygon::ConvexPolygon(const IntervalVector& box) : Polygon(box)
  {
    assert(box.size() == 2);
    //m_v_vertices = GrahamScan::convex_hull(m_v_vertices);
  }
  
  ConvexPolygon::ConvexPolygon(const std::vector<Point>& v_points, bool convex_points) : Polygon(v_points)
  {
    m_v_vertices = Point::delete_redundant_points(m_v_vertices);
    if(!convex_points)
      m_v_vertices = GrahamScan::convex_hull(m_v_vertices);
    m_v_vertices = Point::merge_close_points(m_v_vertices); // todo: remove this, should be useless
  }
  
  const IntervalVector ConvexPolygon::operator&(const IntervalVector& x) const
  {
    // todo: keep this? is it faster than the generic one?

    assert(x.size() == 2);

    if(does_not_exist())
      return IntervalVector(2, Interval::EMPTY_SET);

    //return intersect(*this, x).box();

    IntervalVector reduced_x = x & box();
    IntervalVector inter(2, Interval::EMPTY_SET);

    vector<Edge> v_edges = edges();
    for(size_t i = 0 ; i < v_edges.size() ; i++)
      inter |= v_edges[i] & reduced_x;

    vector<Point> v_x_vertices;
    push_points(reduced_x, v_x_vertices);
    for(size_t i = 0 ; i < v_x_vertices.size() ; i++)
    {
      if(encloses(v_x_vertices[i]) != NO)
        inter |= v_x_vertices[i].box();
    }

    return inter;
  }
  
  const ConvexPolygon ConvexPolygon::intersect(const ConvexPolygon& p1, const ConvexPolygon& p2)
  {
    vector<Point> v_pts;

    // Add all corners of p1 that are inside p2
    for(size_t i = 0 ; i < p1.vertices().size() ; i++)
      if(p2.encloses(p1.vertices()[i]) != NO)
        v_pts.push_back(p1.vertices()[i]);

    // Add all corners of p2 that are inside p1
    for(size_t i = 0 ; i < p2.vertices().size() ; i++)
      if(p1.encloses(p2.vertices()[i]) != NO)
        v_pts.push_back(p2.vertices()[i]);

    // Add all intersection points
    for(size_t i = 0 ; i < p1.edges().size() ; i++)
      for(size_t j = 0 ; j < p2.edges().size() ; j++)
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
      //merged_points = GrahamScan::convex_hull(merged_points);
      //merged_points = Point::delete_redundant_points(merged_points); // todo: keep this?

    return ConvexPolygon(merged_points);
  }
  
  const ConvexPolygon ConvexPolygon::intersect(const ConvexPolygon& p, const ibex::IntervalVector& x)
  {
    assert(x.size() == 2);
    return intersect(p, ConvexPolygon(x));
  }

  /*void ConvexPolygon::simplify(float n)
  {
    if(n == 0.)
      return;

    vector<Point> v_pts = m_v_vertices;
    Point c = Point::center(v_pts);

    ConvexPolygon p(v_pts);
    ConvexPolygon pf(box());

    double da = M_PI/n;
    for(double a = da ; a < M_PI ; a+=da)
    {
      ConvexPolygon p1 = p;
      p1.rotate(a, c);

      IntervalVector hull = p1.box();

      ConvexPolygon p2(hull);
      p2.rotate(-a, c);

      pf = ConvexPolygon::intersect(pf, p2);
    }

    if(pf.vertices().size() < m_v_vertices.size())
      m_v_vertices = ConvexPolygon(pf.vertices()).vertices();
  }*/

  const Interval surface(const Point& p1, const Point& p2, const Point& p3)
  {
    assert(!p1.does_not_exist() && !p2.does_not_exist() && !p3.does_not_exist());

    // Shoelace formula
    return 0.5*abs(p1.x()*p2.y() + p2.x()*p3.y() + p3.x()*p1.y() - p2.x()*p1.y() - p3.x()*p2.y() - p1.x()*p3.y());
  }

  void ConvexPolygon::simplify(int max_edges)
  {
    assert(max_edges >= 3);

    while((int)m_v_vertices.size() > max_edges)
    {
      // Removing the shortest edge
      int n = m_v_vertices.size();

      // Finding shortest edge
      double min_surf = -1.;
      int min_i = -1;
      Point min_inter;

      for(int i = 0 ; i < n ; i++)
      {
        Edge e1(m_v_vertices[(i-1+n)%n], m_v_vertices[i]);
        Edge e2(m_v_vertices[(i+1)%n], m_v_vertices[(i+2)%n]);

        if(Edge::parallel(e1, e2) == NO)
        {
          // Computing new vertex: intersection of neighbour edges
          Point inter = Edge::proj_intersection(e1, e2);

          if(inter.is_unbounded() || inter.does_not_exist()) // todo: this should not happen
            continue;

          if(GrahamScan::orientation(m_v_vertices[i], inter, m_v_vertices[(i+1)%n]) != OrientationInterval::COUNTERCLOCKWISE)
            continue;

          assert(!inter.does_not_exist() && !inter.is_unbounded());
          double surf = surface(m_v_vertices[i], inter, m_v_vertices[(i+1)%n]).lb();

          if(min_i == -1 || surf < min_surf)
          {
            min_inter = inter;
            min_surf = surf;
            min_i = i;
          }
        }
      }

      if(min_i == -1) // unable to simplify at this step
        break;

      assert(!min_inter.does_not_exist());

      // Updating one of the vertices, removing the other one
      m_v_vertices[min_i] = min_inter;
      m_v_vertices.erase(m_v_vertices.begin() + ((min_i+1)%n));
    }
  }
}