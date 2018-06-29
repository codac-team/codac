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
#include "tubex_Edge.h"

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
      std::vector<Edge> getEdges() const;
      const Point operator[](int vertex_id) const;
      ibex::IntervalVector box() const;

      bool isPoint() const;
      bool isSegment() const;
      bool operator==(const Polygon& p) const;
      bool operator!=(const Polygon& p) const;
      bool encloses(const Point& p) const;

      friend std::ostream& operator<<(std::ostream& str, const Polygon& p);

    protected:

      void deleteRedundantPoints();
      
      std::vector<Point> m_v_vertices;
  };
}

#endif