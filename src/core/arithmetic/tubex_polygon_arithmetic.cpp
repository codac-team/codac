/** 
 *  Arithmetic operations on polygons
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \bugfixes   Bertrand Neveu
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_polygon_arithmetic.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  const Polygon operator+(const Polygon& x)
  {
    return x;
  }

  const Polygon operator+(const Polygon& x, const ibex::IntervalVector& v)
  {
    return operator+(v, x);
  }

  const Polygon operator+(const ibex::IntervalVector& v, const Polygon& x)
  {
    assert(x.size() == v.size());

    vector<Point> v_result_pts = x.vertices();
    for(auto& pt : v_result_pts)
    {
      IntervalVector y({pt.x(), pt.y()});
      y = v + y;
      pt = Point(y[0], y[1]);
    }

    return Polygon(v_result_pts);
  }

  const Polygon operator-(const Polygon& x)
  {
    vector<Point> v_result_pts = x.vertices();
    for(auto& pt : v_result_pts)
      pt = Point(-pt.x(), -pt.y());
    return Polygon(v_result_pts);
  }

  const Polygon operator-(const Polygon& x, const ibex::IntervalVector& v)
  {
    return operator+(x, -v);
  }

  const Polygon operator-(const ibex::IntervalVector& v, const Polygon& x)
  {
    return operator+(-x, v);
  }

  const Polygon operator*(const ibex::IntervalMatrix& m, const Polygon& x)
  {
    assert(x.size() == m.nb_cols() && x.size() == m.nb_rows());

    vector<Point> v_result_pts = x.vertices();
    for(auto& pt : v_result_pts)
    {
      IntervalVector y({pt.x(), pt.y()});
      y = m * y;
      pt = Point(y[0], y[1]);
    }

    return Polygon(v_result_pts);
  }
}