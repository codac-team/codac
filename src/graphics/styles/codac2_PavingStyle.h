/** 
 *  \file codac2_PavingStyle.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_StyleProperties.h"

namespace codac2
{
  struct PavingStyle
  {
    StyleProperties boundary, outside, inside;

    PavingStyle();
    PavingStyle(const StyleProperties& boundary, const StyleProperties& outside, const StyleProperties& inside);

    /**
     * \brief Predefined (default) paving style, blue/yellow/green
     */
    static inline PavingStyle default_style()
    {
      return blue_green();
    }

    /**
     * \brief Predefined paving style, blue/yellow/green
     * Used by default in Codac.
     */
    static PavingStyle blue_green();

    /**
     * \brief Predefined paving style, blue/orange/white
     * From Julien Damers.
     */
    static PavingStyle blue_white();

    /**
     * \brief Predefined pink paving style, blue/yellow/pink
     * From Quentin Brateau.
     */
    static PavingStyle blue_pink();

    /**
     * \brief Predefined pink paving style, black and white
     */
    static PavingStyle black_white();
  };
}