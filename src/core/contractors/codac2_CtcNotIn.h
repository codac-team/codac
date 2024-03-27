/** 
 *  \file
 *  CtcNotIn class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCNOTIN__
#define __CODAC2_CTCNOTIN__

#include <map>
#include "codac2_CtcUnion.h"
#include "codac2_CtcFunction.h"

namespace codac2
{
  template<typename T>
  class CtcNotIn : public BoxCtc
  {
    public:

      CtcNotIn(const Function<T>& f, const T& y)
      {
        _ctc_not_in = CtcUnion(); // empty initialization

        for(const auto& complem_y : y.complementary())
          _ctc_not_in |= CtcFunctionOnBox(f, complem_y);
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