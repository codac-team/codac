/** 
 *  GrahamScan class
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_GRAHAMSCAN_H__
#define __TUBEX_GRAHAMSCAN_H__

#include <stack>
#include "ibex_Interval.h"
#include "tubex_Point.h"
#include "tubex_ConvexPolygon.h"

namespace tubex
{
  enum class OrientationInterval { CLOCKWISE, COUNTERCLOCKWISE, UNDEFINED } ;

  class GrahamScan
  {
    public:

      // Prints convex hull of a set of n points.
      static const std::vector<ibex::Vector> convex_hull(const std::vector<ibex::Vector>& v_points);


    protected:

      // A utility function to find next to top in a stack
      static const ibex::Vector next_to_top(const std::stack<ibex::Vector>& s);
      
      // A utility function to swap two points
      static void swap(ibex::Vector& p1, ibex::Vector& p2);

      // A utility function to return square of distance between p1 and p2
      static const ibex::Interval dist(const ibex::IntervalVector& p1, const ibex::IntervalVector& p2);

      // To find orientation of ordered triplet (p, q, r).
      static OrientationInterval orientation(const ibex::IntervalVector& p0, const ibex::IntervalVector& p1, const ibex::IntervalVector& p2);

      friend class PointsSorter;
      friend class ConvexPolygon;
  };

  class PointsSorter
  {
    public:

      PointsSorter(const ibex::Vector& p0);
      bool operator()(const ibex::Vector& p1, const ibex::Vector& p2);

    protected:

      ibex::Vector m_p0 = ibex::Vector(2);
  };
}

#endif