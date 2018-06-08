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
    return x == p.x && y == p.y;
  }

  Point point(double x, double y)
  {
    Point p;
    p.x = x; p.y = y;
    return p;
  }

  void pushPoints(vector<Point>& v_points, const IntervalVector& box)
  {
    v_points.push_back(point(box[0].lb(), box[1].lb()));
    v_points.push_back(point(box[0].lb(), box[1].ub()));
    v_points.push_back(point(box[0].ub(), box[1].ub()));
    v_points.push_back(point(box[0].ub(), box[1].lb()));
  }
}