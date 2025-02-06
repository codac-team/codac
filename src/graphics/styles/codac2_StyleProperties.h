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
#include "codac2_Color.h"

namespace codac2
{
  struct StyleProperties
  {
    Color stroke_color = Color::black();
    Color fill_color = Color::none();

    StyleProperties();
    StyleProperties(const Color& stroke_color);
    StyleProperties(std::initializer_list<Color> colors);

    static StyleProperties inside();
    static StyleProperties outside();
    static StyleProperties boundary();
  };
}