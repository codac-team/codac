/** 
 *  \file
 *  SepInverse class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_SEPINVERSE__
#define __CODAC2_SEPINVERSE__

#include <map>
#include "codac2_Sep.h"
#include "codac2_Function.h"
#include "codac2_CtcInverse.h"
#include "codac2_CtcNotIn.h"

namespace codac2
{
  template<typename T>
  class SepInverse : public Sep
  {
    public:

      SepInverse(const Function<T>& f, const T& y)
        : _ctc_out(f,y), _ctc_in(f,y)
      { }

      SepInverse(const Function<T>& f, const Sep& sep_y)
        : _ctc_out(f,SepCtcOut(sep_y)), _ctc_in(f,SepCtcIn(sep_y))
      { }

      BoxPair separate(const IntervalVector& x) const
      {
        auto x_out = x, x_in = x;
        _ctc_out.contract(x_out);
        _ctc_in.contract(x_in);
        return { x_in, x_out };
      }

    protected:

      const CtcInverse_IntervalVector<T> _ctc_out;
      const CtcInverseNotIn<T> _ctc_in;
  };
}

#endif