/** 
 *  \file
 *  FigureVIBes class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
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
      virtual void draw_box(const IntervalVector& x, const StyleProperties& s = StyleProperties());
      virtual void draw_circle(const Vector& c, double r, const StyleProperties& s = StyleProperties());

      static std::string to_vibes_style(const StyleProperties& s);

    protected:

      static int _has_been_initialized;
      const vibes::Params _params;
  };
}