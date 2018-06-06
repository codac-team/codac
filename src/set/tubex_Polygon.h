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

#ifndef Polygon_HEADER
#define Polygon_HEADER

#define _TUBES_POLYG_WITH_BOOST_ 1

#include <vector>
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
//#include "vibes.h"

#ifdef _TUBES_POLYG_WITH_BOOST_
  #include <boost/geometry/geometries/polygon.hpp>
  #include <boost/geometry/geometries/point_xy.hpp>
  using boost::geometry::get;
#endif

namespace tubex
{
  class Polygon
  {
    public:

      Polygon(const ibex::IntervalVector& box);
      Polygon(const std::vector<ibex::IntervalVector>& v_boxes);
      Polygon(const std::vector<double>& v_x, const std::vector<double>& v_y);

      int nbPoints() const;
      //void show(const vibes::Params& params) const;
      ibex::IntervalVector box() const;

      static Polygon intersect(const Polygon& p1, const Polygon& p2);
      static Polygon translate(const Polygon& p, const ibex::IntervalVector& box);

    //protected:

      #ifdef _TUBES_POLYG_WITH_BOOST_
        Polygon(const boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double> >& p);
        boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double> > m_polygon;
      #endif
  };
}

#endif