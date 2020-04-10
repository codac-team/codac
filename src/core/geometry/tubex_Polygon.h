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
#include "ibex_Vector.h"
#include "ibex_IntervalVector.h"
#include "tubex_Edge.h"
#include "tubex_Point.h"

namespace tubex
{
  class Polygon
  {
    public:

      /// \name Definition
      /// @{

        Polygon();
        Polygon(const Polygon& p);
        Polygon(const std::vector<ibex::Vector>& v_floating_pts);

      /// @}
      /// \name Accessing values
      /// @{

        int size() const;
        int nb_edges() const;
        int nb_vertices() const;
        const std::vector<Edge> edges() const;
        const std::vector<ibex::Vector>& vertices() const;
        const ibex::Vector& operator[](size_t vertex_id) const;
        const ibex::IntervalVector box() const;
        const Point center() const;

      /// @}
      /// \name Tests
      /// @{

        bool is_empty() const;
        bool is_point() const;
        bool is_segment() const;
        bool operator==(const Polygon& p) const;
        bool operator!=(const Polygon& p) const;

      /// @}
      /// \name String
      /// @{

        friend std::ostream& operator<<(std::ostream& str, const Polygon& p);

      /// @}

      
    protected:
      
      std::vector<ibex::Vector> m_v_floating_pts;
  };
}

#endif