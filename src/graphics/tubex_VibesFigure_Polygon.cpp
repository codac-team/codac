/* ============================================================================
 *  tubex-lib - VibesFigure_Polygon class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_VibesFigure_Polygon.h"

using namespace std;
using namespace ibex;

#ifdef _TUBES_POLYG_WITH_BOOST_

  #include <deque>
  #include <boost/geometry.hpp>
  #include <boost/foreach.hpp>

  using boost::geometry::get;
  typedef boost::geometry::model::d2::point_xy<double> point;
  typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double> > polygon;

  namespace tubex
  {
    void VibesFigure_Polygon::draw(const Polygon& p, const vibes::Params& params)
    {
      vector<point> const& points = p.m_polygon.outer();
      vector<double> v_x, v_y;

      for(vector<point>::size_type i = 0; i < points.size(); ++i)
      {
        v_x.push_back(get<0>(points[i]));
        v_y.push_back(get<1>(points[i]));
      } 

      vibes::drawPolygon(v_x, v_y, params);
    }
  }

#endif