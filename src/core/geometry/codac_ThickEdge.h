/** 
 *  ThickEdge class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_THICKEDGE_H__
#define __CODAC_THICKEDGE_H__

#include "codac_Vector.h"
#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_BoolInterval.h"
#include "codac_ThickPoint.h"

namespace codac
{
  class ThickEdge
  {
    public:

      /// \name Definition
      /// @{

        ThickEdge(const ThickPoint& p1, const ThickPoint& p2);
        ThickEdge(const Vector& p1, const Vector& p2);
        const ThickEdge& operator=(const ThickEdge& e);

      /// @}
      /// \name Accessing values
      /// @{

        const ThickPoint& p1() const;
        const ThickPoint& p2() const;
        const Interval length() const;
        const IntervalVector box() const;

      /// @}
      /// \name Tests
      /// @{

        const BoolInterval is_horizontal() const;
        const BoolInterval is_vertical() const;
        const BoolInterval is_degenerated() const;
        bool does_not_exist() const;
        const BoolInterval contains(const ThickPoint& p) const;
        bool operator==(const ThickEdge& e) const;
        bool operator!=(const ThickEdge& e) const;

      /// @}
      /// \name Operators
      /// @{

        const IntervalVector operator&(const IntervalVector& x) const;
        const ThickPoint operator&(const ThickEdge& e) const;
        static const ThickPoint proj_intersection(const ThickEdge& e1, const ThickEdge& e2);

      /// @}
      /// \name String
      /// @{

        friend std::ostream& operator<<(std::ostream& str, const ThickEdge& e);

      /// @}
      /// \name Static methods
      /// @{

        static const BoolInterval parallel(const ThickEdge& e1, const ThickEdge& e2);
        static void push(const IntervalVector& box, std::vector<ThickEdge>& v_edges);

      /// @}


    protected:

      ThickPoint m_pts[2];
  };
}

#endif