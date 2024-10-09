/** 
 *  codac2_Color.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Color.h"

using namespace std;
using namespace codac2;

Color::Color(float r_, float g_, float b_, float alpha_)
  : r(r_), g(g_), b(b_), alpha(alpha_),
  hex_str([&]
  {
    std::stringstream s;
    s << std::hex << std::setfill('0');
    s << std::setw(2) << (int)(r*255) << std::setw(2) << (int)(g*255) << std::setw(2) << (int)(b*255);
    if(alpha != 1.)
      s << std::setw(2) << (int)(alpha*255);
    return "#"+s.str();
  }())
{ 
  assert(r_ >= 0. && r_ <= 1. && g_ >= 0. && g_ <= 1. && b_ >= 0. && b_ <= 1. && alpha_ >= 0. && alpha_ <= 1.);
}

Color::Color(int r_, int g_, int b_, int alpha_)
  : Color((float)(r_/255.), (float)(g_/255.), (float)(b_/255.), (float)(alpha_/255.))
{
  assert(r_ >= 0 && r_ <= 255 && g_ >= 0 && g_ <= 255 && b_ >= 0 && b_ <= 255 && alpha_ >= 0 && alpha_ <= 255);
}