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
    pushPoints(box, m_v_vertices);
    deleteRedundantPoints();
  }

  Polygon::Polygon(const vector<Point>& v_points)
  {
    for(int i = 0 ; i < v_points.size() ; i++)
      m_v_vertices.push_back(v_points[i]);
    deleteRedundantPoints();
  }

  int Polygon::nbVertices() const
  {
    return m_v_vertices.size();
  }

  vector<Point> Polygon::getVertices() const
  {
    return m_v_vertices;
  }

  vector<Edge> Polygon::getEdges() const
  {
    vector<Edge> v_edges;
    for(int i = 0 ; i < m_v_vertices.size() ; i++)
      v_edges.push_back(Edge(m_v_vertices[i], m_v_vertices[(i+1) % m_v_vertices.size()]));
    return v_edges;
  }

  const Point Polygon::operator[](int vertex_id) const
  {
    if(vertex_id < 0 || vertex_id >= nbVertices())
      throw Exception("Polygon::operator[]", "invalid vertex id");
    return m_v_vertices[vertex_id];
  }

  IntervalVector Polygon::box() const
  {
    IntervalVector box(2, Interval::EMPTY_SET);
    for(int i = 0 ; i < m_v_vertices.size() ; i++)
      box |= m_v_vertices[i].box();
    return box;
  }

  bool Polygon::isPoint() const
  {
    return nbVertices() == 1;
  }

  bool Polygon::isSegment() const
  {
    return nbVertices() == 2;
  }

  bool Polygon::operator==(const Polygon& p) const
  {
    if(m_v_vertices.size() != p.m_v_vertices.size())
      return false;

    int i; // looking for first same elements
    for(i = 0 ; i < m_v_vertices.size() ; i++)
      if(m_v_vertices[0] == p.m_v_vertices[i])
        break;

    for(int j = 0 ; j < m_v_vertices.size() ; j++)
      if(m_v_vertices[j] != p.m_v_vertices[(j+i) % p.m_v_vertices.size()])
        return false;

    return true;
  }

  bool Polygon::operator!=(const Polygon& p) const
  {
    if(m_v_vertices.size() != p.m_v_vertices.size())
      return true;

    int i; // looking for first same elements
    for(i = 0 ; i < m_v_vertices.size() ; i++)
      if(m_v_vertices[0] == p.m_v_vertices[i])
        break;

    for(int j = 0 ; j < m_v_vertices.size() ; j++)
      if(m_v_vertices[j] != p.m_v_vertices[(j+i) % p.m_v_vertices.size()])
        return true;

    return false;
  }

  ostream& operator<<(ostream& str, const Polygon& p)
  {
    str << "{";

    if(p.nbVertices() == 0)
      str << " empty ";

    else
    {
      for(int i = 0 ; i < p.nbVertices() ; i++)
      {
        if(i != 0) str << ",";
        str << "(" << p.getVertices()[i].t << "," << p.getVertices()[i].x << ")";
      }
    }

    str << "}";
    return str;
  }

  void Polygon::deleteRedundantPoints()
  {
    vector<Point> v_vertices;
    for(int i = 0 ; i < m_v_vertices.size() ; i++)
      if(m_v_vertices[i].x.is_unbounded()
        || m_v_vertices[i] != m_v_vertices[(i+1) % m_v_vertices.size()])
        v_vertices.push_back(m_v_vertices[i]);
    m_v_vertices = v_vertices;
  }
}