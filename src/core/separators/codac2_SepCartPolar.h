/** 
 *  \file codac2_SepCartPolar.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Verlein Radwan, Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Sep.h"
#include "codac2_Collection.h"

namespace codac2
{
  // For separating a polar box from a Sep expressed in Cartesian coordinates
  class SepCartPolar : public Sep<SepCartPolar>
  {
    public:

      template<typename S>
        requires IsSepBaseOrPtr<S>
      SepCartPolar(const S& s)
        : Sep<SepCartPolar>(2), _sep(s)
      {
        assert_release(size_of(s) == 2);
      }
      
      BoxPair separate(const IntervalVector& x) const;

    protected:

      const Collection<SepBase> _sep;
  };
}