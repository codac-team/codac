/** 
 *  codac2_Color.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Color.h"

using namespace std;
using namespace codac2;

Color::Color()
  : Color({0.,0.,0.,0.})
{ }

Color::Color(const std::array<float,3>& xyz, Model m_)
  : Color({xyz[0],xyz[1],xyz[2],(m_ == Model::RGB ? (float) 255. : (float) 100.)}, m_)
{ }

Color::Color(const std::array<float,4>& xyza, Model m_)
  : std::array<float,4>(xyza), m(m_)
{
  if (m_==Model::RGB)
    assert(xyza[0] >= 0. && xyza[0] <= 255. && xyza[1]>=0. && xyza[1] <= 255. && xyza[2]>=0. && xyza[2] <= 255. && xyza[3]>=0. && xyza[3] <= 255.);
  else if (m_==Model::HSV)
    assert(xyza[0] >= 0. && xyza[0] <= 360. && xyza[1]>=0. && xyza[1] <= 100. && xyza[2]>=0. && xyza[2] <= 100. && xyza[3]>=0. && xyza[3] <= 100.);
}

Color::Color(const std::initializer_list<float> xyza, Model m_)
  : Color(xyza.size() == 3 ? Color(to_array<3>(xyza), m_) : Color(to_array<4>(xyza), m_)) 
{ }

Color::Color(const std::string& hex_str) : m(Model::RGB)
{
  assert(hex_str.size() == 7 || hex_str.size() == 9);
  assert(hex_str[0] == '#');

  int red,green,blue,a;
  std::istringstream(hex_str.substr(1,2)) >> std::hex >> red;
  std::istringstream(hex_str.substr(3,2)) >> std::hex >> green;
  std::istringstream(hex_str.substr(5,2)) >> std::hex >> blue;
  (*this)[0] = (float) (red);
  (*this)[1] = (float) (green);
  (*this)[2] = (float) (blue);

  // Alpha (transparency) component may be appended to the #hexa notation.
  // Value is '255.' (max opacity) by default.
  if(hex_str.size() == 9)
  {
    std::istringstream(hex_str.substr(7,2)) >> std::hex >> a;
    (*this)[3] = (float) (a);
  }
  else
    (*this)[3] = 255.;
}

Color Color::rgb() const
{
  if (m==Model::RGB)
    return *this;
  else
  {
    float r = 0., g = 0., b = 0.;

    // Normalisation des valeurs
    float h = (*this)[0] / 360.; // Hue normalisée (0 à 1)
    float s = (*this)[1] / 100.; // Saturation normalisée (0 à 1)
    float v = (*this)[2] / 100.; // Value normalisée (0 à 1)

    int i = static_cast<int>(h * 6);
    float f = (h * 6) - i;
    i = i % 6;

    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch (i) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }

    // Conversion vers l'échelle [0, 255]
    r *= 255.;
    g *= 255.;
    b *= 255.;

    return Color({r, g, b,std::min<float>(255.,((*this)[3]*2.55))},Model::RGB);
  }
}

Color Color::hsv() const
{
  if (m==Model::HSV)
    return *this;
  else
  {
    float r = (*this)[0]/255.;
    float g = (*this)[1]/255.;
    float b = (*this)[2]/255.;
    float c_max = std::max({r, g, b});
    float c_min = std::min({r, g, b});
    float delta = c_max - c_min;

    float h = 0.0;
    if (delta != 0) {
        if (c_max == r) {
            h = fmod((g - b) / delta, 6.0);
        } else if (c_max == g) {
            h = (b - r) / delta + 2.0;
        } else if (c_max == b) {
            h = (r - g) / delta + 4.0;
        }
        h /= 6.0;
        if (h < 0) {
            h += 1.0;
        }
    }

    float s = (c_max == 0) ? 0 : (delta / c_max);

    float v = c_max;

    h*=360.;
    s*=100.;
    v*=100.;

    return Color({h, s, v,std::min<float>(100.,((*this)[3]/2.55))},Model::HSV);
  }
}

std::string Color::hex_str() const
{
  if (m == Model::RGB)
    {
      std::stringstream s;
      s << std::hex << std::setfill('0');
      s << std::setw(2) << (int)((*this)[0]) << std::setw(2) << (int)((*this)[1]) << std::setw(2) << (int)((*this)[2]);
      if((*this)[3] != 1.)
        s << std::setw(2) << (int)((*this)[3]);
      return "#"+s.str();
    }
  else
    return rgb().hex_str();
}

codac2::Vector Color::vec() const
{
  return codac2::Vector({(*this)[0], (*this)[1], (*this)[2], (*this)[3]});
}