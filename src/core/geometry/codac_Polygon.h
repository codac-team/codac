/** 
 *  Polygon class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_POLYGON_H__
#define __CODAC_POLYGON_H__

#include <vector>
#include "codac_Vector.h"
#include "codac_IntervalVector.h"
#include "codac_ThickEdge.h"
#include "codac_ThickPoint.h"

namespace codac
{
  class Polygon
  {
    public:

      /// \name Definition
      /// @{

        Polygon();
        Polygon(const Polygon& p);
        Polygon(const std::vector<Vector>& v_floating_pts);

      /// @}
      /// \name Accessing values
      /// @{

        int size() const;
        int nb_edges() const;
        int nb_vertices() const;
        const std::vector<ThickEdge> edges() const;
        const std::vector<Vector>& vertices() const;
        const Vector& operator[](size_t vertex_id) const;
        const IntervalVector box() const;
        const ThickPoint center() const;
        const Interval area() const;

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
      
      std::vector<Vector> m_v_floating_pts;
  };
}

#endif