/** 
 *  \file codac2_StylePropertiesBase.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <string>
#include <set>
#include "codac2_Color.h"

namespace codac2
{

  /**
   * \struct StylePropertiesBase
   * \brief Style properties structure, to specify the style of a shape
   * 
   * This class is used to specify the style of a shape, i.e. its line style, line width and layer.
   */
  struct StylePropertiesBase
  {
    std::string line_style = "-";
    double line_width = 0.;
    std::string layer = "alpha";

    /**
     * \brief Default constructor
     * 
     * Black stroke color, transparent fill color, solid line style and layer "alpha"
     */
    StylePropertiesBase();

    /**
     * \brief Constructor with only optionnal parameters
     * 
     * \param param1 Optional parameter, can be layer name, line width or line style
     * \param param2 Optional parameter, can be layer name, line width or line style
     * \param param3 Optional parameter, can be layer name, line width or line style
     */
    StylePropertiesBase(const std::string& param1, const std::string& param2 = "", const std::string& param3 = "");

    /**
     * \brief Parse a parameter and update the style properties accordingly
     * 
     * \param param Parameter to parse, can be a line style, line width or layer name
     */
    void parse_parameter(const std::string& param);

    /**
     * \brief Set of available line styles
     */
    static std::set<std::string> available_line_styles()
    {
      std::set<std::string> line_styles={"-", "--", "..", "-.", "-.."};
      return line_styles;
    };

  };
}