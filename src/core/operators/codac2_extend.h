/** 
 *  \file codac2_extend.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Damien Massé, Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExprWrapper.h"
#include "codac2_analytic_variables.h"

namespace codac2
{
  // extend(f(x),g(x)) = f(x) if x \in D_f, otherwise g(x)

  struct ExtendOp
  {
    template<typename X1,typename X2>
    static std::string str(const X1& x1, const X2& x2)
    {
      return "extend(" + x1->str() + "," + x2->str() + ")";
    }

    template<typename X1, typename X2>
    static std::pair<Index,Index> output_shape(const X1& s1, [[maybe_unused]] const X2& s2)
    {
      auto shape1 = s1->output_shape();
      assert(shape1 == s2->output_shape());
      return shape1;
    }

    template<typename T>
    static inline T fwd(const T& x1, const T& x2)
    {
      return x1 | x2;
    }

    template<typename T>
    static inline T fwd_natural(const T& x1, const T& x2)
    {
      return {
        (x1.def_domain ? x1.a : fwd(x1.a,x2.a)),
        x1.def_domain || x2.def_domain
      };
    }

    template<typename T>
    static inline T fwd_centered(const T& x1, const T& x2)
    {
      return {
        (x1.def_domain ? x1.m : fwd(x1.m,x2.m)),
        (x1.def_domain ? x1.a : fwd(x1.a,x2.a)),
        (x1.def_domain ? x1.da : (x1.da | x2.da)),
        x1.def_domain || x2.def_domain
      };
    }

    template<typename T>
    static inline void bwd(const T& y, T& x1, T& x2)
    {
      // Any value in x1 must be in y
      x1 &= y;
      if (x1.is_empty()) 
      {
        // Only if x1 is empty, values of x2 must be in y
        x2 &= y;
      }
    }
  };

  // Analytic operator
  // The following function can be used to build analytic expressions.

  template<typename T1,typename T2,typename T=ExprType<T1>::Type>
  inline AnalyticExprWrapper<T>
  extend(const T1& x1, const T2& x2)
  {
    return { std::make_shared<AnalyticOperationExpr<ExtendOp,T,T,T>>(
      (AnalyticExprWrapper<T>)x1,(AnalyticExprWrapper<T>)x2) };
  }
}