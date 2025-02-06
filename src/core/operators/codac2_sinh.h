/** 
 *  \file codac2_sinh.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExprWrapper.h"

namespace codac2
{
  struct SinhOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarType fwd_natural(const ScalarType& x1);
    static ScalarType fwd_centered(const ScalarType& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  // Analytic operator
  // The following function can be used to build analytic expressions.

  inline ScalarExpr
  sinh(const ScalarExpr& x1)
  {
    return { std::make_shared<AnalyticOperationExpr<SinhOp,ScalarType,ScalarType>>(x1) };
  }
  
  // Inline functions

  inline Interval SinhOp::fwd(const Interval& x1)
  {
    return sinh(x1);
  }

  inline ScalarType SinhOp::fwd_natural(const ScalarType& x1)
  {
    return {
      fwd(x1.a),
      x1.def_domain
    };
  }

  inline ScalarType SinhOp::fwd_centered(const ScalarType& x1)
  {
    if(centered_form_not_available_for_args(x1))
      return fwd_natural(x1);

    IntervalMatrix d(1,x1.da.size());
    for(Index i = 0 ; i < d.size() ; i++)
      d(0,i) = cosh(x1.a)*x1.da(0,i);

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.def_domain
    };
  }

  inline void SinhOp::bwd(const Interval& y, Interval& x1)
  {
    x1 &= asinh(y);
  }
}