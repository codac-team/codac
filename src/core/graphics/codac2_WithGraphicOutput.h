/** 
 *  \file codac2_WithGraphicOutput.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Figure.h"

namespace codac2
{
  class WithGraphicOutput
  {
    public:

      void set_figure(Figure *g)
      {
        _figure = g;
      }

      // Public attributes freely editable by the user
      StyleProperties inner_style = StyleProperties::inner();
      StyleProperties outer_style = StyleProperties::outer();
      StyleProperties boundary_style = StyleProperties::boundary();

    protected:

      Figure *_figure = nullptr;
  };
}