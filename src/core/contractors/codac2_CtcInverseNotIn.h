/** 
 *  \file codac2_CtcInverseNotIn.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include "codac2_CtcUnion.h"
#include "codac2_CtcInverse.h"

namespace codac2
{
  template<typename Y_, typename X0=IntervalVector, typename... X>
  class CtcInverseNotIn : public CtcUnion<X0,X...>
  {
    public:

      using Y = typename Wrapper<Y_>::Domain; // Y_ is a possible Eigen expr. type
      using OutputType = typename ExprType<Y>::Type;

      CtcInverseNotIn(const AnalyticFunction<OutputType>& f, const Y& y, bool with_centered_form = true)
        : CtcUnion<X0,X...>(f.input_size())
      {
        const bool is_not_in = true;
        Y f_codomain = f.template eval_default_codomain<X0,X...>();
        for(const auto& complem_y : y.complementary())
        {
          Y w = complem_y & f_codomain;
          if(!w.is_empty() && !w.is_subset(y))
            *this |= CtcInverse<Y,X0,X...>(f, complem_y, with_centered_form, is_not_in);
        }
      }

      template<typename C>
        requires IsCtcBaseOrPtr<C,Y>
      CtcInverseNotIn(const AnalyticFunction<OutputType>& f, const C& ctc_compl, bool with_centered_form = true)
        : CtcUnion<X0,X...>(f.input_size())
      {
        const bool is_not_in = true;
        *this |= CtcInverse<Y,X0,X...>(f, ctc_compl, with_centered_form, is_not_in);
      }

      void contract(X0& x0, X&... x) const
      {
        CtcUnion<X0,X...>::contract(x0,x...);
      }
  };

  // Template deduction guides
  // Same as CtcInverse

  // ScalarType

    CtcInverseNotIn(const AnalyticFunction<ScalarType>&, std::initializer_list<double>, bool = true, bool = false) -> 
      CtcInverseNotIn<Interval,IntervalVector>;

    template<typename Y>
    CtcInverseNotIn(const AnalyticFunction<ScalarType>&, std::initializer_list<Y>, bool = true, bool = false) -> 
      CtcInverseNotIn<Interval,IntervalVector>;

    CtcInverseNotIn(const AnalyticFunction<ScalarType>&, const Interval&, bool = true, bool = false) -> 
      CtcInverseNotIn<Interval,IntervalVector>;

    CtcInverseNotIn(const AnalyticFunction<ScalarType>&, double, bool = true, bool = false) -> 
      CtcInverseNotIn<Interval,IntervalVector>;

    template<typename C>
      requires IsCtcBaseOrPtr<C,Interval>
    CtcInverseNotIn(const AnalyticFunction<ScalarType>&, const C&, bool = true, bool = false) -> 
      CtcInverseNotIn<Interval,IntervalVector>;

  // VectorType

    CtcInverseNotIn(const AnalyticFunction<VectorType>&, std::initializer_list<double>, bool = true, bool = false) -> 
      CtcInverseNotIn<IntervalVector,IntervalVector>;

    CtcInverseNotIn(const AnalyticFunction<VectorType>&, std::initializer_list<std::initializer_list<double>>, bool = true, bool = false) -> 
      CtcInverseNotIn<IntervalVector,IntervalVector>;

    CtcInverseNotIn(const AnalyticFunction<VectorType>&, const Vector&, bool = true, bool = false) -> 
      CtcInverseNotIn<IntervalVector,IntervalVector>;

    CtcInverseNotIn(const AnalyticFunction<VectorType>&, const IntervalVector&, bool = true, bool = false) -> 
      CtcInverseNotIn<IntervalVector,IntervalVector>;

    template<typename OtherDerived>
      requires (OtherDerived::RowsAtCompileTime == -1 && OtherDerived::ColsAtCompileTime == 1)
    CtcInverseNotIn(const AnalyticFunction<VectorType>&, const Eigen::MatrixBase<OtherDerived>&, bool = true, bool = false) -> 
      CtcInverseNotIn<IntervalVector,IntervalVector>;

    template<typename C>
      requires IsCtcBaseOrPtr<C,IntervalVector>
    CtcInverseNotIn(const AnalyticFunction<VectorType>&, const C&, bool = true, bool = false) -> 
      CtcInverseNotIn<IntervalVector,IntervalVector>;

  // MatrixType
        
    CtcInverseNotIn(const AnalyticFunction<MatrixType>&, std::initializer_list<std::initializer_list<double>>, bool = true, bool = false) -> 
      CtcInverseNotIn<IntervalMatrix,IntervalVector>;

    CtcInverseNotIn(const AnalyticFunction<MatrixType>&, std::initializer_list<std::initializer_list<std::initializer_list<double>>>, bool = true, bool = false) -> 
      CtcInverseNotIn<IntervalMatrix,IntervalVector>;

    template<typename OtherDerived>
      requires (OtherDerived::RowsAtCompileTime == -1 && OtherDerived::ColsAtCompileTime == -1)
    CtcInverseNotIn(const AnalyticFunction<MatrixType>&, const Eigen::MatrixBase<OtherDerived>&, bool = true, bool = false) -> 
      CtcInverseNotIn<IntervalMatrix,IntervalVector>;

    template<typename C>
      requires IsCtcBaseOrPtr<C,IntervalMatrix>
    CtcInverseNotIn(const AnalyticFunction<MatrixType>&, const C&, bool = true, bool = false) -> 
      CtcInverseNotIn<IntervalMatrix,IntervalVector>;
}