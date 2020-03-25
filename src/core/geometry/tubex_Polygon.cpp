/** 
 *  Polygon class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iostream>
#include "tubex_Polygon.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Polygon::Polygon()
  {

  }

  Polygon::Polygon(const IntervalVector& box)
  {
    assert(box.size() == 2);
    push_points(box, m_v_vertices);
  }

  Polygon::Polygon(const vector<Point>& v_points)
  {
    m_v_vertices = v_points;
    //m_v_vertices = Polygon::delete_redundant_points(m_v_vertices);
  }

  int Polygon::nb_vertices() const
  {
    return m_v_vertices.size();
  }

  const vector<Point>& Polygon::vertices() const
  {
    return m_v_vertices;
  }

  const vector<Edge> Polygon::edges() const
  {
    vector<Edge> v_edges;
    for(size_t i = 0 ; i < m_v_vertices.size() ; i++)
      v_edges.push_back(Edge(m_v_vertices[i], m_v_vertices[(i+1) % m_v_vertices.size()]));
    return v_edges;
  }

  const Point Polygon::operator[](int vertex_id) const
  {
    assert(vertex_id >= 0 && vertex_id < (int)m_v_vertices.size());
    return m_v_vertices[vertex_id];
  }

  const IntervalVector Polygon::box() const
  {
    IntervalVector box(2, Interval::EMPTY_SET);
    for(size_t i = 0 ; i < m_v_vertices.size() ; i++)
    {
      box[0] |= m_v_vertices[i].x();
      box[1] |= m_v_vertices[i].y();
    }
    return box;
  }

  bool Polygon::is_point() const
  {
    return nb_vertices() == 1;
  }

  bool Polygon::is_segment() const
  {
    return nb_vertices() == 2;
  }

  const BoolInterval Polygon::is_convex() const
  {
    int n = m_v_vertices.size();
    bool all_positive = true, all_negative = true;

    for(int i = 0 ; i < n ; i++)
    {
      Interval dx1 = m_v_vertices[(i+1)%n].x() - m_v_vertices[i].x();
      Interval dy1 = m_v_vertices[(i+1)%n].y() - m_v_vertices[i].y();
      Interval dx2 = m_v_vertices[(i+2)%n].x() - m_v_vertices[(i+1)%n].x();
      Interval dy2 = m_v_vertices[(i+2)%n].y() - m_v_vertices[(i+1)%n].y();

      Interval z_cross_product = dx1*dy2 - dy1*dx2;
      assert(!z_cross_product.interior_contains(0.));

      // If points are aligned with some uncertainty
      if(z_cross_product.contains(0.) && !z_cross_product.is_degenerated())
        return MAYBE;

      else if(z_cross_product == 0.) // perfect alignment
        continue;

      else
      {
        all_positive &= z_cross_product.lb() >= 0.;
        all_negative &= z_cross_product.ub() <= 0.;
        assert(!(all_positive && all_negative));

        if((all_positive || all_negative) == false)
          return NO;
      }
    }

    return YES; // same sign for each computed cross product
  }

  bool Polygon::operator==(const Polygon& p) const
  {
    int n = m_v_vertices.size();
    if(n != (int)p.m_v_vertices.size())
      return false;

    int i; // looking for first same elements
    for(i = 0 ; i < n ; i++)
      if(m_v_vertices[0] == p.m_v_vertices[i])
        break;

    int way = 1;
    if(n > 1)
      way = (m_v_vertices[1] == p.m_v_vertices[(i+1) % n]) ? 1 : -1;

    for(int j = 0 ; j < n ; j++)
      if(m_v_vertices[j] != p.m_v_vertices[(i+way*j+n) % n])
        return false;

    // todo: test undefined case

    return true;
  }

  bool Polygon::operator!=(const Polygon& p) const
  {
    int n = m_v_vertices.size();
    if(n != (int)p.m_v_vertices.size())
      return true;

    int i; // looking for first same elements
    for(i = 0 ; i < n ; i++)
      if(m_v_vertices[0] == p.m_v_vertices[i])
        break;

    int way = 1;
    if(n > 1)
      way = (m_v_vertices[1] == p.m_v_vertices[(i+1) % n]) ? 1 : -1;

    for(int j = 0 ; j < n ; j++)
      if(m_v_vertices[j] != p.m_v_vertices[(i+way*j+n) % n])
        return true;

    // todo: test undefined case

    return false;
  }
  
  const BoolInterval Polygon::encloses(const Point& p) const
  {
    if(p.does_not_exist() || does_not_exist())
      return NO;

    if(!p.box().intersects(box()))
      return NO; // fast test

    // Using the ray tracing method:
    //   A ray is defined from p to the right ; if it crosses
    //   'a' times one of the edges, and (a & 1), then p is inside

    vector<Edge> v_edges = edges();
    int a = 0; // the crossing number counter
    int n = v_edges.size();
    Edge ray(p, Point(box()[0].ub() + 1., p.y())); // horizontal edge to the right
    Point prev_e = v_edges[n-1] & ray;

    // Loop through all edges of the polygon, looking for intersections
    for(int i = 0 ; i < n ; i++)
    {
      if(!v_edges[i].box()[1].intersects(p.y())) // no feasible intersection
        continue;

      // Intersecting point
      Point e = v_edges[i] & ray;
      if(!e.does_not_exist()) // intersection to the left of p, not considered
      {
        if(e.x().intersects(p.x()))
          return MAYBE; // uncertainty

        if(e.y().intersects(box()[1].lb()) || e.y().intersects(box()[1].ub()))
          continue; // the ray is horizontaly tangent to the polygon

        if(prev_e.x().intersects(e.x()))
          continue; // point already dealt

        if(e.x().lb() > p.x().ub()) // intersection on the right
          a++;
      }

      prev_e = e;
    }

    return (a & 1) ? YES : NO;
  }

  bool Polygon::does_not_exist() const
  {
    if(m_v_vertices.size() == 0)
      return true;
    for(size_t i = 0 ; i < m_v_vertices.size() ; i++)
      if(m_v_vertices[i].does_not_exist())
        return true;
    return false;
  }

  ostream& operator<<(ostream& str, const Polygon& p)
  {
    str << "{";

    if(p.nb_vertices() == 0 || p.does_not_exist())
      str << " undefined ";

    else
    {
      for(int i = 0 ; i < p.nb_vertices() ; i++)
      {
        if(i != 0) str << ",";
        str << p.vertices()[i];
      }
    }

    str << "}";
    return str;
  }

  void Polygon::rotate(double angle)
  {
    rotate(angle, Point::center(m_v_vertices));
  }

  void Polygon::rotate(double angle, const Point& center)
  {
    for(size_t i = 0 ; i < m_v_vertices.size() ; i++)
    {
      Interval dx = m_v_vertices[i].x() - center.x();
      Interval dy = m_v_vertices[i].y() - center.y();

      m_v_vertices[i] = Point(center.x() + dx*cos(angle) - dy*sin(angle),
                              center.y() + dx*sin(angle) + dy*cos(angle));
    }
  }

  const Polygon& Polygon::inflate_vertices(double rad)
  {
    assert(rad >= 0.);
    for(size_t i = 0 ; i < m_v_vertices.size() ; i++)
      m_v_vertices[i].inflate(rad);
    return *this;
  }

  void Polygon::merge_close_vertices()
  {
    m_v_vertices = Point::merge_close_points(m_v_vertices);
  }

  const vector<Point> Polygon::delete_redundant_points(const vector<Point>& v_pts)
  {
    vector<Point> v_vertices;

    for(size_t i = 0 ; i < v_pts.size() ; )
    {
      if(v_pts[i].does_not_exist())
        continue; // no empty points

      v_vertices.push_back(v_pts[i]);

      size_t j;
      for(j = i+1 ; j < v_vertices.size() ; j++)
        if(!(v_pts[i] == v_vertices[j]
            && v_pts[i].x().is_degenerated()
            && v_pts[i].y().is_degenerated()))
          break; // until next different point

      i = j;
    }

    return v_vertices;
  }
}