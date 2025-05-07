/** 
 *  \file codac2_CtcInnerOuter.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <type_traits>
#include "codac2_Ctc.h"
#include "codac2_Sep.h"
#include "codac2_Collection.h"
#include "codac2_template_tools.h"

namespace codac2
{
  class CtcInner : public Ctc<CtcInner,IntervalVector>
  {
    public:

      template<typename S>
        requires IsSepBaseOrPtr<S>
      CtcInner(const S& s)
        : Ctc<CtcInner,IntervalVector>(size_of(s)), _seps(s)
      { }

      void contract(IntervalVector& x) const
      {
        auto x_sep = _seps.front()->separate(x);
        x &= x_sep.inner;
      }

    protected:

      const Collection<SepBase> _seps;
  };

  class CtcOuter : public Ctc<CtcOuter,IntervalVector>
  {
    public:

      template<typename S>
        requires IsSepBaseOrPtr<S>
      CtcOuter(const S& s)
        : Ctc<CtcOuter,IntervalVector>(size_of(s)), _seps(s)
      { }

      void contract(IntervalVector& x) const
      {
        auto x_sep = _seps.front()->separate(x);
        x &= x_sep.outer;
      }

    protected:

      const Collection<SepBase> _seps;
  };
}