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
      x1.def_domain && !x1.a.contains(0.) // def domain of the derivative of sign
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
      x1.def_domain && !x1.a.contains(0.) // def domain of the derivative of sign
    };
  }

  inline void SignOp::bwd(const Interval& y, Interval& x1)
  {
#if CODAC_GAOL_GUARDS
    if(y.is_empty())
      x1.set_empty();

    if(y.lb() > 0)
      x1 &= Interval(0,oo);

    else if(y.ub() < 0)
      x1 &= Interval(-oo,0);
#else
    // sign_rel() of GAOL v5, the reverse of the sign of GAOL v5 that
    // sign(const Interval&) is (see codac2_Interval_operations_impl.h): the hull
    // of the negative x of x1 where y holds -1, of 0 where y and x1 hold it,
    // and of the positive x of x1 where y holds 1, and the empty set where y
    // holds none of -1, 0 and 1: sign_rel([0.5],x1) is empty, and
    // sign_rel([0],[-2,3]) is [0]. The negative numbers go up to 0 and the
    // positive ones down from 0 in their closed hull: sign_rel([-1],[-2,3]) is
    // [-2,0]. The code above, written for Codac's former sign, a sign of 0
    // being any of [-1,1], only contracted x1 to [0,+oo] when y was positive and
    // to [-oo,0] when y was negative: it kept the whole of x1 for y = [0] and
    // for y = [0,+oo] ([-2,3] for x1 = [-2,3]), and the part of x1 in [0,+oo]
    // for y = [0.5].
    x1 = gaol::sign_rel(y, x1);
#endif
  }
}
