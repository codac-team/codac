/** 
 *  Color features
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <math.h>
#include <cassert>
#include <iostream>
#include "codac_colors.h"

using namespace std;

namespace codac
{
  const string rgb2hex(rgb rgb_value, const char* prefix)
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

    if(rgb_value.alpha != 1.)
    {
      char char_alpha[255];
      sprintf(char_alpha, "%.2X", (int)(rgb_value.alpha * 255));
      result.append(char_alpha);
    }

    return result;
  }

  hsv rgb2hsv(rgb rgb_value)
  {
    hsv hsv_value;
    float min, max, delta;

    hsv_value.alpha = rgb_value.alpha;

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

    rgb_value.alpha = hsv_value.alpha;

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

  rgb make_rgb(int r, int g, int b, int alpha)
  {
    assert(r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255 && alpha >= 0 && alpha <= 255);
    rgb rgb_val;
    rgb_val.r = r/255.; rgb_val.g = g/255.; rgb_val.b = b/255.; rgb_val.alpha = alpha/255.;
    return rgb_val;
  }

  rgb make_rgb(float r, float g, float b, float alpha)
  {
    assert(r >= 0. && r <= 1. && g >= 0. && g <= 1. && b >= 0. && b <= 1. && alpha >= 0. && alpha <= 1.);
    rgb rgb_val;
    rgb_val.r = r; rgb_val.g = g; rgb_val.b = b; rgb_val.alpha = alpha;
    return rgb_val;
  }

  hsv make_hsv(int h, int s, int v, int alpha)
  {
    assert(h >= 0 && h <= 360. && s >= 0 && s <= 100 && v >= 0 && v <= 100 && alpha >= 0 && alpha <= 100);
    hsv hsv_val;
    hsv_val.h = h; hsv_val.s = s/100.; hsv_val.v = v/100.; hsv_val.alpha = alpha/100.;
    return hsv_val;
  }

  hsv make_hsv(float h, float s, float v, float alpha)
  {
    assert(h >= 0. && h <= 360. && s >= 0. && s <= 1. && v >= 0. && v <= 1. && alpha >= 0. && alpha <= 1.);
    hsv hsv_val;
    hsv_val.h = h; hsv_val.s = s; hsv_val.v = v; hsv_val.alpha = alpha;
    return hsv_val;
  }
}