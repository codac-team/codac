/** 
 *  \file codac2_OutputFigure2D.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>
#include <vector>
#include "codac2_Vector.h"
#include "codac2_IntervalVector.h"
#include "codac2_Figure2DInterface.h"

namespace codac2
{
  class Figure2D;

  class OutputFigure2D : public Figure2DInterface
  {
    public:

      OutputFigure2D(const Figure2D& fig)
        : _fig(fig)
      { }

      const size_t& i() const;
      const size_t& j() const;
      
      virtual void update_axes() = 0;
      virtual void update_window_properties() = 0;
      virtual void center_viewbox(const Vector& c, const Vector& r) = 0;

    protected:

      const Figure2D& _fig;
  };
}