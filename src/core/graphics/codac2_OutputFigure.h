/** 
 *  \file
 *  OutputFigure class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <string>
#include "codac2_IntervalVector.h"
#include "codac2_FigureInterface.h"

namespace codac2
{
  class Figure;

  class OutputFigure : public FigureInterface
  {
    public:

      OutputFigure(const Figure& fig)
        : _fig(fig)
      { }

      virtual void update_axes() = 0;
      virtual void update_window_properties() = 0;
      virtual void center_viewbox(const Vector& c, const Vector& r) = 0;

      virtual void draw_box(const IntervalVector& x, const StyleProperties& s = StyleProperties()) = 0;
      virtual void draw_circle(const Vector& c, double r, const StyleProperties& s = StyleProperties()) = 0;

    protected:

      const Figure& _fig;
  };
}