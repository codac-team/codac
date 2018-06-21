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
  }

  Polygon::Polygon(const vector<Point>& v_points)
  {
    for(int i = 0 ; i < v_points.size() ; i++)
      if(v_points[i] != v_points[(i + 1) % v_points.size()])
        m_v_vertices.push_back(v_points[i]);
  }

  int Polygon::nbVertices() const
  {
    return m_v_vertices.size();
  }

  vector<Point> Polygon::getVertices() const
  {
    return m_v_vertices;
  }

  const Point Polygon::operator[](int vertex_id) const
  {
    if(vertex_id < 0 || vertex_id >= nbVertices())
      throw Exception("Polygon::operator[]", "invalid vertex id");
    return m_v_vertices[vertex_id];
  }

  IntervalVector Polygon::box() const
  {
    #ifdef _TUBES_POLYG_WITH_BOOST_

      boostpolygon bp = create_boostpolygon(*this);

      IntervalVector iv_box(2);
      boost::geometry::model::box<boostpoint> box;
      boost::geometry::envelope(bp, box);
      iv_box[0] = Interval(get<0>(box.min_corner()), get<0>(box.max_corner()));
      iv_box[1] = Interval(get<1>(box.min_corner()), get<1>(box.max_corner()));
      return iv_box;

    #endif
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
    #ifdef _TUBES_POLYG_WITH_BOOST_

      boostpolygon bp1 = create_boostpolygon(*this);
      boostpolygon bp2 = create_boostpolygon(p);
      return boost::geometry::equals(bp1, bp2);

    #endif
  }

  bool Polygon::operator!=(const Polygon& p) const
  {
    return !(*this == p);
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
}


#ifdef _TUBES_POLYG_WITH_BOOST_

  boostpoint create_boostpoint(tubex::Point p)
  {
    boostpoint bpt;
    double x, y;
    x = (p.x);// == POS_INFINITY ? BOUNDED_POS_INFINITY : p.x);
    x = (p.x);// == NEG_INFINITY ? BOUNDED_NEG_INFINITY : p.x);
    y = (p.y);// == POS_INFINITY ? BOUNDED_POS_INFINITY : p.y);
    y = (p.y);// == NEG_INFINITY ? BOUNDED_NEG_INFINITY : p.y);
    return boostpoint(x,y);
  }

  boostpolygon create_boostpolygon(const tubex::Polygon& p)
  {
    boostpolygon bp;
    std::vector<boostpoint> v_bpts;

    for(int i = 0 ; i < p.nbVertices() ; i++)
      v_bpts.push_back(create_boostpoint(p[i]));
    v_bpts.push_back(create_boostpoint(p[0])); // closing set (boost standard)
    
    boost::geometry::assign_points(bp, v_bpts);
    return bp;
  }

  tubex::Polygon create_polygon(const boostpolygon& bp)
  {

  }

#endif