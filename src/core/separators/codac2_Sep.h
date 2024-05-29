/** 
 *  \file codac2_Sep.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <memory>
#include "codac2_IntervalVector.h"

namespace codac2
{
  struct BoxPair
  {
    IntervalVector in;
    IntervalVector out;
  };

  inline std::ostream& operator<<(std::ostream& os, const BoxPair& x)
  {
    os << "(" << x.in << "," << x.out << ")";
    return os;
  }

  class Sep
  {
    public:

      Sep(size_t n)
        : _n(n)
      { }

      size_t size() const
      {
        return _n;
      }

      virtual std::shared_ptr<Sep> copy() const = 0;
      virtual BoxPair separate(const IntervalVector& x) const = 0;

    protected:

      const size_t _n;
  };
}