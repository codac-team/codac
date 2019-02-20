/* ============================================================================
 *  tubex-lib - Polygon class
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
#include "tubex_Polygon.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Polygon::Polygon(const IntervalVector& box)
  {
    assert(box.size() == 2);
    push_points(box, m_v_vertices);
    delete_redundant_points();
  }

  Polygon::Polygon(const vector<Point>& v_points)
  {
    assert(!v_points.empty());
    for(int i = 0 ; i < v_points.size() ; i++)
      m_v_vertices.push_back(v_points[i]);
    delete_redundant_points();
  }

  int Polygon::nb_vertices() const
  {
    return m_v_vertices.size();
  }

  const vector<Point>& Polygon::get_vertices() const
  {
    return m_v_vertices;
  }

  const vector<Edge> Polygon::get_edges() const
  {
    vector<Edge> v_edges;
    for(int i = 0 ; i < m_v_vertices.size() ; i++)
      v_edges.push_back(Edge(m_v_vertices[i], m_v_vertices[(i+1) % m_v_vertices.size()]));
    return v_edges;
  }

  const Point Polygon::operator[](int vertex_id) const
  {
    assert(vertex_id >= 0 && vertex_id < m_v_vertices.size());
    return m_v_vertices[vertex_id];
  }

  const IntervalVector Polygon::box() const
  {
    IntervalVector box(2, Interval::EMPTY_SET);
    for(int i = 0 ; i < m_v_vertices.size() ; i++)
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

  bool Polygon::operator==(const Polygon& p) const
  {
    int n = m_v_vertices.size();
    if(n != p.m_v_vertices.size())
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

    return true;
  }

  bool Polygon::operator!=(const Polygon& p) const
  {
    int n = m_v_vertices.size();
    if(n != p.m_v_vertices.size())
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

    return false;
  }
  
  const BoolInterval Polygon::encloses(const Point& p) const
  {
    // Using the ray tracing method:
    //   A ray is defined from p to the right ; if it crosses
    //   'a' times one of the edges, and (a & 1), then p is inside

    vector<Edge> v_edges = get_edges();
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

      if(e.x().intersects(p.x()))
        return MAYBE; // uncertainty

      if(e.y().intersects(box()[1].lb()) || e.y().intersects(box()[1].ub()))
        continue; // the ray is horizontaly tangent to the polygon

      if(prev_e.x().intersects(e.x()))
        continue; // point already dealt
      
      prev_e = e;

      if(e.x().lb() > p.x().ub()) // intersection on the right
        a++;
    }

    return (a & 1) ? YES : NO;
  }

  ostream& operator<<(ostream& str, const Polygon& p)
  {
    str << "{";

    if(p.nb_vertices() == 0)
      str << " empty ";

    else
    {
      for(int i = 0 ; i < p.nb_vertices() ; i++)
      {
        if(i != 0) str << ",";
        str << p.get_vertices()[i];
      }
    }

    str << "}";
    return str;
  }

  void Polygon::delete_redundant_points()
  {
    vector<Point> v_vertices;
    int n = m_v_vertices.size();

    for(int i = 0 ; i < n ; i++)
    {
      if(m_v_vertices[i].box().is_unbounded())
      {
        v_vertices.push_back(m_v_vertices[i]);
        continue;
      }

      bool diff_from_all_prev_points = true;
      for(int j = i-1 ; j >= 0 ; j--)
        diff_from_all_prev_points &= m_v_vertices[i] != m_v_vertices[j];
      
      if(diff_from_all_prev_points)
        v_vertices.push_back(m_v_vertices[i]);
    }

    m_v_vertices = v_vertices;
  }
}