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

#ifndef __TUBEX_POLYGON_H__
#define __TUBEX_POLYGON_H__

#include <vector>
#include "ibex_IntervalVector.h"
#include "ibex_BoolInterval.h"
#include "tubex_Point.h"
#include "tubex_Edge.h"

namespace tubex
{
  // Note: clockwise polygon
  
  class Polygon
  {
    public:

      Polygon();
      Polygon(const ibex::IntervalVector& box);
      Polygon(const std::vector<Point>& v_points);

      int nb_vertices() const;
      const std::vector<Point>& vertices() const;
      const std::vector<Edge> edges() const;
      const Point operator[](int vertex_id) const;
      const ibex::IntervalVector box() const;

      bool is_point() const;
      bool is_segment() const;
      bool operator==(const Polygon& p) const;
      bool operator!=(const Polygon& p) const;
      const ibex::BoolInterval encloses(const Point& p) const;
      bool does_not_exist() const;
      bool operator()(const Point& p1, const Point& p2);

      void rotate(double angle);
      void rotate(double angle, const Point& center);

      const Polygon& inflate_vertices(double rad);
      void merge_close_vertices();

      friend std::ostream& operator<<(std::ostream& str, const Polygon& p);

    protected:
      
      std::vector<Point> m_v_vertices;
  };
}

#endif