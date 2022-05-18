/** 
 *  GrahamScan class
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_GRAHAMSCAN_H__
#define __CODAC_GRAHAMSCAN_H__

#include <stack>
#include "codac_Interval.h"
#include "codac_ThickPoint.h"
#include "codac_ConvexPolygon.h"

namespace codac
{
  enum class OrientationInterval { CLOCKWISE, COUNTERCLOCKWISE, UNDEFINED } ;

  class GrahamScan
  {
    public:

      // Prints convex hull of a set of n points.
      static const std::vector<Vector> convex_hull(const std::vector<Vector>& v_points);


    protected:

      // A utility function to find next to top in a stack
      static const Vector next_to_top(const std::stack<Vector>& s);
      
      // A utility function to swap two points
      static void swap(Vector& p1, Vector& p2);

      // A utility function to return square of distance between p1 and p2
      static const Interval dist(const IntervalVector& p1, const IntervalVector& p2);

      // To find orientation of ordered triplet (p, q, r).
      static OrientationInterval orientation(const IntervalVector& p0, const IntervalVector& p1, const IntervalVector& p2);

      friend class ThickPointsSorter;
      friend class ConvexPolygon;
  };

  class ThickPointsSorter
  {
    public:

      ThickPointsSorter(const Vector& p0);
      bool operator()(const Vector& p1, const Vector& p2);

    protected:

      Vector m_p0 = Vector(2);
  };
}

#endif