/** 
 *  \file
 *  WithGraphicOutput class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
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

    protected:

      Figure *_figure;
  };
}