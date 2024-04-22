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
#include "codac2_AnalyticFunction.h"
#include "codac2_Ctc.h"
#include "codac2_CtcWrapper.h"

namespace codac2
{
  template<typename Y>
  class CtcInverse : virtual public Ctc
  {
    public:

      template<typename C>
      CtcInverse(const AnalyticFunction<typename Wrapper<Y>::Domain>& f, const C& ctc_y)
        : _f(f), _ctc_y(ctc_y)
      { }

      CtcInverse(const AnalyticFunction<typename Wrapper<Y>::Domain>& f, const Y& y)
        : CtcInverse(f, CtcWrapper_<Y>(y))
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

          // Forward evaluation
          _f.expr()->fwd_eval(v,_f.args());
          //if constexpr(sizeof...(X) == 1) // with centered form
          //  _f.expr()->value(v) &= _f.centered_eval(x...);
          
          // f(x) \in [y]
          _ctc_y.front().contract(_f.expr()->value(v).a);
          
          // Backward evaluation
          _f.expr()->bwd_eval(v);

        _f.intersect_from_args(v, x...);
      }

    protected:

      const AnalyticFunction<typename Wrapper<Y>::Domain> _f;
      const Collection<Ctc_<Y>> _ctc_y;
  };

  template<typename Y,typename X>
  class CtcInverse_ : public CtcInverse<Y>, public Ctc_<X>
  {
    public:

      CtcInverse_(const AnalyticFunction<typename Wrapper<Y>::Domain>& f, const Y& y)
        : Ctc_<X>(f.args()[0]->size() /* f must have only one arg, see following assert */),
          CtcInverse<Y>(f,y)
      {
        assert(f.args().size() == 1);
      }

      CtcInverse_(const AnalyticFunction<typename Wrapper<Y>::Domain>& f, const Ctc_<Y>& ctc_y)
        : CtcInverse<Y>(f,ctc_y)
      {
        assert(f.args().size() == 1);
      }

      virtual std::shared_ptr<Ctc> copy() const
      {
        return std::make_shared<CtcInverse_<Y,X>>(*this);
      }

      void contract(X& x) const
      {
        CtcInverse<Y>::contract(x);
      }
  };
}