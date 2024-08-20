/** 
 *  \file codac2_BoolInterval.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <ostream>

namespace codac2
{
  enum class BoolInterval
  {
    EMPTY = 0x01 & 0x02,
    FALSE = 0x01,
    TRUE = 0x02,
    UNKNOWN = 0x01 | 0x02
  };

  constexpr BoolInterval operator&(BoolInterval a, BoolInterval b)
  { return static_cast<BoolInterval>(static_cast<int>(a) & static_cast<int>(b)); }

  constexpr BoolInterval operator|(BoolInterval a, BoolInterval b)
  { return static_cast<BoolInterval>(static_cast<int>(a) | static_cast<int>(b)); }
  
  /**
   * \brief Streams out a BoolInterval
   * 
   * \param os the stream to be updated
   * \param x the boolean interval to stream out
   * \return a reference to the updated stream
   */
  inline std::ostream& operator<<(std::ostream& os, const BoolInterval& x)
  {
    switch(x)
    {
      case BoolInterval::EMPTY:
        os << "[ empty ]";
        break;
      case BoolInterval::FALSE:
        os << "[ false ]";
        break;
      case BoolInterval::TRUE:
        os << "[ true ]";
        break;
      case BoolInterval::UNKNOWN:
        os << "[ true, false ]";
        break;
    }
    return os;
  }
}