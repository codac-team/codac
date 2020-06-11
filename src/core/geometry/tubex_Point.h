/** 
 *  Point class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_POINT_H__
#define __TUBEX_POINT_H__

#ifdef _MSC_VER
// Enable additional features in math.h.
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // _USE_MATH_DEFINES
#include <math.h>
#endif // _MSC_VER

#include <vector>
#include "ibex_Vector.h"
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_BoolInterval.h"

// todo: remove this (polygons in unbounded case)
#include <limits>
#define BOUNDED_INFINITY numeric_limits<float>::max()

namespace tubex
{
  class Point // todo: derive from IntervalVector?
  {
    public:

      /// \name Definition
      /// @{

        Point(); // undefined point
        explicit Point(const ibex::Vector& p);
        explicit Point(const ibex::IntervalVector& p);
        Point(const ibex::Interval& x, const ibex::Interval& y);
        const Point& operator=(const Point& p);
  
      /// @}
      /// \name Accessing values
      /// @{

        const ibex::Interval& x() const;
        const ibex::Interval& y() const;
        const ibex::Interval& operator[](size_t id) const;
        const ibex::IntervalVector& box() const;
        const ibex::Vector mid() const;
        double max_diam() const;
        const std::vector<ibex::Vector> bounds_pts() const;
  
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

        static const ibex::BoolInterval aligned(const Point& a, const Point& b, const Point& c);
        static const Point center(const std::vector<Point> v_pts);
        static void push(const ibex::IntervalVector& box, std::vector<Point>& v_pts);
        static void push(const ibex::IntervalVector& box, std::vector<ibex::Vector>& v_pts);
        static std::vector<Point> to_Points(const std::vector<ibex::Vector>& v_pts);
        static std::vector<Point> remove_identical_pts(const std::vector<Point>& v_pts);
        static std::vector<ibex::Vector> remove_identical_pts(const std::vector<ibex::Vector>& v_pts);

      /// @}


    protected:

      // Reliable representation of points:
      ibex::IntervalVector m_pt = ibex::IntervalVector(2, ibex::Interval::EMPTY_SET);
  };
}

#endif