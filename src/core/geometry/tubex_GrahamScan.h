/* ============================================================================
 *  tubex-lib - GrahamScan class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_GRAHAMSCAN_H__
#define __TUBEX_GRAHAMSCAN_H__

#include <stack>
#include "ibex_Interval.h"
#include "tubex_Point.h"
#include "tubex_ConvexPolygon.h"

namespace tubex
{
  typedef enum { CLOCKWISE, COUNTERCLOCKWISE, UNDEFINED } OrientationInterval;

  class GrahamScan
  {
    public:

      // Prints convex hull of a set of n points.
      static const std::vector<Point> convex_hull(const std::vector<Point>& v_points);

    //protected:

      // A utility function to find next to top in a stack
      static const Point next_to_top(const std::stack<Point>& s);
      
      // A utility function to swap two points
      static void swap(Point& p1, Point& p2);

      // A utility function to return square of distance between p1 and p2
      static const ibex::Interval dist(const Point& p1, const Point& p2);

      // To find orientation of ordered triplet (p, q, r).
      static OrientationInterval orientation(const Point& a, const Point& b, const Point& c);

    protected:
  };

  class PointsSorter
  {
    public:

      PointsSorter(const Point& p0);
      bool operator()(const Point& p1, const Point& p2);

    protected:

      Point m_p0 = Point(ibex::Interval::EMPTY_SET, ibex::Interval::EMPTY_SET);
  };
}

#endif