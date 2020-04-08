/** 
 *  Polygon class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_POLYGON_H__
#define __TUBEX_POLYGON_H__

#include <vector>
#include "ibex_IntervalVector.h"
#include "ibex_BoolInterval.h"
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

      int size() const;
      int nb_vertices() const;
      const std::vector<Point>& vertices() const;
      const std::vector<Edge> edges() const;
      const Point operator[](int vertex_id) const; // todo: use size_t
      const ibex::IntervalVector box() const;

      bool is_point() const;
      bool is_segment() const;
      const ibex::BoolInterval is_convex() const;
      bool operator==(const Polygon& p) const;
      bool operator!=(const Polygon& p) const;
      const ibex::BoolInterval encloses(const Point& p) const;
      bool does_not_exist() const;
      bool operator()(const Point& p1, const Point& p2);

      void rotate(const ibex::Interval& theta);
      void rotate(const ibex::Interval& theta, const Point& center);

      const Polygon& inflate_vertices(double rad);
      void merge_close_vertices();

      friend std::ostream& operator<<(std::ostream& str, const Polygon& p);
      
      static const std::vector<Point> delete_redundant_points(const std::vector<Point>& v_pts);

    protected:
      
      std::vector<Point> m_v_vertices;
  };
}

#endif