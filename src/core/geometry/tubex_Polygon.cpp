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
#include "tubex_Exception.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Polygon::Polygon()
  {

  }

  Polygon::Polygon(const IntervalVector& box)
  {
    push_points(box, m_v_vertices);
    delete_redundant_points();
  }

  Polygon::Polygon(const vector<Point>& v_points)
  {
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
    if(vertex_id < 0 || vertex_id >= nb_vertices())
      throw Exception("Polygon::operator[]", "invalid vertex id");
    return m_v_vertices[vertex_id];
  }

  const IntervalVector Polygon::box() const
  {
    IntervalVector box(2, Interval::EMPTY_SET);
    for(int i = 0 ; i < m_v_vertices.size() ; i++)
    {
      box[0] |= m_v_vertices[i].t();
      box[1] |= m_v_vertices[i].x();
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
  
  bool Polygon::encloses(const Point& p) const
  {
    // Note: code patterned after [Franklin, 2000]

    int cn = 0; // the  crossing number counter
    vector<Point> v_vertices = get_vertices();
    int n = v_vertices.size();

    // Loop through all edges of the polygon
    for(int i = 0 ; i < n ; i++)
    {
      if(p.box().is_subset(v_vertices[i].box()))
        return true;

      // Edge from v[i] to v[i+1]
      if(((v_vertices[i].x().mid() <= p.x().mid()) && (v_vertices[(i+1)%n].x().mid() > p.x().mid())) // an upward crossing
      || ((v_vertices[i].x().mid() > p.x().mid()) && (v_vertices[(i+1)%n].x().mid() <= p.x().mid()))) // a downward crossing
      {
        // Compute  the actual edge-ray intersect t-coordinate
        float vt = (float)(p.x().mid() - v_vertices[i].x().mid()) / (v_vertices[(i+1)%n].x().mid() - v_vertices[i].x().mid());
        if(p.t().mid() < v_vertices[i].t().mid() + vt * (v_vertices[(i+1)%n].t().mid() - v_vertices[i].t().mid())) // p.t() < intersect
          ++cn; // a valid crossing of x=p.x() right of p.t()
      }
    }

    return (cn & 1); // 0 if even (out), and 1 if odd (in)
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
    for(int i = 0 ; i < m_v_vertices.size() ; i++)
      if(m_v_vertices[i].x().is_unbounded()
        || m_v_vertices[i] != m_v_vertices[(i+1) % m_v_vertices.size()])
        v_vertices.push_back(m_v_vertices[i]);
    m_v_vertices = v_vertices;
  }
}