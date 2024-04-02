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

#pragma once

#include <map>
#include "codac2_Sep.h"
#include "codac2_Function.h"
#include "codac2_CtcInverse.h"
#include "codac2_CtcInverseNotIn.h"

namespace codac2
{
  template<typename Y>
  class SepInverse : public Sep
  {
    using X = IntervalVector;

    public:

      SepInverse(const Function<Y>& f, const Y& y)
        : _ctc_out(f,y), _ctc_in(f,y)
      { }

      SepInverse(const Function<Y>& f, const Sep& sep_y)
        : _ctc_out(f,SepCtcOut(sep_y)), _ctc_in(f,SepCtcIn(sep_y))
      { }

      SepInverse(const CtcInverse_<Y,X>& ctc_out, const CtcInverseNotIn<Y,X>& ctc_in)
        : _ctc_out(ctc_out), _ctc_in(ctc_in)
      { }

      SepInverse(const SepInverse<Y>& s)
        : SepInverse<Y>(s._ctc_out, s._ctc_in)
      { }

      virtual std::shared_ptr<Sep> copy() const
      {
        return std::make_shared<SepInverse<Y>>(*this);
      }

      virtual BoxPair separate(const IntervalVector& x) const
      {
        auto x_out = x, x_in = x;
        _ctc_out.contract(x_out);
        _ctc_in.contract(x_in);
        return { x_in, x_out };
      }

    protected:

      const CtcInverse_<Y,X> _ctc_out;
      const CtcInverseNotIn<Y,X> _ctc_in;
  };
}