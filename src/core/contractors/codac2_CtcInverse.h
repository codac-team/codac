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

#pragma once

#include <map>
#include "codac2_Function.h"
#include "codac2_Ctc.h"
#include "codac2_CtcWrapper.h"

namespace codac2
{
  template<typename Y>
  class CtcInverse : public Ctc
  {
    public:

      CtcInverse(const Function<Y>& f, const Y& y)
        : _f(f), _ctc_y(std::make_shared<CtcWrapper_<Y>>(y))
      { }

      template<typename C>
      CtcInverse(const Function<Y>& f, const C& ctc_y)
        : _f(f), _ctc_y(std::make_shared<C>(ctc_y))
      { }

      CtcInverse(const CtcInverse<Y>& c)
        : _f(c._f), _ctc_y(std::dynamic_pointer_cast<Ctc_<Y>>(c._ctc_y->copy()))
      { }

      virtual std::shared_ptr<Ctc> copy() const
      {
        return std::make_shared<CtcInverse<Y>>(*this);
      }

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

      const Function<Y>& function() const
      {
        return _f;
      }

    protected:

      const Function<Y> _f;
      const std::shared_ptr<Ctc_<Y>> _ctc_y;
  };

  template<typename Y,typename X>
  class CtcInverse_ : public Ctc_<X>, public CtcInverse<Y>
  {
    public:

      CtcInverse_(const Function<Y>& f, const Y& y)
        : CtcInverse<Y>(f,y)
      {
        assert(f.args().size() == 1);
      }

      CtcInverse_(const Function<Y>& f, const Ctc_<X>& ctc_y)
        : CtcInverse<Y>(f,ctc_y)
      {
        assert(f.args().size() == 1);
      }

      CtcInverse_(const CtcInverse_<Y,X>& c)
        : CtcInverse<Y>(c)
      { }

      virtual std::shared_ptr<Ctc> copy() const
      {
        return this->CtcInverse<Y>::copy();
      }

      void contract(X& x) const
      {
        CtcInverse<Y>::contract(x);
      }
  };
}