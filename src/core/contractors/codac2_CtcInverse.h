/** 
 *  \file
 *  CtcInverse class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCINVERSE__
#define __CODAC2_CTCINVERSE__

#include <map>
#include "codac2_Function.h"
#include "codac2_Ctc.h"
#include "codac2_CtcWrapper.h"

namespace codac2
{
  template<typename T>
  class CtcInverse : public Ctc
  {
    public:

      CtcInverse(const Function<T>& f, const T& y)
        : _f(f), _ctc_y(std::make_shared<CtcWrapper_<T>>(y))
      { }

      template<typename C>
      CtcInverse(const Function<T>& f, const C& ctc_y)
        : _f(f), _ctc_y(std::make_shared<C>(ctc_y))
      { }

      template<typename... X>
      void contract(X&... x) const
      {
        ValuesMap v;
        _f.fill_from_args(v, x...);

        // Forward/backward algorithm:
        _f.expr()->fwd_eval(v);
        _ctc_y->contract(_f.expr()->value(v));
        _f.expr()->bwd_eval(v);

        _f.intersect_from_args(v, x...);
      }

    protected:

      const Function<T> _f;
      const std::shared_ptr<Ctc_<T>> _ctc_y;
  };

  template<typename T>
  class CtcInverse_IntervalVector : public Ctc_<IntervalVector>, public CtcInverse<T>
  {
    public:

      CtcInverse_IntervalVector(const Function<T>& f, const T& y)
        : CtcInverse<T>(f,y)
      {
        assert(f.args().size() == 1);
      }

      CtcInverse_IntervalVector(const Function<T>& f, const Ctc_<T>& ctc_y)
        : CtcInverse<T>(f,ctc_y)
      {
        assert(f.args().size() == 1);
      }

      void contract(IntervalVector& x) const
      {
        CtcInverse<T>::contract(x);
      }
  };
}

#endif