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
#include <cassert>
#include <iostream>
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

  rgb make_rgb(int r, int g, int b)
  {
    assert(r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255);
    rgb rgb_val;
    rgb_val.r = r/255.; rgb_val.g = g/255.; rgb_val.b = b/255.;
    return rgb_val;
  }

  rgb make_rgb(float r, float g, float b)
  {
    assert(r >= 0. && r <= 1. && g >= 0. && g <= 1. && b >= 0. && b <= 1.);
    rgb rgb_val;
    rgb_val.r = r; rgb_val.g = g; rgb_val.b = b;
    return rgb_val;
  }
    
  rgb haxby_color_map(float ratio)
  {
    assert(ratio >= 0. && ratio <= 1.);

    rgb haxby_cm[32];
    haxby_cm[ 0] = make_rgb(10,0,121);
    haxby_cm[ 1] = make_rgb(40,0,150);
    haxby_cm[ 2] = make_rgb(20,5,175);
    haxby_cm[ 3] = make_rgb(0,10,200);
    haxby_cm[ 4] = make_rgb(0,25,212);
    haxby_cm[ 5] = make_rgb(0,40,224);
    haxby_cm[ 6] = make_rgb(26,102,240);
    haxby_cm[ 7] = make_rgb(13,129,248);
    haxby_cm[ 8] = make_rgb(25,175,255);
    haxby_cm[ 9] = make_rgb(50,190,255);
    haxby_cm[10] = make_rgb(68,202,255);
    haxby_cm[11] = make_rgb(97,225,240);
    haxby_cm[12] = make_rgb(106,235,225);
    haxby_cm[13] = make_rgb(124,235,200);
    haxby_cm[14] = make_rgb(138,236,174);
    haxby_cm[15] = make_rgb(172,245,168);
    haxby_cm[16] = make_rgb(205,255,162);
    haxby_cm[17] = make_rgb(223,245,141);
    haxby_cm[18] = make_rgb(240,236,121);
    haxby_cm[19] = make_rgb(247,215,104);
    haxby_cm[20] = make_rgb(255,189,87);
    haxby_cm[21] = make_rgb(255,160,69);
    haxby_cm[22] = make_rgb(244,117,75);
    haxby_cm[23] = make_rgb(238,80,78);
    haxby_cm[24] = make_rgb(255,90,90);
    haxby_cm[25] = make_rgb(255,124,124);
    haxby_cm[26] = make_rgb(255,158,158);
    haxby_cm[27] = make_rgb(245,179,174);
    haxby_cm[28] = make_rgb(255,196,196);
    haxby_cm[29] = make_rgb(255,215,215);
    haxby_cm[30] = make_rgb(255,235,235);
    haxby_cm[31] = make_rgb(255,254,253);

    float value31 = 31.*ratio;

    if(ceil(value31) == floor(value31))
      return haxby_cm[(int)ceil(value31)];

    float local_ratio = (value31 - floor(value31)) / (ceil(value31) - floor(value31));
    rgb rgb_lb = haxby_cm[(int)floor(value31)];
    rgb rgb_ub = haxby_cm[(int)ceil(value31)];
    
    return make_rgb((float)(rgb_lb.r + (rgb_ub.r - rgb_lb.r) * local_ratio),
                    (float)(rgb_lb.g + (rgb_ub.g - rgb_lb.g) * local_ratio),
                    (float)(rgb_lb.b + (rgb_ub.b - rgb_lb.b) * local_ratio));
  }
    
  rgb dem_color_map(float ratio)
  {
    assert(ratio >= 0. && ratio <= 1.);

    rgb haxby_cm[16];
    haxby_cm[ 0] = make_rgb(39,90,211);
    haxby_cm[ 1] = make_rgb(40,123,245);
    haxby_cm[ 2] = make_rgb(45,155,253);
    haxby_cm[ 3] = make_rgb(73,209,255);
    haxby_cm[ 4] = make_rgb(100,230,254);
    haxby_cm[ 5] = make_rgb(118,235,226);
    haxby_cm[ 6] = make_rgb(135,236,187);
    haxby_cm[ 7] = make_rgb(194,252,165);
    haxby_cm[ 8] = make_rgb(217,251,151);
    haxby_cm[ 9] = make_rgb(233,241,131);
    haxby_cm[10] = make_rgb(252,201,96);
    haxby_cm[11] = make_rgb(255,184,84);
    haxby_cm[12] = make_rgb(255,170,75);
    haxby_cm[13] = make_rgb(255,167,83);
    haxby_cm[14] = make_rgb(255,200,158);
    haxby_cm[15] = make_rgb(255,233,217);

    float value31 = 15.*ratio;

    if(ceil(value31) == floor(value31))
      return haxby_cm[(int)ceil(value31)];

    float local_ratio = (value31 - floor(value31)) / (ceil(value31) - floor(value31));
    rgb rgb_lb = haxby_cm[(int)floor(value31)];
    rgb rgb_ub = haxby_cm[(int)ceil(value31)];
    
    return make_rgb((float)(rgb_lb.r + (rgb_ub.r - rgb_lb.r) * local_ratio),
                    (float)(rgb_lb.g + (rgb_ub.g - rgb_lb.g) * local_ratio),
                    (float)(rgb_lb.b + (rgb_ub.b - rgb_lb.b) * local_ratio));
  }
}