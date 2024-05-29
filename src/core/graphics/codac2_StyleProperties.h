/** 
 *  \file codac2_StyleProperties.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>

namespace codac2
{
  /**
   * \struct ColorRGB
   * \brief Represents an RGB value
   */
  struct ColorRGB
  {
    float r;          ///< red, value between 0. and 1.
    float g;          ///< green, value between 0. and 1.
    float b;          ///< blue, value between 0. and 1.
    float alpha = 1.; ///< opacity, value between 0. (transparent) and 1. (opaque)
    mutable std::string hex_str;

    explicit ColorRGB(int r_, int g_, int b_, int alpha_ = 255)
      : r(r_/255.), g(g_/255.), b(b_/255.), alpha(alpha_/255.)
    {
      assert(r_ >= 0 && r_ <= 255 && g_ >= 0 && g_ <= 255 && b_ >= 0 && b_ <= 255 && alpha_ >= 0 && alpha_ <= 255);
    }

    explicit ColorRGB(float r_, float g_, float b_, float alpha_ = 1.)
      : r(r_), g(g_), b(b_), alpha(alpha_)
    { 
      assert(r_ >= 0. && r_ <= 1. && g_ >= 0. && g_ <= 1. && b_ >= 0. && b_ <= 1. && alpha_ >= 0. && alpha_ <= 1.);
    }

    std::string to_hex_str(const std::string& prefix = "#") const;

    static ColorRGB black()     { return ColorRGB(0,   0,   0);   };
    static ColorRGB white()     { return ColorRGB(255, 255, 255); };
    static ColorRGB green()     { return ColorRGB(144, 242, 0);   };
    static ColorRGB cyan()      { return ColorRGB(75,  207, 250); };
    static ColorRGB yellow()    { return ColorRGB(255, 211, 42);  };
    static ColorRGB red()       { return ColorRGB(209, 59,  0);   };
    static ColorRGB dark_gray() { return ColorRGB(112, 112, 112); };
  };

  /**
   * \brief Represents an RGB value in a HTML standard
   *
   * \param c the color in RGB format
   * \param prefix optional characters ("#" by default)
   * \return the HTML string
   */
  std::string rgb2hex(ColorRGB c, const char* prefix = "#");

  struct StyleProperties
  {
    ColorRGB stroke_color = ColorRGB::black();
    ColorRGB fill_color = []() { auto c = ColorRGB::white(); c.alpha = 0.; return c; }();

    StyleProperties();
    StyleProperties(const std::string& vibes_style);
    StyleProperties(const char* vibes_style);

    static StyleProperties inner();
    static StyleProperties outer();
    static StyleProperties boundary();
  };
}