/* ============================================================================
 *  tubex-lib - Polygon class
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
#include "tubex_Polygon.h"

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
    Polygon::Polygon()
    {

    }

    Polygon::Polygon(const polygon& p)
    {
      m_polygon = p;
    }

    Polygon::Polygon(const IntervalVector& box)
    {
      vector<double> v_x, v_y;
      v_x.push_back(box[0].lb()); v_y.push_back(box[1].lb());
      v_x.push_back(box[0].ub()); v_y.push_back(box[1].lb());
      v_x.push_back(box[0].ub()); v_y.push_back(box[1].ub());
      v_x.push_back(box[0].lb()); v_y.push_back(box[1].ub());
      Polygon poly_points(v_x, v_y);
      boost::geometry::convex_hull(poly_points.m_polygon, m_polygon);
    }

    Polygon::Polygon(const vector<IntervalVector>& v_boxes)
    {
      vector<double> v_x, v_y;
      for(int i = 0 ; i < v_boxes.size() ; i++)
      {
        v_x.push_back(v_boxes[i][0].lb());
        v_y.push_back(v_boxes[i][1].lb());
        v_x.push_back(v_boxes[i][0].ub());
        v_y.push_back(v_boxes[i][1].lb());
        v_x.push_back(v_boxes[i][0].lb());
        v_y.push_back(v_boxes[i][1].ub());
        v_x.push_back(v_boxes[i][0].ub());
        v_y.push_back(v_boxes[i][1].ub());
      }

      Polygon poly_points(v_x, v_y);
      boost::geometry::convex_hull(poly_points.m_polygon, m_polygon);
    }

    Polygon::Polygon(const vector<double>& v_x, const vector<double>& v_y)
    {
      vector<point> pts;
      for(int i = 0 ; i < v_x.size() ; i++)
        pts.push_back(point(v_x[i], v_y[i]));
      boost::geometry::assign_points(m_polygon, pts);
    }

    int Polygon::nbPoints() const
    {
      return m_polygon.outer().size();
    }

    IntervalVector Polygon::box() const
    {
      IntervalVector iv_box(2);
      boost::geometry::model::box<point> box;
      boost::geometry::envelope(m_polygon, box);
      iv_box[0] = Interval(get<0>(box.min_corner()), get<0>(box.max_corner()));
      iv_box[1] = Interval(get<1>(box.min_corner()), get<1>(box.max_corner()));
      return iv_box;
    }

    void Polygon::makeConvex()
    {
      boost::geometry::convex_hull(m_polygon, m_polygon);
    }

    Polygon Polygon::intersect(const Polygon& p1, const Polygon& p2)
    {
      deque<polygon> output;
      boost::geometry::intersection(p1.m_polygon, p2.m_polygon, output);
      BOOST_FOREACH(polygon const& p, output)
        return Polygon(p);
    }

    Polygon Polygon::translate(const Polygon& p, const IntervalVector& translation)
    {
      vector<point> const& points = p.m_polygon.outer();
      vector<IntervalVector> v_boxes;

      for(vector<point>::size_type i = 0; i < points.size(); ++i)
      {
        IntervalVector point_box(2);
        point_box[0] = Interval(get<0>(points[i]));
        point_box[1] = Interval(get<1>(points[i]));
        point_box += translation;
        v_boxes.push_back(point_box);
      }

      return Polygon(v_boxes);
    }
  }

#endif