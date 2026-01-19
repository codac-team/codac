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
#include <set>
#include "codac2_StylePropertiesBase.h"
#include "codac2_Color.h"

namespace codac2
{

  /**
   * \struct StyleProperties
   * \brief Style properties structure, to specify the style of a shape
   * 
   * This class is used to specify the style of a shape, including the stroke color, fill color,  line style, line width and layer.
   */
  struct StyleProperties : public StylePropertiesBase
  {
    Color stroke_color = Color::black();
    Color fill_color = Color::none();

    /**
     * \brief Default constructor
     * 
     * Black stroke color, transparent fill color, solid line style and layer "alpha"
     */
    StyleProperties();

    /**
     * \brief Constructor with only optionnal parameters
     * 
     * \param param1 Optional parameter, can be layer name, line width, line style or z-value
     * \param param2 Optional parameter, can be layer name, line width, line style or z-value
     * \param param3 Optional parameter, can be layer name, line width, line style or z-value
     * \param param4 Optional parameter, can be layer name, line width, line style or z-value
     */
    StyleProperties(const std::string& param1, const std::string& param2 = "", const std::string& param3 = "", const std::string& param4 = "");

    /**
     * \brief Constructor from a stroke color, and three optional parameters
     * 
     * \param stroke_color Stroke color
     * \param param1 Optional parameter, can be layer name, line width, line style or z-value
     * \param param2 Optional parameter, can be layer name, line width, line style or z-value
     * \param param3 Optional parameter, can be layer name, line width, line style or z-value
     * \param param4 Optional parameter, can be layer name, line width, line style or z-value
     */
    StyleProperties(const Color& stroke_color, const std::string& param1 = "", const std::string& param2 = "", const std::string& param3 = "", const std::string& param4 = "");

    /**
     * \brief Constructor from a stroke and eventually a fill color, and three optional parameters
     * 
     * \param colors list of colors (stroke color and eventually fill color)
     * \param param1 Optional parameter, can be layer name, line width, line style or z-value
     * \param param2 Optional parameter, can be layer name, line width, line style or z-value
     * \param param3 Optional parameter, can be layer name, line width, line style or z-value
     * \param param4 Optional parameter, can be layer name, line width, line style or z-value
     */
    StyleProperties(std::initializer_list<Color> colors, const std::string& param1 = "", const std::string& param2 = "", const std::string& param3 = "", const std::string& param4 = "");

    /**
     * \brief Predefined "inside" style, dark gray edge and green fill
     */
    static StyleProperties inside();

    /**
     * \brief Predefined "outside" style, dark gray edge and light blue fill
     */
    static StyleProperties outside();

    /**
     * \brief Predefined "boundary" style, dark gray edge and yellow fill
     */
    static StyleProperties boundary();

  };
}