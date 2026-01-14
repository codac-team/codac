/** 
 *  \file codac2_TimePropag.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <ostream>

namespace codac2
{
  /**
   * \enum TimePropag
   * \brief Enumeration specifying the temporal propagation way (forward or backward in time).
   * 
   * The logical operators ``&`` and ``|`` can be used to combine ``TimePropag`` values.
   */
  enum class TimePropag
  {
    FWD = 0x01,
    BWD = 0x02,
    FWD_BWD = 0x01 | 0x02
  };

  constexpr TimePropag operator&(TimePropag a, TimePropag b)
  { return static_cast<TimePropag>(static_cast<int>(a) & static_cast<int>(b)); }

  constexpr TimePropag operator|(TimePropag a, TimePropag b)
  { return static_cast<TimePropag>(static_cast<int>(a) | static_cast<int>(b)); }
  
  /**
   * \brief Streams out a TimePropag
   * 
   * \param os the stream to be updated
   * \param x the time propagation to stream out
   * \return a reference to the updated stream
   */
  inline std::ostream& operator<<(std::ostream& os, const TimePropag& x)
  {
    switch(x)
    {
      case TimePropag::FWD:
        os << "[ fwd ]";
        break;
      case TimePropag::BWD:
        os << "[ bwd ]";
        break;
      case TimePropag::FWD_BWD:
        os << "[ fwd, bwd ]";
        break;
    }
    return os;
  }
}