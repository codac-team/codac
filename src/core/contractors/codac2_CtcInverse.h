/** 
 *  \file codac2_CtcInverse.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include "codac2_AnalyticFunction.h"
#include "codac2_Ctc.h"
#include "codac2_CtcWrapper.h"
#include "codac2_Collection.h"
#include "codac2_arith_mul.h"
#include "codac2_Wrapper.h"

namespace codac2
{
  class CtcNot;
  
  template<typename Y>
  class CtcInverse_// : virtual public Ctc
  {
    public:

      template<typename C>
        requires IsCtcBaseOrPtr<C,Y>
      CtcInverse_(const AnalyticFunction<typename ExprType<Y>::Type>& f, const C& ctc_y, bool with_centered_form = true, bool is_not_in = false)
        : _f(f), _ctc_y(ctc_y), _with_centered_form(with_centered_form), _is_not_in(is_not_in)
      {
        assert_release([&]() { return f.output_size() == size_of(ctc_y); }()
          && "CtcInverse_: invalid dimension of image argument ('y' or 'ctc_y')");
      }

      CtcInverse_(const AnalyticFunction<typename ExprType<Y>::Type>& f, const Y& y, bool with_centered_form = true, bool is_not_in = false)
        : CtcInverse_(f, CtcWrapper<Y>(y), with_centered_form, is_not_in)
      { }

      //std::shared_ptr<CtcBase<X>> copy() const
      //{
      //  return std::make_shared<CtcInverse_<Y>>(*this);
      //}

      template<typename... X>
      void contract(X&... x) const
      {
        return contract_(*_ctc_y.front(), x...);
      }

      template<typename... X>
      void contract_(const Y& y, X&... x) const
      {
        return contract_(CtcWrapper<Y>(y), x...);
      }

      template<typename... X>
      void contract_(const CtcBase<Y>& ctc_y, X&... x) const
      {
        ValuesMap v;
        // Setting user values into a map before the tree evaluation
        _f.fill_from_args(v, x...);

        // Forward/backward algorithm:

          // [1/4] Forward evaluation
          _f.expr()->fwd_eval(v, _f.args().total_size(), !_with_centered_form);
          auto& val_expr = _f.expr()->value(v);

          if(_is_not_in && !val_expr.def_domain)
            return; // <-- iota: if the input x is outside the definition 
          // domain of one of the involved expressions, or their combinations,
          // then the inner contraction is disabled.
          
          // [2/4] Performing top contraction (from the root of the tree) with
          // the contractor expressing the output domain Y.
          // The underlying constraint is:  f(x) \in [y]
          ctc_y.contract(val_expr.a);

          // [3/4 - optional]
          // The contraction can be significantly improved using a centered form
          // expression (enabled by default). This step must be processed before the
          // backward part of the FwdBwd algorithm (the .m, .a values must not be
          // changed before the centered evaluation).
          if(_with_centered_form && val_expr.def_domain && !val_expr.da.is_unbounded() && val_expr.da.size() != 0)
          {
            // todo: the above condition !val_expr.da.is_unbounded() should not be necesary,
            // possible bug in MulOp in case of unbounded domain?
            using X0 = std::tuple_element_t<0, std::tuple<X...>>;

            if constexpr(sizeof...(X) == 1 && std::is_same_v<X0,IntervalVector>)
            {
              X0& x_ = std::get<0>(std::tie(x...));
              X0 x_mid = X0(x_.mid());

              assert(val_expr.a.size() == val_expr.m.size());
              IntervalVector fm { val_expr.a - val_expr.m };

              if constexpr(std::is_same_v<Y,IntervalMatrix>)
              {
                std::cout << "CtcInverse_: matrices expressions not (yet) supported with centered form" << std::endl;
              }

              else
              {
                IntervalVector p = x_ - x_mid;
                MulOp::bwd(fm, val_expr.da, p);
                x_ &= p + x_mid;
              }
            }

            else
            {
              // Centered form not (yet) implemented for multi-nonvector-arguments
            }
          }
          
        // [4/4] Backward evaluation
        _f.expr()->bwd_eval(v); // recursive backward from the root to the leaves
        _f.intersect_from_args(v, x...); // updating input values
      }

      const AnalyticFunction<typename ExprType<Y>::Type>& function() const
      {
        return _f;
      }

    protected:

      const AnalyticFunction<typename ExprType<Y>::Type> _f;
      const Collection<CtcBase<Y>> _ctc_y;
      bool _with_centered_form;
      bool _is_not_in = false;
  };

  template<typename Y,typename X=IntervalVector>
  class CtcInverse : public Ctc<CtcInverse<Y,X>,X>, public CtcInverse_<Y>
  {
    public:

      CtcInverse(const AnalyticFunction<typename ExprType<Y>::Type>& f, const typename Wrapper<Y>::Domain& y, bool with_centered_form = true, bool is_not_in = false)
        : Ctc<CtcInverse<Y,X>,X>(f.args()[0]->size() /* f must have only one arg, see following assert */),
          CtcInverse_<Y>(f, y, with_centered_form,is_not_in)
      {
        assert_release(f.args().size() == 1 && "f must have only one arg");
      }

      template<typename C>
        requires IsCtcBaseOrPtr<C,Y>
      CtcInverse(const AnalyticFunction<typename ExprType<Y>::Type>& f, const C& ctc_y, bool with_centered_form = true, bool is_not_in = false)
        : Ctc<CtcInverse<Y,X>,X>(f.args()[0]->size() /* f must have only one arg, see following assert */),
          CtcInverse_<Y>(f, ctc_y, with_centered_form,is_not_in)
      {
        assert_release(f.args().size() == 1 && "f must have only one arg");
      }

      void contract(X& x) const
      {
        CtcInverse_<Y>::contract(x);
      }
  };

  // Template deduction guides

  // ScalarType

    CtcInverse(const AnalyticFunction<ScalarType>&, std::initializer_list<double>, bool = true, bool = false) -> 
      CtcInverse<Interval,IntervalVector>;

    template<typename Y>
    CtcInverse(const AnalyticFunction<ScalarType>&, std::initializer_list<Y>, bool = true, bool = false) -> 
      CtcInverse<Interval,IntervalVector>;

    CtcInverse(const AnalyticFunction<VectorType>&, const Interval&, bool = true, bool = false) -> 
      CtcInverse<Interval,IntervalVector>;

    template<typename C>
      requires IsCtcBaseOrPtr<C,Interval>
    CtcInverse(const AnalyticFunction<ScalarType>&, const C&, bool = true, bool = false) -> 
      CtcInverse<Interval,IntervalVector>;

  // VectorType

    CtcInverse(const AnalyticFunction<VectorType>&, std::initializer_list<double>, bool = true, bool = false) -> 
      CtcInverse<IntervalVector,IntervalVector>;

    CtcInverse(const AnalyticFunction<VectorType>&, std::initializer_list<std::initializer_list<double>>, bool = true, bool = false) -> 
      CtcInverse<IntervalVector,IntervalVector>;

    CtcInverse(const AnalyticFunction<VectorType>&, const Vector&, bool = true, bool = false) -> 
      CtcInverse<IntervalVector,IntervalVector>;

    CtcInverse(const AnalyticFunction<VectorType>&, const IntervalVector&, bool = true, bool = false) -> 
      CtcInverse<IntervalVector,IntervalVector>;

    template<typename OtherDerived>
      requires (OtherDerived::RowsAtCompileTime == -1 && OtherDerived::ColsAtCompileTime == 1)
    CtcInverse(const AnalyticFunction<VectorType>&, const Eigen::MatrixBase<OtherDerived>&, bool = true, bool = false) -> 
      CtcInverse<IntervalVector,IntervalVector>;

    template<typename C>
      requires IsCtcBaseOrPtr<C,IntervalVector>
    CtcInverse(const AnalyticFunction<VectorType>&, const C&, bool = true, bool = false) -> 
      CtcInverse<IntervalVector,IntervalVector>;

  // MatrixType
        
    CtcInverse(const AnalyticFunction<MatrixType>&, std::initializer_list<std::initializer_list<double>>, bool = true, bool = false) -> 
      CtcInverse<IntervalMatrix,IntervalVector>;

    CtcInverse(const AnalyticFunction<MatrixType>&, std::initializer_list<std::initializer_list<std::initializer_list<double>>>, bool = true, bool = false) -> 
      CtcInverse<IntervalMatrix,IntervalVector>;

    template<typename OtherDerived>
      requires (OtherDerived::RowsAtCompileTime == -1 && OtherDerived::ColsAtCompileTime == -1)
    CtcInverse(const AnalyticFunction<VectorType>&, const Eigen::MatrixBase<OtherDerived>&, bool = true, bool = false) -> 
      CtcInverse<IntervalMatrix,IntervalVector>;

    template<typename C>
      requires IsCtcBaseOrPtr<C,IntervalMatrix>
    CtcInverse(const AnalyticFunction<MatrixType>&, const C&, bool = true, bool = false) -> 
      CtcInverse<IntervalMatrix,IntervalVector>;
}