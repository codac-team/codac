/** 
 *  codac2_StyleProperties.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_StyleProperties.h"

using namespace std;
using namespace codac2;

StyleProperties::StyleProperties()
{ }

StyleProperties::StyleProperties(const std::string& param1, const std::string& param2, const std::string& param3, const std::string& param4)
  : StylePropertiesBase(param1, param2, param3, param4)
{ }

StyleProperties::StyleProperties(const Color& stroke_color_, const std::string& param1, const std::string& param2, const std::string& param3, const std::string& param4)
  : StylePropertiesBase(param1, param2, param3, param4), stroke_color(stroke_color_), fill_color(Color::none())
{ }

StyleProperties::StyleProperties(std::initializer_list<Color> colors, const std::string& param1, const std::string& param2, const std::string& param3, const std::string& param4)
  : StylePropertiesBase(param1, param2, param3, param4), stroke_color(*colors.begin())
{
  assert(colors.size() <= 2);
  if (colors.size() == 1)
    fill_color = Color::none();
  else
    fill_color = *std::prev(colors.end());
}

StyleProperties StyleProperties::inside()
{
  StyleProperties s;
  s.stroke_color = Color::dark_gray();
  s.fill_color = Color::green();
  s.layer = "inside";
  s.z_value = -1.;
  return s;
}

StyleProperties StyleProperties::outside()
{
  StyleProperties s;
  s.stroke_color = Color::dark_gray();
  s.fill_color = Color::light_blue();
  s.layer = "outside";
  s.z_value = -3.;
  return s;
}

StyleProperties StyleProperties::boundary()
{
  StyleProperties s;
  s.stroke_color = Color::dark_gray();
  s.fill_color = Color::yellow();
  s.layer = "boundary";
  s.z_value = -2.;
  return s;
}