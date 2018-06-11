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

#ifndef Polygon_HEADER
#define Polygon_HEADER

#include <vector>
#include "ibex_IntervalVector.h"
#include "tubex_Point.h"

//#define _TUBES_POLYG_WITH_BOOST_ 0

namespace tubex
{
  class Polygon
  {
    public:

      Polygon();
      Polygon(const ibex::IntervalVector& box);
      Polygon(const std::vector<Point>& v_points);

      int nbVertices() const;
      std::vector<Point> getVertices() const;
      const Point operator[](int vertex_id) const;
      ibex::IntervalVector box() const;

      bool isPoint() const;
      bool isSegment() const;
      bool operator==(const Polygon& p) const;
      bool operator!=(const Polygon& p) const;

      friend std::ostream& operator<<(std::ostream& str, const Polygon& p);

    protected:
      
      std::vector<Point> m_v_vertices;
  };
}

#endif

#ifdef _TUBES_POLYG_WITH_BOOST_
  
  #include <deque>
  #include <boost/geometry.hpp>
  #include <boost/foreach.hpp>
  #include <boost/geometry/geometries/polygon.hpp>
  #include <boost/geometry/geometries/segment.hpp>
  #include <boost/geometry/geometries/point_xy.hpp>

  using boost::geometry::get;
  typedef boost::geometry::model::d2::point_xy<double> boostpoint;
  typedef boost::geometry::model::polygon<boostpoint> boostpolygon;
  typedef boost::geometry::model::segment<boostpoint> boostsegment;

  #include <limits>
  #define BOUNDED_NEG_INFINITY -9. //numeric_limits<float>::lowest()+1
  #define BOUNDED_POS_INFINITY 9. //numeric_limits<float>::max()-1

  // Related functions
  boostpoint create_boostpoint(tubex::Point p);
  boostpolygon create_boostpolygon(const tubex::Polygon& p);
  tubex::Polygon create_polygon(const boostpolygon& bp);

#endif