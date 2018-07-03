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
  
  ConvexPolygon::ConvexPolygon(const std::vector<Point>& v_points) : Polygon(v_points)
  {
    // todo: (test convexity, or make it convex)
  }
  
  const IntervalVector ConvexPolygon::operator&(const IntervalVector& x) const
  {
    // todo: the following could be easily optimized

    IntervalVector reduced_x = x & box();
    IntervalVector inter(2, Interval::EMPTY_SET);

    vector<Edge> v_edges = getEdges();
    for(int i = 0 ; i < v_edges.size() ; i++)
      inter |= v_edges[i] & reduced_x;

    vector<Point> v_x_vertices;
    pushPoints(reduced_x, v_x_vertices);
    for(int i = 0 ; i < v_x_vertices.size() ; i++)
      if(this->encloses(v_x_vertices[i]))
        inter |= v_x_vertices[i].box();

    return inter;
  }
}