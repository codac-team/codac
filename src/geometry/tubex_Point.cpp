/* ============================================================================
 *  tubex-lib - Point class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#include "tubex_Point.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  bool Point::operator==(const Point& p) const
  {
    return t == p.t && x == p.x;
  }

  bool Point::operator!=(const Point& p) const
  {
    return t != p.t || x != p.x;
  }

  Point point(double t, double x)
  {
    Point p;
    p.t = t; p.x = x;
    return p;
  }

  /*Point point(double t, Vector x)
  {
    Point p;
    p.t = t; p.x = x;
    return p;
  }*/

  void pushPoints(vector<Point>& v_points, const IntervalVector& box)
  {
    // todo
    //v_points.push_back(point(box[0].lb(), box[1].lb()));
    //v_points.push_back(point(box[0].lb(), box[1].ub()));
    //v_points.push_back(point(box[0].ub(), box[1].ub()));
    //v_points.push_back(point(box[0].ub(), box[1].lb()));
  }
}