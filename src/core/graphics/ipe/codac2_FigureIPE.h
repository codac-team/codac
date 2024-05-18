/** 
 *  \file
 *  FigureIPE class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <fstream>
#include "codac2_Figure.h"
#include "codac2_OutputFigure.h"
#include "codac2_IntervalVector.h"
#include "vibes.h"

namespace codac2
{
  class FigureIPE : public OutputFigure
  {
    public:

      FigureIPE(const Figure& fig);
      ~FigureIPE();
      void update_axes();
      void update_window_properties();
      void center_viewbox(const Vector& c, const Vector& r);
      virtual void draw_box(const IntervalVector& x, const StyleProperties& s = StyleProperties());

    protected:

      double scale_x(double x) const;
      double scale_y(double y) const;
      void print_header_page();

      std::ofstream _f, _f_temp_content;
      const double _ipe_grid_size = 500.;
      Vector _ratio { 1., 1. };

      std::map<std::string,ColorRGB> _colors;
  };
}