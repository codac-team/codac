/** 
 *  \file codac2_arith_div.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_AnalyticType.h"
#include "codac2_AnalyticExprWrapper.h"
#include "codac2_arith_mul.h"

namespace codac2
{
  struct DivOp
  {
    static Interval fwd(const Interval& x1, const Interval& x2);
    static ScalarType fwd_natural(const ScalarType& x1, const ScalarType& x2);
    static ScalarType fwd_centered(const ScalarType& x1, const ScalarType& x2);
    static void bwd(const Interval& y, Interval& x1, Interval& x2);

    static IntervalVector fwd(const IntervalVector& x1, const Interval& x2);
    static VectorType fwd_natural(const VectorType& x1, const ScalarType& x2);
    static VectorType fwd_centered(const VectorType& x1, const ScalarType& x2);
    static void bwd(const IntervalVector& y, IntervalVector& x1, Interval& x2);
  };

  // operator/
  // The following functions can be used to build analytic expressions.
    
  inline ScalarExpr
  operator/(const ScalarExpr& x1, const ScalarExpr& x2)
  {
    return { std::make_shared<AnalyticOperationExpr<DivOp,ScalarType,ScalarType,ScalarType>>(x1,x2) };
  }
  
  inline VectorExpr
  operator/(const VectorExpr& x1, const ScalarExpr& x2)
  {
    return { std::make_shared<AnalyticOperationExpr<DivOp,VectorType,VectorType,ScalarType>>(x1,x2) };
  }

  // Inline functions

  inline Interval DivOp::fwd(const Interval& x1, const Interval& x2)
  {
    return x1 / x2;
  }

  inline ScalarType DivOp::fwd_natural(const ScalarType& x1, const ScalarType& x2)
  {
    return {
      fwd(x1.a, x2.a),
      x1.def_domain && x2.def_domain && x2.a != 0. // def domain of the derivative of div
    };
  }

  inline ScalarType DivOp::fwd_centered(const ScalarType& x1, const ScalarType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);
    
    assert(x1.da.size() == x2.da.size());

    IntervalMatrix d(1,x1.da.size());
    for(Index i = 0 ; i < d.size() ; i++)
      d(0,i) = (x1.da(0,i)*x2.a-x1.a*x2.da(0,i))/sqr(x2.a);

    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d,
      x1.def_domain && x2.def_domain && x2.a != 0. // def domain of the derivative of div
    };
  }

  inline void DivOp::bwd(const Interval& y, Interval& x1, Interval& x2)
  {
    if((x1 &= y*x2).is_empty())
      x2.set_empty();

    else
    {
      Interval tmp = y;
      MulOp::bwd(x1, tmp, x2);
      if(x2.is_empty())
        x1.set_empty();
    }
  }

  inline IntervalVector DivOp::fwd(const IntervalVector& x1, const Interval& x2)
  {
    return x1 / x2;
  }

  inline VectorType DivOp::fwd_natural(const VectorType& x1, const ScalarType& x2)
  {
    return {
      fwd(x1.a, x2.a),
      x1.def_domain && x2.def_domain && x2.a != 0. // def domain of the derivative of div
    };
  }

  inline VectorType DivOp::fwd_centered(const VectorType& x1, const ScalarType& x2)
  {
    if(centered_form_not_available_for_args(x1,x2))
      return fwd_natural(x1,x2);
    
    assert(x1.da.size() == x2.da.size());

    IntervalMatrix d(1,x1.da.size());
    assert_release(false && "not implemented yet");

    return {
      fwd(x1.m, x2.m),
      fwd(x1.a, x2.a),
      d,
      x1.def_domain && x2.def_domain && x2.a != 0. // def domain of the derivative of div
    };
  }

  inline void DivOp::bwd(const IntervalVector& y, IntervalVector& x1, Interval& x2)
  {
    assert(x1.size() == y.size());
    for(Index i = 0 ; i < x1.size() ; i++)
      DivOp::bwd(y[i], x1[i], x2);
  }
}