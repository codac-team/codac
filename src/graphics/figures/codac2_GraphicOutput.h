/** 
 *  \file codac2_GraphicOutput.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

namespace codac2
{
  /**
   * \enum GraphicOutput
   * \brief Enumeration for graphic output backends
   */
  enum class GraphicOutput
  {
    VIBES = 0x01,
    IPE   = 0x02,
    RERUN = 0x04,
    OBJ   = 0x08
  };

  constexpr int operator&(GraphicOutput a, GraphicOutput b)
  { return static_cast<int>(static_cast<int>(a) & static_cast<int>(b)); }

  constexpr GraphicOutput operator|(GraphicOutput a, GraphicOutput b)
  { return static_cast<GraphicOutput>(static_cast<int>(a) | static_cast<int>(b)); }
}
