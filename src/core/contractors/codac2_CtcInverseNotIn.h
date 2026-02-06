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
  template<typename Y,typename X=IntervalVector>
  class CtcInverseNotIn : public CtcUnion<X>
  {
    public:

      CtcInverseNotIn(const AnalyticFunction<typename ExprType<Y>::Type>& f, const Y& y, bool with_centered_form = true)
        : CtcUnion<X>(f.args()[0]->size() /* f must have only one arg, see following assert */)
      {
        assert_release(f.args().size() == 1 && "f must have only one arg");
        const bool is_not_in = true;

        Y f_codomain(y);

        if constexpr(std::is_same_v<X,Interval>)
          f_codomain = f.eval(Interval());
        else if constexpr(std::is_same_v<X,IntervalVector>)
          f_codomain = f.eval(IntervalVector(f.args()[0]->size()));
        else
          std::cout << "CtcInverseNotIn: matrices expressions not (yet) supported" << std::endl;

        for(const auto& complem_y : y.complementary())
        {
          Y w = complem_y & f_codomain;
          if(!w.is_empty() && !w.is_subset(y))
            *this |= CtcInverse<Y,X>(f, complem_y, with_centered_form, is_not_in);
        }
      }

      template<typename C>
        requires IsCtcBaseOrPtr<C,Y>
      CtcInverseNotIn(const AnalyticFunction<typename ExprType<Y>::Type>& f, const C& ctc_compl, bool with_centered_form = true)
        : CtcUnion<X>(f.args()[0]->size() /* f must have only one arg, see following assert */)
      {
        assert_release(f.args().size() == 1 && "f must have only one arg");
        const bool is_not_in = true;
        
        *this |= CtcInverse<Y,X>(f, ctc_compl, with_centered_form, is_not_in);
      }

      void contract(X& x) const
      {
        CtcUnion<X>::contract(x);
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