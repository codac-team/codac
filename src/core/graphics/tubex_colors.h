/** 
 *  \file
 *  Color features
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_COLORS_H__
#define __TUBEX_COLORS_H__

#include <string>

namespace tubex
{
  /**
   * \struct rgb
   * \brief Represents an RGB value
   */
  typedef struct
  {
    float r; ///< red, a value between 0 and 1
    float g; ///< green, a value between 0 and 1
    float b; ///< blue, a value between 0 and 1
  } rgb;

  /**
   * \struct hsv
   * \brief Represents an HSV value
   */
  typedef struct
  {
    float h; ///< hue, angle value in degrees between 0 and 359
    float s; ///< saturation, a value between 0 and 1
    float v; ///< value (lightness), a value between 0 and 1
  } hsv;

  /**
   * \brief Represents an RGB value in a HTML standard
   *
   * \param rgb_value
   * \param alpha optional transparency component, between 0 and 255, (-1 if disabled)
   * \param prefix optional caracters ("#" by default)
   * \return the HTML string
   */
  const std::string rgb2hex(rgb rgb_value, int alpha = -1, const char* prefix = "#");

  /**
   * \brief Converts HSV to RGB
   *
   * \param hsv_value
   * \return RGB value
   */
  rgb hsv2rgb(hsv hsv_value);

  /**
   * \brief Converts RGB to HSV
   *
   * \param rgb_value
   * \return HSV value
   */
  hsv rgb2hsv(rgb rgb_value);
}

#endif