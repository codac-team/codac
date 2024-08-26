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
    IntervalVector inner;
    IntervalVector outer;
  };

  inline std::ostream& operator<<(std::ostream& os, const BoxPair& x)
  {
    os << "(" << x.inner << "," << x.outer << ")";
    return os;
  }

  class SepBase
  {
    public:

      SepBase(size_t n)
        : _n(n)
      {
        assert(n > 0);
      }

      size_t size() const
      {
        return _n;
      }

      virtual std::shared_ptr<SepBase> copy() const = 0;
      virtual BoxPair separate(const IntervalVector& x) const = 0;

    protected:

      const size_t _n;
  };

  template<typename S>
  class Sep : public SepBase
  {
    public:

      Sep(size_t n)
        : SepBase(n)
      { }

      virtual std::shared_ptr<SepBase> copy() const
      {
        return std::make_shared<S>(*dynamic_cast<const S*>(this));
      }
  };
}