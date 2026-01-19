/** 
 *  codac2_StyleGradientProperties.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_StyleGradientProperties.h"

using namespace std;
using namespace codac2;

StyleGradientProperties::StyleGradientProperties()
{ }

StyleGradientProperties::StyleGradientProperties(const std::string& param1, const std::string& param2, const std::string& param3, const std::string& param4)
  : StylePropertiesBase(param1, param2, param3, param4)
{ }

StyleGradientProperties::StyleGradientProperties(const ColorMap& cmap_, const std::string& param1, const std::string& param2, const std::string& param3, const std::string& param4)
  : StylePropertiesBase(param1, param2, param3, param4), cmap(cmap_)
{ }