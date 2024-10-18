/** 
 *  \file codac2_Figure2D_IPE.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include <fstream>
#include "codac2_Figure2D.h"
#include "codac2_OutputFigure2D.h"
#include "codac2_Vector.h"
#include "codac2_IntervalVector.h"
#include "vibes.h"

namespace codac2
{
  class Figure2D_IPE : public OutputFigure2D
  {
    public:

      Figure2D_IPE(const Figure2D& fig);
      ~Figure2D_IPE();
      void update_axes();
      void update_window_properties();
      void center_viewbox(const Vector& c, const Vector& r);
      void begin_path(const StyleProperties& s);

      // Geometric shapes
      void draw_point(const Vector& c, const StyleProperties& s = StyleProperties());
      void draw_box(const IntervalVector& x, const StyleProperties& s = StyleProperties());
      void draw_circle(const Vector& c, double r, const StyleProperties& s = StyleProperties());
      void draw_ring(const Vector& c, const Interval& r, const StyleProperties& s = StyleProperties());
      void draw_polyline(const std::vector<Vector>& x, float tip_length, const StyleProperties& s = StyleProperties());
      void draw_polygone(const std::vector<Vector>& x, const StyleProperties& s = StyleProperties());
      void draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& s = StyleProperties());
      void draw_ellipse(const Vector& c, const Vector& ab, double theta, const StyleProperties& s = StyleProperties());

      // Robots
      void draw_tank(const Vector& x, float size, const StyleProperties& s = StyleProperties());
      void draw_AUV(const Vector& x, float size, const StyleProperties& s = StyleProperties());

    protected:

      double scale_x(double x) const;
      double scale_y(double y) const;
      double scale_length(double y) const;
      void print_header_page();

      std::ofstream _f, _f_temp_content;
      const double _ipe_grid_size = 500.;
      Vector _ratio { 1., 1. };

      std::map<std::string,Color> _colors;
  };
}