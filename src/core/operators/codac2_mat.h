/** 
 *  \file codac2_matrix.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExprWrapper.h"

namespace codac2
{
  struct MatrixOp
  {
    template<typename... X>
    static std::string str(const X&... x)
    {
      std::string s = (("\t" + x->str() + ",\n") + ...);
      s.pop_back(); s.pop_back(); // removes last separation
      return "[\n" + s + "\n]";
    }

    template<typename X1, typename... X>
    static std::pair<Index,Index> output_shape(const X1& s1, [[maybe_unused]] const X&... s)
    {
      auto shape1=s1->output_shape();
      assert_release(shape1.second==1);
      return { shape1.first, 1+sizeof...(X) };
    }

    static inline void set_col_i(IntervalMatrix& m, const IntervalVector& x, Index i)
    {
      assert(i >= 0 && i < m.cols());
      assert_release(x.size() == m.rows());
      m.col(i) = x;
    }

    template<typename X1,typename... X>
      requires ((X1::ColsAtCompileTime == 1) && ((X::ColsAtCompileTime == 1) && ...))
    static inline IntervalMatrix fwd(const X1& x1, const X&... x)
    {
      IntervalMatrix m(x1.size(), 1+sizeof...(X));
      Index i = 0;
      MatrixOp::set_col_i(m, x1, i++); // first column x1
      (MatrixOp::set_col_i(m, x, i++), ...); // other columns
      return m;
    }

    template<typename... X>
      requires (std::is_base_of_v<VectorType,X> && ...)
    static inline MatrixType fwd_natural(const X&... x)
    {
      return {
        MatrixOp::fwd(x.a...),
        (x.def_domain && ...)
      };
    }

    static inline void fill_diff_matrix(IntervalMatrix &d, 
	const IntervalMatrix &dax, Index &l) {
       d.middleRows(l,dax.rows())=dax;
       l += dax.rows();
    }


    template<typename X1, typename... X>
      requires (std::is_base_of_v<VectorType,X1> 
		&& (std::is_base_of_v<VectorType,X> && ...))
    static inline MatrixType fwd_centered(const X1& x1, const X&... x)
    {
      if (centered_form_not_available_for_args(x1,x...))
        return fwd_natural(x1,x...);

      IntervalMatrix d(x1.a.size()*(1+sizeof...(X)),x1.da.cols());
      Index l=0;
      d.topRows(x1.da.rows()) = x1.da;
      l += x1.da.rows();
      ( MatrixOp::fill_diff_matrix(d,x.da,l) , ...);
      assert (l==d.rows());
      
      return {
        MatrixOp::fwd(x1.m,x.m...),
        MatrixOp::fwd(x1.a,x.a...),
        d,
        (x1.def_domain && (x.def_domain && ...))
      };
    }

    template<typename... X>
      requires (std::is_base_of_v<IntervalVector,X> && ...)
    static inline void bwd(const IntervalMatrix& y, X&... x)
    {
      Index i = 0;
      ((x &= y.col(i++)), ...);
    }
  };

  // Analytic operator
  // The following function can be used to build analytic expressions.

  // Generic variadic case, cannot handle const values (int, double) for now

  template<typename... X>
  inline MatrixExpr
  mat(const std::shared_ptr<AnalyticExpr<X>>&... x)
  {
    return { std::make_shared<AnalyticOperationExpr<MatrixOp,MatrixType,X...>>(
      AnalyticOperationExpr<MatrixOp,MatrixType,X...>(x...)) };
  }
}
