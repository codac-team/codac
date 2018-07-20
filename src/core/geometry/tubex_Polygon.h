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
#include "tubex_Point.h"
#include "tubex_Edge.h"

namespace tubex
{
  class Polygon
  {
    public:

      Polygon();
      Polygon(const ibex::IntervalVector& box);
      Polygon(const std::vector<Point>& v_points);

      int nb_vertices() const;
      const std::vector<Point>& get_vertices() const;
      const std::vector<Edge> get_edges() const;
      const Point operator[](int vertex_id) const;
      const ibex::IntervalVector box() const;

      bool is_point() const;
      bool is_segment() const;
      bool operator==(const Polygon& p) const;
      bool operator!=(const Polygon& p) const;
      bool encloses(const Point& p) const;

      friend std::ostream& operator<<(std::ostream& str, const Polygon& p);

    protected:

      void delete_redundant_points();
      
      std::vector<Point> m_v_vertices;
  };
}

#endif