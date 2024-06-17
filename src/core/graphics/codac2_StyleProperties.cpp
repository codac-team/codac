/** 
 *  codac2_StyleProperties.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Figure.h"
#include "codac2_FigureVIBes.h"
#include "codac2_FigureIPE.h"

using namespace std;
using namespace codac2;

string Color::to_hex_str(const string& prefix) const
{
  if(hex_str.empty())
  {
    hex_str = prefix;
   
    char char_r[255];
    sprintf(char_r, "%.2X", (int)(r * 255));
    hex_str.append(char_r);
    char char_g[255];
    sprintf(char_g, "%.2X", (int)(g * 255));
    hex_str.append(char_g);
    char char_b[255];
    sprintf(char_b, "%.2X", (int)(b * 255));
    hex_str.append(char_b);

    if(alpha != 1.)
    {
      char char_alpha[255];
      sprintf(char_alpha, "%.2X", (int)(alpha * 255));
      hex_str.append(char_alpha);
    }
  }

  return hex_str;
}

StyleProperties::StyleProperties()
{ }

StyleProperties::StyleProperties(const Color& stroke_color_)
  : stroke_color(stroke_color_), fill_color(Color::none())
{ }

StyleProperties::StyleProperties(std::initializer_list<Color> colors)
  : stroke_color(*colors.begin()), fill_color(*std::prev(colors.end()))
{
  assert(colors.size() <= 2);
}

StyleProperties::StyleProperties(const string& vibes_style)
  : stroke_color(Color::green()), fill_color(Color::none())
{ }

StyleProperties StyleProperties::inner()
{
  StyleProperties s;
  s.stroke_color = Color::dark_gray();
  s.fill_color = Color::green();
  return s;
}

StyleProperties StyleProperties::outer()
{
  StyleProperties s;
  s.stroke_color = Color::dark_gray();
  s.fill_color = Color::cyan();
  return s;
}

StyleProperties StyleProperties::boundary()
{
  StyleProperties s;
  s.stroke_color = Color::dark_gray();
  s.fill_color = Color::yellow();
  return s;
}