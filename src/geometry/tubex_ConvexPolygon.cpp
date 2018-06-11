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
#include "tubex_ConvexPolygon.h"
#include "tubex_Exception.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  ConvexPolygon::ConvexPolygon() : Polygon()
  {

  }
  
  ConvexPolygon::ConvexPolygon(const IntervalVector& box) : Polygon(box)
  {
    
  }
  
  ConvexPolygon::ConvexPolygon(const vector<IntervalVector>& v_boxes)
  {
    // todo

    /* make convex:
    boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double> > poly;
    boost::geometry::convex_hull(m_polygon, poly);
    m_polygon = poly;
    */
  }
  
  ConvexPolygon::ConvexPolygon(const std::vector<Point>& v_points) : Polygon(v_points)
  {
    // todo (test convexity, or make it convex)
  }
  
  ConvexPolygon operator&(const ConvexPolygon& p1, const ConvexPolygon& p2)
  {
    // todo

    /*boostpolygon bp1 = create_boostpolygon(p1);
    boostpolygon bp2 = create_boostpolygon(p2);

    deque<boostpolygon> output;
    boost::geometry::intersection(bp1, bp2, output);
    BOOST_FOREACH(boostpolygon const& p, output)
      return create_polyon(p);*/
  }
}