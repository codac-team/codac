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
      CtcInverse(const AnalyticFunction<typename Wrapper<Y>::Domain>& f, const C& ctc_y, bool with_centered_form = true, bool is_not_in = false)
        : _f(f), _ctc_y(ctc_y), _with_centered_form(with_centered_form), _is_not_in(is_not_in)
      { }

      CtcInverse(const AnalyticFunction<typename Wrapper<Y>::Domain>& f, const Y& y, bool with_centered_form = true, bool is_not_in = false)
        : CtcInverse(f, CtcWrapper_<Y>(y), with_centered_form, is_not_in)
      { }

      virtual std::shared_ptr<Ctc> copy() const
      {
        return std::make_shared<CtcInverse<Y>>(*this);
      }

      template<typename... X>
      void contract(X&... x) const
      {
        contract(_with_centered_form, x...);
      }

      template<typename... X>
      void contract(bool with_centered_form, X&... x) const
      {
        ValuesMap v;
        _f.fill_from_args(v, x...);

        // Forward/backward algorithm:

          // Forward evaluation
          _f.expr()->fwd_eval(v,_f.args(),cart_prod(x...));

          if(_is_not_in && !_f.expr()->value(v).def_domain)
            return;
          
          // f(x) \in [y]
          _ctc_y.front().contract(_f.expr()->value(v).a);

         // Improving contraction with centered form

        if(with_centered_form && _f.expr()->value(v).def_domain)
        {
          using X0 = std::tuple_element_t<0, std::tuple<X...>>;

          if(sizeof...(X) == 1 && std::is_same_v<X0,IntervalVector>)
          {
            auto& vop = _f.expr()->value(v);
            X0& x_ = std::get<0>(std::tie(x...));
            X0 x_mid = X0(x_.mid());

            IntervalVector fm(vop.da.rows());

            if constexpr(std::is_same_v<Y,Interval>)
            {
              assert(fm.size() == 1);
              fm[0] = _f.expr()->value(v).a - vop.m;
            }

            else if constexpr(std::is_same_v<Y,IntervalVector>)
            {
              assert(fm.size() == vop.m.size());
              fm = _f.expr()->value(v).a - vop.m;
            }

            else
            {
              assert(false && "Matrices expressions not (yet) supported with centered form");
            }

            IntervalVector p = x_ - x_mid;
            MulOp::bwd(fm, vop.da, p);
            x_ &= p + x_mid;
          }

          else
          {
            assert(false && "Not (yet) implemented for multi-nonvector-arguments");
            // Not (yet) implemented for multi-nonvector-arguments
          }
        }

        // Backward evaluation
        _f.expr()->bwd_eval(v);
        _f.intersect_from_args(v, x...);          
      }

    protected:

      const AnalyticFunction<typename Wrapper<Y>::Domain> _f;
      const Collection<Ctc_<Y>> _ctc_y;
      bool _with_centered_form;
      bool _is_not_in = false;
  };

  template<typename Y,typename X>
  class CtcInverse_ : public CtcInverse<Y>, public Ctc_<X>
  {
    public:

      CtcInverse_(const AnalyticFunction<typename Wrapper<Y>::Domain>& f, const Y& y, bool with_centered_form = true, bool is_not_in = false)
        : Ctc_<X>(f.args()[0]->size() /* f must have only one arg, see following assert */),
          CtcInverse<Y>(f,y,with_centered_form,is_not_in)
      {
        assert(f.args().size() == 1);
      }

      CtcInverse_(const AnalyticFunction<typename Wrapper<Y>::Domain>& f, const Ctc_<Y>& ctc_y, bool with_centered_form = true, bool is_not_in = false)
        : CtcInverse<Y>(f,ctc_y,with_centered_form,is_not_in)
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