/** 
 *  \file codac2_tan.h
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
  struct TanOp
  {
    static Interval fwd(const Interval& x1);
    static ScalarType fwd_natural(const ScalarType& x1);
    static ScalarType fwd_centered(const ScalarType& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  // Analytic operator
  // The following function can be used to build analytic expressions.

  inline ScalarExpr
  tan(const ScalarExpr& x1)
  {
    return { std::make_shared<AnalyticOperationExpr<TanOp,ScalarType,ScalarType>>(x1) };
  }
  
  // Inline functions

  inline Interval TanOp::fwd(const Interval& x1)
  {
    return tan(x1);
  }

  inline ScalarType TanOp::fwd_natural(const ScalarType& x1)
  {
    return {
      fwd(x1.a),
      x1.def_domain && cos(x1.a) != 0. // def domain of the derivative of tan
    };
  }

  inline ScalarType TanOp::fwd_centered(const ScalarType& x1)
  {
    if(centered_form_not_available_for_args(x1))
      return fwd_natural(x1);

    IntervalMatrix d(1,x1.da.size());
    for(Index i = 0 ; i < d.size() ; i++)
      d(0,i) = x1.da(0,i)/sqr(cos(x1.a));

    return {
      fwd(x1.m),
      fwd(x1.a),
      d,
      x1.def_domain && cos(x1.a) != 0. // def domain of the derivative of tan
    };
  }

  inline void TanOp::bwd(const Interval& y, Interval& x1)
  {
    x1 = gaol::atan_rel(y, x1);
  }
}