/** 
 *  \file codac2_SepPolarCart.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Benoît Desrochers, (Simon Rohou)
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Sep.h"
#include "codac2_Collection.h"

namespace codac2
{
  // For separating a Cartesian box from a Sep expressed in polar coordinates
  class SepPolarCart : public Sep<SepPolarCart>
  {
    public:

      template<typename S>
        requires IsSepBaseOrPtr<S>
      SepPolarCart(const S& s)
        : Sep<SepPolarCart>(2), _sep(s)
      {
        assert_release(size_of(s) == 2);
      }
      
      BoxPair separate(const IntervalVector& x) const;

    protected:

      const Collection<SepBase> _sep;
  };
}