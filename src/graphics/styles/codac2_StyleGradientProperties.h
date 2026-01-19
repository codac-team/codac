/** 
 *  \file codac2_StyleGradientProperties.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>
#include <set>
#include "codac2_StylePropertiesBase.h"
#include "codac2_ColorMap.h"

namespace codac2
{

  /**
   * \struct StyleGradientProperties
   * \brief Style properties structure, to specify the style of a shape
   * 
   * This class is used to specify the style of a shape, including the color map, line style, line width and layer.
   */
  struct StyleGradientProperties : public StylePropertiesBase
  {
    ColorMap cmap = ColorMap::basic();

    /**
     * \brief Default constructor
     * 
     * Black stroke color, transparent fill color, solid line style and layer "alpha"
     */
    StyleGradientProperties();

    /**
     * \brief Constructor with only optionnal parameters
     * 
     * \param param1 Optional parameter, can be layer name, line width, line style or z-value
     * \param param2 Optional parameter, can be layer name, line width, line style or z-value
     * \param param3 Optional parameter, can be layer name, line width, line style or z-value
     * \param param4 Optional parameter, can be layer name, line width, line style or z-value
     */
    StyleGradientProperties(const std::string& param1, const std::string& param2 = "", const std::string& param3 = "", const std::string& param4 = "");

    /**
     * \brief Constructor from a color map, and three optional parameters
     * 
     * \param cmap Color map
     * \param param1 Optional parameter, can be layer name, line width, line style or z-value
     * \param param2 Optional parameter, can be layer name, line width, line style or z-value
     * \param param3 Optional parameter, can be layer name, line width, line style or z-value
     * \param param4 Optional parameter, can be layer name, line width, line style or z-value
     */
    StyleGradientProperties(const ColorMap& cmap, const std::string& param1 = "", const std::string& param2 = "", const std::string& param3 = "", const std::string& param4 = "");

  };
}