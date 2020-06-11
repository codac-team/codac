/** 
 *  GrahamScan class
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <algorithm>
#include <iostream>
#include <iomanip>
#include "tubex_GrahamScan.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Class GrahamScan

  const vector<Vector> GrahamScan::convex_hull(const vector<Vector>& v_pts_)
  {
    vector<Vector> v_pts = Point::remove_identical_pts(v_pts_);

    if(v_pts.size() <= 3)
      return v_pts;

    #ifndef NDEBUG
      for(const auto& pt : v_pts)
        assert(pt.size() == 2 && "operation not supported for other dimensions");
    #endif
    
    // Based on some sources from OpenGenus Foundation

    // Find the bottommost point

      int id_min = 0;
      double y_min = v_pts[0][1];

      for(size_t i = 1 ; i < v_pts.size() ; i++)
      {
        double y = v_pts[i][1];

        // Pick the bottom-most or chose the left most point in case of tie
        if((y < y_min) || (y_min == y && v_pts[i][0] < v_pts[id_min][0]))
        {
          y_min = v_pts[i][1];
          id_min = i;
        }
      }

    // Place the bottom-most point at first position
    
      GrahamScan::swap(v_pts[0], v_pts[id_min]);

    // Sort n-1 points with respect to the first point.

      // A point p1 comes before p2 in sorted output if p2
      // has larger polar angle (in counterclockwise
      // direction) than p1

      Vector p0 = v_pts[0];
      sort(v_pts.begin(), v_pts.end(), PointsSorter(p0));

    // If two or more points make same angle with p0,
    // remove all but the one that is farthest from p0

      // Remember that, in above sorting, our criteria was
      // to keep the farthest point at the end when more than
      // one points have same angle.
      size_t m = 1; // Initialize size of modified array
      for(size_t i = 1 ; i < v_pts.size() ; i ++)
      {
        // Keep removing i while angle of i and i+1 is same
        // with respect to p0
        while(i < v_pts.size()-1 && Point::aligned(Point(p0), Point(v_pts[i]), Point(v_pts[i+1])) == YES)
          i++; 
        v_pts[m] = v_pts[i];
        m++; // Update size of modified array
      }

    // Create an empty stack and push first three points to it.

      vector<Vector> v_hull;

      stack<Vector> s;
      s.push(v_pts[0]);
      s.push(v_pts[1]);
      s.push(v_pts[2]);

    // Process remaining n-3 points

      for(size_t i = 3 ; i < m ; i++)
      {
        // Keep removing top while the angle formed by
        // points next-to-top, top, and v_pts[i] makes
        // a non-left turn
        while(s.size() > 1 && GrahamScan::orientation(next_to_top(s), s.top(), v_pts[i]) == OrientationInterval::CLOCKWISE)
          s.pop();
        s.push(v_pts[i]);
      }

      while(!s.empty())
      {
        v_hull.push_back(s.top());
        s.pop();
      }
      reverse(v_hull.begin(), v_hull.end());

    return v_hull;
  }

  const Vector GrahamScan::next_to_top(const stack<Vector>& s)
  {
    stack<Vector> stemp = s;
    Vector p = stemp.top();
    stemp.pop();
    Vector q = stemp.top();
    stemp.push(p);
    return q;
  }

  void GrahamScan::swap(Vector& p1, Vector& p2)
  {
    Vector temp = p1;
    p1 = p2;
    p2 = temp;
  }

  const Interval GrahamScan::dist(const IntervalVector& p1, const IntervalVector& p2)
  {
    return pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2);
  }

  OrientationInterval GrahamScan::orientation(const IntervalVector& p0, const IntervalVector& p1, const IntervalVector& p2)
  {
    const IntervalVector pa = p1 - p0, pb = p2 - p0;
    const Interval cross_prod = pa[0]*pb[1] - pa[1]*pb[0];

    if(cross_prod.contains(0.)) return OrientationInterval::UNDEFINED; // possibly collinear
    return (cross_prod.lb() > 0.) ? OrientationInterval::COUNTERCLOCKWISE : OrientationInterval::CLOCKWISE;
  }

  // Class PointsSorter

  PointsSorter::PointsSorter(const Vector& p0)
  {
    m_p0 = p0;
  }

  bool PointsSorter::operator()(const Vector& p1, const Vector& p2)
  {
    // Find orientation
    OrientationInterval o = GrahamScan::orientation(m_p0, p1, p2);

    if(o == OrientationInterval::COUNTERCLOCKWISE)
      return true;

    else if(o == OrientationInterval::UNDEFINED && GrahamScan::dist(m_p0, p1).ub() <= GrahamScan::dist(m_p0, p2).lb())
      return true;

    return false;
  }
}