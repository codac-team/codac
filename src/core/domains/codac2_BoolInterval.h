/** 
 *  \file codac2_BoolInterval.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

namespace codac2
{
  enum class BoolInterval
  {
    EMPTY = 0x01 & 0x02,
    NO = 0x01,
    YES = 0x02,
    UNKNOWN = 0x01 | 0x02
  };

  inline BoolInterval operator&(BoolInterval a, BoolInterval b)
  { return static_cast<BoolInterval>(static_cast<int>(a) & static_cast<int>(b)); }

  inline BoolInterval operator|(BoolInterval a, BoolInterval b)
  { return static_cast<BoolInterval>(static_cast<int>(a) | static_cast<int>(b)); }
}