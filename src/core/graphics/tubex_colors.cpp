/** 
 *  Color features
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <math.h>
#include "tubex_colors.h"

using namespace std;

namespace tubex
{
  const string rgb2hex(rgb rgb_value, int alpha, const char* prefix)
  {
    string result;
    result.append(prefix);
   
    char char_r[255];
    sprintf(char_r, "%.2X", (int)(rgb_value.r * 255));
    result.append(char_r);
    char char_g[255];
    sprintf(char_g, "%.2X", (int)(rgb_value.g * 255));
    result.append(char_g);
    char char_b[255];
    sprintf(char_b, "%.2X", (int)(rgb_value.b * 255));
    result.append(char_b);

    if(alpha != -1)
    {
      char char_alpha[255];
      sprintf(char_alpha, "%.2X", alpha);
      result.append(char_alpha);
    }

    return result;
  }

  hsv rgb2hsv(rgb rgb_value)
  {
    hsv hsv_value;
    float min, max, delta;

    min = rgb_value.r < rgb_value.g ? rgb_value.r : rgb_value.g;
    min = min < rgb_value.b ? min : rgb_value.b;

    max = rgb_value.r > rgb_value.g ? rgb_value.r : rgb_value.g;
    max = max > rgb_value.b ? max : rgb_value.b;

    hsv_value.v = max; // v
    delta = max - min;

    if(delta < 0.00001)
    {
      hsv_value.s = 0;
      hsv_value.h = 0; // undefined, maybe nan?
      return hsv_value;
    }

    if(max > 0.0) // note: if max == 0, this divide would cause a crash
      hsv_value.s = (delta / max); // s
    
    else
    {
      // if max is 0, then r = g = b = 0              
      // s = 0, h is undefined
      hsv_value.s = 0.0;
      hsv_value.h = NAN; // its now undefined
      return hsv_value;
    }

    if(rgb_value.r >= max)
      hsv_value.h = (rgb_value.g - rgb_value.b) / delta; // between yellow & magenta

    else
    {
      if(rgb_value.g >= max)
        hsv_value.h = 2.0 + (rgb_value.b - rgb_value.r) / delta; // between cyan & yellow

      else
        hsv_value.h = 4.0 + (rgb_value.r - rgb_value.g) / delta; // between magenta & cyan
    }

    hsv_value.h *= 60.0; // degrees

    if(hsv_value.h < 0.0)
      hsv_value.h += 360.0;

    return hsv_value;
  }

  rgb hsv2rgb(hsv hsv_value)
  {
    float hh, p, q, t, ff;
    int i;
    rgb rgb_value;

    if(hsv_value.s <= 0.0)
    {
      rgb_value.r = hsv_value.v;
      rgb_value.g = hsv_value.v;
      rgb_value.b = hsv_value.v;
      return rgb_value;
    }

    hh = hsv_value.h;
    if(hh >= 360.0)
      hh = 0.0;
    hh /= 60.0;
    i = (int)hh;
    ff = hh - i;
    p = hsv_value.v * (1.0 - hsv_value.s);
    q = hsv_value.v * (1.0 - (hsv_value.s * ff));
    t = hsv_value.v * (1.0 - (hsv_value.s * (1.0 - ff)));

    switch(i)
    {
      case 0:
        rgb_value.r = hsv_value.v;
        rgb_value.g = t;
        rgb_value.b = p;
        break;

      case 1:
        rgb_value.r = q;
        rgb_value.g = hsv_value.v;
        rgb_value.b = p;
        break;

      case 2:
        rgb_value.r = p;
        rgb_value.g = hsv_value.v;
        rgb_value.b = t;
        break;

      case 3:
        rgb_value.r = p;
        rgb_value.g = q;
        rgb_value.b = hsv_value.v;
        break;

      case 4:
        rgb_value.r = t;
        rgb_value.g = p;
        rgb_value.b = hsv_value.v;
        break;

      case 5:
      default:
        rgb_value.r = hsv_value.v;
        rgb_value.g = p;
        rgb_value.b = q;
        break;
    }

    return rgb_value;     
  }
}