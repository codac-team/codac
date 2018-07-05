/* ============================================================================
 *  tubex-lib - VibesFigure_Polygon class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_VIBESFIGUREPOLYGON_H__
#define __TUBEX_VIBESFIGUREPOLYGON_H__

#include "vibes.h"
#include "tubex_Polygon.h"

namespace tubex
{
  class VibesFigure_Polygon
  {
    public:

      static void draw(const Polygon& p, const vibes::Params& params);
  };
}

#endif