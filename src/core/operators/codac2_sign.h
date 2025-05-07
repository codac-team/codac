/** 
 *  \file codac2_sign.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExprWrapper.h"

namespace codac2
{
  struct SignOp
  {
    template<typename X1>
    static std::string str(const X1& x1)
    {
      return "sign(" + x1->str() + ")";
    }

    template<typename X1>
    static std::pair<Index,Index> output_shape([[maybe_unused]] const X1& s1)
    {
      return {1,1};
    }

    static Interval fwd(const Interval& x1);
    static ScalarType fwd_natural(const ScalarType& x1);
    static ScalarType fwd_centered(const ScalarType& x1);
    static void bwd(const Interval& y, Interval& x1);
  };

  // Analytic operator
  // The following function can be used to build analytic expressions.

  inline ScalarExpr
  sign(const ScalarExpr& x1)
  {
    return { std::make_shared<AnalyticOperationExpr<SignOp,ScalarType,ScalarType>>(x1) };
  }
    
  // Inline functions

  inline Interval SignOp::fwd(const Interval& x1)
  {
    return sign(x1);
  }

  inline ScalarType SignOp::fwd_natural(const ScalarType& x1)
  {
    return {
      fwd(x1.a),
      x1.def_domain && x1.a != 0. // def domain of the derivative of sign
    };
  }

  inline ScalarType SignOp::fwd_centered(const ScalarType& x1)
  {
    if(centered_form_not_available_for_args(x1))
      return fwd_natural(x1);

    return {
      fwd(x1.m),
      fwd(x1.a),
      IntervalMatrix::zero(1,x1.da.size()),
      x1.def_domain && x1.a != 0. // def domain of the derivative of sign
    };
  }

  inline void SignOp::bwd(const Interval& y, Interval& x1)
  {
    if(y.is_empty())
      x1.set_empty();

    if(y.lb() > 0)
      x1 &= Interval(0,oo);

    else if(y.ub() < 0)
      x1 &= Interval(-oo,0);
  }
}
