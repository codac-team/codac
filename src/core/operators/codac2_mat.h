/** 
 *  \file codac2_matrix.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
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
    static void fwd_i(IntervalMatrix& m, const IntervalVector& x, Index i);

    template<typename... X>
      requires (std::is_base_of_v<Domain,X> && ...)
    static inline IntervalMatrix fwd(const X&... x)
    {
      throw std::runtime_error("MatrixOp not fully implemented yet");
      IntervalMatrix m(1, sizeof...(X));
      Index i = 0;
      (MatrixOp::fwd_i(m, x, i++), ...);
      return m;
    }

    template<typename... X>
      requires (std::is_base_of_v<VectorType,X> && ...)
    static inline MatrixType fwd_natural(const X&... x)
    {
      throw std::runtime_error("MatrixOp not fully implemented yet");
      return {
        IntervalMatrix({x.a...}),
        true // todo with variadic
      };
    }

    template<typename... X>
      requires (std::is_base_of_v<VectorType,X> && ...)
    static inline MatrixType fwd_centered(const X&... x)
    {
      throw std::runtime_error("MatrixOp not fully implemented yet");
      return {
        IntervalMatrix({x.m...}),
        IntervalMatrix({x.a...}),
        IntervalMatrix(0,0), // not supported yet for matrices
        true // todo with variadic
      };
    }

    template<typename... X>
      requires (std::is_base_of_v<IntervalVector,X> && ...)
    static inline void bwd(const IntervalMatrix& y, X&... x)
    {
      throw std::runtime_error("MatrixOp not fully implemented yet");
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

  // Inline functions

  inline void MatrixOp::fwd_i(IntervalMatrix& m, const IntervalVector& x, Index i)
  {
    assert(i >= 0 && i < m.cols());
    m.resize(x.size(),m.cols());
    m.col(i) = x;
  }
}