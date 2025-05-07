/** 
 *  codac2_geometry.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <iostream>
#include "codac2_geometry.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  OrientationInterval orientation(const IntervalVector& p1, const IntervalVector& p2, const IntervalVector& p3)
  {
    assert_release(p1.size() == 2 && p2.size() == 2 && p3.size() == 2);

    Interval o = (p2[1]-p1[1])*(p3[0]-p2[0])-(p2[0]-p1[0])*(p3[1]-p2[1]); 

    if(o.is_empty())
      return OrientationInterval::EMPTY;
    else if(o == 0.)
      return OrientationInterval::COLINEAR;
    else if(o.contains(0.))
      return OrientationInterval::UNKNOWN;
    else if(o.lb() > 0.)
      return OrientationInterval::COUNTERCLOCKWISE;
    else 
      return OrientationInterval::CLOCKWISE;
  }

  BoolInterval aligned(const IntervalVector& p1, const IntervalVector& p2, const IntervalVector& p3)
  {
    assert_release(p1.size() == 2 && p2.size() == 2 && p3.size() == 2);

    Interval cross_product = (p2[0]-p1[0])*(p3[1]-p1[1])-(p2[1]-p1[1])*(p3[0]-p1[0]);

    if(cross_product == 0.)
      return BoolInterval::TRUE;

    else if(!cross_product.contains(0.))
      return BoolInterval::FALSE;

    else
      return BoolInterval::UNKNOWN;
  }

  void swap(IntervalVector& x1, IntervalVector& x2)
  {
    IntervalVector x1_ = x1;
    x1 = x2; x2 = x1_;
  }

  Interval dist(const IntervalVector& p1, const IntervalVector& p2)
  {
    return sqr(p1[0]-p2[0]) + sqr(p1[1]-p2[1]);
  }

  vector<IntervalVector> convex_hull(vector<IntervalVector> pts)
  {
    // Removing duplicates
    sort(pts.begin(), pts.end(), 
      [](const IntervalVector& a, const IntervalVector& b) {
        return a[0].mid() < b[0].mid() || (a[0].mid() == b[0].mid() && a[1].lb() < b[1].lb());
      });
    pts.erase(unique(pts.begin(), pts.end()), pts.end());

    if(pts.size() < 3)
      return pts;

    // Implementation of a Graham scan method.
    // Based on some sources from OpenGenus Foundation.

    // Find the bottommost point

      size_t id_min = 0;
      double y_min = pts[0][1].mid();

      for(size_t i = 1 ; i < pts.size() ; i++)
      {
        double y = pts[i][1].mid();

        // Pick the bottom-most or chose the left most point in case of tie
        if((y < y_min) || (y_min == y && pts[i][0].mid() < pts[id_min][0].mid()))
        {
          y_min = pts[i][1].mid();
          id_min = i;
        }
      }

    // Place the bottom-most point at first position
      
      assert(id_min < pts.size());
      swap(pts[0], pts[id_min]);

    // Sort n-1 points in increasing order of the angle they and the point P0 make with the x-axis

      IntervalVector p0 = pts[0];
      sort(pts.begin(), pts.end(),
        [&p0](const IntervalVector& p1, const IntervalVector& p2) // p1 < p2
        {
          auto o = orientation(p1, p0, p2);

          if(o == OrientationInterval::COUNTERCLOCKWISE)
            return true;

          else if((o == OrientationInterval::UNKNOWN || o == OrientationInterval::COLINEAR)
              && dist(p0, p1).mid() < dist(p0, p2).mid())
            return true;

          return false;
        });

    // If two or more points make same angle with p0,
    // remove all but the one that is farthest from p0

      // Remember that, in above sorting, our criteria was
      // to keep the farthest point at the end when more than
      // one points have same angle.
      size_t m = 1; // Initialize size of modified array
      for(size_t i = 1 ; i < pts.size() ; i++)
      {
        // Keep removing i while angle of i and i+1 is same
        // with respect to p0
        while(i < pts.size()-1 && aligned(p0, pts[i], pts[i+1]) == BoolInterval::TRUE)
          i++;
        pts[m] = pts[i];
        m++; // Update size of modified array
      }

    // Create an empty stack and push first three points to it.

      list<IntervalVector> l({ pts[0], pts[1], pts[2] });

    // Process remaining n-3 points

      for(size_t i = 3 ; i < m ; i++)
      {
        // Keep removing top while the angle formed by
        // points next-to-top, top, and pts[i] makes
        // a non-left turn
        while(l.size() > 1 &&
          orientation(*std::prev(std::prev(l.end())), l.back(), pts[i]) == OrientationInterval::COUNTERCLOCKWISE)
        {
          l.pop_back();
        }
        
        l.push_back(pts[i]);
      }

    // Merge consecutive points
    pts.clear();
    pts.push_back(l.front()); l.pop_front();

    for(const auto& li : l)
    {
      if(li.intersects(pts.back()))
        pts.back() |= li;
      else
        pts.push_back(li);
    }

    return pts;
  }

  IntervalVector rotate(const IntervalVector& p, const Interval& a, const IntervalVector& c)
  {
    assert_release(p.size() == 2 && c.size() == 2);
    return {
      (p[0]-c[0])*cos(a)-(p[1]-c[1])*sin(a)+c[0],
      (p[0]-c[0])*sin(a)+(p[1]-c[1])*cos(a)+c[1]
    };
  }
}