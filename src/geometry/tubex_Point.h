/* ============================================================================
 *  tubex-lib - Point structure
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef Point_HEADER
#define Point_HEADER

#include <vector>
#include "ibex_IntervalVector.h"
//#include "ibex_Vector.h"

namespace tubex
{
  struct Point
  {
    double t;
    double x;
    //ibex::Vector x = ibex::Vector(1);

    bool operator==(const Point& p) const;
    bool operator!=(const Point& p) const;
  };

  Point point(double t, double x);
  //Point point(double t, ibex::Vector x);
  void pushPoints(const ibex::IntervalVector& box, std::vector<Point>& v_points);
}

#endif