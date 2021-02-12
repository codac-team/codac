/** 
 *  Edge class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_EDGE_H__
#define __CODAC_EDGE_H__

#include "codac_Vector.h"
#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_BoolInterval.h"
#include "codac_Point.h"

namespace codac
{
  class Edge
  {
    public:

      /// \name Definition
      /// @{

        Edge(const Point& p1, const Point& p2);
        Edge(const Vector& p1, const Vector& p2);
        const Edge& operator=(const Edge& e);

      /// @}
      /// \name Accessing values
      /// @{

        const Point& p1() const;
        const Point& p2() const;
        const Interval length() const;
        const IntervalVector box() const;

      /// @}
      /// \name Tests
      /// @{

        const BoolInterval is_horizontal() const;
        const BoolInterval is_vertical() const;
        const BoolInterval is_degenerated() const;
        bool does_not_exist() const;
        const BoolInterval contains(const Point& p) const;
        bool operator==(const Edge& e) const;
        bool operator!=(const Edge& e) const;

      /// @}
      /// \name Operators
      /// @{

        const IntervalVector operator&(const IntervalVector& x) const;
        const Point operator&(const Edge& e) const;
        static const Point proj_intersection(const Edge& e1, const Edge& e2);

      /// @}
      /// \name String
      /// @{

        friend std::ostream& operator<<(std::ostream& str, const Edge& e);

      /// @}
      /// \name Static methods
      /// @{

        static const BoolInterval parallel(const Edge& e1, const Edge& e2);
        static void push(const IntervalVector& box, std::vector<Edge>& v_edges);

      /// @}


    protected:

      Point m_pts[2];
  };
}

#endif