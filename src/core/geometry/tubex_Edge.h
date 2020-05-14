/** 
 *  Edge class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_EDGE_H__
#define __TUBEX_EDGE_H__

#include "ibex_Vector.h"
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_BoolInterval.h"
#include "tubex_Point.h"

namespace tubex
{
  class Edge
  {
    public:

      /// \name Definition
      /// @{

        Edge(const Point& p1, const Point& p2);
        Edge(const ibex::Vector& p1, const ibex::Vector& p2);
        const Edge& operator=(const Edge& e);

      /// @}
      /// \name Accessing values
      /// @{

        const Point& p1() const;
        const Point& p2() const;
        const ibex::Interval length() const;
        const ibex::IntervalVector box() const;

      /// @}
      /// \name Tests
      /// @{

        const ibex::BoolInterval is_horizontal() const;
        const ibex::BoolInterval is_vertical() const;
        const ibex::BoolInterval is_degenerated() const;
        bool does_not_exist() const;
        const ibex::BoolInterval contains(const Point& p) const;
        bool operator==(const Edge& e) const;
        bool operator!=(const Edge& e) const;

      /// @}
      /// \name Operators
      /// @{

        const ibex::IntervalVector operator&(const ibex::IntervalVector& x) const;
        const Point operator&(const Edge& e) const;
        static const Point proj_intersection(const Edge& e1, const Edge& e2);

      /// @}
      /// \name String
      /// @{

        friend std::ostream& operator<<(std::ostream& str, const Edge& e);

      /// @}
      /// \name Static methods
      /// @{

        static const ibex::BoolInterval parallel(const Edge& e1, const Edge& e2);
        static void push(const ibex::IntervalVector& box, std::vector<Edge>& v_edges);

      /// @}


    protected:

      Point m_pts[2];
  };
}

#endif