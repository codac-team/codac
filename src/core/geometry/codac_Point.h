/** 
 *  Point class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_POINT_H__
#define __CODAC_POINT_H__

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
  class Point // todo: derive from IntervalVector?
  {
    public:

      /// \name Definition
      /// @{

        Point(); // undefined point
        explicit Point(const Vector& p);
        explicit Point(const IntervalVector& p);
        Point(const Interval& x, const Interval& y);
        const Point& operator=(const Point& p);
  
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
        bool operator==(const Point& p) const;
        bool operator!=(const Point& p) const;
  
      /// @}
      /// \name Setting values
      /// @{

        const Point& inflate(double rad);
  
      /// @}
      /// \name String
      /// @{

        friend std::ostream& operator<<(std::ostream& str, const Point& p);
  
      /// @}
      /// \name Static methods
      /// @{

        static const BoolInterval aligned(const Point& a, const Point& b, const Point& c);
        static const Point center(const std::vector<Point> v_pts);
        static void push(const IntervalVector& box, std::vector<Point>& v_pts);
        static void push(const IntervalVector& box, std::vector<Vector>& v_pts);
        static std::vector<Point> to_Points(const std::vector<Vector>& v_pts);
        static std::vector<Point> remove_identical_pts(const std::vector<Point>& v_pts);
        static std::vector<Vector> remove_identical_pts(const std::vector<Vector>& v_pts);

      /// @}


    protected:

      // Reliable representation of points:
      IntervalVector m_pt = IntervalVector(2, Interval::EMPTY_SET);
  };
}

#endif