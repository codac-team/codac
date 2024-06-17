/** 
 *  \file codac2_FigureVIBes.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Figure.h"
#include "codac2_OutputFigure.h"
#include "codac2_IntervalVector.h"
#include "vibes.h"

namespace codac2
{
  class FigureVIBes : public OutputFigure
  {
    public:

      FigureVIBes(const Figure& fig);
      ~FigureVIBes();

      void update_axes();
      void update_window_properties();
      void center_viewbox(const Vector& c, const Vector& r);
      
      // Geometric shapes
      void draw_point(const Vector& c, const StyleProperties& s = StyleProperties());
      void draw_box(const IntervalVector& x, const StyleProperties& s = StyleProperties());
      void draw_circle(const Vector& c, double r, const StyleProperties& s = StyleProperties());
      void draw_polyline(const std::vector<Vector>& x, float tip_length, const StyleProperties& s = StyleProperties());
      void draw_polygone(const std::vector<Vector>& x, const StyleProperties& s = StyleProperties());
      void draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& s = StyleProperties());

      // Robots
      void draw_tank(const Vector& x, float size, const StyleProperties& s = StyleProperties());
      void draw_AUV(const Vector& x, float size, const StyleProperties& s = StyleProperties());

      static std::string to_vibes_style(const StyleProperties& s);

    protected:

      static int _has_been_initialized;
      const vibes::Params _params;
  };
}