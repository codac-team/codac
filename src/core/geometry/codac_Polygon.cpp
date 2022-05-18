/** 
 *  Polygon class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <iostream>
#include <iomanip>
#include "codac_IntervalMatrix.h"
#include "codac_Polygon.h"
#include "codac_GrahamScan.h"
#include "codac_VIBesFig.h"

using namespace std;
using namespace ibex;

namespace codac
{
  // Definition

  Polygon::Polygon()
  {

  }

  Polygon::Polygon(const Polygon& p)
    : m_v_floating_pts(p.m_v_floating_pts)
  {

  }
  
  Polygon::Polygon(const vector<Vector>& v_floating_pts)
    : m_v_floating_pts(v_floating_pts)
  {
    
  }


  // Accessing values

  int Polygon::size() const
  {
    return 2; // for now, only 2d polygons are supported
  }

  int Polygon::nb_edges() const
  {
    return m_v_floating_pts.size();
  }

  int Polygon::nb_vertices() const
  {
    return m_v_floating_pts.size();
  }

  const vector<ThickEdge> Polygon::edges() const
  {
    size_t n = m_v_floating_pts.size();
    vector<ThickEdge> v_edges(n,ThickEdge(ThickPoint(),ThickPoint()));
    for(size_t i = 0 ; i < n ; i++)
      v_edges[i] = ThickEdge(ThickPoint(m_v_floating_pts[i]), ThickPoint(m_v_floating_pts[(i+1)%n]));
    return v_edges;
  }

  const vector<Vector>& Polygon::vertices() const
  {
    return m_v_floating_pts;
  }

  const Vector& Polygon::operator[](size_t vertex_id) const
  {
    assert(vertex_id >= 0 && vertex_id < m_v_floating_pts.size());
    return m_v_floating_pts[vertex_id];
  }

  const IntervalVector Polygon::box() const
  {
    IntervalVector box(2, Interval::EMPTY_SET);
    for(const auto& pt : m_v_floating_pts)
      box |= pt;
    return box;
  }

  const ThickPoint Polygon::center() const
  {
    IntervalVector center(2, 0.);
    for(const auto& pt : m_v_floating_pts)
      center += pt;
    center *= (1. / m_v_floating_pts.size());
    return ThickPoint(center);;
  }

  const Interval Polygon::area() const
  {
    Interval a(0.);
    size_t n = m_v_floating_pts.size();

    for(size_t i = 0 ; i < n ; i++)
    {
      const Vector& pi = m_v_floating_pts[i];
      const Vector& pip1 = m_v_floating_pts[(i+1)%n];

      a += pi[0]*pip1[1] - pip1[0]*pi[1];
    }

    return 0.5*a;
  }


  // Tests

  bool Polygon::is_empty() const
  {
    return m_v_floating_pts.size() == 0;
  }

  bool Polygon::is_point() const
  {
    return m_v_floating_pts.size() == 1;
  }

  bool Polygon::is_segment() const
  {
    return m_v_floating_pts.size() == 2;
  }

  bool Polygon::operator==(const Polygon& p) const
  {
    size_t n = m_v_floating_pts.size();
    if(n != p.m_v_floating_pts.size())
      return false;

    size_t i; // looking for same reference of first value
    for(i = 0 ; i < n ; i++)
      if(m_v_floating_pts[0] == p.m_v_floating_pts[i])
        break;

    size_t way = 1;
    if(n > 1)
      way = (m_v_floating_pts[1] == p.m_v_floating_pts[(i+1)%n]) ? 1 : -1;

    for(size_t j = 0 ; j < n ; j++)
      if(m_v_floating_pts[j] != p.m_v_floating_pts[(i+way*j+n)%n])
        return false;

    // todo: test undefined case

    return true;
  }

  bool Polygon::operator!=(const Polygon& p) const
  {
    size_t n = m_v_floating_pts.size();
    if(n != p.m_v_floating_pts.size())
      return true;

    size_t i; // looking for same reference of first value
    for(i = 0 ; i < n ; i++)
      if(m_v_floating_pts[0] == p.m_v_floating_pts[i])
        break;

    size_t way = 1;
    if(n > 1)
      way = (m_v_floating_pts[1] == p.m_v_floating_pts[(i+1)%n]) ? 1 : -1;

    for(size_t j = 0 ; j < n ; j++)
      if(m_v_floating_pts[j] != p.m_v_floating_pts[(i+way*j+n)%n])
        return true;

    // todo: test undefined case

    return false;
  }


  // String

  ostream& operator<<(ostream& str, const Polygon& p)
  {
    str << "{";

    if(p.nb_vertices() == 0 || p.is_empty())
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
}