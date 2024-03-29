/** 
 *  \file
 *  CtcInverseNotIn class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCINVERSENOTIN__
#define __CODAC2_CTCINVERSENOTIN__

#include <map>
#include "codac2_CtcUnion.h"
#include "codac2_CtcInverse.h"

namespace codac2
{
  template<typename T>
  class CtcInverseNotIn : public Ctc_<IntervalVector>
  {
    public:

      CtcInverseNotIn(const Function<T>& f, const T& y)
      {
        _ctc_not_in = CtcUnion(); // empty initialization

        for(const auto& complem_y : y.complementary())
          _ctc_not_in |= CtcInverse_IntervalVector(f, complem_y);
      }

      void contract(IntervalVector& x) const
      {
        _ctc_not_in.contract(x);
      }

    protected:
      
      CtcUnion _ctc_not_in;
  };
}

#endif