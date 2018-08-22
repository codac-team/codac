/* ============================================================================
 *  tubex-lib - VibesFigure class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_COLORS_H__
#define __TUBEX_COLORS_H__

#include <string>

namespace tubex
{
  typedef struct
  {
    float r; // a fraction between 0 and 1
    float g; // a fraction between 0 and 1
    float b; // a fraction between 0 and 1
  } rgb;

  typedef struct
  {
    float h; // angle in degrees between 0 and 359
    float s; // a fraction between 0 and 1
    float v; // a fraction between 0 and 1
  } hsv;

  const std::string rgb2hex(rgb rgb_value, int alpha = -1, const char* prefix = "#");
  rgb hsv2rgb(hsv hsv_value);
  hsv rgb2hsv(rgb rgb_value);
}

#endif