/** 
 *  \file
 *  Sep class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
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

  class Sep
  {
    public:

      constexpr size_t size() const
      {
        return 2;
      }

      virtual std::shared_ptr<Sep> copy() const = 0;
      virtual BoxPair separate(const IntervalVector& x) const = 0;
  };
}