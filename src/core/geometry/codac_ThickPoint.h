/** 
 *  ThickPoint class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_THICKPOINT_H__
#define __CODAC_THICKPOINT_H__

#ifdef _MSC_VER
// Enable additional features in math.h.
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // _USE_MATH_DEFINES
#include <math.h>
#endif // _MSC_VER

#include <vector>
#include "codac_Vector.h"
#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_BoolInterval.h"

// todo: remove this (polygons in unbounded case)
#include <limits>
#define BOUNDED_INFINITY numeric_limits<float>::max()

namespace codac
{
  class ThickPoint // todo: derive from IntervalVector?
  {
    public:

      /// \name Definition
      /// @{

        ThickPoint(); // undefined point
        explicit ThickPoint(const Vector& p);
        explicit ThickPoint(const IntervalVector& p);
        ThickPoint(const Interval& x, const Interval& y);
        const ThickPoint& operator=(const ThickPoint& p);
  
      /// @}
      /// \name Accessing values
      /// @{

        const Interval& x() const;
        const Interval& y() const;
        const Interval& operator[](size_t id) const;
        const IntervalVector& box() const;
        const Vector mid() const;
        double max_diam() const;
        const std::vector<Vector> bounds_pts() const;
  
      /// @}
      /// \name Tests
      /// @{

        bool is_unbounded() const;
        bool does_not_exist() const;
        bool operator==(const ThickPoint& p) const;
        bool operator!=(const ThickPoint& p) const;
  
      /// @}
      /// \name Setting values
      /// @{

        const ThickPoint& inflate(double rad);
  
      /// @}
      /// \name String
      /// @{

        friend std::ostream& operator<<(std::ostream& str, const ThickPoint& p);
  
      /// @}
      /// \name Static methods
      /// @{

        static const BoolInterval aligned(const ThickPoint& a, const ThickPoint& b, const ThickPoint& c);
        static const ThickPoint center(const std::vector<ThickPoint> v_pts);
        static void push(const IntervalVector& box, std::vector<ThickPoint>& v_pts);
        static void push(const IntervalVector& box, std::vector<Vector>& v_pts);
        static std::vector<ThickPoint> to_ThickPoints(const std::vector<Vector>& v_pts);
        static std::vector<ThickPoint> remove_identical_pts(const std::vector<ThickPoint>& v_pts);
        static std::vector<Vector> remove_identical_pts(const std::vector<Vector>& v_pts);

      /// @}


    protected:

      // Reliable representation of points:
      IntervalVector m_pt = IntervalVector(2, Interval::EMPTY_SET);
  };
}

#endif