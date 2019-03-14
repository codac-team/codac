/* ============================================================================
 *  tubex-lib - ConvexPolygon class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#include <iostream>
#include <iomanip>
#include "tubex_GrahamScan.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  // Class GrahamScan

  const Point GrahamScan::next_to_top(const stack<Point>& s)
  {
    stack<Point> stemp = s;
    Point p = stemp.top();
    stemp.pop();
    Point q = stemp.top();
    stemp.push(p);
    return q;
  }

  void GrahamScan::swap(Point& p1, Point& p2)
  {
    Point temp = p1;
    p1 = p2;
    p2 = temp;
  }

  const Interval GrahamScan::dist(const Point& p1, const Point& p2)
  {
    assert(!p1.does_not_exist());
    assert(!p2.does_not_exist());

    return pow(p1.x() - p2.x(), 2) + pow(p1.y() - p2.y(), 2);
  }

  OrientationInterval GrahamScan::orientation(const Point& a, const Point& b, const Point& c)
  {
    assert(!a.does_not_exist());
    assert(!b.does_not_exist());
    assert(!c.does_not_exist());

    const Interval val = (b.y()-a.y()) * (c.x()-b.x()) - (b.x()-a.x()) * (c.y()-b.y());

    if(val.contains(0.)) return UNDEFINED; // possibly colinear
    return (val.lb() > 0.) ? CLOCKWISE : COUNTERCLOCKWISE;
  }

  const vector<Point> GrahamScan::convex_hull(const vector<Point>& v_points)
  {
    vector<Point> v_pts;
    for(int i = 0 ; i < v_points.size() ; i++)
      if(!v_points[i].does_not_exist())
      {
        // todo: optimize this
        if(!v_points[i].x().is_degenerated() || !v_points[i].y().is_degenerated())
        {
          v_pts.push_back(Point(v_points[i].x().lb(), v_points[i].y().lb()));
          v_pts.push_back(Point(v_points[i].x().ub(), v_points[i].y().lb()));
          v_pts.push_back(Point(v_points[i].x().ub(), v_points[i].y().ub()));
          v_pts.push_back(Point(v_points[i].x().lb(), v_points[i].y().ub()));
        }
        
        else
          v_pts.push_back(v_points[i]);
      }

    v_pts = Point::delete_redundant_points(v_pts);

    if(v_pts.size() <= 3)
      return v_pts;

    // Based on some sources from OpenGenus Foundation

    // Find the bottommost point

      int id_min = 0;
      double y_min = v_pts[0].y().lb();

      for(int i = 1 ; i < v_pts.size() ; i++)
      {
        double y = v_pts[i].y().lb();

        // Pick the bottom-most or chose the left most point in case of tie
        if((y < y_min) || (y_min == y && v_pts[i].x().lb() < v_pts[id_min].x().lb()))
        {
          y_min = v_pts[i].y().lb();
          id_min = i;
        }
      }

    // Place the bottom-most point at first position
    
      GrahamScan::swap(v_pts[0], v_pts[id_min]);

    // Sort n-1 points with respect to the first point.

      // A point p1 comes before p2 in sorted ouput if p2
      // has larger polar angle (in counterclockwise
      // direction) than p1

      Point p0 = v_pts[0];
      sort(v_pts.begin(), v_pts.end(), PointsSorter(p0));

    // If two or more points make same angle with p0,
    // remove all but the one that is farthest from p0

      // Remember that, in above sorting, our criteria was
      // to keep the farthest point at the end when more than
      // one points have same angle.
      int m = 1; // Initialize size of modified array
      for(int i = 1 ; i < v_pts.size() ; i ++)
      {
        // Keep removing i while angle of i and i+1 is same
        // with respect to p0
        while(i < v_pts.size()-1 && Point::aligned(p0, v_pts[i], v_pts[i+1]) == YES)
          i++; 
        v_pts[m] = v_pts[i];
        m++; // Update size of modified array
      }

    // Create an empty stack and push first three points to it.

      vector<Point> v_hull;

      stack<Point> s;
      s.push(v_pts[0]);
      s.push(v_pts[1]);
      s.push(v_pts[2]);

    // Process remaining n-3 points

      for(int i = 3 ; i < m ; i++)
      {
        // Keep removing top while the angle formed by
        // points next-to-top, top, and v_pts[i] makes
        // a non-left turn
        while(s.size() > 1 && GrahamScan::orientation(next_to_top(s), s.top(), v_pts[i]) == CLOCKWISE)
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

  // Class PointsSorter

  PointsSorter::PointsSorter(const Point& p0)
  {
    assert(p0.x().is_degenerated() && p0.y().is_degenerated());
    m_p0 = p0;
  }

  bool PointsSorter::operator()(const Point& p1, const Point& p2)
  {
    // Uncertain points should be divided by GrahamScan
    assert(p1.x().is_degenerated() && p1.y().is_degenerated());
    assert(p2.x().is_degenerated() && p2.y().is_degenerated());

    // Find orientation
    OrientationInterval o = GrahamScan::orientation(m_p0, p1, p2);

    if(o == COUNTERCLOCKWISE)
      return true;

    else if(o == UNDEFINED && GrahamScan::dist(m_p0, p1).lb() <= GrahamScan::dist(m_p0, p2).lb())
      return true;

    return false;
  }
}